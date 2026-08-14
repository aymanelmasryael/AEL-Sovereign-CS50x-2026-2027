#!/usr/bin/env python3
# ==============================================================================
# Project   : AEL Sovereign — CS50x 2026-2027
# Module    : week6_dna
# File      : dna.py
# Author    : Ayman Elmasry — AEL Digital Studio
# ------------------------------------------------------------------------------
# Description:
#   Forensic STR (Short Tandem Repeat) profiling: given a CSV database of
#   known profiles and a target DNA sequence, identify the person whose
#   STR-repeat counts exactly match the target. This is the classic CS50 "DNA"
#   assignment, written at production quality.
#
# STR background:
#   A Short Tandem Repeat is a short nucleotide motif (e.g. "AGAT") that
#   appears consecutively and repeatedly in an individual's genome. The NUMBER
#   of consecutive repeats at a given locus is unique enough to act as a
#   fingerprint. A forensic sample is profiled by counting, for every marker,
#   the longest run of consecutive repeats; the sample is matched to a person
#   only when EVERY marker run-length equals the database entry.
#
# Matching strategy:
#   longest_run(word) scans every starting index; at each index it greedily
#   extends a match forward in whole-stride steps of the motif length and
#   records the maximum run found anywhere in the sequence. This is the
#   textbook sliding-window "longest consecutive substring" scan.
#
# Command line:
#   python3 dna.py databases/small.csv sequences/1.txt
#   -> prints the matching person's name, or "No match".
#
# Complexity:
#   Time : O(n * m * k) — outer scan over the n-character sequence; each
#         starting position advances in m-length (`len(motif)`) strides,
#         capped at the sequence length k.
#   Space: O(m + t) — the target profile dictionary keyed by STR name.
# ==============================================================================

import csv
import sys


def longest_run(sequence: str, motif: str) -> int:
    """
    Return the length (in repeats) of the longest consecutive run of `motif`
    contained anywhere in `sequence`, or 0 if the motif never appears.

    For every possible starting index `i` in the sequence, we inspect the
    substring of length `len(motif)` beginning at `i`. If it matches, we
    advance in whole-motif steps and re-test, so a chain of k contiguous
    repeats contributes a run counted once per aligned frame. The window
    slides one character at a time, guaranteeing that every alignment phase
    is explored and the global maximum is found.
    """
    longest = 0
    motif_len = len(motif)
    seq_len = len(sequence)

    for i in range(seq_len):
        run = 0
        # Probe forward in strides of `motif_len` chars while matches persist.
        start = i
        while sequence[start:start + motif_len] == motif:
            run += 1
            start += motif_len
        longest = max(longest, run)

    return longest


def load_database(path: str):
    """
    Read the profile CSV and return (rows, str_names).

    The first column is ALWAYS the person's name; every subsequent column is
    one STR marker whose values are the repeat counts for that person. The
    marker names are collected separately from the header so that counts can
    be compared without ever touching the name column.
    """
    with open(path, newline="") as file:
        reader = csv.DictReader(file)
        rows = list(reader)

    # Header minus the leading "name" field yields the STR markers in order.
    str_names = list(rows[0].keys())[1:] if rows else []

    return rows, str_names


def build_target_profile(sequence: str, str_names: list) -> dict:
    """
    Derive the STR fingerprint of the unknown sample.

    For each marker in the database we compute the longest consecutive repeat
    run within the target sequence. The result maps marker name -> run length
    (as int), ready to be diffed against every known profile.
    """
    return {name: longest_run(sequence, name) for name in str_names}


def find_match(rows: list, target: dict, str_names: list):
    """
    Return the first person whose profile matches the target on EVERY marker.

    A dictionary is a match only when, marker by marker, the database count is
    numerically equal to the target's run length. Because no two profiles in
    a real database share every count, the first exact match is authoritative;
    when no row agrees on all markers the function returns None, which the
    caller reports as "No match".
    """
    for row in rows:
        if all(int(row[name]) == target[name] for name in str_names):
            return row["name"]
    return None


def main() -> None:
    """
    Driver: validate CLI args, load data, compute the target profile, and print.

    Exit discipline: malformed invocations terminate with a usage error and a
    non-zero status; successful runs print exactly one line — a name or the
    literal string "No match" — to satisfy check50's output contract.
    """
    if len(sys.argv) != 3:
        sys.exit("Usage: python dna.py data.csv sequence.txt")

    database_path, sequence_path = sys.argv[1], sys.argv[2]

    rows, str_names = load_database(database_path)

    with open(sequence_path) as file:
        sequence = file.read()

    target_profile = build_target_profile(sequence, str_names)
    match = find_match(rows, target_profile, str_names)

    print(match if match else "No match")


if __name__ == "__main__":
    main()