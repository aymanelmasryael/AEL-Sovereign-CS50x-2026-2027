/**
 * @file hello.c
 * @brief CS50x Problem Set 1 — Hello: greets the user by name.
 *
 * @author Ayman Elmasry — AEL Digital Studio
 * @project AEL Sovereign — CS50x 2026-2027
 *
 * @details
 *   The canonical first program of the course. It demonstrates two of the
 *   most fundamental building blocks of C: reading input through the CS50
 *   library's `get_string` and writing formatted output through `printf`.
 *   No command-line arguments are involved; the program is entirely driven
 *   by an interactive prompt.
 *
 * Algorithm:
 *   1. Prompt the user with "What is your name? " and read a full line.
 *   2. Emit a single, personalized greeting line via one printf call.
 *
 * Complexity:
 *   Time  — O(1): one prompt and one printf, independent of name length.
 *   Space — O(1): only a pointer to the library-managed string is retained.
 */

#include <cs50.h>
#include <stdio.h>

int main(void)
{
    /* Ask for the user's name and remember it for the greeting. */
    string name = get_string("What is your name? ");

    /* Print the required "hello, <name>" greeting line. */
    printf("hello, %s\n", name);

    return 0;
}
