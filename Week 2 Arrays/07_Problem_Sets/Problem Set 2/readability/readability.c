/**
 * ============================================================================
 *  PROJECT : AEL Sovereign — CS50x 2026-2027 Master Solutions
 *  FILE    : readability.c
 *  AUTHOR  : Ayman Elmasry — AEL Digital Studio
 *  ---------------------------------------------------------------------------
 *  DESCRIPTION
 *    Estimates the U.S. grade level required to comprehend an arbitrary piece
 *    of English prose, employing the classic Coleman–Liau index. Reads a
 *    single line ("Text: ") and prints a single grade verdict, conforming
 *    exactly to the check50 contract (Before Grade 1 / Grade N / Grade 16+).
 *
 *  THE COLEMAN–LIAU FORMULA (1975)
 *    Using only letter, word, and sentence counts grounded in 100-word units:
 *        L = (letters / words) * 100          -- avg letters per 100 words
 *        S = (sentences / words) * 100        -- avg sentences per 100 words
 *        index = round(0.0588 * L - 0.296 * S - 15.8)
 *    Methodologically the *sign* convention: Raising L increases the index
 *    (longer words → harder), while raising S decreases it (more sentence
 *    breaks → simpler prose). The formula maps a 4th-grade text to ~4 and a
 *    20th-grade text to 20, and the -15.8 constant centres the zero offset.
 *
 *  TOKENIZATION RULES (matching the reference distribution code)
 *    - Letter : any single alphabetic character (a-z, A-Z).
 *    - Word   : any maximal whitespace-delimited run. The first word exists
 *               before the first blank of the very first sentence, so the
 *               word count starts at 1 and increments at each whitespace.
 *    - Sentence : terminated only by '.', '!' or '?' (ellipses excluded).
 *
 *  COMPLEXITY
 *    Time  : O(len) — one pass over the string; all counters final in place.
 *    Space : O(len) — the heap resident input from get_string.
 *
 *  COMPILE  : gcc -o readability readability.c -lcs50
 * ============================================================================
 */

#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

/* Coleman–Liau linear-regression coefficients (as published, 1975). */
#define CLI_COEF_LETTERS 0.0588
#define CLI_COEF_SENT    -0.296
#define CLI_COEF_OFFSET  -15.8

/* Grade-boundary thresholds demanded by the problem specification. */
#define GRADE_FLOOR  1
#define GRADE_CEILING 16

/* Forward declarations for the modular analysis pipeline. */
int  count_letters(string text);
int  count_words(string text);
int  count_sentences(string text);
int  cli_index(int letters, int words, int sentences);
void report_grade(int index);

int main(void)
{
    string text = get_string("Text: ");

    int letters   = count_letters(text);
    int words     = count_words(text);
    int sentences = count_sentences(text);

    int index = cli_index(letters, words, sentences);
    report_grade(index);

    return 0;
}

/**
 * Tally of alphabetic characters in the text.
 * isalpha() is locale-aware ASCII recognition; underscores and digits are
 * deliberately excluded because they are not "sight-vocabulary" letters.
 */
int count_letters(string text)
{
    int letters = 0;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if (isalpha(text[i]))
        {
            letters++;
        }
    }
    return letters;
}

/**
 * Tally of whitespace-delimited words.
 * Invariant reasoning: every blank separates two words and the head of the
 * string is guaranteed to start a word, hence words == blanks + 1. Counting
 * blanks and seeding at 1 is simpler and equivalent to counting runs.
 */
int count_words(string text)
{
    int words = 1;                /* the text always opens in word one.   */
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if (isblank(text[i]))     /* space OR horizontal tab delimiters. */
        {
            words++;
        }
    }
    return words;
}

/**
 * Tally of sentence terminators in the text.
 * Only '.' '!' '?' close a sentence per the specification; a stray `...`
 * counted each dot matches the reference implementation precisely.
 */
int count_sentences(string text)
{
    int sentences = 0;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        char c = text[i];
        if (c == '.' || c == '!' || c == '?')
        {
            sentences++;
        }
    }
    return sentences;
}

/**
 * Computes the Coleman–Liau readability index for the analysed text.
 * Both rates are promoted to double BEFORE multiplication to preserve the
 * fractional information that integer division would truncate away, then
 * the composite is rounded to the nearest integer per the publication.
 */
int cli_index(int letters, int words, int sentences)
{
    double per_100_words_L = (double) letters   / words * 100.0;
    double per_100_words_S = (double) sentences / words * 100.0;

    double raw = CLI_COEF_LETTERS * per_100_words_L
               + CLI_COEF_SENT    * per_100_words_S
               + CLI_COEF_OFFSET;

    return (int) round(raw);
}

/**
 * Emits the pedagogically bucketed grade verdict required by check50:
 *   index <  1     -> "Before Grade 1"
 *   index > 16     -> "Grade 16+"
 *   otherwise      -> "Grade <index>"
 */
void report_grade(int index)
{
    if (index < GRADE_FLOOR)
    {
        printf("Before Grade 1\n");
    }
    else if (index > GRADE_CEILING)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %d\n", index);
    }
}