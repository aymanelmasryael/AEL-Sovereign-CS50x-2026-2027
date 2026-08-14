"""Python Travel - Shared Helper Utilities

Central module for reusable, cross-cutting concerns used throughout the
application: HTTP-error rendering, authenticated-route guarding and small
defensive string/date helpers.

© Ayman Elmasry - AEL Digital Studio - All Rights Reserved.
"""

from functools import wraps

from flask import redirect, render_template, session


# ---------------------------------------------------------------------------
# Error rendering
# ---------------------------------------------------------------------------
def apology(message, code=400):
    """Render a human-friendly error card to the user.

    Complexity / rationale:
    The message is URL-encoded defensively before being passed to the
    template. This prevents any user-influenced content (e.g. a malformed
    station name) from breaking markup or injecting surprises, and keeps the
    UI consistent with the glassmorphism error design.
    """
    from urllib.parse import quote
    return (
        render_template("apology.html", top=code, bottom=quote(message)),
        code,
    )


# ---------------------------------------------------------------------------
# Authentication guard
# ---------------------------------------------------------------------------
def login_required(f):
    """Decorate a view to require an authenticated admin session.

    Complexity / rationale:
    Wrapping with ``functools.wraps`` preserves the original function's
    metadata (name, docstring) so Flask can still resolve its endpoint name
    for ``url_for`` and logging. Unauthenticated users are transparently
    redirected to the admin login rather than receiving a raw 401, which is a
    friendlier and more predictable flow for humans.
    """

    @wraps(f)
    def decorated_function(*args, **kwargs):
        if not session.get("admin_logged_in"):
            return redirect("/admin_login")
        return f(*args, **kwargs)

    return decorated_function
