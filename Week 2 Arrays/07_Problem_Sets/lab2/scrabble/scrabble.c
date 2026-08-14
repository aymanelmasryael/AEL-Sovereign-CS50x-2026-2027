/**
 * ============================================================================
 *  PROJECT : AEL Sovereign — CS50x 2026-2027 Master Solutions
 *  FILE    : scrabble.c
 *  AUTHOR  : Ayman Elmasry — AEL Digital Studio
 *  ---------------------------------------------------------------------------
 *  DESCRIPTION
 *    A two-player Scrabble score adjudicator from Lab 2. Each player submits
 *    a word; the program sums the classic English-letter point distribution
 *    (1, 3, 3, 2, 1, 4, ...) for alphabetic characters and declares a winner.
 *    The printed verdict ("Player 1 wins!", "Player 2 wins!", "Tie!") and the
 *    prompts ("Player 1: ", "Player 2: ") reproduce the check50 contract.
 *
 *  DESIGN-BUG SURVIVAL NOTES (assignment fandangles)
 *    - POINTS is indexed by letter *rank* (A=0 ... Z=25), which is computed
 *      by subtracting the appropriate ASCII base (65 for 'A', 97 for 'a').
 *      The original CS50 distribution is a ready-made minefield: writing
 *      `int POINTS[]` versus `const int` changes nothing functionally, but
 *      miscounting the 26-element table silently stretches array bounds.
 *    - The point table is declared `static const` so an accidental write is
 *      a link/compile-time error rather than silent heap corruption — a
 *      cheap contract that makes the assignment-operator pitfalls visible.
 *    - Non-letters (digits, spaces, punctuation) are deliberately ignored:
 *      `isalpha()` gates both arms, guaranteeing POINTS[c - base] is never
 *      indexed with a stray (possibly negative) series.
 *
 *  COMPLEXITY
 *    Time  : O(|word1| + |word2|)   — single linear pass per word.
 *    Space : O(1)                   — the fixed 26-entry point table.
 *
 *  COMPILE  : gcc -o scrabble scrabble.c -lcs50
 * ============================================================================
 */

#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

/* Official Scrabble letter scores, aligned with the alphabet (A .. Z). */
static const int POINTS[26] = {
    1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10
};

/* ASCII offsets used to map a letter to its 0-indexed table rank. */
#define ASCII_UPPERCASE_BASE 'A'   /* int 65 */
#define ASCII_LOWERCASE_BASE 'a'   /* int 97 */

/* Forward declarations for a modular adjudication pipeline. */
string obtain_word(int player);
int    compute_score(string word);
void   announce_winner(int score1, int score2);

int main(void)
{
    string word1 = obtain_word(1);
    string word2 = obtain_word(2);

    int score1 = compute_score(word1);
    int score2 = compute_score(word2);

    announce_winner(score1, score2);

    return 0;
}

/**
 * Prompts the numbered player for their word. The prompt string itself is
 * byte-for-byte required by the grading harness ("Player 1: ", "Player 2: ").
 */
string obtain_word(int player)
{
    return get_string("Player %i: ", player);
}

/**
 * Tallies the Scrabble value of `word`.
 *
 * INDEXING DERIVATION
 *   'C' (int 67): rank = 67 - 65 = 2  -> POINTS[2] = 3   (correct C score).
 *   'z' (int 122): rank = 122 - 97 = 25 -> POINTS[25] = 10 (correct Z score).
 * The rank is guaranteed to lie in the valid half-open interval [0, 26) by
 * the enclosing `isalpha()` test, so no out-of-bounds access can occur.
 */
int compute_score(string word)
{
    int score = 0;

    for (int i = 0, n = strlen(word); i < n; i++)
    {
        char c = word[i];

        if (isupper(c))
        {
            score += POINTS[c - ASCII_UPPERCASE_BASE];
        }
        else if (islower(c))
        {
            score += POINTS[c - ASCII_LOWERCASE_BASE];
        }
        /* Non-letters contribute zero points by design. */
    }

    return score;
}

/**
 * Emits the three-way comparison result in the exact grade-harness
 * vocabulary. Pure asymmetric comparison (no absolute values needed)
 * mirrors the reference implementation's control flow precisely.
 */
void announce_winner(int score1, int score2)
{
    if (score1 > score2)
    {
        printf("Player 1 wins!\n");
    }
    else if (score2 > score1)
    {
        printf("Player 2 wins!\n");
    }
    else
    {
        printf("Tie!\n");
    }
}