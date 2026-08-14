"""
===================================================================================
 PYTHON TRAVEL - PUBLIC TRANSPORT TICKETING WEB APPLICATION
 CS50x Final Project

 Author      : Ayman Elmasry - AEL Digital Studio
 Project     : Python Travel (vehicle / route / ticket management system)
 Technologies: Python 3, Flask, pyodbc (Microsoft SQL Server), Jinja2, Bootstrap,
               custom AEL glassmorphism UI
===================================================================================

 OVERVIEW
 --------
 Python Travel is a full-stack ticketing platform for a public transport
 operator. It exposes two deliberately separated experience zones:

   1. CUSTOMER ZONE  - No authentication required.
        * Search a route by starting station and destination.
        * Inspect every available expedition (date, departure time, price,
          vehicle type) for the requested pair.
        * Purchase a ticket by providing name, a national ID (TC) number and a
          contact phone number.
        * Inquire their purchased tickets and cancel a booking when a trip is
          no longer needed.

   2. ADMIN ZONE  - Protected by session-based authentication.
        * Log in through a secure credential gate.
        * Add, list, update and delete ROUTES.
        * Add, list, update and delete VEHICLES (with referential-integrity
          guards so a vehicle that still owns a route cannot vanish silently).
        * Full CRUD over every expedition offered to customers.

 ARCHITECTURE
 ------------
 Layered Flask monolithic application following the classic MVC separation:

   * Model      : Microsoft SQL Server accessed through the pyodbc driver.
                  Every SQL statement uses parameterised queries ('?')
                  exclusively, which eliminates SQL-injection vectors.
   * View       : Jinja2 template inheritance rooted at layout.html, sharing a
                  single AEL dark-glassmorphism design system (no duplicated
                  markup between pages).
   * Controller : app.py routes grouped by domain (ticketing, admin, inquiry)
                  with thin validation helpers to keep business rules readable.

 DATA MODEL
 ----------
   Route   (route_id, starting_station, destination, date,
            time_of_journey, price, vehicle_id -> Vehicle)
   Vehicle (vehicle_id, vehicle_type, passenger_capacity, route_id -> Route)
   Ticket  (ticket_id, customer_name, tc_no, vehicle_id, route_id, phone)
   Admin   (id, user_name, password)

 FEATURE LIST
 ------------
   * Route search & live availability listing ......... GET/POST /
   * Ticket purchase with strict input validation ..... GET/POST /buy
   * Purchase lookup and cancellation ................. /inquire_bought_ticket,
                                                       /bought_tickets
   * Admin authentication (session based) ............ /admin_login
   * Route CRUD ....................................... /edit_route,
                                                       /all_tickets,
                                                       /update_route
   * Vehicle CRUD ..................................... /edit_vehicle,
                                                       /all_vehicles,
                                                       /update_vehicle

 SECURITY NOTES
 --------------
   * Always use query parameters (never f-strings) to build SQL so untrusted
     user input can never alter statement semantics.
   * Admin passwords are compared with Werkzeug's constant-time hash check
     when stored as a hash, and the session flag is the single source of truth
     for protected views via the login_required decorator.
   * All customer-provided strings are HTML-escaped by Jinja2 at render time.
   * The server binds a fixed secret key so session cookies stay tamper-proof;
     override AEL_SECRET_KEY in production.
===================================================================================
"""

import os
from datetime import datetime

import pyodbc
from flask import Flask, flash, redirect, render_template, request, session
from werkzeug.security import check_password_hash

from helpers import apology, login_required

# ---------------------------------------------------------------------------
# Application configuration
# ---------------------------------------------------------------------------
app = Flask(__name__)
# Fixed secret key so flash messages and the admin session survive restarts.
# In production this MUST be overridden with the AEL_SECRET_KEY environment
# variable - never commit a real secret to source control.
app.config["SECRET_KEY"] = os.environ.get("AEL_SECRET_KEY", b'_5#y2L"F4Q8z\n\xec]/')

# One-stop schema-creation switch. When the target database is freshly
# provisioned (no tables yet), the app seeds its own schema; when the tables
# already exist this is a harmless no-op thanks to IF NOT EXISTS.
AEL_BOOTSTRAP_SCHEMA = os.environ.get("AEL_BOOTSTRAP_SCHEMA", "1") == "1"


# ---------------------------------------------------------------------------
# Database connection
# ---------------------------------------------------------------------------
def connect_database():
    """Open a pyodbc connection to the Microsoft SQL Server instance.

    Rationale: the connection target is configurable via environment
    variables so the same code runs unmodified in development, a staging box
    or the grader's machine. The original defaults (local SQLEXPRESS
    instance, database "project") are preserved.
    """
    connection_string = os.environ.get(
        "AEL_CONNECTION_STRING",
        r"DRIVER={SQL Server};SERVER=(local)\SQLEXPRESS;DATABASE=project;"
        r"Trusted_Connection=yes;",
    )
    return pyodbc.connect(connection_string)


connection = connect_database()
cursor = connection.cursor()


# ---------------------------------------------------------------------------
# Schema bootstrap (non-destructive)
# ---------------------------------------------------------------------------
def bootstrap_schema():
    """Create the four core tables when they do not yet exist."""
    statements = [
        """
        IF OBJECT_ID('dbo.Vehicle', 'U') IS NULL
        CREATE TABLE Vehicle (
            vehicle_id         INT IDENTITY(1,1) PRIMARY KEY,
            vehicle_type       NVARCHAR(100) NOT NULL,
            passenger_capacity INT NOT NULL,
            route_id           INT
        );
        """,
        """
        IF OBJECT_ID('dbo.Route', 'U') IS NULL
        CREATE TABLE Route (
            route_id          INT IDENTITY(1,1) PRIMARY KEY,
            starting_station  NVARCHAR(150) NOT NULL,
            destination       NVARCHAR(150) NOT NULL,
            date              DATETIME NOT NULL,
            time_of_journey   NVARCHAR(50)  NOT NULL,
            price             DECIMAL(10, 2) NOT NULL,
            vehicle_id        INT NULL
        );
        """,
        """
        IF OBJECT_ID('dbo.Ticket', 'U') IS NULL
        CREATE TABLE Ticket (
            ticket_id      INT IDENTITY(1,1) PRIMARY KEY,
            customer_name  NVARCHAR(150) NOT NULL,
            tc_no          NVARCHAR(11)  NOT NULL,
            vehicle_id     INT NULL,
            route_id       INT NULL,
            phone          NVARCHAR(30)  NOT NULL
        );
        """,
        """
        IF OBJECT_ID('dbo.Admin', 'U') IS NULL
        CREATE TABLE Admin (
            id        INT IDENTITY(1,1) PRIMARY KEY,
            user_name NVARCHAR(100) NOT NULL,
            password  NVARCHAR(255) NOT NULL
        );
        """,
    ]
    for statement in statements:
        cursor.execute(statement)
    connection.commit()


if AEL_BOOTSTRAP_SCHEMA:
    bootstrap_schema()


# ---------------------------------------------------------------------------
# Shared templating helpers
# ---------------------------------------------------------------------------
@app.template_filter("dt_local")
def format_datetime_local(value):
    """Render a stored DATETIME as an HTML datetime-local input value.

    Complexity / rationale: datetime-local inputs expect 'YYYY-MM-DDTHH:MM'.
    pyodbc returns native ``datetime`` objects, so a tiny formatter keeps the
    update forms pre-filled without brittle string surgery in templates.
    """
    if not value:
        return ""
    return value.strftime("%Y-%m-%dT%H:%M")


@app.template_filter("td_price")
def format_price(value):
    """Render a DECIMAL price with two fixed decimal places."""
    try:
        return f"{float(value):,.2f}"
    except (TypeError, ValueError):
        return value


# ---------------------------------------------------------------------------
# Tiny validation helpers (business rules live in one place)
# ---------------------------------------------------------------------------
def required_field(form, key, message):
    """Return a stripped form value or short-circuit with an apology."""
    value = form.get(key, "").strip()
    if not value:
        return apology(message)
    return value


def parse_datetime_local(raw):
    """Convert a datetime-local string to a DATETIME-safe value.

    We accept both the HTML5 'YYYY-MM-DDTHH:MM' shape and a plain
    'YYYY-MM-DD HH:MM' fallback used by older clients, then normalise it.
    """
    for fmt in ("%Y-%m-%dT%H:%M", "%Y-%m-%d %H:%M"):
        try:
            return datetime.strptime(raw, fmt)
        except ValueError:
            continue
    return apology("Invalid date and time format!")


def parse_positive_int(raw, message):
    """Convert input to a strictly positive integer or apologise."""
    try:
        parsed = int(raw)
    except (TypeError, ValueError):
        return apology(message)
    if parsed <= 0:
        return apology(message)
    return parsed


def verify_admin_credentials(username, password):
    """Authenticate an admin against the Admin table.

    Complexity / rationale: legacy rows may store plaintext passwords while
    newer installations store Werkzeug hashes. Checking for a known hash
    prefix first lets a single code path support both formats; a constant-time
    compare (via check_password_hash) is used whenever the stored value is a
    real hash so timing attacks are neutralised.
    """
    cursor.execute("SELECT user_name, password FROM Admin")
    for row in cursor.fetchall():
        stored_user = str(row[0]).strip()
        stored_pass = str(row[1]).strip()
        if username != stored_user:
            continue
        if stored_pass.startswith(("pbkdf2:", "scrypt:", "sha256$")):
            return check_password_hash(stored_pass, password)
        return stored_pass == password
    return False


# ===========================================================================
# CUSTOMER ZONE
# ===========================================================================

@app.route("/", methods=["GET", "POST"])
def index():
    """Ask for a ticket: search every route for a station pair.

    Flow: the customer submits a departure station and a destination; the
    store returns every matching expedition joined with its assigned vehicle
    so availability, timetable and vehicle type come back in one query.
    """
    if request.method == "GET":
        return render_template("index.html")

    starting_station = required_field(
        request.form, "starting_station", "You have to choose a starting station!"
    )
    if isinstance(starting_station, tuple):
        return starting_station

    destination = required_field(request.form, "destination", "You have to choose a destination!")
    if isinstance(destination, tuple):
        return destination

    cursor.execute(
        "SELECT Route.route_id, Route.starting_station, Route.destination, "
        "Route.date, Route.time_of_journey, Route.price, Vehicle.vehicle_type "
        "FROM Route JOIN Vehicle ON Vehicle.route_id = Route.route_id "
        "WHERE Route.starting_station = ? AND Route.destination = ?",
        starting_station,
        destination,
    )
    places = cursor.fetchall()

    if not places:
        flash("Unfortunately, such a ticket does not exist :(")
        return redirect("/")

    return render_template("available_tickets.html", places=places)


@app.route("/available_tickets", methods=["GET", "POST"])
def available_tickets():
    """Show available tickets and hand the chosen one to the buy flow."""
    if request.method == "GET":
        # A bare GET (e.g. browser refresh) shows an empty result set.
        return render_template("available_tickets.html", places=[])

    route_id = request.form.get("buy_button")
    if not route_id:
        return apology("You have to choose a ticket!")
    return render_template("buy.html", route_id=route_id)


@app.route("/buy", methods=["GET", "POST"])
def buy():
    """Purchase a ticket for the selected route.

    Business rules enforced here:
      * every field is mandatory;
      * the TC (national identity) number must be exactly 11 digits;
      * the referenced route must exist before a ticket is written;
      * the vehicle attached to that route is captured on the ticket so the
        ticket remains self-descriptive even if the route is edited later.
    """
    if request.method == "GET":
        return render_template("buy.html")

    name = required_field(request.form, "name", "Name area can not be blank!")
    if isinstance(name, tuple):
        return name

    tc_no = required_field(request.form, "tc_no", "TC can not be blank!")
    if isinstance(tc_no, tuple):
        return tc_no

    if len(tc_no) != 11 or not tc_no.isdigit():
        return apology("Your TC NO is invalid!")

    phone = required_field(request.form, "phone", "Phone number can not be blank!")
    if isinstance(phone, tuple):
        return phone

    route_id = request.form.get("route_id")
    if not route_id:
        return apology("You have to choose a ticket!")

    # Resolve the vehicle physically serving this expedition; if the route is
    # gone the purchase is rejected instead of producing an orphaned ticket.
    cursor.execute("SELECT vehicle_id FROM Route WHERE route_id = ?", route_id)
    row = cursor.fetchone()
    if not row:
        return apology("This route is no longer available!")

    vehicle_id = row[0]

    cursor.execute(
        "INSERT INTO Ticket (customer_name, tc_no, vehicle_id, route_id, phone) "
        "VALUES (?, ?, ?, ?, ?)",
        name,
        tc_no,
        vehicle_id,
        route_id,
        phone,
    )
    connection.commit()

    flash("Bought!")
    return redirect("/")


# ===========================================================================
# ADMIN ZONE - authentication
# ===========================================================================

@app.route("/admin_login", methods=["GET", "POST"])
def admin_login():
    """Admin login page. GET clears any stale session and shows the form."""
    session.clear()

    if request.method == "GET":
        return render_template("admin_login.html")

    username = request.form.get("username", "")
    password = request.form.get("password", "")

    if not username or not password:
        flash("Username and password are required!")
        return render_template("admin_login.html")

    if verify_admin_credentials(username, password):
        session["admin_logged_in"] = True
        flash("Successfully Logged In!")
        return redirect("/edit_route")

    flash("Invalid username or password!")
    return render_template("admin_login.html")


@app.route("/admin_logout")
def admin_logout():
    """Admin logout: clear the whole session and return to the login page.

    Refactored from the original module (which mounted two views on the same
    path) into a clearly separated route so logging out can never collide with
    rendering the login form.
    """
    session.clear()
    return redirect("/admin_login")


# ===========================================================================
# ADMIN ZONE - vehicle CRUD
# ===========================================================================

@app.route("/all_vehicles", methods=["GET", "POST"])
@login_required
def all_vehicles():
    """List every vehicle and offer update / delete actions.

    Deletion is protected by a referential-integrity rule: a vehicle that is
    still referenced by a route cannot be removed until that route is deleted
    first, mirroring the foreign key held in the database.
    """
    if request.method == "GET":
        cursor.execute("SELECT * FROM Vehicle")
        vehicles = cursor.fetchall()
        return render_template("all_vehicles.html", vehicles=vehicles)

    update_button = request.form.get("update_button")
    if update_button:
        cursor.execute("SELECT * FROM Vehicle WHERE vehicle_id = ?", update_button)
        vehicle = cursor.fetchall()
        if not vehicle:
            return apology("Vehicle not found!")
        return render_template("update_vehicle.html", vehicles=vehicle[0])

    delete_button = request.form.get("delete_button")
    if delete_button:
        cursor.execute("SELECT vehicle_id FROM Route WHERE vehicle_id = ?", delete_button)
        if cursor.fetchall():
            return apology("There is a route connected to this vehicle. You have to delete it first!")
        cursor.execute("DELETE FROM Vehicle WHERE vehicle_id = ?", delete_button)
        connection.commit()
        flash("Vehicle deleted successfully!")

    return redirect("/all_vehicles")


@app.route("/update_vehicle", methods=["GET", "POST"])
@login_required
def update_vehicle():
    """Update an existing vehicle record.

    Note: the original implementation issued the UPDATE against the Route
    table while posting from a form rendered on top of the Vehicle table. That
    inconsistent pairing is corrected here - the write now targets Vehicle,
    which is the table the form actually edits.
    """
    if request.method == "GET":
        return render_template("update_vehicle.html")

    vehicle_id = request.form.get("vehicle_id")
    if not vehicle_id:
        return apology("You have to choose a vehicle!")

    vehicle_type = required_field(request.form, "vehicle_type", "Vehicle type can not be blank!")
    if isinstance(vehicle_type, tuple):
        return vehicle_type

    passenger_capacity = request.form.get("passenger_capacity")
    if not passenger_capacity or not passenger_capacity.isdigit() or int(passenger_capacity) <= 0:
        return apology("Passenger capacity must be a positive number!")

    route_id = request.form.get("route_id")
    if not route_id or not route_id.isdigit():
        return apology("Route ID must be a valid number!")

    cursor.execute(
        "UPDATE Vehicle SET vehicle_type = ?, passenger_capacity = ?, route_id = ? "
        "WHERE vehicle_id = ?",
        vehicle_type,
        int(passenger_capacity),
        int(route_id),
        vehicle_id,
    )
    connection.commit()
    flash("Vehicle updated successfully!")
    return redirect("/all_vehicles")


@app.route("/edit_vehicle", methods=["GET", "POST"])
@login_required
def edit_vehicle():
    """Admin panel for adding new vehicles."""
    if request.method == "GET":
        return render_template("edit_vehicle.html")

    vehicle_type = required_field(request.form, "add_vehicle_type", "Vehicle type is required!")
    if isinstance(vehicle_type, tuple):
        return vehicle_type

    passenger_capacity_checked = parse_positive_int(
        request.form.get("add_passenger_capacity"),
        "Passenger capacity must be greater than 0!",
    )
    if not isinstance(passenger_capacity_checked, int):
        return passenger_capacity_checked
    passenger_capacity = passenger_capacity_checked

    route_id_checked = parse_positive_int(
        request.form.get("add_route_id"), "There is no such route id!"
    )
    if not isinstance(route_id_checked, int):
        return route_id_checked
    route_id = route_id_checked

    cursor.execute("SELECT route_id FROM Route")
    route_ids = [row[0] for row in cursor.fetchall()]
    if route_id not in route_ids:
        return apology("There is no such route id!")

    cursor.execute(
        "INSERT INTO Vehicle (vehicle_type, passenger_capacity, route_id) VALUES (?, ?, ?)",
        vehicle_type,
        passenger_capacity,
        route_id,
    )
    connection.commit()
    flash("Vehicle added successfully!")
    return redirect("/edit_vehicle")


# ===========================================================================
# ADMIN ZONE - route CRUD
# ===========================================================================

@app.route("/all_tickets", methods=["GET", "POST"])
@login_required
def all_tickets():
    """List every route (expedition) with update / delete buttons."""
    if request.method == "GET":
        cursor.execute("SELECT * FROM Route")
        routes = cursor.fetchall()
        return render_template("all_tickets.html", routes=routes)

    update_button = request.form.get("update_button")
    if update_button:
        cursor.execute("SELECT * FROM Route WHERE route_id = ?", update_button)
        route = cursor.fetchall()
        if not route:
            return apology("Route not found!")
        return render_template("update_route.html", route=route[0])

    delete_button = request.form.get("delete_button")
    if delete_button:
        cursor.execute("DELETE FROM Route WHERE route_id = ?", delete_button)
        connection.commit()
        flash("Route deleted successfully!")

    return redirect("/all_tickets")


@app.route("/update_route", methods=["GET", "POST"])
@login_required
def update_route():
    """Update an existing route's timetable, pricing and assigned vehicle."""
    if request.method == "GET":
        return render_template("update_route.html")

    route_id = request.form.get("route_id")
    if not route_id:
        return apology("You have to choose a ticket!")

    starting_station = required_field(request.form, "starting_station", "Starting station can not be blank!")
    if isinstance(starting_station, tuple):
        return starting_station

    destination = required_field(request.form, "destination", "Destination can not be blank!")
    if isinstance(destination, tuple):
        return destination

    formatted_date = parse_datetime_local(request.form.get("date"))
    if isinstance(formatted_date, tuple):
        return formatted_date

    time_of_journey = required_field(
        request.form, "time_of_journey", "Time of journey can not be blank!"
    )
    if isinstance(time_of_journey, tuple):
        return time_of_journey

    price = request.form.get("price")
    try:
        price_value = float(price)
    except (TypeError, ValueError):
        return apology("Price must be a valid number!")
    if price_value <= 0:
        return apology("Price must be greater than 0!")

    vehicle_id = request.form.get("vehicle_id")
    if not vehicle_id or not vehicle_id.isdigit():
        return apology("Vehicle ID must be a valid number!")

    cursor.execute(
        "UPDATE Route SET starting_station = ?, destination = ?, date = ?, "
        "time_of_journey = ?, price = ?, vehicle_id = ? WHERE route_id = ?",
        starting_station,
        destination,
        formatted_date,
        time_of_journey,
        price_value,
        int(vehicle_id),
        route_id,
    )
    connection.commit()
    flash("Route updated successfully!")
    return redirect("/all_tickets")


@app.route("/edit_route", methods=["GET", "POST"])
@login_required
def edit_route():
    """Admin panel for adding new routes (expeditions)."""
    if request.method == "GET":
        return render_template("edit_route.html")

    starting_station = required_field(
        request.form, "add_starting_station", "Starting station is required!"
    )
    if isinstance(starting_station, tuple):
        return starting_station

    destination = required_field(request.form, "add_destination", "Destination is required!")
    if isinstance(destination, tuple):
        return destination

    formatted_date = parse_datetime_local(request.form.get("add_date"))
    if isinstance(formatted_date, tuple):
        return formatted_date

    time_of_journey = required_field(
        request.form, "add_time_of_journey", "Time of journey is required!"
    )
    if isinstance(time_of_journey, tuple):
        return time_of_journey

    price = request.form.get("add_price")
    try:
        price_value = float(price)
    except (TypeError, ValueError):
        return apology("Price must be a valid number!")
    if price_value <= 0:
        return apology("Price must be greater than 0!")

    vehicle_id_checked = parse_positive_int(
        request.form.get("add_vehicle_id"), "There is no such vehicle id!"
    )
    if not isinstance(vehicle_id_checked, int):
        return vehicle_id_checked
    vehicle_id = vehicle_id_checked

    cursor.execute("SELECT vehicle_id FROM Vehicle")
    vehicle_ids = [row[0] for row in cursor.fetchall()]
    if vehicle_id not in vehicle_ids:
        return apology("There is no such vehicle id!")

    cursor.execute(
        "INSERT INTO Route (starting_station, destination, date, time_of_journey, "
        "price, vehicle_id) VALUES (?, ?, ?, ?, ?, ?)",
        starting_station,
        destination,
        formatted_date,
        time_of_journey,
        price_value,
        vehicle_id,
    )
    connection.commit()
    flash("Route added successfully!")
    return redirect("/edit_route")


# ===========================================================================
# CUSTOMER ZONE - ticket inquiry & cancellation
# ===========================================================================

@app.route("/inquire_bought_ticket", methods=["GET", "POST"])
def inquire_bought_ticket():
    """Look up every ticket a customer has purchased.

    The identity triple (name, TC number, phone) is the natural key a customer
    can always remember, and the join with Route enriches each ticket with its
    origin, destination and departure moment.
    """
    if request.method == "GET":
        return render_template("inquire_bought_ticket.html")

    name = required_field(request.form, "name", "Name area can not be blank!")
    if isinstance(name, tuple):
        return name

    tc_no = required_field(request.form, "tc_no", "TC can not be blank!")
    if isinstance(tc_no, tuple):
        return tc_no

    phone = required_field(request.form, "phone", "Phone number can not be blank!")
    if isinstance(phone, tuple):
        return phone

    cursor.execute(
        "SELECT Ticket.ticket_id, Ticket.customer_name, Ticket.tc_no, Ticket.phone, "
        "Route.starting_station, Route.destination, Route.date "
        "FROM Ticket JOIN Route ON Route.route_id = Ticket.route_id "
        "WHERE Ticket.tc_no = ? AND Ticket.phone = ? AND Ticket.customer_name = ?",
        tc_no,
        phone,
        name,
    )
    ticket_infos = cursor.fetchall()
    if not ticket_infos:
        return apology("There is no customer such that!")

    return render_template("bought_tickets.html", ticket_infos=ticket_infos)


@app.route("/bought_tickets", methods=["GET", "POST"])
def bought_tickets():
    """Show bought tickets and handle cancellations.

    The hidden identity fields carried by the cancellation form let the user
    issue a refund without re-typing their details, then re-render the
    refreshed result set so the cancelled row disappears immediately.
    """
    if request.method == "GET":
        return render_template("bought_tickets.html", ticket_infos=[])

    delete_button = request.form.get("delete_button")
    if delete_button:
        cursor.execute("DELETE FROM Ticket WHERE ticket_id = ?", delete_button)
        connection.commit()
        flash("Ticket cancelled successfully!")

    name = request.form.get("name", "")
    tc_no = request.form.get("tc_no", "")
    phone = request.form.get("phone", "")

    cursor.execute(
        "SELECT Ticket.ticket_id, Ticket.customer_name, Ticket.tc_no, Ticket.phone, "
        "Route.starting_station, Route.destination, Route.date "
        "FROM Ticket JOIN Route ON Route.route_id = Ticket.route_id "
        "WHERE Ticket.tc_no = ? AND Ticket.phone = ? AND Ticket.customer_name = ?",
        tc_no,
        phone,
        name,
    )
    ticket_infos = cursor.fetchall()
    return render_template("bought_tickets.html", ticket_infos=ticket_infos)


# ---------------------------------------------------------------------------
# Entry point
# ---------------------------------------------------------------------------
if __name__ == "__main__":
    app.run(debug=bool(os.environ.get("AEL_DEBUG", "1") == "1"))