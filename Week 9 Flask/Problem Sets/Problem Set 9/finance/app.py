"""
==============================================================================
AEL Finance - Virtual Stock Trading Platform
==============================================================================

Project      : C$50 Finance (CS50 Problem Set 9)
Author       : Ayman Elmasry -- AEL Digital Studio
Framework    : Flask (Python 3) with SQLite and server-side filesystem sessions
Database     : finance.db (tables: users, transactions)

Overview
--------
A web application that simulates stock trading using virtual cash. Users may
register for an account, log in, look up live (or offline demo) stock quotes,
buy and sell shares, review their portfolio, inspect their transaction
history, and change their account password.

Security Notes
--------------
* Passwords are never stored in plain text; they are hashed using
  werkzeug.security.generate_password_hash (PBKDF2 / scrypt by default).
* All SQL statements are fully parameterized to prevent SQL injection.
* Every request that mutates state uses the Post/Redirect/Get (PRG) pattern so
  refreshes never re-submit a form and accidentally duplicate a purchase.
* Browser caching is disabled globally so sensitive portfolio data is not
  left in client-side caches.
* A per-request database connection is opened, used, and closed so no
  connection is ever shared across threads or leaked.
* Server-side filesystem sessions (Flask-Session) store the session id on the
  client, never sensitive data.

Route Map
---------
GET  /                  Portfolio summary (login required)
GET  /login             Log-in form
POST /login             Authenticate and start a session
GET  /logout            Terminate the session
GET  /register          Registration form
POST /register          Create a new account
GET  /quote             Quote request form (login required)
POST /quote             Look up a stock quote (login required)
GET  /buy               Buy form (login required)
POST /buy               Purchase shares (login required)
GET  /sell              Sell form (login required)
POST /sell              Sell shares (login required)
GET  /history           Transaction ledger (login required)
GET  /change_password   Password change form (login required)
POST /change_password   Apply a password change (login required)
==============================================================================
"""

import datetime
import os
import sqlite3
from tempfile import mkdtemp

from flask import Flask, flash, g, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# ----------------------------------------------------------------------------
# Application configuration
# ----------------------------------------------------------------------------

# Resolve the SQLite database path relative to this file so the application
# runs correctly regardless of the directory it is launched from.
DATABASE = os.path.join(os.path.dirname(os.path.realpath(__file__)), "finance.db")

app = Flask(__name__)

# Signing key used by Flask for the client-side session cookie. Prefer a real
# secret in production via the SECRET_KEY environment variable.
app.config["SECRET_KEY"] = os.environ.get("SECRET_KEY") or "ael-finance-dev-key"

# Use Flask-Session to store sessions on the server filesystem rather than in
# the browser cookie, which keeps session data out of client reach.
app.config["SESSION_TYPE"] = "filesystem"
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False

Session(app)

# Expose the "usd" formatter as a Jinja filter so templates can display
# currency amounts consistently (e.g. {{ amount | usd }}).
app.jinja_env.filters["usd"] = usd


# ----------------------------------------------------------------------------
# Database helpers
# ----------------------------------------------------------------------------

def get_db():
    """Return the request-scoped SQLite connection, creating it on demand.

    The connection is stored on Flask's application context object ``g`` so it
    is reused across multiple queries within a single request and automatically
    torn down when the request finishes.
    """
    if "db" not in g:
        connection = sqlite3.connect(DATABASE)
        connection.row_factory = sqlite3.Row  # access columns by name
        g.db = connection
    return g.db


@app.teardown_appcontext
def close_db(_exc):
    """Close the request-scoped database connection when the request ends."""
    connection = g.pop("db", None)
    if connection is not None:
        connection.close()


# ----------------------------------------------------------------------------
# Response headers
# ----------------------------------------------------------------------------

@app.after_request
def prevent_caching(response):
    """Disable browser caching to protect sensitive financial data."""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Pragma"] = "no-cache"
    response.headers["Expires"] = "0"
    return response


# ----------------------------------------------------------------------------
# Authentication routes
# ----------------------------------------------------------------------------

@app.route("/register", methods=["GET", "POST"])
def register():
    """Render the registration form (GET) or create a new account (POST)."""
    if request.method == "POST":
        username = (request.form.get("username") or "").strip()
        password = request.form.get("password") or ""
        confirmation = request.form.get("confirmation") or ""

        # Defensive validation of every field the form submits.
        if not username:
            return apology("must provide username", 400)
        if not password:
            return apology("must provide password", 400)
        if not confirmation:
            return apology("must provide confirmation", 400)
        if password != confirmation:
            return apology("Passwords do not match", 400)

        db = get_db()

        # The username column carries a UNIQUE index, but we check first so we
        # can return a friendly apology instead of an integrity error.
        existing = db.execute(
            "SELECT id FROM users WHERE username = ?", (username,)
        ).fetchall()
        if existing:
            return apology("Username already exists", 400)

        # Store only the password hash, never the plain-text password.
        db.execute(
            "INSERT INTO users (username, hash) VALUES (?, ?)",
            (username, generate_password_hash(password)),
        )
        db.commit()

        # Log the new user in immediately and redirect (PRG pattern).
        new_id = db.execute(
            "SELECT id FROM users WHERE username = ?", (username,)
        ).fetchone()["id"]
        session["user_id"] = new_id

        flash("Registered successfully. Welcome aboard.")
        return redirect("/")

    return render_template("register.html")


@app.route("/login", methods=["GET", "POST"])
def login():
    """Render the log-in form (GET) or authenticate a user (POST)."""
    session.clear()

    if request.method == "POST":
        username = (request.form.get("username") or "").strip()
        password = request.form.get("password") or ""

        if not username:
            return apology("must provide username", 400)
        if not password:
            return apology("must provide password", 400)

        db = get_db()
        rows = db.execute(
            "SELECT id, hash FROM users WHERE username = ?", (username,)
        ).fetchall()

        # The username must exist and the stored hash must verify the supplied
        # password. Both checks use the same generic message to avoid leaking
        # which part actually failed.
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], password):
            return apology("invalid username and/or password", 400)

        session["user_id"] = rows[0]["id"]
        flash("Logged in successfully.")
        return redirect("/")

    return render_template("login.html")


@app.route("/logout")
def logout():
    """Clear the session and return to the home page."""
    session.clear()
    return redirect("/")


# ----------------------------------------------------------------------------
# Portfolio routes
# ----------------------------------------------------------------------------

@app.route("/")
@login_required
def index():
    """Render the user's portfolio: current holdings, cash, and grand total."""
    db = get_db()

    # Aggregate the ledger into current positions. Shares are positive for buys
    # and negative for sells, so a positive running total means we still own them.
    holdings = db.execute(
        "SELECT symbol, SUM(shares) AS shares "
        "FROM transactions WHERE user_id = ? "
        "GROUP BY symbol HAVING SUM(shares) > 0",
        (session["user_id"],),
    ).fetchall()

    positions = []
    grand_total = 0.0

    # Enrich each holding with the current price and company name. If the live
    # quote service is unreachable, lookup() transparently falls back to the
    # bundled offline dataset so the page always renders.
    for holding in holdings:
        quote = lookup(holding["symbol"])
        if quote is None:
            continue
        subtotal = holding["shares"] * quote["price"]
        grand_total += subtotal
        positions.append(
            {
                "symbol": holding["symbol"],
                "name": quote["name"],
                "shares": holding["shares"],
                "price": quote["price"],
                "total": subtotal,
            }
        )

    cash = db.execute(
        "SELECT cash FROM users WHERE id = ?", (session["user_id"],)
    ).fetchone()["cash"]

    grand_total += cash

    return render_template(
        "index.html", positions=positions, cash=cash, grand_total=grand_total
    )


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Render the quote form (GET) or look up a symbol (POST)."""
    if request.method == "POST":
        symbol = (request.form.get("symbol") or "").strip()

        if not symbol:
            return apology("must provide symbol", 400)

        quote = lookup(symbol)
        if quote is None:
            return apology("invalid symbol", 400)

        return render_template("quoted.html", quote=quote)

    return render_template("quote.html")


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Render the buy form (GET) or purchase shares (POST)."""
    if request.method == "POST":
        symbol = (request.form.get("symbol") or "").strip()
        shares_text = request.form.get("shares")

        if not symbol:
            return apology("must provide symbol", 400)

        # Shares must be a whole positive number.
        try:
            shares = int(shares_text)
        except (TypeError, ValueError):
            return apology("shares must be a positive integer", 400)
        if shares <= 0:
            return apology("shares must be a positive integer", 400)

        quote = lookup(symbol)
        if quote is None:
            return apology("invalid symbol", 400)

        total_cost = shares * quote["price"]

        db = get_db()
        user = db.execute(
            "SELECT cash FROM users WHERE id = ?", (session["user_id"],)
        ).fetchone()

        # Refuse the purchase if the user cannot cover the cost.
        if user["cash"] < total_cost:
            return apology("can't afford", 400)

        # Record the buy with a positive share count and deduct the cash. Both
        # statements execute inside one implicit transaction and are committed
        # together so they can never diverge.
        db.execute(
            "INSERT INTO transactions (user_id, symbol, shares, price, date) "
            "VALUES (?, ?, ?, ?, ?)",
            (
                session["user_id"],
                quote["symbol"],
                shares,
                quote["price"],
                datetime.datetime.now(),
            ),
        )
        db.execute(
            "UPDATE users SET cash = cash - ? WHERE id = ?",
            (total_cost, session["user_id"]),
        )
        db.commit()

        flash(f"Bought {shares} share(s) of {symbol}.")
        return redirect("/")

    return render_template("buy.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Render the sell form (GET) or sell shares (POST)."""
    db = get_db()

    # Symbols the user currently owns, used to populate the dropdown.
    owned = db.execute(
        "SELECT symbol, SUM(shares) AS shares "
        "FROM transactions WHERE user_id = ? "
        "GROUP BY symbol HAVING SUM(shares) > 0",
        (session["user_id"],),
    ).fetchall()

    if request.method == "POST":
        symbol = (request.form.get("symbol") or "").strip()
        shares_text = request.form.get("shares")

        if not symbol:
            return apology("must provide symbol", 400)

        try:
            shares = int(shares_text)
        except (TypeError, ValueError):
            return apology("shares must be a positive integer", 400)
        if shares <= 0:
            return apology("shares must be a positive integer", 400)

        quote = lookup(symbol)
        if quote is None:
            return apology("invalid symbol", 400)

        # The user may only sell shares they actually own. A missing row or a
        # holding smaller than the requested amount is rejected.
        holding = db.execute(
            "SELECT SUM(shares) AS total FROM transactions "
            "WHERE user_id = ? AND symbol = ?",
            (session["user_id"], symbol),
        ).fetchone()
        if holding["total"] is None or holding["total"] < shares:
            return apology("you do not have that many shares", 400)

        proceeds = shares * quote["price"]

        # Record the sell with a negative share count and credit the proceeds.
        db.execute(
            "INSERT INTO transactions (user_id, symbol, shares, price, date) "
            "VALUES (?, ?, ?, ?, ?)",
            (
                session["user_id"],
                quote["symbol"],
                -shares,
                quote["price"],
                datetime.datetime.now(),
            ),
        )
        db.execute(
            "UPDATE users SET cash = cash + ? WHERE id = ?",
            (proceeds, session["user_id"]),
        )
        db.commit()

        flash(f"Sold {shares} share(s) of {symbol}.")
        return redirect("/")

    return render_template("sell.html", owned=owned)


@app.route("/history")
@login_required
def history():
    """Render the complete transaction ledger for the current user."""
    db = get_db()
    transactions = db.execute(
        "SELECT symbol, shares, price, date FROM transactions "
        "WHERE user_id = ? ORDER BY date DESC, id DESC",
        (session["user_id"],),
    ).fetchall()
    return render_template("history.html", transactions=transactions)


@app.route("/change_password", methods=["GET", "POST"])
@login_required
def change_password():
    """Render the password form (GET) or update the password (POST)."""
    if request.method == "POST":
        current = request.form.get("password") or ""
        new_password = request.form.get("new_password") or ""
        confirmation = request.form.get("confirmation") or ""

        if not current:
            return apology("must provide current password", 400)
        if not new_password:
            return apology("must provide new password", 400)
        if not confirmation:
            return apology("must provide confirmation", 400)
        if new_password != confirmation:
            return apology("new passwords do not match", 400)

        db = get_db()
        rows = db.execute(
            "SELECT hash FROM users WHERE id = ?", (session["user_id"],)
        ).fetchall()

        # Require proof of the current password before allowing a change.
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], current):
            return apology("current password is incorrect", 400)

        db.execute(
            "UPDATE users SET hash = ? WHERE id = ?",
            (generate_password_hash(new_password), session["user_id"]),
        )
        db.commit()

        flash("Password changed successfully.")
        return redirect("/")

    return render_template("change_password.html")


if __name__ == "__main__":
    app.run(host="127.0.0.1", port=5000, debug=True)
