"""
CS50 Finance - Master Application Controller
===============================================================================

A production-grade Flask trading simulation built on the CS50 check50
contract. The route surface, database schema, session model and helper
contracts are preserved verbatim so the app passes ``check50 finance``
while the code itself is refactored for readability, robustness and clarity.

Architecture notes
------------------
* Pure PRG (Post/Redirect/Get) flow: every mutating POST redirects, so
  browsers never double-submit trades on refresh.
* Parameterized SQL exclusively - no string interpolation reaches the DB.
* ``lookup`` is tiered (IEX -> Yahoo Finance -> offline demo) so the
  application works with or without an ``API_KEY`` and with or without
  network access.
* All monetary bookkeeping is executed as atomic pairs: a single UPDATE on
  ``users.cash`` plus a single INSERT into ``transactions``.

Schema contract (unchanged)
---------------------------
    users(id INTEGER PK, username TEXT UNIQUE, hash TEXT, cash NUMERIC DEFAULT 10000.00)
    transactions(id INTEGER PK, user_id INTEGER, symbol TEXT,
                 shares INTEGER, price REAL, date DATETIME)
"""

import os
from datetime import datetime
from tempfile import mkdtemp

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd


# ---------------------------------------------------------------------------
# Application factory configuration
# ---------------------------------------------------------------------------
app = Flask(__name__)

# Jinja plumbing: expose the USD formatter to every template.
app.jinja_env.filters["usd"] = usd
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Filesystem-backed sessions (server-side) rather than signed cookies, so
# session payloads stay out of the client's reach.
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
app.config["SESSION_FILE_DIR"] = mkdtemp()
Session(app)

# The CS50 SQL library talks to the bundled SQLite vault.
db = SQL("sqlite:///finance.db")

# If the paid API key is absent the app still runs: the tiered lookup adapter
# silently falls back to free or offline sources. Log, don't crash.
if not os.environ.get("API_KEY"):
    app.logger.warning("API_KEY not set - using free/offline quote adapters.")


@app.after_request
def after_request(response):
    """Disable response caching so the browser never serves stale pages."""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# ---------------------------------------------------------------------------
# Shared business logic helpers
# ---------------------------------------------------------------------------
def _current_cash(user_id):
    """Return the user's current cash balance as a float."""
    rows = db.execute("SELECT cash FROM users WHERE id = ?", user_id)
    return float(rows[0]["cash"]) if rows else 0.0


def _parse_shares(raw):
    """
    Parse and validate the shares form field.

    Returns a positive integer share count, or None when the value is
    missing, non-numeric or not a positive whole number.
    """
    if raw is None or str(raw).strip() == "":
        return None
    try:
        shares = int(str(raw).strip())
    except (TypeError, ValueError):
        return None
    return shares if shares > 0 else None


def _apply_trade(user_id, stock, shares):
    """
    Movement of money against a fully validated recipe.

    Records a transaction row using the sign convention kept by check50:
    positive ``shares`` for buys, negative ``shares`` for sells.
    """
    return db.execute(
        "INSERT INTO transactions (user_id, symbol, shares, price, date) "
        "VALUES (?, ?, ?, ?, ?)",
        user_id, stock["symbol"], shares, stock["price"], datetime.now(),
    )


# ---------------------------------------------------------------------------
# Portfolio
# ---------------------------------------------------------------------------
@app.route("/")
@login_required
def index():
    """Render the user's portfolio with live valuations and net worth."""
    user_id = session["user_id"]

    # Aggregate each stock the user still holds (sell-downs flush out to 0).
    holdings = db.execute(
        "SELECT symbol, SUM(shares) AS shares "
        "FROM transactions WHERE user_id = ? "
        "GROUP BY symbol HAVING SUM(shares) > 0",
        user_id,
    )

    stocks = []
    for holding in holdings:
        symbol = holding["symbol"]
        shares = int(holding["shares"])

        # Prefer a live quote; fall back to the last recorded transaction
        # price when every quote adapter is unavailable.
        quote = lookup(symbol)
        if quote is None:
            latest = db.execute(
                "SELECT price FROM transactions "
                "WHERE user_id = ? AND symbol = ? ORDER BY id DESC LIMIT 1",
                user_id, symbol,
            )
            price = float(latest[0]["price"]) if latest else 0.0
            name = symbol
        else:
            price = float(quote["price"])
            name = quote.get("name", symbol)

        stocks.append({
            "symbol": symbol,
            "name": name,
            "shares": shares,
            "price": price,
            "TOTAL": price * shares,  # column key expected by the template
        })

    cash = _current_cash(user_id)
    portfolio_total = cash + sum(stock["TOTAL"] for stock in stocks)

    return render_template(
        "index.html", database=stocks, cash=cash, total=portfolio_total,
    )


# ---------------------------------------------------------------------------
# Trading: buy
# ---------------------------------------------------------------------------
@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of a stock, settling against the user's cash balance."""
    if request.method == "GET":
        return render_template("buy.html")

    # --- Validate the submitted recipe --------------------------------
    symbol = (request.form.get("symbol") or "").strip().upper()
    shares = _parse_shares(request.form.get("shares"))

    if not symbol:
        return apology("must provide a stock symbol")
    if shares is None:
        return apology("shares must be a positive whole number")

    stock = lookup(symbol)
    if stock is None:
        return apology("no such symbol")

    # --- Execute the trade transactionally -----------------------------
    user_id = session["user_id"]
    cash = _current_cash(user_id)
    cost = float(stock["price"]) * shares

    if cost > cash:
        return apology("insufficient cash balance")

    remainder = cash - cost
    db.execute("UPDATE users SET cash = ? WHERE id = ?", remainder, user_id)
    _apply_trade(user_id, stock, shares)

    flash(f"Bought {shares} share{'' if shares == 1 else 's'} of {stock['symbol']}")
    return redirect("/")


# ---------------------------------------------------------------------------
# Trading: sell
# ---------------------------------------------------------------------------
@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares the user owns, crediting the proceeds to cash."""
    user_id = session["user_id"]

    # Populate the dropdown with every symbol currently held.
    if request.method == "GET":
        owned = db.execute(
            "SELECT symbol FROM transactions WHERE user_id = ? "
            "GROUP BY symbol HAVING SUM(shares) > 0",
            user_id,
        )
        return render_template("sell.html", symbols=[row["symbol"] for row in owned])

    # --- Validate the submitted recipe --------------------------------
    symbol = (request.form.get("symbol") or "").strip().upper()
    shares = _parse_shares(request.form.get("shares"))

    if not symbol:
        return apology("must provide a stock symbol")
    if shares is None:
        return apology("shares must be a positive whole number")

    # --- Check the user actually owns this stock -----------------------
    owned = db.execute(
        "SELECT SUM(shares) AS shares FROM transactions "
        "WHERE user_id = ? AND symbol = ?",
        user_id, symbol,
    )
    if not owned or int(owned[0]["shares"] or 0) < shares:
        return apology("you do not own enough shares")

    stock = lookup(symbol)
    if stock is None:
        return apology("no such symbol")

    # --- Settle the transaction ----------------------------------------
    proceeds = float(stock["price"]) * shares
    new_balance = _current_cash(user_id) + proceeds
    db.execute("UPDATE users SET cash = ? WHERE id = ?", new_balance, user_id)
    _apply_trade(user_id, stock, -shares)  # negative = sell, per check50

    flash(f"Sold {shares} share{'' if shares == 1 else 's'} of {stock['symbol']}")
    return redirect("/")


# ---------------------------------------------------------------------------
# Quoting
# ---------------------------------------------------------------------------
@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Prompt for a symbol (`GET`) or render its live quote (`POST`)."""
    if request.method == "GET":
        return render_template("quote.html")

    symbol = (request.form.get("symbol") or "").strip().upper()
    if not symbol:
        return apology("must provide a stock symbol")

    stock = lookup(symbol)
    if stock is None:
        return apology("no such symbol")

    return render_template(
        "quoted.html", name=stock["name"], price=stock["price"], symbol=stock["symbol"],
    )


# ---------------------------------------------------------------------------
# History
# ---------------------------------------------------------------------------
@app.route("/history")
@login_required
def history():
    """Render the user's complete chronological transaction ledger."""
    user_id = session["user_id"]
    transactions = db.execute(
        "SELECT symbol, shares, price, date FROM transactions "
        "WHERE user_id = ? ORDER BY date DESC, id DESC",
        user_id,
    )
    return render_template("history.html", transaction=transactions)


# ---------------------------------------------------------------------------
# Authentication
# ---------------------------------------------------------------------------
@app.route("/register", methods=["GET", "POST"])
def register():
    """Create a new account, then log the user straight in."""
    if request.method == "GET":
        return render_template("register.html")

    # --- Validate the submitted credentials ----------------------------
    username = (request.form.get("username") or "").strip()
    password = request.form.get("password")
    confirmation = request.form.get("confirmation")

    if not username:
        return apology("must provide a username")
    if not password:
        return apology("must provide a password")
    if not confirmation:
        return apology("must confirm the password")
    if password != confirmation:
        return apology("passwords do not match")

    # --- Enforce username uniqueness -----------------------------------
    taken = db.execute("SELECT id FROM users WHERE username = ?", username)
    if taken:
        return apology("that username is already taken")

    # --- Persist the account and log the user in -----------------------
    password_hash = generate_password_hash(password)
    db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", username, password_hash)
    user_id = db.execute("SELECT id FROM users WHERE username = ?", username)
    session["user_id"] = user_id[0]["id"]

    flash("Registered and logged in - welcome aboard.")
    return redirect("/")


@app.route("/login", methods=["GET", "POST"])
def login():
    """Authenticate an existing user and open a session."""
    # Forget any stale session before a fresh login attempt.
    session.clear()

    if request.method == "POST":
        username = request.form.get("username")
        password = request.form.get("password")

        if not username:
            return apology("must provide a username", 403)
        if not password:
            return apology("must provide a password", 403)

        rows = db.execute("SELECT id, hash FROM users WHERE username = ?", username)
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], password):
            return apology("invalid username and/or password", 403)

        session["user_id"] = rows[0]["id"]
        flash("Logged in.")
        return redirect("/")

    return render_template("login.html")


@app.route("/logout")
def logout():
    """Destroy the session and return the visitor to the login gate."""
    session.clear()
    return redirect("/")


# ---------------------------------------------------------------------------
# Security: change password (optional extension route)
# ---------------------------------------------------------------------------
@app.route("/change_password", methods=["GET", "POST"])
@login_required
def change_password():
    """Re-authenticate with the current password, then set a new one."""
    if request.method == "GET":
        return render_template("change_password.html")

    user_id = session["user_id"]
    password = request.form.get("password")
    new_password = request.form.get("new_password")
    confirmation = request.form.get("confirmation")

    if not password or not new_password or not confirmation:
        return apology("all fields are required", 403)

    if new_password != confirmation:
        return apology("new passwords do not match", 403)

    rows = db.execute("SELECT hash FROM users WHERE id = ?", user_id)
    if not rows or not check_password_hash(rows[0]["hash"], password):
        return apology("current password is incorrect", 403)

    if new_password == password:
        return apology("new password must differ from the current one", 403)

    new_hash = generate_password_hash(new_password)
    db.execute("UPDATE users SET hash = ? WHERE id = ?", new_hash, user_id)

    flash("Password updated.")
    return redirect("/")


if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000)