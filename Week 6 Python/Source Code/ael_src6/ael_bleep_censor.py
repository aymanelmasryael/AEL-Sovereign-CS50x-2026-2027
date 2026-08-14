#!/usr/bin/env python3
# ==============================================================================
# AEL CS Encyclopedia™ - © Ayman Elmasry
# Week 6 Python: ael_bleep_censor.py (Enterprise Bleep Text Censor Engine)
# ==============================================================================

import sys

def main():
    if len(sys.argv) != 2:
        print("Usage: python ael_bleep_censor.py banned_dictionary.txt")
        sys.exit(1)

    dictionary_path = sys.argv[1]
    banned_words = set()

    # Load banned words into a hashed set for O(1) constant-time lookup
    try:
        with open(dictionary_path, "r", encoding="utf-8") as f:
            for line in f:
                word = line.strip().lower()
                if word:
                    banned_words.add(word)
    except FileNotFoundError:
        print(f"Error: Banned dictionary file '{dictionary_path}' could not be located.")
        sys.exit(1)

    print("================================================================")
    print("            AEL ENTERPRISE TEXT SCRUBBING SUBSYSTEM")
    print("================================================================")
    
    try:
        raw_input = input("What message would you like to censor?\n> ").strip()
        tokens = raw_input.split()
        
        censored_tokens = []
        for token in tokens:
            # Check lowercased version of token against the banned set
            if token.lower() in banned_words:
                censored_tokens.append("*" * len(token))
            else:
                censored_tokens.append(token)

        print("\nCensored output:")
        print(" ".join(censored_tokens))

    except (KeyboardInterrupt, EOFError):
        print("\nExecution terminated.")
        sys.exit(0)

if __name__ == "__main__":
    main()
