#!/usr/bin/env python3
# ==============================================================================
# AEL CS Encyclopedia™ - © Ayman Elmasry
# Week 7 SQL & AI: ael_fiftyville_forensics.py (Multi-Table Crime Investigation)
# ==============================================================================

import sqlite3

def setup_fiftyville_tables(conn: sqlite3.Connection):
    cursor = conn.cursor()
    cursor.executescript("""
        CREATE TABLE crime_scene_reports (id INTEGER PRIMARY KEY, year INTEGER, month INTEGER, day INTEGER, street TEXT, description TEXT);
        CREATE TABLE flights (id INTEGER PRIMARY KEY, origin_airport_id INTEGER, destination_airport_id INTEGER, year INTEGER, month INTEGER, day INTEGER, hour INTEGER, minute INTEGER);
        CREATE TABLE airports (id INTEGER PRIMARY KEY, abbreviation TEXT, full_name TEXT, city TEXT);
        CREATE TABLE passengers (flight_id INTEGER, passport_number TEXT);
        CREATE TABLE people (id INTEGER PRIMARY KEY, name TEXT, phone_number TEXT, passport_number TEXT, license_plate TEXT);
        CREATE TABLE atm_transactions (id INTEGER PRIMARY KEY, account_number INTEGER, year INTEGER, month INTEGER, day INTEGER, atm_location TEXT, transaction_type TEXT, amount INTEGER);
        CREATE TABLE bank_accounts (account_number INTEGER, person_id INTEGER);

        INSERT INTO crime_scene_reports VALUES (1, 2026, 7, 28, 'Humphrey Street', 'Theft took place at 10:15am. Three known witnesses.');

        INSERT INTO airports VALUES (1, 'CSF', 'Fiftyville Regional', 'Fiftyville');
        INSERT INTO airports VALUES (2, 'DXB', 'Dubai International', 'Dubai');
        INSERT INTO airports VALUES (3, 'LHR', 'Heathrow', 'London');

        INSERT INTO flights VALUES (36, 1, 2, 2026, 7, 29, 8, 20); -- Earliest flight next morning

        INSERT INTO people VALUES (501, 'Bruce', '(367) 555-5533', '5773159633', '432OG99');
        INSERT INTO people VALUES (502, 'Taylor', '(286) 555-6063', '1988161715', '1106N58');

        INSERT INTO passengers VALUES (36, '5773159633'); -- Bruce on flight to Dubai

        INSERT INTO bank_accounts VALUES (4901, 501);
        INSERT INTO atm_transactions VALUES (1001, 4901, 2026, 7, 28, 'Leggett Street', 'withdraw', 50);
    """)
    conn.commit()

def run_investigation(conn: sqlite3.Connection):
    cursor = conn.cursor()

    print("================================================================")
    print("        AEL SOVEREIGN FIFTYVILLE FORENSIC INVESTIGATOR")
    print("================================================================\n")

    print("[CRIME REPORT ANALYSIS]")
    cursor.execute("SELECT description FROM crime_scene_reports WHERE date(year||'-0'||month||'-'||day) = '2026-07-28' AND street = 'Humphrey Street'")
    print(f"  └── Incident: {cursor.fetchone()[0]}\n")

    print("[CROSS-EXAMINING ATM TRANSACTIONS & FLIGHT LOGS TO DUBAI (INTERSECT MATRIX)]")
    cursor.execute("""
        SELECT name FROM people
        JOIN bank_accounts ON people.id = bank_accounts.person_id
        JOIN atm_transactions ON bank_accounts.account_number = atm_transactions.account_number
        WHERE atm_transactions.year = 2026 AND atm_transactions.month = 7 AND atm_transactions.day = 28
        AND atm_transactions.atm_location = 'Leggett Street' AND atm_transactions.transaction_type = 'withdraw'

        INTERSECT

        SELECT name FROM people
        JOIN passengers ON people.passport_number = passengers.passport_number
        JOIN flights ON passengers.flight_id = flights.id
        WHERE flights.year = 2026 AND flights.month = 7 AND flights.day = 29
        AND flights.origin_airport_id = (SELECT id FROM airports WHERE city = 'Fiftyville')
        ORDER BY flights.hour, flights.minute LIMIT 1;
    """)
    thief = cursor.fetchone()[0]
    print(f"  └── [PRIME SUSPECT IDENTIFIED]: {thief}")

    cursor.execute("""
        SELECT city FROM airports WHERE id = (
            SELECT destination_airport_id FROM flights
            WHERE flights.year = 2026 AND flights.month = 7 AND flights.day = 29
            AND flights.origin_airport_id = (SELECT id FROM airports WHERE city = 'Fiftyville')
            ORDER BY flights.hour, flights.minute LIMIT 1
        )
    """)
    destination = cursor.fetchone()[0]
    print(f"  └── [ESCAPE FLIGHT DESTINATION]: {destination}\n")
    print("Forensic match closed successfully.")

def main():
    conn = sqlite3.connect(":memory:")
    setup_fiftyville_tables(conn)
    run_investigation(conn)
    conn.close()

if __name__ == "__main__":
    main()
