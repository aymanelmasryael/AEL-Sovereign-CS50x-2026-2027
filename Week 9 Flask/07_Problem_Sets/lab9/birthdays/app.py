"""
==============================================================================
AEL Birthdays - Birthday Reminder Ledger
==============================================================================

Project      : Lab 9 - Birthdays (CS50)
Author       : Ayman Elmasry -- AEL Digital Studio
Framework    : Flask (Python 3) with SQLite
Database     : birthdays.db (table: birthdays: id, name, month, day)

Overview
--------
A small web application that records birthdays and lists them in a ledger.
A user submits a name, a month (1-12), and a day (1-31); the entry is stored
in the database and the updated ledger is rendered on the home page.

Security Notes
--------------
* All SQL statements are fully parameterized to prevent SQL injection.
* Input is strictly validated on the server: a non-empty name, a valid month,
  and a valid calendar day. Invalid submissions re-render the form with an
  explanatory error rather than silently discarding input.
* The Post/Redirect/Get (PRG) pattern is used for successful insertions so a
  browser refresh never duplicates a record.
* Browser caching is disabled so stale birthday data is never displayed.
* A per-request database connection is opened and closed automatically so no
  connection leaks across requests.
==============================================================================
"""

import os
import sqlite3

from flask import Flask, g, redirect, render_template, request, url_for

# ----------------------------------------------------------------------------
# Application configuration
# ----------------------------------------------------------------------------

# Locate the database next to this file so the app is directory-agnostic.
DATABASE = os.path.join(os.path.dirname(os.path.realpath(__file__)), "birthdays.db")

app = Flask(__name__)
app.config["SECRET_KEY"] = os.environ.get("SECRET_KEY") or "ael-birthdays-dev-key"


# ----------------------------------------------------------------------------
# Database helpers
# ----------------------------------------------------------------------------

def get_db():
    """Return the request-scoped SQLite connection, creating it on demand."""
    if "db" not in g:
        connection = sqlite3.connect(DATABASE)
        connection.row_factory = sqlite3.Row
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
    """Disable browser caching to keep the rendered ledger fresh."""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Pragma"] = "no-cache"
    response.headers["Expires"] = "0"
    return response


# ----------------------------------------------------------------------------
# Validation helpers
# ----------------------------------------------------------------------------

def _valid_month(value):
    """Return True when value is a whole number in the range 1..12."""
    try:
        return 1 <= int(value) <= 12
    except (TypeError, ValueError):
        return False


def _valid_day(value):
    """Return True when value is a whole number in the range 1..31."""
    try:
        return 1 <= int(value) <= 31
    except (TypeError, ValueError):
        return False


# ----------------------------------------------------------------------------
# Routes
# ----------------------------------------------------------------------------

@app.route("/", methods=["GET", "POST"])
def index():
    """Render the birthday form and ledger (GET) or add a birthday (POST)."""
    if request.method == "POST":
        name = (request.form.get("name") or "").strip()
        month = request.form.get("month")
        day = request.form.get("day")

        # Validate every field defensively, collecting a human-readable error.
        error = None
        if not name:
            error = "Please provide a name."
        elif not _valid_month(month):
            error = "Month must be a whole number between 1 and 12."
        elif not _valid_day(day):
            error = "Day must be a whole number between 1 and 31."

        if error is not None:
            db = get_db()
            birthdays = db.execute("SELECT * FROM birthdays").fetchall()
            # Re-render with the user's input preserved and an error banner so
            # they can correct the entry without retyping everything.
            return render_template(
                "index.html",
                birthdays=birthdays,
                error=error,
                name=name,
                month=month,
                day=day,
            )

        db = get_db()
        db.execute(
            "INSERT INTO birthdays (name, month, day) VALUES (?, ?, ?)",
            (name, int(month), int(day)),
        )
        db.commit()

        # PRG pattern: redirect so a refresh cannot duplicate the insertion.
        return redirect(url_for("index"))

    db = get_db()
    birthdays = db.execute("SELECT * FROM birthdays").fetchall()
    return render_template("index.html", birthdays=birthdays)


if __name__ == "__main__":
    app.run(host="127.0.0.1", port=5001, debug=True)
