"""
AEL Sovereign — CS50x 2026-2027
Problem Set 6: Sentimental Mario (More)
Author: Ayman Elmasry — AEL Digital Studio

Prompts the user for a pyramid height between 1 and 8, re-prompting until
a valid value is entered, and prints two adjacent right-aligned pyramids
of hashes separated by two spaces.
"""


def get_height():
    """
    Repeatedly prompt for a height until an integer between 1 and 8
    (inclusive) is provided.  Non-numeric input is ignored.
    """
    while True:
        try:
            height = int(input("Height: "))
        except ValueError:
            continue
        if 1 <= height <= 8:
            return height


def print_pyramids(height):
    """
    Print two mirrored half-pyramids of the requested height.  Each row
    is right-aligned within a field as wide as the height, and the two
    halves are separated by exactly two spaces.
    """
    for row in range(1, height + 1):
        spaces = " " * (height - row)
        hashes = "#" * row
        print(f"{spaces}{hashes}  {hashes}")


def main():
    """Prompt for a height and render the double pyramid."""
    height = get_height()
    print_pyramids(height)


if __name__ == "__main__":
    main()
