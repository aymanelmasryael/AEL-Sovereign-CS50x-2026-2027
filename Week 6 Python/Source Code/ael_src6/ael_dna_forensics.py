#!/usr/bin/env python3
# ==============================================================================
# AEL CS Encyclopedia™ - © Ayman Elmasry
# Week 6 Python: ael_dna_forensics.py (Forensic DNA STR Profiling Engine)
# ==============================================================================

import csv
import sys

def compute_max_str_repeats(dna_sequence: str, str_sub: str) -> int:
    """
    Computes the maximum longest run of consecutive iterations of the STR subsequence
    within the overarching DNA primary string.
    """
    max_run = 0
    sub_len = len(str_sub)
    seq_len = len(dna_sequence)

    for i in range(seq_len):
        current_run = 0
        
        # Look ahead in chunks of sub_len
        while dna_sequence[i + (current_run * sub_len) : i + ((current_run + 1) * sub_len)] == str_sub:
            current_run += 1
            
        if current_run > max_run:
            max_run = current_run

    return max_run

def main():
    if len(sys.argv) != 3:
        print("Usage: python ael_dna_forensics.py data.csv sequence.txt")
        sys.exit(1)

    database_path = sys.argv[1]
    sequence_path = sys.argv[2]

    # Read DNA primary string
    try:
        with open(sequence_path, "r", encoding="utf-8") as file:
            dna_sequence = file.read().strip()
    except FileNotFoundError:
        print(f"Error: Target sequence file '{sequence_path}' could not be located.")
        sys.exit(1)

    # Read CSV database and evaluate profiles
    try:
        with open(database_path, "r", encoding="utf-8") as file:
            reader = csv.DictReader(file)
            str_headers = reader.fieldnames[1:] # Skip 'name' column

            # Compute actual matches from the sequence
            actual_str_counts = {str_sub: str(compute_max_str_repeats(dna_sequence, str_sub)) for str_sub in str_headers}

            # Search for a matching profile
            for row in reader:
                match = True
                for str_sub in str_headers:
                    if row[str_sub] != actual_str_counts[str_sub]:
                        match = False
                        break
                if match:
                    print(row["name"])
                    return

            print("No match")

    except FileNotFoundError:
        print(f"Error: Database CSV '{database_path}' could not be located.")
        sys.exit(1)

if __name__ == "__main__":
    main()
