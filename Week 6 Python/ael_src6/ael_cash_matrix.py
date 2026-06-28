#!/usr/bin/env python3
# ==============================================================================
# AEL CS Encyclopedia™ - © Ayman Elmasry
# Week 6 Python: ael_cash_matrix.py (Micro-Cash Greedy Optimization Engine)
# ==============================================================================

import sys

def get_cents_input() -> int:
    """
    Continually prompts the user for a valid floating point cash allocation,
    subsequently multiplying by 100 to eliminate floating point imprecision.
    """
    while True:
        try:
            val = input("Change owed: ").strip()
            amount = float(val)
            if amount >= 0:
                # Rounding to prevent floating point representation drift
                return round(amount * 100)
        except ValueError:
            # Continue prompting upon non-floating input
            pass
        except (KeyboardInterrupt, EOFError):
            print("\nExecution terminated.")
            sys.exit(0)

def compute_minimal_coins(cents: int) -> int:
    """
    Evaluates the greedy coin minimization matrix.
    Denominations: Quarters (25), Dimes (10), Nickels (5), Pennies (1).
    """
    coins = 0
    denominations = [25, 10, 5, 1]
    
    for coin in denominations:
        if cents >= coin:
            count = cents // coin
            coins += count
            cents %= coin
            
    return coins

def main():
    print("================================================================")
    print("           AEL MICRO-CASH GREEDY OPTIMIZATION ENGINE")
    print("================================================================")
    
    cents = get_cents_input()
    minimal_coins = compute_minimal_coins(cents)
    print(minimal_coins)

if __name__ == "__main__":
    main()
