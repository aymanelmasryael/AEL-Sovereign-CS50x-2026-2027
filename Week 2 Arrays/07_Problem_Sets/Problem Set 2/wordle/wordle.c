/**
 * @file wordle.c
 * @brief CS50x Problem Set 2 — Wordle: terminal word-guessing game.
 *
 * @author Ayman Elmasry — AEL Digital Studio
 * @project AEL Sovereign — CS50x 2026-2027
 *
 * @details
 *   Implements the classic Wordle game for word lengths 5 through 8.
 *   The player has as many guesses as the word is letters long and must
 *   guess a randomly chosen word drawn from a bundled dictionary file.
 *   Each letter of a guess is scored against the secret word and rendered
 *   as a colored tile: green for a correct letter in the correct position,
 *   yellow for a letter in the word but misplaced, and red otherwise.
 *   A per-letter "matched" bookkeeping pass guarantees each occurrence of
 *   a duplicated letter in the secret can be consumed only once, which is
 *   precisely how the official game avoids phantom yellow tiles.
 *
 * Algorithm:
 *   1. Parse the optional word length argument (default 5, range 5..8);
 *      reject malformed usage with the contract message and exit code 1.
 *   2. Load the aligned dictionary and choose a uniformly random secret.
 *   3. For each round:
 *        a. Prompt for a guess, requiring exact length and dictionary
 *           membership.
 *        b. Classify each position via check_word (green/yellow/red).
 *        c. Reveal any newly placed letters in the hidden word.
 *        d. Print the hidden-word state, then the colored feedback row.
 *        e. On a full match, announce victory and exit 0.
 *   4. If turns run out, announce defeat and reveal the secret word.
 *
 * Complexity:
 *   Time  — O(w) per turn for checks (w = word length, bounded by 8),
 *           plus O(n) for the dictionary membership scan and O(n) once
 *           for loading, where n is the dictionary size.
 *   Space — O(n) for the retained dictionary plus O(w) working state.
 */

#include <cs50.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* ---- Game configuration constants (no magic numbers) ---- */
#define MIN_WORDSIZE 5
#define MAX_WORDSIZE 8
#define DEFAULT_WORDSIZE 5
#define MAX_WORDS 1000

/* ---- ANSI escape sequences for terminal coloring ---- */
#define ANSI_RESET "\x1b[0m"
#define ANSI_DARK_GREY "\x1b[1;30m"
#define ANSI_TITLE "\x1b[1;33m"
#define ANSI_SUCCESS "\x1b[1;32m"
#define ANSI_FAILURE "\x1b[1;31m"
#define ANSI_TILE_GREEN "\x1b[38;2;255;255;255m\x1b[48;2;106;170;100m"
#define ANSI_TILE_YELLOW "\x1b[38;2;255;255;255m\x1b[48;2;201;180;88m"
#define ANSI_TILE_RED "\x1b[38;2;255;255;255m\x1b[48;2;220;50;47m"

/* Outcome classes for one letter of a guess. */
typedef enum
{
    STATUS_ABSENT = 0,   /* Letter is not in the secret word (red tile).   */
    STATUS_PRESENT = 1,  /* Letter is in the word but misplaced (yellow).  */
    STATUS_EXACT = 2     /* Letter is correct at this position (green).    */
} status_t;

/* The dictionary for the requested length, held in a flat 2-D buffer so
   the program needs no dynamic allocation and owns no allocated memory. */
static char g_words[MAX_WORDS][MAX_WORDSIZE + 1];
static int g_word_count = 0;

/**
 * @brief Locate and open the dictionary file for a given word length.
 *
 * The official build environment runs inside the wordle directory and
 * expects "wordle{size}.txt". To also work when the process is launched
 * from an enclosing directory, several candidate paths are probed.
 *
 * @param wordsize The requested word length (identifies the file).
 * @return An open FILE*, or NULL if none of the candidates resolved.
 */
static FILE *open_word_file(int wordsize)
{
    static const char *templates[] =
    {
        "wordle%i.txt",    /* official distribution filename              */
        "%i.txt",          /* this repository's word-list naming          */
        "wordle/%i.txt",   /* launch from the directory above wordle/     */
        "./%i.txt"         /* explicit current-directory form             */
    };

    for (int i = 0; i < (int) (sizeof(templates) / sizeof(templates[0])); i++)
    {
        char path[64];
        sprintf(path, templates[i], wordsize);

        FILE *wordlist = fopen(path, "r");
        if (wordlist != NULL)
        {
            return wordlist;
        }
    }

    return NULL;
}

/**
 * @brief Populate the global dictionary from the appropriate word file.
 *
 * @param wordsize The requested word length.
 * @return true if at least one word was loaded, false otherwise.
 */
static bool load_words(int wordsize)
{
    FILE *wordlist = open_word_file(wordsize);
    if (wordlist == NULL)
    {
        return false;
    }

    char buffer[MAX_WORDSIZE + 1];
    while (fscanf(wordlist, "%s", buffer) == 1 && g_word_count < MAX_WORDS)
    {
        if (strlen(buffer) == (size_t) wordsize)
        {
            strcpy(g_words[g_word_count], buffer);
            g_word_count++;
        }
    }

    fclose(wordlist);
    return g_word_count > 0;
}

/**
 * @brief Prompt the player until a valid guess is supplied.
 *
 * A guess is accepted only when its length equals wordsize and it appears
 * verbatim inside the active dictionary.
 *
 * @param wordsize The required guess length.
 * @return A pointer to the validated guess string.
 */
static string get_guess(int wordsize)
{
    while (true)
    {
        string guess = get_string("Input a %i-letter word: ", wordsize);

        if (strlen(guess) != (size_t) wordsize)
        {
            printf("Input must be %i letters\n", wordsize);
            continue;
        }

        bool found = false;
        for (int i = 0; i < g_word_count; i++)
        {
            if (strcmp(guess, g_words[i]) == 0)
            {
                found = true;
                break;
            }
        }

        if (found)
        {
            return guess;
        }

        printf("Word must be in the file.\n");
    }
}

/**
 * @brief Score each letter of a guess against the secret word.
 *
 * @param guess    The player's word.
 * @param wordsize The word length.
 * @param status   Output array: one status_t per position.
 * @param secret   The hidden word being guessed.
 * @return true if every position matched exactly.
 */
static bool check_word(string guess, int wordsize, status_t status[], string secret)
{
    bool exact = true;
    bool matched[MAX_WORDSIZE];

    for (int i = 0; i < wordsize; i++)
    {
        matched[i] = false;
        status[i] = STATUS_ABSENT;
    }

    /* First pass: lock in positions that match exactly. Those letters are
       marked consumed so a later pass cannot reuse them incorrectly. */
    for (int i = 0; i < wordsize; i++)
    {
        if (guess[i] == secret[i])
        {
            status[i] = STATUS_EXACT;
            matched[i] = true;
        }
        else
        {
            exact = false;
        }
    }

    /* Second pass: every remaining guess letter searches for an unused
       occurrence elsewhere in the secret. Because matched[] is honored,
       duplicated secret letters can never yield redundant yellow tiles. */
    for (int i = 0; i < wordsize; i++)
    {
        if (status[i] == STATUS_EXACT)
        {
            continue;
        }

        for (int j = 0; j < wordsize; j++)
        {
            if (!matched[j] && guess[i] == secret[j])
            {
                status[i] = STATUS_PRESENT;
                matched[j] = true;
                break;
            }
        }
    }

    return exact;
}

/**
 * @brief Render a guess with per-letter colored tiles.
 *
 * @param guess    The validated guess.
 * @param status   The scoring produced by check_word.
 * @param wordsize The word length.
 */
static void print_word(string guess, status_t status[], int wordsize)
{
    for (int i = 0; i < wordsize; i++)
    {
        switch (status[i])
        {
            case STATUS_EXACT:
                printf(ANSI_TILE_GREEN "%c" ANSI_RESET, guess[i]);
                break;
            case STATUS_PRESENT:
                printf(ANSI_TILE_YELLOW "%c" ANSI_RESET, guess[i]);
                break;
            default:
                printf(ANSI_TILE_RED "%c" ANSI_RESET, guess[i]);
                break;
        }
    }

    printf("\n");
}

/**
 * @brief Print the progression of the hidden word.
 *
 * Positions already pinned down by the player are shown as their real
 * letters; everything else is hidden behind a dark-grey underscore.
 *
 * @param secret   The hidden word.
 * @param wordsize The word length.
 * @param opened   Per-position flag: whether that letter is revealed.
 */
static void hide_word(string secret, int wordsize, bool opened[])
{
    for (int i = 0; i < wordsize; i++)
    {
        if (opened[i])
        {
            printf("%c", secret[i]);
        }
        else
        {
            printf(ANSI_DARK_GREY "_" ANSI_RESET);
        }
    }

    printf("\n");
}

int main(int argc, string argv[])
{
    int wordsize = DEFAULT_WORDSIZE;

    if (argc == 2)
    {
        wordsize = atoi(argv[1]);
        if (wordsize < MIN_WORDSIZE || wordsize > MAX_WORDSIZE)
        {
            printf("Usage: ./wordle wordsize\n");
            return 1;
        }
    }
    else if (argc != 1)
    {
        printf("Usage: ./wordle wordsize\n");
        return 1;
    }

    if (!load_words(wordsize))
    {
        printf("Error: could not load word list.\n");
        return 1;
    }

    /* Seed from the current time and pick the secret uniformly at random. */
    srand((unsigned int) time(NULL));
    string secret = g_words[rand() % g_word_count];

    /* No letters of the secret have been revealed yet. */
    bool opened[MAX_WORDSIZE] = { false };

    printf("This is WORDLE\n");
    printf("You have %i tries to guess the %i-letter word I'm thinking of\n",
           wordsize, wordsize);
    printf("\n");

    for (int turn = 1; turn <= wordsize; turn++)
    {
        printf("Round %i/%i\n", turn, wordsize);

        string guess = get_guess(wordsize);

        status_t status[MAX_WORDSIZE];
        bool correct = check_word(guess, wordsize, status, secret);

        /* Reveal the letters the player just pinned down. */
        for (int i = 0; i < wordsize; i++)
        {
            if (status[i] == STATUS_EXACT)
            {
                opened[i] = true;
            }
        }

        printf(ANSI_TITLE "Hidden word:" ANSI_RESET "\n");
        hide_word(secret, wordsize, opened);
        print_word(guess, status, wordsize);
        printf("\n");

        if (correct)
        {
            printf(ANSI_SUCCESS "You won!" ANSI_RESET "\n");
            return 0;
        }
    }

    printf(ANSI_FAILURE "You ran out of turns!" ANSI_RESET
           " The word was: %s\n", secret);
    return 0;
}