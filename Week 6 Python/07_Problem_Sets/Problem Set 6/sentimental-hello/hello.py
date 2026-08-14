#!/usr/bin/env python3
# ==============================================================================
# Project   : AEL Sovereign — CS50x 2026-2027
# Module    : week6_sentimental_hello
# File      : hello.py
# Author    : Ayman Elmasry — AEL Digital Studio
# ------------------------------------------------------------------------------
# Description:
#   Greets the user by name. Reads a single line from standard input and echoes
#   a personalized "hello, <name>" greeting — the canonical CS50 hello program
#   re-expressed in Python.
#
# Objectives:
#   - Demonstrate prompt-driven stdin interaction.
#   - Emphasise idiomatic f-string interpolation over legacy concatenation.
#   - Guard the entry point with the `__main__` idiom for reusability/import.
#
# Complexity:
#   Time : O(n) — walking the input string during the f-string read/format.
#   Space: O(n) — the name string held in memory for the duration of main().
# ==============================================================================


def main() -> None:
    """
    Prompt the user for their name and print a personalised greeting.
    The prompt deliberately ends with a newline, mirroring the exact
    check50-expected behaviour of the standard CS50 hello program.
    """
    name = input("What is your name?\n")
    print(f"hello, {name}")


if __name__ == "__main__":
    main()