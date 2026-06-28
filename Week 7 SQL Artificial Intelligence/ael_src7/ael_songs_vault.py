#!/usr/bin/env python3
# ==============================================================================
# AEL CS Encyclopedia™ - © Ayman Elmasry
# Week 7 SQL & AI: ael_songs_vault.py (Spotify-Inspired Audio Analytics)
# ==============================================================================

import sqlite3

def load_songs_database(conn: sqlite3.Connection):
    cursor = conn.cursor()
    cursor.executescript("""
        CREATE TABLE artists (id INTEGER PRIMARY KEY, name TEXT);
        CREATE TABLE songs (id INTEGER PRIMARY KEY, name TEXT, artist_id INTEGER, danceability REAL, energy REAL, tempo REAL, duration_ms INTEGER);

        INSERT INTO artists (id, name) VALUES (1, 'Drake');
        INSERT INTO artists (id, name) VALUES (2, 'The Weeknd');
        INSERT INTO artists (id, name) VALUES (3, 'Kendrick Lamar');

        INSERT INTO songs VALUES (10, 'God''s Plan', 1, 0.75, 0.45, 77.2, 198973);
        INSERT INTO songs VALUES (11, 'Blinding Lights', 2, 0.51, 0.73, 171.0, 200040);
        INSERT INTO songs VALUES (12, 'Starboy', 2, 0.68, 0.59, 186.0, 230453);
        INSERT INTO songs VALUES (13, 'HUMBLE.', 3, 0.91, 0.62, 150.0, 177000);
    """)
    conn.commit()

def run_audio_analytics(conn: sqlite3.Connection):
    cursor = conn.cursor()

    print("================================================================")
    print("        AEL SOVEREIGN AUDIO VAULT ANALYTICS PROCESSOR")
    print("================================================================\n")

    # 1. Select all songs by The Weeknd
    print("[QUERY 1: SELECT name FROM songs WHERE artist_id = (SELECT id FROM artists WHERE name = 'The Weeknd')]")
    cursor.execute("""
        SELECT name FROM songs WHERE artist_id = (
            SELECT id FROM artists WHERE name = 'The Weeknd'
        )
    """)
    for row in cursor.fetchall():
        print(f"  └── {row[0]}")
    print()

    # 2. Calculate average energy of the entire catalog
    print("[QUERY 2: SELECT AVG(energy) FROM songs]")
    cursor.execute("SELECT AVG(energy) FROM songs")
    print(f"  └── Aggregate Mean Energy: {cursor.fetchone()[0]:.4f}\n")

    # 3. Find top danceable songs
    print("[QUERY 3: SELECT name, danceability FROM songs ORDER BY danceability DESC LIMIT 2]")
    cursor.execute("SELECT name, danceability FROM songs ORDER BY danceability DESC LIMIT 2")
    for row in cursor.fetchall():
        print(f"  └── {row[0]} (Danceability Index: {row[1]})")
    print("\nVault inspection successfully executed.")

def main():
    conn = sqlite3.connect(":memory:")
    load_songs_database(conn)
    run_audio_analytics(conn)
    conn.close()

if __name__ == "__main__":
    main()
