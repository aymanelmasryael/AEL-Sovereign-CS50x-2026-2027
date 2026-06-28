#!/usr/bin/env python3
# ==============================================================================
# AEL CS Encyclopedia™ - © Ayman Elmasry
# Week 7 SQL & AI: ael_imdb_matrix.py (IMDb Cinematic Relational Query Engine)
# ==============================================================================

import sqlite3
import sys

def build_sample_db(conn: sqlite3.Connection):
    """
    Constructs an in-memory IMDb relational schema mimicking the CS50x movies database.
    """
    cursor = conn.cursor()
    cursor.executescript("""
        CREATE TABLE movies (id INTEGER PRIMARY KEY, title TEXT, year INTEGER);
        CREATE TABLE stars (movie_id INTEGER, person_id INTEGER);
        CREATE TABLE people (id INTEGER PRIMARY KEY, name TEXT, birth INTEGER);
        CREATE TABLE ratings (movie_id INTEGER, rating REAL, votes INTEGER);

        INSERT INTO movies (id, title, year) VALUES (1, 'Inception', 2010);
        INSERT INTO movies (id, title, year) VALUES (2, 'The Dark Knight', 2008);
        INSERT INTO movies (id, title, year) VALUES (3, 'Interstellar', 2014);

        INSERT INTO people (id, name, birth) VALUES (100, 'Christopher Nolan', 1970);
        INSERT INTO people (id, name, birth) VALUES (101, 'Christian Bale', 1974);
        INSERT INTO people (id, name, birth) VALUES (102, 'Leonardo DiCaprio', 1974);
        INSERT INTO people (id, name, birth) VALUES (103, 'Matthew McConaughey', 1969);

        INSERT INTO stars (movie_id, person_id) VALUES (1, 102);
        INSERT INTO stars (movie_id, person_id) VALUES (2, 101);
        INSERT INTO stars (movie_id, person_id) VALUES (3, 103);

        INSERT INTO ratings (movie_id, rating, votes) VALUES (1, 8.8, 2200000);
        INSERT INTO ratings (movie_id, rating, votes) VALUES (2, 9.0, 2600000);
        INSERT INTO ratings (movie_id, rating, votes) VALUES (3, 8.6, 1800000);
    """)
    conn.commit()

def execute_sovereign_queries(conn: sqlite3.Connection):
    cursor = conn.cursor()

    print("================================================================")
    print("        AEL SOVEREIGN IMDB RELATIONAL MATRIX ENGINE")
    print("================================================================\n")

    # Query 1: List all movies released in 2008
    print("[QUERY 1: SELECT title FROM movies WHERE year = 2008]")
    cursor.execute("SELECT title FROM movies WHERE year = 2008")
    for row in cursor.fetchall():
        print(f"  └── {row[0]}")
    print()

    # Query 2: Find the average rating of all movies in the database
    print("[QUERY 2: SELECT AVG(rating) FROM ratings]")
    cursor.execute("SELECT AVG(rating) FROM ratings")
    avg_rating = cursor.fetchone()[0]
    print(f"  └── Average Rating: {avg_rating:.2f}\n")

    # Query 3: List all movies starring Leonardo DiCaprio using subqueries
    print("[QUERY 3: SELECT title FROM movies WHERE id IN (SELECT movie_id FROM stars WHERE person_id = (SELECT id FROM people WHERE name = 'Leonardo DiCaprio'))]")
    cursor.execute("""
        SELECT title FROM movies WHERE id IN (
            SELECT movie_id FROM stars WHERE person_id = (
                SELECT id FROM people WHERE name = 'Leonardo DiCaprio'
            )
        )
    """)
    for row in cursor.fetchall():
        print(f"  └── {row[0]}")
    print()

    # Query 4: INNER JOIN demonstration
    print("[QUERY 4: SELECT movies.title, ratings.rating FROM movies JOIN ratings ON movies.id = ratings.movie_id ORDER BY ratings.rating DESC]")
    cursor.execute("""
        SELECT movies.title, ratings.rating 
        FROM movies 
        JOIN ratings ON movies.id = ratings.movie_id 
        ORDER BY ratings.rating DESC
    """)
    for row in cursor.fetchall():
        print(f"  └── {row[0]} (Rating: {row[1]})")
    print("\nExecution completed successfully.")

def main():
    conn = sqlite3.connect(":memory:")
    build_sample_db(conn)
    execute_sovereign_queries(conn)
    conn.close()

if __name__ == "__main__":
    main()
