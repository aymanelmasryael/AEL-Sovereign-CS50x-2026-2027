#!/usr/bin/env python3
# ==============================================================================
# AEL CS Encyclopedia™ - © Ayman Elmasry
# Week 6 Python: ael_credit_luhn.py (Luhn Credit Card Verification Subsystem)
# ==============================================================================

import sys

def verify_luhn(card_number: str) -> bool:
    """
    Executes the Luhn algorithm verification on a clean numeric string.
    """
    total_sum = 0
    is_alternate = False

    # Traverse from rightmost digit to leftmost
    for char in reversed(card_number):
        digit = int(char)
        if is_alternate:
            doubled = digit * 2
            total_sum += (doubled // 10) + (doubled % 10)
        else:
            total_sum += digit
        is_alternate = not is_alternate

    return (total_sum % 10) == 0

def identify_card_issuer(card_number: str) -> str:
    """
    Determines the issuing banking institution based on industry prefixes and lengths.
    """
    length = len(card_number)
    if not verify_luhn(card_number):
        return "INVALID"

    # AMEX: 15 digits, prefix 34 or 37
    if length == 15 and (card_number.startswith("34") or card_number.startswith("37")):
        return "AMEX"
    
    # MASTERCARD: 16 digits, prefix 51 through 55
    if length == 16 and (51 <= int(card_number[:2]) <= 55):
        return "MASTERCARD"
    
    # VISA: 13 or 16 digits, prefix 4
    if (length == 13 or length == 16) and card_number.startswith("4"):
        return "VISA"

    return "INVALID"

def main():
    print("================================================================")
    print("        AEL SOVEREIGN CREDIT CARD VERIFICATION ENGINE")
    print("================================================================")
    
    try:
        raw_input = input("Number: ").strip()
        if not raw_input.isdigit():
            print("INVALID")
            return
        
        result = identify_card_issuer(raw_input)
        print(result)
        
    except (KeyboardInterrupt, EOFError):
        print("\nExecution terminated.")
        sys.exit(0)

if __name__ == "__main__":
    main()
