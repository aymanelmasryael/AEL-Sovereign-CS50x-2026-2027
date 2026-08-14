import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash
from datetime import datetime

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    user_id = session["user_id"]
    transaction_db = db.execute("SELECT symbol, SUM(shares) AS shares, price FROM transactions WHERE user_id = ? GROUP BY symbol", user_id)
    cash_db = db.execute("SELECT cash FROM users WHERE id = ?", user_id)
    cash = cash_db[0]["cash"]

    return render_template("index.html", database=transaction_db, cash=cash)

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == "GET":
        return render_template("buy.html")
    else:
        symbol = request.form.get("symbol")
        try:
            shares = int(request.form.get("shares"))
        except ValueError:
            return apology("Value Error.")

        if not symbol:
            return apology("Symbol can not be null!")

        stock = lookup(symbol.upper())

        if not stock:
            return apology("Symbol does not exist.")

        if shares < 1:
            return apology("Share must be greater than 0!")

        transaction_value = shares * stock["price"]
        user_id = session["user_id"]
        cash_db = db.execute("SELECT cash FROM users WHERE id=:id", id=user_id)
        user_cash = cash_db[0]["cash"]

        if user_cash < transaction_value:
            return apology("Not enough money.")

        remainder = user_cash - transaction_value

        db.execute("UPDATE users SET cash = ? WHERE id = ?", remainder, user_id)

        date = datetime.now()

        db.execute("INSERT INTO transactions (user_id, symbol, shares, price, date) VALUES (?, ?, ?, ?, ?)", user_id, stock["symbol"], shares, stock["price"], date)

        flash("Bought!")
        return redirect("/")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    user_id = session["user_id"]
    transaction_db = db.execute("SELECT * FROM transactions WHERE user_id = :id", id=user_id)
    return render_template("history.html", transaction=transaction_db)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    if request.method == "GET":
        return render_template("quote.html")
    else:
        symbol = request.form.get("symbol")
        if not symbol:
            return apology("Symbol can not be null!")
        stock = lookup(symbol.upper())

        if not stock:
            return apology("Symbol does not exist.")
        return render_template("quoted.html", name=stock["name"], price=stock["price"], symbol=stock["symbol"])

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "GET":
        return render_template("register.html")
    else:
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        if not username:
            return apology("Username can not be blank!")

        if not password:
            return apology("Password can not be blank!")

        if not confirmation:
            return apology("Confirmation can not be blank!")

        if db.execute("SELECT username FROM users WHERE username = ?", username):
            return apology("Sorry, this username is taken.")

        if password != confirmation:
            return apology("Password and confirmation must be same!")

        hash = generate_password_hash(password)

        registered_user = db.execute("INSERT INTO users (username, hash) VALUES(?, ?)", username, hash)

        session["user_id"] = registered_user
        flash("Registered.")

        return redirect("/")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "GET":
        user_id = session["user_id"]
        rows = db.execute("SELECT symbol FROM transactions WHERE user_id = :id GROUP BY symbol HAVING SUM(shares) > 0", id=user_id)

        return render_template("sell.html", symbols=[row["symbol"] for row in rows])
    else:
        symbol = request.form.get("symbol")
        shares = int(request.form.get("shares"))

        if not symbol:
            return apology("Symbol can not be null!")

        stock = lookup(symbol.upper())

        if not stock:
            return apology("Symbol does not exist.")

        if shares < 1:
            return apology("Share must be greater than 0!")

        transaction_value = shares * stock["price"]
        user_id = session["user_id"]
        cash_db = db.execute("SELECT cash FROM users WHERE id=:id", id=user_id)
        user_cash = cash_db[0]["cash"]

        shares_db = db.execute("SELECT shares FROM transactions WHERE user_id = :id AND symbol = :symbol GROUP BY symbol", id=user_id, symbol=symbol)
        user_shares = shares_db[0]["shares"]

        if shares > user_shares:
            return apology("You don't have enough shares!")

        remainder = user_cash + transaction_value

        db.execute("UPDATE users SET cash = ? WHERE id = ?", remainder, user_id)

        date = datetime.now()

        db.execute("INSERT INTO transactions (user_id, symbol, shares, price, date) VALUES (?, ?, ?, ?, ?)", user_id, stock["symbol"], -shares, stock["price"], date)

        flash("Sold!")
        return redirect("/")


@app.route("/change_password", methods=["GET", "POST"])
@login_required
def change_password():
    """Change the password"""
    if request.method == "GET":
        return render_template("change_password.html")
    else:
        user_id = session["user_id"]
        password = request.form.get("password")
        new_password = request.form.get("new_password")
        confirm = request.form.get("confirmation")

        rows = db.execute("SELECT hash FROM users WHERE id = ?", user_id)

        if not password or not new_password or not confirm:
            return apology("Password can not be null!", 403)

        if new_password != confirm:
            return apology("Passwords must be same!")

        if new_password == password:
            return apology("New password can not be same with old password!")

        if not check_password_hash(rows[0]["hash"], password):
            return apology("Previous password is wrong!")

        hash = generate_password_hash(new_password)

        db.execute("UPDATE users SET hash = ? WHERE id = ?", hash, user_id)

        flash("Password Changed.")

        return redirect("/")
