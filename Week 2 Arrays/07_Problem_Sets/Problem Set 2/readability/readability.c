/**
 * @file readability.c
 * @brief CS50x Problem Set 2 — Readability: Coleman-Liau grade estimator.
 *
 * @author Ayman Elmasry — AEL Digital Studio
 * @project AEL Sovereign — CS50x 2026-2027
 *
 * @details
 *   Estimates the U.S. school grade level of a body of text using the
 *   Coleman-Liau readability index. The program counts letters, words and
 *   sentences, then maps those counts through the classic formula:
 *
 *       index = 0.0588 * L - 0.296 * S - 15.8
 *
 *   where L = letters per 100 words and S = sentences per 100 words.
 *   The rounded index is reported as a grade, with the two contract
 *   clamps: indices below 1 print "Before Grade 1" and indices at or
 *   above 16 print "Grade 16+".
 *
 * Algorithm:
 *   1. Read the text with get_string.
 *   2. Count letters (alphabetic), words (whitespace-delimited tokens)
 *      and sentences (terminated by '.', '!' or '?').
 *   3. Derive the per-100-word rates and evaluate the formula.
 *   4. Clamp and print the grade.
 *
 * Complexity:
 *   Time  — O(n): each character is examined a constant number of times
 *           across the three counting passes.
 *   Space — O(1): only scalar counters, independent of text length.
 */

#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

/* Coleman-Liau formula coefficients and contract clamps. */
#define LETTERS_COEF 0.0588
#define SENTENCES_COEF 0.296
#define FORMULA_OFFSET 15.8
#define PER_WORDS_SCALE 100.0
#define GRADE_FLOOR 1
#define GRADE_CEILING 16

/**
 * @brief Count the alphabetic letters in a string.
 *
 * @param text The text to scan.
 * @return The number of characters for which isalpha is true.
 */
static int count_letters(string text)
{
    int letters = 0;

    for (int i = 0; text[i] != '\0'; i++)
    {
        if (isalpha(text[i]))
        {
            letters++;
        }
    }

    return letters;
}

/**
 * @brief Count the words in a string as whitespace-delimited tokens.
 *
 * @param text The text to scan.
 * @return The number of words detected.
 */
static int count_words(string text)
{
    int words = 0;
    bool inside_word = false;

    for (int i = 0; text[i] != '\0'; i++)
    {
        if (isspace(text[i]))
        {
            inside_word = false;
        }
        else if (!inside_word)
        {
            /* The first non-space character after a break begins a word. */
            inside_word = true;
            words++;
        }
    }

    return words;
}

/**
 * @brief Count the sentences in a string.
 *
 * @param text The text to scan.
 * @return The number of '.', '!' or '?' characters found.
 */
static int count_sentences(string text)
{
    int sentences = 0;

    for (int i = 0; text[i] != '\0'; i++)
    {
        if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            sentences++;
        }
    }

    return sentences;
}

/**
 * @brief Compute the (unclamped) Coleman-Liau index for a text.
 *
 * @param letters   Number of alphabetic characters.
 * @param words     Number of words.
 * @param sentences Number of sentences.
 * @return The rounded readability index.
 */
static int coleman_liau_grade(int letters, int words, int sentences)
{
    /* Express both rates relative to a fixed block of 100 words. */
    double letters_per_100 = (double) letters / words * PER_WORDS_SCALE;
    double sentences_per_100 = (double) sentences / words * PER_WORDS_SCALE;

    double index = LETTERS_COEF * letters_per_100
                 - SENTENCES_COEF * sentences_per_100
                 - FORMULA_OFFSET;

    return (int) round(index);
}

int main(void)
{
    string text = get_string("Text: ");

    int letters = count_letters(text);
    int words = count_words(text);
    int sentences = count_sentences(text);

    int index = coleman_liau_grade(letters, words, sentences);

    if (index < GRADE_FLOOR)
    {
        printf("Before Grade 1\n");
    }
    else if (index >= GRADE_CEILING)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", index);
    }

    return 0;
}
