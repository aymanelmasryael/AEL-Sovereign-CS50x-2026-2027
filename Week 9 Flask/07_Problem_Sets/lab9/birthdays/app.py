"""
CS50 Birthdays - Single-View Birthday Ledger
===============================================================================

A small but polished Flask application that lets visitors record and review
birthdays. The behaviour contract is intentionally minimal and unchanged:
the single ``/`` route handles both ``GET`` (render the form and existing
entries) and ``POST`` (persist a new entry).

Contract preserved for check50
------------------------------
    * Table        -> ``birthdays`` with columns (id, name, month, day)
    * Form fields  -> ``name``, ``month``, ``day``
    * Template var -> ``birthdays`` iterated to render each stored entry
    * PRG          -> every successful POST redirects back to ``/``
"""

from cs50 import SQL
from flask import Flask, redirect, render_template, request

# Configure the application
app = Flask(__name__)

# Ensure templates are auto-reloaded as files change
app.config["TEMPLATES_AUTO_RELOAD"] = True

# CS50 library bound to the SQLite vault
db = SQL("sqlite:///birthdays.db")


@app.after_request
def after_request(response):
    """Disable caching so the ledger always reflects the latest rows."""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


def _as_positive_int(raw, minimum, maximum):
    """
    Parse a numeric form value within an inclusive range.

    Returns the parsed integer, or None when the value is absent, not a
    whole number, or outside [minimum, maximum].
    """
    if raw is None or str(raw).strip() == "":
        return None
    try:
        value = int(str(raw).strip())
    except (TypeError, ValueError):
        return None
    return value if minimum <= value <= maximum else None


@app.route("/", methods=["GET", "POST"])
def index():
    """Handle new submissions (POST) and render the ledger (GET)."""
    if request.method == "POST":
        # --- Validate the submitted birthday ---------------------------
        name = (request.form.get("name") or "").strip()
        month = _as_positive_int(request.form.get("month"), 1, 12)
        day = _as_positive_int(request.form.get("day"), 1, 31)

        if not name:
            return redirect("/")
        if month is None or day is None:
            return redirect("/")

        # --- Persist and apply the PRG pattern -------------------------
        db.execute(
            "INSERT INTO birthdays (name, month, day) VALUES (?, ?, ?)",
            name, month, day,
        )
        return redirect("/")

    # GET - load every stored birthday for display.
    birthdays = db.execute("SELECT * FROM birthdays ORDER BY month, day, name")
    return render_template("index.html", birthdays=birthdays)


if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000)
