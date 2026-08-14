#!/usr/bin/env python3
# ==============================================================================
# Project   : AEL Sovereign — CS50x 2026-2027
# Module    : week6_sentimental_mario_more
# File      : mario.py
# Author    : Ayman Elmasry — AEL Digital Studio
# ------------------------------------------------------------------------------
# Description:
#   Builds the classic double-sided Mario pyramid of '#' blocks (the "more
#   comfortable" variant). The user supplies a height between 1 and 8; input is
#   validated in a retry loop until a legal integer is provided. Each row is
#   centred with spaces and separated by a fixed two-space gap, matching the
#   check50 output specification byte-for-byte.
#
# Algorithm:
#   For row r in [1..height]:
#       leading_spaces = height - r
#       left_bricks    = r
#       gap            = "  "
#       right_bricks   = r
#   This produces an isosceles shape whose apex is the single-brick top row.
#
# Complexity:
#   Time : O(h^2) — the pyramid emits Sum(r) = h(h+1)/2 bricks plus fillers.
#   Space: O(1)   — output is streamed; only constant-sized row strings exist.
# ==============================================================================


def get_height() -> int:
    """
    Loop until the user supplies an integer height in the inclusive range 1..8.

    Non-numeric input raises ValueError, which is swallowed so the loop simply
    re-prompts. Out-of-range integers fall through to the next iteration, and
    only a legal height terminates the loop. This guarantees the returned
    value can be used to index pyramid rows without any bounds checking.
    """
    while True:
        try:
            height = int(input("Height: "))
        except ValueError:
            continue  # ignore junk; silently re-prompt

        if 1 <= height <= 8:
            return height


def print_pyramid(height: int) -> None:
    """
    Render a two-sided '#' pyramid of the given height.

    Row indices are 1-based for readability: row 1 is the apex (a single
    brick), row `height` is the base. The two columns are joined by the
    fixed two-space separator required by the CS50 specification.
    """
    for row in range(1, height + 1):
        print(" " * (height - row), end="")  # right-justify the left column
        print("#" * row, end="")             # left column of bricks
        print("  ", end="")                  # mandatory two-space gap
        print("#" * row)                     # right column of bricks


def main() -> None:
    """Acquire a validated height and print the corresponding pyramid."""
    print_pyramid(get_height())


if __name__ == "__main__":
    main()