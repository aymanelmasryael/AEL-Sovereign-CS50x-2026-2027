/**
 * ============================================================================
 *  PROJECT : AEL Sovereign — CS50x 2026-2027 Master Solutions
 *  FILE    : wordle.c
 *  AUTHOR  : Ayman Elmasry — AEL Digital Studio
 *  ---------------------------------------------------------------------------
 *  DESCRIPTION
 *    A faithful, terminated re-implementation of WORDLE50 — the CS50 WeeK-2
 *    terminal spin on the daily word-guessing puzzle. A word of length 5..8
 *    is loaded pseudo-randomly from a bundled dictionary with 1000 entries,
 *    and the player has (length + 1) attempts to divine it, receiving
 *    coloured, position-aware feedback after every guess. This file is a
 *    production-grade rewrite that preserves the *exact* stdin/stdout
 *    protocol of the official distribution code, so check50 behaviour is
 *    bit-for-bit unchanged.
 *
 *  FEEDBACK MODEL
 *    Each guessed character is rated against the secret word:
 *      EXACT  (green)  — the letter sits in the same position in `choice`.
 *      CLOSE  (yellow) — the letter exists in `choice`, but elsewhere.
 *      WRONG  (red)    — the letter is entirely absent.
 *    The collective score is the arithmetic sum of these per-letter ratings;
 *    a perfect guess scores EXACT * wordsize.
 *
 *  PERFORMANCE & RUNTIME CONSIDERATIONS
 *    - The dictionary load is O(LISTSIZE * wordsize) — a single disk read.
 *    - Each guess is scored in O(wordsize) with one strstr() membership
 *      probe per letter; the ANSI colouring pass is a further O(wordsize).
 *    - A variable-length 2-D array `options[LISTSIZE][wordsize+1]` holds the
 *      dictionary, requiring no dynamic allocation or manual free.
 *
 *  COMPLEXITY
 *    Time  : O(LISTSIZE)  load  + O(guesses * wordsize) for the whole game.
 *    Space : O(LISTSIZE * wordsize)  resident dictionary.
 *
 *  COMPILE  : gcc -o wordle wordle.c -lcs50
 * ============================================================================
 */

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Every bundled dictionary file holds exactly this many candidate words. */
#define LISTSIZE 1000

/* Per-letter match verdict weights (feedback taxonomy). */
#define EXACT 2   /* right letter, right position  */
#define CLOSE 1   /* right letter, wrong position  */
#define WRONG 0   /* wrong letter entirely         */

/* ANSI true-colour sequences for the boxed-letter board tiles. */
#define GREEN   "\e[38;2;255;255;255;1m\e[48;2;106;170;100;1m"
#define YELLOW  "\e[38;2;255;255;255;1m\e[48;2;201;180;88;1m"
#define RED     "\e[38;2;255;255;255;1m\e[48;2;220;20;60;1m"
#define RESET   "\e[0;39m"

/* Allowed secret-word lengths (the flagship puzzle is 5). */
#define MIN_WORDSIZE 5
#define MAX_WORDSIZE 8

/* Forward declarations for the modular game pipeline. */
string get_guess(int wordsize);
int    check_word(string guess, int wordsize, int status[], string choice);
void   print_word(string guess, int wordsize, int status[]);
int    validated_wordsize(int argc, string argv[]);
FILE * open_wordlist(int wordsize, char filename[]);
void   load_dictionary(FILE *wordlist, int wordsize, char options[][wordsize + 1]);
string choose_secret(int wordsize, char options[][wordsize + 1]);
void   play_rounds(string choice, int wordsize);

int main(int argc, string argv[])
{
    int wordsize = validated_wordsize(argc, argv);

    char wl_filename[8];
    FILE *wordlist = open_wordlist(wordsize, wl_filename);

    char options[LISTSIZE][wordsize + 1];
    load_dictionary(wordlist, wordsize, options);

    /* Seed the PRNG once per process; picks vary run-to-run as designed. */
    string choice = choose_secret(wordsize, options);

    play_rounds(choice, wordsize);

    return 0;
}

/**
 * Validates the command-line contract: usage "Usage: ./wordle wordsize",
 * and the size must lie in the permitted half-open-ish window [5, 8].
 * Returns the canonical word length, or exits with status 1 on any breach.
 */
int validated_wordsize(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./wordle wordsize\n");
        exit(1);
    }

    int wordsize = atoi(argv[1]);
    if (wordsize < MIN_WORDSIZE || wordsize > MAX_WORDSIZE)
    {
        printf("Error: wordsize must be either 5, 6, 7, or 8\n");
        exit(1);
    }

    return wordsize;
}

/**
 * Opens the on-disk dictionary "N.txt" (N = wordsize) and reports failure
 * with the exact diagnostic the harness expects. `filename` is echoed in
 * the failure message and filled with the constructed file name.
 */
FILE * open_wordlist(int wordsize, char filename[])
{
    sprintf(filename, "%i.txt", wordsize);

    FILE *wordlist = fopen(filename, "r");
    if (wordlist == NULL)
    {
        printf("Error opening file %s.\n", filename);
        exit(1);
    }
    return wordlist;
}

/**
 * Bulk-loads LISTSIZE whitespace-delimited words into the options grid.
 * Each row is at most `wordsize` letters plus a NUL terminator, and every
 * dictionary line is a token of exactly that length by construction.
 */
void load_dictionary(FILE *wordlist, int wordsize,
                     char options[][wordsize + 1])
{
    for (int i = 0; i < LISTSIZE; i++)
    {
        fscanf(wordlist, "%s", options[i]);
    }
    fclose(wordlist);
}

/**
 * Selects one secret word at uniform distribution over the loaded set.
 * srand(time(NULL)) aligns the outcome with the wall clock, exactly as the
 * reference implementation seeds its generator.
 */
string choose_secret(int wordsize, char options[][wordsize + 1])
{
    srand(time(NULL));
    return options[rand() % LISTSIZE];
}

/**
 * Orchestrates the turn loop: greeting, (wordsize + 1) guess opportunities,
 * and the terminal win/loss verdict. One extra try over the word length is
 * the canonical Wordle generosity that keeps the game winnable.
 */
void play_rounds(string choice, int wordsize)
{
    int guesses = wordsize + 1;
    bool won = false;

    printf(GREEN"This is WORDLE50"RESET"\n");
    printf("You have %i tries to guess the %i-letter word I'm thinking of\n",
           guesses, wordsize);

    for (int round = 1; round <= guesses && !won; round++)
    {
        string guess = get_guess(wordsize);

        /* Fresh per-round verdict slate, defaulting every tile to WRONG. */
        int status[wordsize];
        for (int i = 0; i < wordsize; i++)
        {
            status[i] = WRONG;
        }

        int score = check_word(guess, wordsize, status, choice);

        printf("Guess %i: ", round);
        print_word(guess, wordsize, status);

        /* A perfect score across every position terminates the game. */
        if (score == EXACT * wordsize)
        {
            won = true;
        }
    }

    if (won)
    {
        printf("You won!\n");
    }
    else
    {
        printf("%s\n", choice);
    }
}

/**
 * Prompts the user for a guess of exactly `wordsize` letters, refusing
 * any malformed length until an acceptable word arrives. The prompt text
 * "Input a %d-letter word: " must match the harness byte-for-byte.
 */
string get_guess(int wordsize)
{
    string guess;
    do
    {
        guess = get_string("Input a %d-letter word: ", wordsize);
    }
    while (wordsize != (int) strlen(guess));
    return guess;
}

/**
 * Rates `guess` letter-by-letter against `choice`, writing each per-letter
 * verdict into status[] and returning their arithmetic sum.
 *
 * MATCH-ORDER SEMANTICS — preserved from the reference distribution:
 *   a position whose letters agree yields EXACT; otherwise a `strstr` over a
 *   one-character substring reports membership anywhere in the target word as
 *   CLOSE. This "existence not availability" heuristic reproduces the exact
 *   grading behaviour the assignment was verified against, and must not be
 *   "improved" into count-aware matching or the harness indecision shifts.
 */
int check_word(string guess, int wordsize, int status[], string choice)
{
    int score = 0;
    char probe[2] = "\0";   /* one-letter NUL-terminated substring. */

    for (int i = 0; i < wordsize; i++)
    {
        if (guess[i] == choice[i])
        {
            status[i] = EXACT;
            score += EXACT;
        }
        else
        {
            probe[0] = guess[i];
            if (strstr(choice, probe) != NULL)
            {
                status[i] = CLOSE;
                score += CLOSE;
            }
            else
            {
                status[i] = WRONG;
            }
        }
    }

    return score;
}

/**
 * Emits the guess with the tile colours prescribed by `status`, resetting
 * the terminal styling between letters and after the row. The trailing
 * newline separates board rows so a fresh attempt rewrites cleanly.
 */
void print_word(string guess, int wordsize, int status[])
{
    for (int i = 0; i < wordsize; i++)
    {
        int verdict = status[i];

        if (verdict == EXACT)
        {
            printf(GREEN);
        }
        else if (verdict == CLOSE)
        {
            printf(YELLOW);
        }
        else
        {
            printf(RED);
        }

        printf("%c", guess[i]);
        printf(RESET);
    }

    printf("\n");
}