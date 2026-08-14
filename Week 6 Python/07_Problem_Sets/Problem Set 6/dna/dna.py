"""
AEL Sovereign — CS50x 2026-2027
Problem Set 6: DNA
Author: Ayman Elmasry — AEL Digital Studio

Identifies a person based on the length of consecutive short tandem
repeats (STRs) found inside a given strand of DNA.  Given a CSV database
of people and their STR profiles, the program scans a DNA sequence and
reports the name of the one person whose profile matches, or "No match".

Usage:
    python3 dna.py <database.csv> <sequence.txt>
"""

import csv
import sys


def load_database(path):
    """
    Read a CSV profile database and return the list of STR names and the
    list of people, each represented as a dictionary of STR counts.

    The first row of the CSV contains the column headers: "name" followed
    by the name of every STR tracked in the database.  Every subsequent row
    describes one person: their name and the count of each STR in their DNA.
    """
    str_names = []
    database = []

    with open(path) as file:
        reader = csv.DictReader(file)
        str_names = reader.fieldnames[1:]
        for row in reader:
            person = {"name": row["name"]}
            for name in str_names:
                person[name] = int(row[name])
            database.append(person)

    return str_names, database


def longest_run(dna, subsequence):
    """
    Return the length of the longest consecutive run of the given STR
    (subsequence) inside the DNA string.

    A run is measured by repeatedly matching the exact STR back to back.
    Once a match is found at an index, the scan slides forward by the
    length of the STR to test whether the same STR repeats immediately.
    """
    longest = 0
    sub_len = len(subsequence)
    index = 0

    while index < len(dna):
        if dna[index:index + sub_len] == subsequence:
            run = 0
            position = index
            while dna[position:position + sub_len] == subsequence:
                run += 1
                position += sub_len
            if run > longest:
                longest = run
            index = position
        else:
            index += 1

    return longest


def find_match(dna, str_names, database):
    """
    Compare the STR counts measured from a DNA sequence against every
    person in the database.  Return the matching person's name, or None
    if no person's profile matches every STR count.
    """
    profile = {name: longest_run(dna, name) for name in str_names}

    for person in database:
        if all(person[name] == profile[name] for name in str_names):
            return person["name"]

    return None


def main():
    """Parse command-line arguments, load the data, and report the match."""
    if len(sys.argv) != 3:
        sys.exit("Usage: python3 dna.py <database.csv> <sequence.txt>")

    str_names, database = load_database(sys.argv[1])

    with open(sys.argv[2]) as file:
        dna = file.read()

    match = find_match(dna, str_names, database)
    if match is None:
        print("No match")
    else:
        print(match)


if __name__ == "__main__":
    main()
