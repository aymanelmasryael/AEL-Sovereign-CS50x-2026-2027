import csv
import sys


def main():

    if len(sys.argv) != 3:
        sys.exit("Usage: python dna.py data.csv sequence.txt")

    database = []
    with open(sys.argv[1]) as file:
        reader = csv.DictReader(file)
        for row in reader:
            database.append(row)

    with open(sys.argv[2]) as file:
        dna_sequence = file.read()

    headers = list(database[0].keys())[1:]  # [1:] because I don't want to include 'name' field
    longest_matches = {}
    for header in headers:
        longest_matches[header] = longest_match(dna_sequence, header)

    name = "No match"
    for row in database:
        match = 0
        for header in headers:
            if int(row[header]) == longest_matches[header]:
                match += 1
        if match == len(headers):
            name = row["name"]

    print(name)

    return


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
