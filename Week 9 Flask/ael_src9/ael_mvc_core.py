#!/usr/bin/env python3
"""
===================================================================================
 🏛️ AEL SOVEREIGN SOURCE VAULT: ael_src9
    FLASK MVC MASTER CONTROLLER & SQLITE TELEMETRY ENGINE
===================================================================================
 © Ayman Elmasry — AEL Digital Studio™ · All Rights Reserved.
 Built for Harvard CS50x 2026-2027
===================================================================================
"""

from flask import Flask, render_template, request, redirect, session
import sqlite3
import os

app = Flask(__name__)
app.secret_key = "AEL_SOVEREIGN_CRYPTOGRAPHIC_MASTER_KEY_2026_2027"

DB_FILE = "ael_matrix.db"

def init_db():
    """Initializes the local SQLite master vault if it doesn't exist."""
    conn = sqlite3.connect(DB_FILE)
    c = conn.cursor()
    c.execute('''
        CREATE TABLE IF NOT EXISTS sovereign_telemetry (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            directive TEXT NOT NULL,
            payload TEXT NOT NULL,
            timestamp DATETIME DEFAULT CURRENT_TIMESTAMP
        )
    ''')
    
    # Insert initial pre-verified sovereign seed data if table is empty
    c.execute("SELECT COUNT(*) FROM sovereign_telemetry")
    if c.fetchone()[0] == 0:
        initial_seeds = [
            ("MANDATE_01", "Flask MVC Thread-Local Proxy Initialized"),
            ("MANDATE_02", "Jinja2 Dynamic Dark-Mode Glassmorphism Markup Rendered"),
            ("MANDATE_03", "Cryptographic Set-Cookie Session Store Guard Activated")
        ]
        c.executemany("INSERT INTO sovereign_telemetry (directive, payload) VALUES (?, ?)", initial_seeds)
        conn.commit()
    conn.close()

# Initialize the vault table at startup
init_db()

@app.route("/", methods=["GET", "POST"])
def master_matrix():
    """
    Master Controller handling both GET request table injection and POST transaction updates.
    Demonstrates elite MVC decoupling between SQLite database records and Jinja2 markup.
    """
    if request.method == "POST":
        directive = request.form.get("directive", "").strip()
        payload = request.form.get("payload", "").strip()
        
        if directive and payload:
            conn = sqlite3.connect(DB_FILE)
            c = conn.cursor()
            c.execute("INSERT INTO sovereign_telemetry (directive, payload) VALUES (?, ?)", (directive, payload))
            conn.commit()
            conn.close()
            session["last_action"] = f"Appended Directive {directive} successfully."
            
        return redirect("/")
        
    else:
        conn = sqlite3.connect(DB_FILE)
        conn.row_factory = sqlite3.Row
        c = conn.cursor()
        c.execute("SELECT * FROM sovereign_telemetry ORDER BY id DESC")
        records = c.fetchall()
        conn.close()
        
        last_action = session.get("last_action", "System Active. Enter new telemetry directive below.")
        
        return render_template("ael_dashboard.html", items=records, status_message=last_action)

@app.route("/reset")
def reset_vault():
    """Demolishes current session state and clears database telemetry."""
    session.clear()
    if os.path.exists(DB_FILE):
        os.remove(DB_FILE)
    init_db()
    session["last_action"] = "Sovereign Master Vault reset to default state successfully."
    return redirect("/")

if __name__ == "__main__":
    # Runs the standalone development WSGI engine
    app.run(debug=True, host="0.0.0.0", port=5000)
