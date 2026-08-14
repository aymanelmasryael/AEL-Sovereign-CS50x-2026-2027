"""
AEL Sovereign — CS50x 2026-2027
Problem Set 6: Sentimental Credit
Author: Ayman Elmasry — AEL Digital Studio

Validates a credit card number with the Luhn algorithm and identifies the
issuing company (American Express, MasterCard, or Visa), printing the
result followed by a newline.
"""


def get_number():
    """
    Prompt for a card number and return it as an integer.  The prompt is
    repeated until the user supplies a value that can be parsed as an int.
    """
    while True:
        try:
            return int(input("Number: "))
        except ValueError:
            continue


def luhn_sum(number):
    """
    Compute the Luhn checksum of a card number.  Starting from the second
    digit from the right, every other digit is doubled; when doubling a
    digit produces a two-digit value, its digits are added together.  The
    checksum is the sum of all the doubled and untouched digits.
    """
    digits = str(number)
    total = 0

    # Double every other digit, beginning with the second-to-last.
    for index in range(len(digits) - 2, -1, -2):
        doubled = int(digits[index]) * 2
        total += doubled // 10 + doubled % 10

    # Add every remaining digit, including the final digit on the right.
    for index in range(len(digits) - 1, -1, -2):
        total += int(digits[index])

    return total


def identify_issuer(number):
    """
    Return the issuer of a card as "VISA", "AMEX", or "MASTERCARD", or
    "INVALID" if the number fails the Luhn check or matches no issuer.
    """
    digits = str(number)
    length = len(digits)

    # A valid card number must produce a Luhn checksum divisible by 10.
    if luhn_sum(number) % 10 != 0:
        return "INVALID"

    # American Express: 15 digits, beginning with 34 or 37.
    if length == 15 and digits[:2] in ("34", "37"):
        return "AMEX"

    # MasterCard: 16 digits, beginning with 51 through 55.
    if length == 16 and digits[:2] in ("51", "52", "53", "54", "55"):
        return "MASTERCARD"

    # Visa: 13 or 16 digits, beginning with 4.
    if length in (13, 16) and digits[0] == "4":
        return "VISA"

    return "INVALID"


def main():
    """Prompt for a number and print the identified issuer."""
    number = get_number()
    print(identify_issuer(number))


if __name__ == "__main__":
    main()
