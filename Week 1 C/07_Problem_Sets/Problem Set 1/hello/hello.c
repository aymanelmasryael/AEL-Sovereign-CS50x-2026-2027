/**
 * ============================================================================
 *  PROJECT : AEL Sovereign — CS50x 2026-2027 Master Solutions
 *  FILE    : hello.c
 *  AUTHOR  : Ayman Elmasry — AEL Digital Studio
 *  ---------------------------------------------------------------------------
 *  DESCRIPTION
 *    The canonical "first C program" re-engineered to professional standards.
 *    It acquires the user's name via the CS50 library, then emits the classic
 *    greeting that check50 verifies byte-for-byte.
 *
 *  ALGORITHM (STEP BY STEP)
 *    1. Prompt the console user for their name using get_string, which
 *       safely allocates a heap-backed string and guards against buffer
 *       overflows (unlike a fixed char[] + scanf("%s") pair).
 *    2. Emit the validated greeting "hello, <name>" on stdout.
 *
 *  COMPLEXITY
 *    Time  : O(1)  — a single prompt and a single printf.
 *    Space : O(n)  — proportional to the length of the name supplied by
 *                    the user (the heap string returned by get_string).
 *
 *  COMPILE  : gcc -o hello hello.c -lcs50
 * ============================================================================
 */

#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // Solicitation prompt must remain byte-identical for automated grading.
    string name = get_string("What's your name? ");

    // Output greeting; trailing newline required by the check harness.
    printf("hello, %s\n", name);

    return 0;
}
