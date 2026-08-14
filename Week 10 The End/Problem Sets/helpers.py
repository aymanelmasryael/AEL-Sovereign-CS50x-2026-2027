"""
helpers.py — AEL Sovereign Fleet Manager
========================================

Shared utility functions for the Flask application.

Author: Ayman Elmasry — AEL Digital Studio

login_required
    Decorator that blocks a view unless an administrator session exists.
    Guests are bounced to the login page with a flash notice.

apology(message, code)
    Renders a styled error panel (templates/apology.html) carrying a status
    code in the {{ top }} slot and a human-readable explanation in the
    {{ bottom }} slot. Used for every validation and access failure.
"""

from functools import wraps

from flask import flash, redirect, render_template, session, url_for


def login_required(view):
    """
    Protect an endpoint from anonymous access.

    When no administrator id is present in the session the decorated view
    flashes a notice and redirects to the admin login page.
    """

    @wraps(view)
    def wrapped(*args, **kwargs):
        if session.get("admin_id") is None:
            flash("Administrator access required. Please sign in.", "error")
            return redirect(url_for("admin_login"))
        return view(*args, **kwargs)

    return wrapped


def apology(message, code=400):
    """Render the error panel with the HTTP status and explanation text."""
    return render_template("apology.html", top=code, bottom=message), code
