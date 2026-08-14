"""
AEL Sovereign Fleet Manager
===========================

Author:  Ayman Elmasry — AEL Digital Studio
Course:  CS50x 2026-2027 — Final Project
Stack:   Flask + SQLite (Python 3 stdlib sqlite3 module)

PROJECT OVERVIEW
----------------
AEL Sovereign Fleet Manager is a self-contained web application that runs a
public-transport fleet. Operators maintain a catalogue of vehicles and a
board of scheduled routes; customers search departures by origin and
destination, reserve seats, and later look up their own bookings. A single
administrator signs in to manage the fleet and to audit every ticket issued.

ARCHITECTURE
------------
The application follows a classic monolithic Flask layout:

  app.py                entry point, routing and business logic
  helpers.py            login_required decorator and apology() renderer
  templates/*.html      Jinja2 views (layout, dashboard, fleet, tickets, admin)
  static/styles.css     the complete visual design system
  fleet.db              SQLite database, created and seeded on first run

The database is initialised lazily on startup with CREATE TABLE IF NOT EXISTS
statements and seeded with a pair of demo vehicles, two demo routes and one
administrator account. Every SQL statement is parameterised; user input is
never interpolated into a query string.

DATA MODEL
----------
  admins    (id, username UNIQUE, password_hash, created_at)
  vehicles  (id, fleet_code UNIQUE, model, vehicle_type, capacity, status,
             created_at)
  routes    (id, route_code UNIQUE, origin, destination, departure_time,
             base_fare, vehicle_id -> vehicles.id, created_at)
  tickets   (id, reference UNIQUE, route_id -> routes.id, passenger_name,
             passenger_phone, seats, total_amount, issued_at)

Seat availability is a derived value: the assigned vehicle's capacity minus
the seats already sold on that route. Fleet revenue is the sum of every
ticket's total amount. Ticket references are generated with the secrets
module and carry the AEL- prefix.

FEATURES
--------
  1. Public dashboard   fleet size, route count, live revenue and tickets
                        sold, plus a feed of the most recent bookings.
  2. Fleet catalogue    read-only public views of vehicles and routes with
                        live seat availability per departure.
  3. Customer booking   search departures by origin/destination, buy tickets
                        (capacity validated server-side), and look bookings
                        up by passenger name and phone number.
  4. Admin panel        single account guarded by a hashed password, full
                        CRUD over vehicles and routes, and a complete ticket
                        audit trail.

SECURITY NOTES
--------------
  * Administrator passwords are stored as Werkzeug PBKDF2 hashes (never
    plain text) and verified with check_password_hash.
  * All SQL uses bound parameters via the sqlite3 module; the only dynamic
    parts of query text are fixed clauses assembled by application code.
  * Admin endpoints are protected by the login_required() decorator and
    authenticated through Flask signed session cookies.
  * Every mutating form uses Post/Redirect/Get so that refreshing the page
    never re-submits a purchase; feedback is delivered via flash messages.
  * Educational demo: replace the development secret key and the seeded
    credentials before any real deployment.
"""

import os
import secrets
import sqlite3

from flask import (Flask, flash, g, redirect, render_template, request,
                   session, url_for)
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required

# ---------------------------------------------------------------------------
# Application configuration
# ---------------------------------------------------------------------------

app = Flask(__name__)
# Session signing key. Prefer the environment variable; the fallback value is
# intended for local development only and must never ship to production.
app.config["SECRET_KEY"] = os.environ.get("AEL_SECRET_KEY") or \
    "ael-sovereign-development-key-do-not-use-in-production"

# The SQLite database lives next to the application module.
DATABASE = os.path.join(app.root_path, "fleet.db")


# ---------------------------------------------------------------------------
# Database plumbing
# ---------------------------------------------------------------------------

def get_db():
    """Return a per-request SQLite connection stored on Flask's g object."""
    if "db" not in g:
        conn = sqlite3.connect(DATABASE)
        conn.row_factory = sqlite3.Row
        conn.execute("PRAGMA foreign_keys = ON")
        g.db = conn
    return g.db


@app.teardown_appcontext
def close_db(_exc):
    """Close the request-scoped database connection when the context ends."""
    db = g.pop("db", None)
    if db is not None:
        db.close()


# ---------------------------------------------------------------------------
# Schema and demo seed data
# ---------------------------------------------------------------------------

SCHEMA = """
CREATE TABLE IF NOT EXISTS admins (
    id            INTEGER PRIMARY KEY AUTOINCREMENT,
    username      TEXT NOT NULL UNIQUE,
    password_hash TEXT NOT NULL,
    created_at    TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE IF NOT EXISTS vehicles (
    id           INTEGER PRIMARY KEY AUTOINCREMENT,
    fleet_code   TEXT NOT NULL UNIQUE,
    model        TEXT NOT NULL,
    vehicle_type TEXT NOT NULL DEFAULT 'Bus',
    capacity     INTEGER NOT NULL CHECK (capacity > 0),
    status       TEXT NOT NULL DEFAULT 'active',
    created_at   TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE IF NOT EXISTS routes (
    id             INTEGER PRIMARY KEY AUTOINCREMENT,
    route_code     TEXT NOT NULL UNIQUE,
    origin         TEXT NOT NULL,
    destination    TEXT NOT NULL,
    departure_time TEXT NOT NULL,
    base_fare      REAL NOT NULL CHECK (base_fare >= 0),
    vehicle_id     INTEGER NOT NULL REFERENCES vehicles(id),
    created_at     TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE IF NOT EXISTS tickets (
    id              INTEGER PRIMARY KEY AUTOINCREMENT,
    reference       TEXT NOT NULL UNIQUE,
    route_id        INTEGER NOT NULL REFERENCES routes(id),
    passenger_name  TEXT NOT NULL,
    passenger_phone TEXT NOT NULL,
    seats           INTEGER NOT NULL CHECK (seats > 0),
    total_amount    REAL NOT NULL CHECK (total_amount >= 0),
    issued_at       TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP
);

CREATE INDEX IF NOT EXISTS idx_routes_origin  ON routes (origin);
CREATE INDEX IF NOT EXISTS idx_routes_dest    ON routes (destination);
CREATE INDEX IF NOT EXISTS idx_tickets_route  ON tickets (route_id);
CREATE INDEX IF NOT EXISTS idx_tickets_lookup ON tickets (passenger_phone);
"""

DEMO_VEHICLES = [
    ("SV-101", "AEL Aurora 3400", "Coach", 48, "active"),
    ("SV-102", "AEL Titan Cityliner", "Bus", 36, "active"),
]

DEMO_ROUTES = [
    ("R-CAI-ALX", "Cairo", "Alexandria", "08:30", 22.50, 1),
    ("R-CAI-LUX", "Cairo", "Luxor", "22:00", 38.00, 2),
]

# Single seeded administrator used for the demo.
ADMIN_USERNAME = "ael_admin"
ADMIN_PASSWORD = "sovereign"


def init_db():
    """Create every table (if missing) and seed demo data on first run."""
    db = get_db()
    db.executescript(SCHEMA)

    if db.execute("SELECT COUNT(*) AS n FROM admins").fetchone()["n"] == 0:
        db.execute(
            "INSERT INTO admins (username, password_hash) VALUES (?, ?)",
            (ADMIN_USERNAME, generate_password_hash(ADMIN_PASSWORD)),
        )

    if db.execute("SELECT COUNT(*) AS n FROM vehicles").fetchone()["n"] == 0:
        db.executemany(
            "INSERT INTO vehicles (fleet_code, model, vehicle_type, capacity, status)"
            " VALUES (?, ?, ?, ?, ?)",
            DEMO_VEHICLES,
        )

    if db.execute("SELECT COUNT(*) AS n FROM routes").fetchone()["n"] == 0:
        db.executemany(
            "INSERT INTO routes (route_code, origin, destination, departure_time,"
            " base_fare, vehicle_id) VALUES (?, ?, ?, ?, ?, ?)",
            DEMO_ROUTES,
        )

    db.commit()


# Initialise the schema and demo data when the module first loads.
with app.app_context():
    init_db()


# ---------------------------------------------------------------------------
# Shared helpers
# ---------------------------------------------------------------------------

def is_admin():
    """Return True when the current session belongs to the administrator."""
    return session.get("admin_id") is not None


def next_fleet_code():
    """Generate the next sequential vehicle fleet code, e.g. SV-103."""
    db = get_db()
    row = db.execute(
        """
        SELECT fleet_code FROM vehicles
        WHERE fleet_code LIKE 'SV-%'
        ORDER BY CAST(SUBSTR(fleet_code, 4) AS INTEGER) DESC
        LIMIT 1
        """
    ).fetchone()
    seq = 100
    if row is not None:
        seq = int(row["fleet_code"].split("-")[1])
    return f"SV-{seq + 1}"


def make_route_code(origin, destination):
    """Build a readable route code from origin/destination, e.g. R-CAI-ALX."""
    def clean(token):
        return "".join(ch for ch in token.upper() if ch.isalnum())[:3]
    return f"R-{clean(origin)}-{clean(destination)}"


def attach_availability(rows):
    """Augment route rows with their live booked/available seat counts."""
    enriched = []
    for row in rows:
        item = dict(row)
        item["seats_sold"] = item["seats_sold"]
        item["available"] = item["capacity"] - item["seats_sold"]
        enriched.append(item)
    return enriched


@app.template_filter("money")
def money_filter(amount):
    """Jinja filter that renders a number as US-style currency."""
    return f"${amount:,.2f}"


# ---------------------------------------------------------------------------
# Public pages
# ---------------------------------------------------------------------------

@app.route("/")
def index():
    """Public dashboard: headline stats plus the most recent bookings."""
    db = get_db()

    stats = {
        "vehicles": db.execute("SELECT COUNT(*) AS n FROM vehicles").fetchone()["n"],
        "routes": db.execute("SELECT COUNT(*) AS n FROM routes").fetchone()["n"],
        "tickets": db.execute("SELECT COUNT(*) AS n FROM tickets").fetchone()["n"],
        "revenue": db.execute(
            "SELECT COALESCE(SUM(total_amount), 0) AS s FROM tickets"
        ).fetchone()["s"],
    }

    recent = db.execute(
        """
        SELECT t.reference, t.passenger_name, t.seats, t.total_amount, t.issued_at,
               r.route_code, r.origin, r.destination
        FROM tickets t
        JOIN routes r ON r.id = t.route_id
        ORDER BY t.issued_at DESC, t.id DESC
        LIMIT 8
        """
    ).fetchall()

    return render_template("index.html", stats=stats, recent=recent,
                           tickets_full=None, admin=False)


@app.route("/vehicles")
def vehicles():
    """Public fleet catalogue; management forms appear for admins only."""
    db = get_db()
    fleet = db.execute(
        """
        SELECT v.*, COUNT(r.id) AS assigned_routes
        FROM vehicles v
        LEFT JOIN routes r ON r.vehicle_id = v.id
        GROUP BY v.id
        ORDER BY v.fleet_code
        """
    ).fetchall()
    return render_template("vehicles.html", fleet=fleet, admin=is_admin())


@app.route("/routes")
def routes():
    """Public route board with live seat availability per departure."""
    db = get_db()
    rows = db.execute(
        """
        SELECT r.*, v.fleet_code, v.model AS vehicle_model, v.capacity,
               COALESCE(SUM(t.seats), 0) AS seats_sold
        FROM routes r
        JOIN vehicles v ON v.id = r.vehicle_id
        LEFT JOIN tickets t ON t.route_id = r.id
        GROUP BY r.id
        ORDER BY r.departure_time, r.origin
        """
    ).fetchall()

    all_vehicles = db.execute(
        "SELECT id, fleet_code, model FROM vehicles"
        " WHERE status = 'active' ORDER BY fleet_code"
    ).fetchall()

    return render_template(
        "routes.html",
        routes=attach_availability(rows),
        vehicles=all_vehicles,
        admin=is_admin(),
    )


@app.route("/tickets")
def tickets():
    """Customer hub: departure search, purchase form and booking lookup."""
    db = get_db()

    # Search results ride on the URL so the search is idempotent (GET).
    origin = request.args.get("origin", "").strip()
    destination = request.args.get("destination", "").strip()
    results = None

    if origin or destination:
        where, params = [], []
        if origin:
            where.append("r.origin LIKE ?")
            params.append(f"%{origin}%")
        if destination:
            where.append("r.destination LIKE ?")
            params.append(f"%{destination}%")
        clause = " AND ".join(where)

        rows = db.execute(
            f"""
            SELECT r.*, v.fleet_code, v.model AS vehicle_model, v.capacity,
                   COALESCE(SUM(t.seats), 0) AS seats_sold
            FROM routes r
            JOIN vehicles v ON v.id = r.vehicle_id
            LEFT JOIN tickets t ON t.route_id = r.id
            WHERE {clause}
            GROUP BY r.id
            ORDER BY r.departure_time, r.origin
            """,
            params,
        ).fetchall()
        results = attach_availability(rows)

    # Booking lookup data survives the Post/Redirect/Get hop via the URL.
    lookup_name = request.args.get("lookup_name", "").strip()
    lookup_phone = request.args.get("lookup_phone", "").strip()
    bookings = None
    if lookup_name and lookup_phone:
        bookings = db.execute(
            """
            SELECT t.reference, t.passenger_name, t.seats, t.total_amount,
                   t.issued_at, r.route_code, r.origin, r.destination,
                   r.departure_time
            FROM tickets t
            JOIN routes r ON r.id = t.route_id
            WHERE t.passenger_name = ? AND t.passenger_phone = ?
            ORDER BY t.issued_at DESC, t.id DESC
            """,
            (lookup_name, lookup_phone),
        ).fetchall()

    return render_template(
        "tickets.html",
        search_origin=origin,
        search_destination=destination,
        results=results,
        lookup_name=lookup_name,
        lookup_phone=lookup_phone,
        bookings=bookings,
    )


# ---------------------------------------------------------------------------
# Customer actions
# ---------------------------------------------------------------------------

@app.route("/tickets/buy", methods=["POST"])
def buy_ticket():
    """Validate and record a ticket purchase, then redirect (PRG)."""
    db = get_db()

    try:
        route_id = int(request.form.get("route_id", 0))
        seats = int(request.form.get("seats", 0))
    except (TypeError, ValueError):
        return apology("Invalid booking payload.", 400)

    name = request.form.get("passenger_name", "").strip()
    phone = request.form.get("passenger_phone", "").strip()

    if not name or not phone:
        return apology("Passenger name and phone are required.", 400)
    if seats < 1:
        return apology("You must reserve at least one seat.", 400)

    route = db.execute(
        """
        SELECT r.*, v.capacity, v.status AS vehicle_status
        FROM routes r
        JOIN vehicles v ON v.id = r.vehicle_id
        WHERE r.id = ?
        """,
        (route_id,),
    ).fetchone()

    if route is None:
        return apology("That route no longer exists.", 404)
    if route["vehicle_status"] != "active":
        return apology("This route's vehicle is not in service.", 400)

    sold = db.execute(
        "SELECT COALESCE(SUM(seats), 0) AS s FROM tickets WHERE route_id = ?",
        (route_id,),
    ).fetchone()["s"]
    remaining = route["capacity"] - sold

    if seats > remaining:
        return apology(f"Only {remaining} seat(s) left on this departure.", 400)

    reference = "AEL-" + secrets.token_hex(3).upper()
    total = seats * route["base_fare"]

    db.execute(
        """
        INSERT INTO tickets (reference, route_id, passenger_name,
                             passenger_phone, seats, total_amount)
        VALUES (?, ?, ?, ?, ?, ?)
        """,
        (reference, route_id, name, phone, seats, total),
    )
    db.commit()

    flash(
        f"Booking {reference} confirmed: {seats} seat(s) for {name} "
        f"({route['origin']} to {route['destination']}).",
        "success",
    )
    return redirect(url_for("tickets"))


@app.route("/tickets/lookup", methods=["POST"])
def lookup_tickets():
    """Collect lookup credentials and forward them to the tickets page (PRG)."""
    name = request.form.get("lookup_name", "").strip()
    phone = request.form.get("lookup_phone", "").strip()

    if not name or not phone:
        flash("Enter both the name and the phone number used for the booking.",
              "error")
        return redirect(url_for("tickets"))

    return redirect(url_for("tickets", lookup_name=name, lookup_phone=phone))


# ---------------------------------------------------------------------------
# Administrator session
# ---------------------------------------------------------------------------

@app.route("/admin/login", methods=["GET", "POST"])
def admin_login():
    """Authenticate the single administrator account."""
    if is_admin():
        return redirect(url_for("admin_dashboard"))

    if request.method == "POST":
        username = request.form.get("username", "").strip()
        password = request.form.get("password", "")

        row = get_db().execute(
            "SELECT * FROM admins WHERE username = ?", (username,)
        ).fetchone()

        if row is None or not check_password_hash(row["password_hash"], password):
            return apology("Invalid credentials. Access denied.", 403)

        session.clear()
        session["admin_id"] = row["id"]
        session["admin_name"] = row["username"]
        flash(f"Welcome back, {row['username']}.", "success")
        return redirect(url_for("admin_dashboard"))

    return render_template(
        "admin_login.html",
        demo_user=ADMIN_USERNAME,
        demo_pass=ADMIN_PASSWORD,
    )


@app.route("/admin/logout")
@login_required
def admin_logout():
    """End the administrator session."""
    session.clear()
    flash("You have been signed out.", "info")
    return redirect(url_for("index"))


# ---------------------------------------------------------------------------
# Administrator dashboard and fleet management
# ---------------------------------------------------------------------------

@app.route("/admin")
@login_required
def admin_dashboard():
    """Admin overview: the dashboard template with the full audit trail."""
    db = get_db()
    stats = {
        "vehicles": db.execute("SELECT COUNT(*) AS n FROM vehicles").fetchone()["n"],
        "routes": db.execute("SELECT COUNT(*) AS n FROM routes").fetchone()["n"],
        "tickets": db.execute("SELECT COUNT(*) AS n FROM tickets").fetchone()["n"],
        "revenue": db.execute(
            "SELECT COALESCE(SUM(total_amount), 0) AS s FROM tickets"
        ).fetchone()["s"],
    }

    all_tickets = db.execute(
        """
        SELECT t.reference, t.passenger_name, t.passenger_phone, t.seats,
               t.total_amount, t.issued_at, r.route_code, r.origin, r.destination
        FROM tickets t
        JOIN routes r ON r.id = t.route_id
        ORDER BY t.issued_at DESC, t.id DESC
        """
    ).fetchall()

    return render_template(
        "index.html", stats=stats, recent=None, tickets_full=all_tickets,
        admin=True,
    )


@app.route("/admin/vehicles/add", methods=["POST"])
@login_required
def add_vehicle():
    """Register a new vehicle in the fleet."""
    db = get_db()
    model = request.form.get("model", "").strip()
    vehicle_type = request.form.get("vehicle_type", "").strip()
    status = request.form.get("status", "active").strip()

    try:
        capacity = int(request.form.get("capacity", "0").strip())
    except ValueError:
        return apology("Capacity must be a whole number.", 400)

    if not model:
        return apology("A vehicle model is required.", 400)
    if capacity < 1:
        return apology("Capacity must be at least 1 seat.", 400)
    if status not in ("active", "maintenance", "retired"):
        status = "active"

    try:
        db.execute(
            """
            INSERT INTO vehicles (fleet_code, model, vehicle_type, capacity, status)
            VALUES (?, ?, ?, ?, ?)
            """,
            (next_fleet_code(), model, vehicle_type or "Bus", capacity, status),
        )
        db.commit()
    except sqlite3.IntegrityError:
        return apology("A fleet code collision occurred; please retry.", 400)

    flash("Vehicle added to the fleet.", "success")
    return redirect(url_for("vehicles"))


@app.route("/admin/vehicles/<int:vehicle_id>/update", methods=["POST"])
@login_required
def update_vehicle(vehicle_id):
    """Update the details of an existing vehicle."""
    db = get_db()
    model = request.form.get("model", "").strip()
    vehicle_type = request.form.get("vehicle_type", "").strip()
    status = request.form.get("status", "active").strip()

    try:
        capacity = int(request.form.get("capacity", "0").strip())
    except ValueError:
        return apology("Capacity must be a whole number.", 400)

    if not model:
        return apology("A vehicle model is required.", 400)
    if capacity < 1:
        return apology("Capacity must be at least 1 seat.", 400)
    if status not in ("active", "maintenance", "retired"):
        status = "active"

    cursor = db.execute(
        """
        UPDATE vehicles
        SET model = ?, vehicle_type = ?, capacity = ?, status = ?
        WHERE id = ?
        """,
        (model, vehicle_type or "Bus", capacity, status, vehicle_id),
    )
    db.commit()
    if cursor.rowcount == 0:
        return apology("Vehicle not found.", 404)
    flash("Vehicle updated.", "success")
    return redirect(url_for("vehicles"))


@app.route("/admin/vehicles/<int:vehicle_id>/delete", methods=["POST"])
@login_required
def delete_vehicle(vehicle_id):
    """Retire a vehicle unless it still carries scheduled routes."""
    db = get_db()
    assigned = db.execute(
        "SELECT COUNT(*) AS n FROM routes WHERE vehicle_id = ?", (vehicle_id,)
    ).fetchone()["n"]
    if assigned > 0:
        return apology("Reassign or remove that vehicle's routes first.", 400)

    cursor = db.execute("DELETE FROM vehicles WHERE id = ?", (vehicle_id,))
    db.commit()
    if cursor.rowcount == 0:
        return apology("Vehicle not found.", 404)
    flash("Vehicle removed from the fleet.", "success")
    return redirect(url_for("vehicles"))


@app.route("/admin/routes/add", methods=["POST"])
@login_required
def add_route():
    """Schedule a new route on an active vehicle."""
    db = get_db()
    origin = request.form.get("origin", "").strip()
    destination = request.form.get("destination", "").strip()
    departure_time = request.form.get("departure_time", "").strip()

    try:
        base_fare = float(request.form.get("base_fare", "0").strip())
        vehicle_id = int(request.form.get("vehicle_id", "0").strip())
    except ValueError:
        return apology("Fare and vehicle must be valid numbers.", 400)

    if not origin or not destination or not departure_time:
        return apology("Origin, destination and departure time are required.", 400)
    if origin.lower() == destination.lower():
        return apology("Origin and destination must differ.", 400)
    if base_fare < 0:
        return apology("Fare cannot be negative.", 400)

    vehicle = db.execute(
        "SELECT * FROM vehicles WHERE id = ?", (vehicle_id,)
    ).fetchone()
    if vehicle is None:
        return apology("The selected vehicle does not exist.", 400)
    if vehicle["status"] != "active":
        return apology("Only active vehicles can carry routes.", 400)

    try:
        db.execute(
            """
            INSERT INTO routes (route_code, origin, destination, departure_time,
                                base_fare, vehicle_id)
            VALUES (?, ?, ?, ?, ?, ?)
            """,
            (make_route_code(origin, destination), origin, destination,
             departure_time, base_fare, vehicle_id),
        )
        db.commit()
    except sqlite3.IntegrityError:
        return apology("A route with this code already exists.", 400)

    flash("Route scheduled.", "success")
    return redirect(url_for("routes"))


@app.route("/admin/routes/<int:route_id>/update", methods=["POST"])
@login_required
def update_route(route_id):
    """Amend an existing route's details."""
    db = get_db()
    origin = request.form.get("origin", "").strip()
    destination = request.form.get("destination", "").strip()
    departure_time = request.form.get("departure_time", "").strip()

    try:
        base_fare = float(request.form.get("base_fare", "0").strip())
        vehicle_id = int(request.form.get("vehicle_id", "0").strip())
    except ValueError:
        return apology("Fare and vehicle must be valid numbers.", 400)

    if not origin or not destination or not departure_time:
        return apology("Origin, destination and departure time are required.", 400)
    if origin.lower() == destination.lower():
        return apology("Origin and destination must differ.", 400)
    if base_fare < 0:
        return apology("Fare cannot be negative.", 400)

    vehicle = db.execute(
        "SELECT * FROM vehicles WHERE id = ?", (vehicle_id,)
    ).fetchone()
    if vehicle is None:
        return apology("The selected vehicle does not exist.", 400)
    if vehicle["status"] != "active":
        return apology("Only active vehicles can carry routes.", 400)

    cursor = db.execute(
        """
        UPDATE routes
        SET origin = ?, destination = ?, departure_time = ?, base_fare = ?,
            vehicle_id = ?
        WHERE id = ?
        """,
        (origin, destination, departure_time, base_fare, vehicle_id, route_id),
    )
    db.commit()
    if cursor.rowcount == 0:
        return apology("Route not found.", 404)
    flash("Route updated.", "success")
    return redirect(url_for("routes"))


@app.route("/admin/routes/<int:route_id>/delete", methods=["POST"])
@login_required
def delete_route(route_id):
    """Delete a route once it no longer carries bookings."""
    db = get_db()
    sold = db.execute(
        "SELECT COUNT(*) AS n FROM tickets WHERE route_id = ?", (route_id,)
    ).fetchone()["n"]
    if sold > 0:
        return apology("Tickets exist for this route; it cannot be deleted.", 400)

    cursor = db.execute("DELETE FROM routes WHERE id = ?", (route_id,))
    db.commit()
    if cursor.rowcount == 0:
        return apology("Route not found.", 404)
    flash("Route removed.", "success")
    return redirect(url_for("routes"))


if __name__ == "__main__":
    app.run(debug=True)
