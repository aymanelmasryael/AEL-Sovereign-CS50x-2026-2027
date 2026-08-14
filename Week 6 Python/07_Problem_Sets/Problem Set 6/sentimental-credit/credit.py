#!/usr/bin/env python3
# ==============================================================================
# Project   : AEL Sovereign — CS50x 2026-2027
# Module    : week6_sentimental_credit
# File      : credit.py
# Author    : Ayman Elmasry — AEL Digital Studio
# ------------------------------------------------------------------------------
# Description:
#   Verifies a credit card number using the Luhn checksum algorithm and then
#   identifies the issuing network (AMEX, MASTERCARD or VISA). Non-compliant
#   numbers are reported as INVALID. The behaviour (prompt "Number: " and the
#   exact INVALID/VISA/MASTERCARD/AMEX verdict strings) matches the CS50
#   check50 specification precisely.
#
# Luhn algorithm (Hans Peter Luhn, 1954):
#   1. Working from the rightmost digit, double the value of every second digit
#      (i.e. the digits at even positions when counting from the right).
#   2. If doubling produces a two-digit value, sum its two digits (equivalent
#      to adding 9 for the doubled digits 10..18).
#   3. Sum every digit with the adjusted doubled values; the number is valid
#      iff the total modulo 10 equals zero.
#
# Issuer rules:
#   - AMEX      : 15 digits, first two digits 34 or 37.
#   - MASTERCARD: 16 digits, first two digits in the range 51..55.
#   - VISA      : 13 or 16 digits, first digit 4.
#
# Complexity:
#   Time : O(d) — one digit pass for the Luhn check plus constant prefix/length
#                 inspection, where d is the number of digits (d <= 16).
#   Space: O(1) — only a handful of scalar accumulators are maintained.
# ==============================================================================


def luhn_sum(card_number: str) -> int:
    """
    Compute the Luhn checksum for a clean digit string.

    Digits are processed right-to-left. Every second digit (tracked by the
    `parity` flag, starting at the second-to-last) is doubled; for a doubled
    value >= 10 the two constituent digits are summed before being added. The
    returned total is used by `verify_luhn` to test divisibility by ten.
    """
    total = 0
    parity = False  # when True, the current right-to-left digit is "doubled"

    for digit in reversed(card_number):
        value = int(digit)
        if parity:
            doubled = value * 2
            total += doubled // 10 + doubled % 10  # split two-digit products
        else:
            total += value
        parity = not parity

    return total


def verify_luhn(card_number: str) -> bool:
    """Return True iff `card_number` passes the Luhn checksum check."""
    return luhn_sum(card_number) % 10 == 0


def identify_issuer(card_number: str) -> str:
    """
    Classify a card number into its issuing network.

    The Luhn check is performed first; a chemical mismatch is rejected
    immediately. Otherwise the required (length, prefix) fingerprint for each
    network is matched in order. The first network whose constraints are
    satisfied wins; if none matches, the number is INVALID.
    """
    if not verify_luhn(card_number):
        return "INVALID"

    length = len(card_number)
    first_two = card_number[:2]

    # AMEX: 15 digits, prefix 34 or 37.
    if length == 15 and first_two in ("34", "37"):
        return "AMEX"

    # MASTERCARD: 16 digits, prefix 51..55.
    if length == 16 and first_two in ("51", "52", "53", "54", "55"):
        return "MASTERCARD"

    # VISA: 13 or 16 digits, leading digit 4.
    if length in (13, 16) and card_number.startswith("4"):
        return "VISA"

    return "INVALID"


def get_clean_number() -> str:
    """
    Prompt for a card number until a valid non-empty digit string is provided.

    A retry loop rejects empty input and any non-digit characters (which would
    otherwise make the Luhn arithmetic meaningless), while still allowing a
    leading digit of zero and arbitrary numeric lengths. Leading/trailing
    whitespace is stripped so a stray space does not corrupt validation.
    """
    while True:
        raw = input("Number: ").strip()
        if raw.isdigit():
            return raw


def main() -> None:
    """Acquire a clean card number, classify it, and print the verdict."""
    card_number = get_clean_number()
    print(identify_issuer(card_number))


if __name__ == "__main__":
    main()