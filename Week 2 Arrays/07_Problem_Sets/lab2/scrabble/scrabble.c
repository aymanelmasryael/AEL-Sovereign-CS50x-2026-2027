/**
 * @file scrabble.c
 * @brief CS50x Lab 2 — Scrabble: two-player word score duel.
 *
 * @author Ayman Elmasry — AEL Digital Studio
 * @project AEL Sovereign — CS50x 2026-2027
 *
 * @details
 *   Scores a word the way Scrabble does, using the game's canonical letter
 *   values, and declares the higher-scoring player the winner. Scoring is
 *   case-insensitive and ignores every non-alphabetic character. A single
 *   immutable table indexed by (upper-cased letter - 'A') provides O(1)
 *   lookup per character.
 *
 * Algorithm:
 *   1. Read each player's word via get_string.
 *   2. For every alphabetic character, upper-case it and add the matching
 *      table entry to that player's running total.
 *   3. Compare totals and print exactly one of the three contract lines:
 *      "Player 1 wins!", "Player 2 wins!" or "Tie!".
 *
 * Complexity:
 *   Time  — O(n): a single pass over each word, n being its length.
 *   Space — O(1): the 26-entry scoring table is fixed and immutable.
 */

#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

/* Canonical Scrabble values, listed alphabetically: index 0 == 'A'. */
static const int POINTS[26] =
{
    1, 3, 3, 2,  1, 4, 2, 4,
    1, 8, 5, 1,  3, 1, 1, 3,
    10, 1, 1, 1, 1, 4, 4, 8,
    4, 10
};

#define LETTER_A 'A'
#define ALPHABET_SIZE 26

/**
 * @brief Compute the Scrabble score of a word.
 *
 * @param word The word to score (case is irrelevant).
 * @return The accumulated point total.
 */
static int compute_score(string word)
{
    int score = 0;

    for (int i = 0; word[i] != '\0'; i++)
    {
        /* Non-alphabetic characters carry no score and are skipped. */
        if (isalpha(word[i]))
        {
            int index = toupper(word[i]) - LETTER_A;
            if (index >= 0 && index < ALPHABET_SIZE)
            {
                score += POINTS[index];
            }
        }
    }

    return score;
}

int main(void)
{
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    int score1 = compute_score(word1);
    int score2 = compute_score(word2);
    string winner = NULL;

    if (score1 > score2)
    {
        winner = "Player 1 wins!";
    }
    else if (score2 > score1)
    {
        winner = "Player 2 wins!";
    }
    else
    {
        winner = "Tie!";
    }

    printf("%s\n", winner);

    return 0;
}