import csv
import sys

def main():
    if len(sys.argv) != 3:
        print("Usage: python dna.py data.csv sequence.txt")
        sys.exit(1)

    # Read database into memory
    database = []
    with open(sys.argv[1], "r") as file:
        reader = csv.DictReader(file)
        subsequences = reader.fieldnames[1:]
        for row in reader:
            database.append(row)

    # Read DNA sequence into memory
    with open(sys.argv[2], "r") as file:
        sequence = file.read()

    # Find longest match of each STR in DNA sequence
    matches = {}
    for str_seq in subsequences:
        matches[str_seq] = longest_match(sequence, str_seq)

    # Check database for matching profiles
    for person in database:
        match = True
        for str_seq in subsequences:
            if int(person[str_seq]) != matches[str_seq]:
                match = False
                break
        if match:
            print(person["name"])
            return

    print("No match")

def longest_match(sequence, subsequence):
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    for i in range(sequence_length):
        count = 0
        while True:
            start = i + count * subsequence_length
            end = start + subsequence_length
            if sequence[start:end] == subsequence:
                count += 1
            else:
                break
        longest_run = max(longest_run, count)
    return longest_run

if __name__ == "__main__":
    main()
