from flask import Flask, flash, redirect, render_template, request, session
import pyodbc
from werkzeug.security import check_password_hash, generate_password_hash
from datetime import datetime
from helpers import apology, login_required

app = Flask(__name__)
app.secret_key = b'_5#y2L"F4Q8z\n\xec]/'  # to use the flash message

# database connection
connection_string = (
    r'DRIVER={SQL Server};'
    r'SERVER=(local)\SQLEXPRESS;'  # YOUR SERVER NAME
    r'DATABASE=project;'  # YOUR DATABASE NAME
    r'Trusted_Connection=yes;'
)
connection = pyodbc.connect(connection_string)
db = connection.cursor()


@app.route("/", methods=["GET", "POST"])
def index():
    """Ask for Ticket"""

    if request.method == "GET":
        return render_template("index.html")
    else:
        starting_station = request.form.get("starting_station")
        if not starting_station:
            return apology("You have to choose a starting station!")
        destination = request.form.get("destination")
        if not destination:
            return apology("You have to choose a destination!")
        db.execute("SELECT Route.route_id, Route.starting_station, Route.destination, "
                   "Route.date, Route.time_of_journey, Route.price, Vehicle.vehicle_type "
                   "FROM Route JOIN Vehicle ON Vehicle.route_id = Route.route_id "
                   "WHERE Route.starting_station = ? AND Route.destination = ?", starting_station, destination)

        places = db.fetchall()
        if not places:
            flash("Unfortunately, such a ticket does not exist :(")
            return redirect("/")
        return render_template("available_tickets.html", places=places)


@app.route("/available_tickets", methods=["GET", "POST"])
def available_tickets():
    """Show Available Tickets"""

    if request.method == "GET":
        return render_template("available_tickets.html")
    else:
        route_id = request.form.get("buy_button")
        return render_template("buy.html", route_id=route_id)


@app.route("/buy", methods=["GET", "POST"])
def buy():
    """Buy Ticket"""

    if request.method == "GET":
        return render_template("buy.html")
    else:
        name = request.form.get("name")
        if not name:
            return apology("Name area can not be blank!")
        tc_no = request.form.get("tc_no")
        if not tc_no:
            return apology("TC can not be blank!")
        if len(tc_no) != 11:
            return apology("Your TC NO is invalid!")
        phone = request.form.get("phone")
        if not phone:
            return apology("Phone number can not be blank!")
        route_id = request.form.get("route_id")
        if not route_id:
            return apology("You have to choose a ticket!")
        db.execute("SELECT vehicle_id FROM Route WHERE route_id = ?", route_id)
        vehicle_id = db.fetchall()[0][0]
        db.execute("INSERT INTO Ticket (customer_name, tc_no, vehicle_id, route_id, phone) VALUES (?, ?, ?, ?, ?)",
                   name, tc_no, vehicle_id, route_id, phone)
        db.commit()
        flash("Bought!")
        return redirect("/")


@app.route("/admin_login", methods=["GET", "POST"])
def admin_login():
    """Admin Login page"""

    session.clear()

    if request.method == "POST":
        username = request.form.get("username")
        password = request.form.get("password")

        db.execute("SELECT user_name, password FROM Admin")
        admin_info = db.fetchall()

        for row in admin_info:
            admin_username = row[0].strip()
            admin_password_hash = generate_password_hash(row[1].strip())  # hash the password

            if username == admin_username and check_password_hash(admin_password_hash, password):
                session["admin_logged_in"] = True
                flash("Successfully Logged In!")
                return redirect("/edit_route")

        flash("Invalid username or password!")
        return render_template("admin_login.html")
    else:
        return render_template("admin_login.html")


@app.route("/admin_login")
def admin_logout():
    """Admin logout"""

    session.clear()
    return redirect("/admin_login")


@app.route("/all_vehicles", methods=["GET", "POST"])
@login_required
def all_vehicles():
    """Show All Vehicles To Delete And Update"""

    if request.method == "GET":
        db.execute("SELECT * FROM Vehicle")
        vehicle = db.fetchall()
        return render_template("all_vehicles.html", vehicles=vehicle)
    else:
        update_button = request.form.get("update_button")
        if update_button:
            vehicle_id = request.form.get("update_button")
            db.execute("SELECT * FROM Vehicle WHERE vehicle_id = ?", vehicle_id)
            vehicle = db.fetchall()
            return render_template("update_vehicle.html", vehicles=vehicle[0])
        delete_button = request.form.get("delete_button")
        if delete_button:
            db.execute("SELECT vehicle_id FROM Route WHERE vehicle_id = ?", delete_button)
            vehicle = db.fetchall()
            if vehicle:
                return apology("There is a route connected to this vehicle. You have to delete it first!")
            db.execute("DELETE FROM Vehicle WHERE vehicle_id = ?", delete_button)
            db.commit()
            flash("Vehicle deleted successfully!")
        return redirect("/all_vehicles")


@app.route("/update_vehicle", methods=["GET", "POST"])
@login_required
def update_vehicle():
    """Update Vehicle"""

    if request.method == "GET":
        return render_template("update_vehicle.html")
    else:
        vehicle_id = request.form.get("vehicle_id")
        vehicle_type = request.form.get("vehicle_type")
        passenger_capacity = request.form.get("passenger_capacity")
        route_id = request.form.get("route_id")

        db.execute("UPDATE Route SET vehicle_type = ?, passenger_capacity = ?, route_id = ? WHERE vehicle_id = ?",
                   vehicle_type, passenger_capacity, route_id, vehicle_id)
        db.commit()

        return redirect("/all_tickets")


@app.route("/all_tickets", methods=["GET", "POST"])
@login_required
def all_tickets():
    """Show All Tickets To Delete And Update"""

    if request.method == "GET":
        db.execute("SELECT * from Route")
        routes = db.fetchall()
        return render_template("all_tickets.html", routes=routes)
    else:
        update_button = request.form.get("update_button")
        if update_button:
            route_id = request.form.get("update_button")
            db.execute("SELECT * FROM Route WHERE route_id = ?", route_id)
            route = db.fetchall()
            return render_template("update_route.html", route=route[0])
        delete_button = request.form.get("delete_button")
        if delete_button:
            db.execute("DELETE FROM Route WHERE route_id = ?", delete_button)
            db.commit()
            flash("Route deleted successfully!")
        return redirect("/all_tickets")


@app.route("/update_route", methods=["GET", "POST"])
@login_required
def update_route():
    """Update Route"""

    if request.method == "GET":
        return render_template("update_route.html")
    else:
        route_id = request.form.get("route_id")
        starting_station = request.form.get("starting_station")
        destination = request.form.get("destination")
        date = request.form.get("date")
        formatted_date = datetime.strptime(date, "%Y-%m-%dT%H:%M")
        time_of_journey = request.form.get("time_of_journey")
        price = request.form.get("price")
        vehicle_id = request.form.get("vehicle_id")

        db.execute("UPDATE Route SET starting_station = ?, destination = ?, date = ?, "
                   "time_of_journey = ?, price = ?, vehicle_id = ? WHERE route_id = ?", starting_station, destination,
                   formatted_date, time_of_journey, price, vehicle_id, route_id)
        db.commit()

        return redirect("/all_tickets")


@app.route('/edit_route', methods=["GET", "POST"])
@login_required
def edit_route():
    """Edit Routes"""

    if request.method == "GET":
        return render_template("edit_route.html")
    else:
        starting_station = request.form['add_starting_station']
        destination = request.form['add_destination']
        date = request.form['add_date']
        formatted_date = datetime.strptime(date, "%Y-%m-%dT%H:%M")
        time_of_journey = request.form['add_time_of_journey']
        price = int(request.form['add_price'])
        if price <= 0:
            return apology("Price must be greater than 0!")

        vehicle_id = int(request.form['add_vehicle_id'])
        db.execute("SELECT vehicle_id FROM Vehicle")
        vehicle_ids = [row[0] for row in db.fetchall()]
        if vehicle_id not in vehicle_ids:
            return apology("There is no such vehicle id!")

        query = "INSERT INTO Route (starting_station, destination, date, time_of_journey, price, vehicle_id) " \
                "VALUES (?, ?, ?, ?, ?, ?)"

        db.execute(query, (starting_station, destination, formatted_date, time_of_journey, price, vehicle_id))
        db.commit()

        flash("Route added successfully!")
        return redirect('/edit_route')


@app.route('/edit_vehicle', methods=["GET", "POST"])
@login_required
def edit_vehicle():
    """Edit Vehicles"""

    if request.method == "GET":
        return render_template("edit_vehicle.html")
    else:
        vehicle_type = request.form['add_vehicle_type']
        passenger_capacity = int(request.form['add_passenger_capacity'])
        if passenger_capacity <= 5:
            return apology("passenger capacity must be greater than 5!")

        route_id = int(request.form['add_route_id'])
        db.execute("SELECT route_id FROM Route")
        route_ids = [row[0] for row in db.fetchall()]
        if route_id not in route_ids:
            return apology("There is no such route id!")

        query = "INSERT INTO Vehicle (vehicle_type, passenger_capacity, route_id) VALUES (?, ?, ?)"

        db.execute(query, (vehicle_type, passenger_capacity, route_id))
        db.commit()

        flash("Vehicle added successfully!")
        return redirect('/edit_vehicle')


@app.route("/inquire_bought_ticket", methods=["GET", "POST"])
def inquire_bought_ticket():
    """Inquire Bought Tickets"""

    if request.method == "GET":
        return render_template("inquire_bought_ticket.html")
    else:
        name = request.form.get("name")
        if not name:
            return apology("Name area can not be blank!")
        tc_no = request.form.get("tc_no")
        if not tc_no:
            return apology("TC can not be blank!")
        phone = request.form.get("phone")
        if not phone:
            return apology("Phone number can not be blank!")

        db.execute("SELECT Ticket.ticket_id, Ticket.customer_name, Ticket.tc_no, Ticket.phone, "
                   "Route.starting_station, Route.destination, Route.date "
                   "FROM Ticket JOIN Route ON Route.route_id = Ticket.route_id "
                   "WHERE Ticket.tc_no = ? AND Ticket.phone = ? AND Ticket.customer_name = ?", tc_no, phone, name)
        ticket_infos = db.fetchall()
        if not ticket_infos:
            return apology("There is no customer such that!")

        return render_template("bought_tickets.html", ticket_infos=ticket_infos)


@app.route("/bought_tickets", methods=["GET", "POST"])
def bought_tickets():
    """Show Bought Tickets"""

    if request.method == "GET":
        return render_template("bought_tickets.html")
    else:
        delete_button = request.form.get("delete_button")
        if delete_button:
            db.execute("DELETE FROM Ticket WHERE ticket_id = ?", delete_button)
            db.commit()
            flash("Ticket cancelled successfully!")

        name = request.form.get("name")
        tc_no = request.form.get("tc_no")
        phone = request.form.get("phone")

        db.execute("SELECT Ticket.ticket_id, Ticket.customer_name, Ticket.tc_no, Ticket.phone, "
                   "Route.starting_station, Route.destination, Route.date "
                   "FROM Ticket JOIN Route ON Route.route_id = Ticket.route_id "
                   "WHERE Ticket.tc_no = ? AND Ticket.phone = ? AND Ticket.customer_name = ?", tc_no, phone, name)
        ticket_infos = db.fetchall()

        return render_template("bought_tickets.html", ticket_infos=ticket_infos)


if __name__ == '__main__':
    app.debug = True
    app.run()

    db.close()
    connection.close()
