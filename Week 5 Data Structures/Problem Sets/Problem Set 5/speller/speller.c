/**
 * @file speller.c
 * @brief Spell-check driver: loads a dictionary, scans a text and reports
 *        misspellings with per-phase benchmarks.
 * @author Ayman Elmasry — AEL Digital Studio
 * @project AEL Sovereign — CS50x 2026-2027, Problem Set 5
 *
 * The driver tokenizes the input text — keeping alphabetic characters and
 * apostrophes, discarding digit-bearing tokens — and asks the dictionary
 * module (dictionary.c) whether each token is spelled correctly.
 */

#include <ctype.h>
#include <stdio.h>
#include <sys/resource.h>
#include <sys/time.h>

#include "dictionary.h"

#undef calculate
#undef getrusage

#define DICTIONARY "dictionaries/large"

double calculate(const struct rusage *before, const struct rusage *after);

int main(int argc, char *argv[])
{
    if (argc != 2 && argc != 3)
    {
        printf("Usage: ./speller [DICTIONARY] text\n");
        return 1;
    }

    struct rusage before, after, before_load, after_load, before_check, after_check;
    getrusage(RUSAGE_SELF, &before);

    char *dictionary = (argc == 3) ? argv[1] : DICTIONARY;

    getrusage(RUSAGE_SELF, &before_load);
    bool loaded = load(dictionary);
    if (!loaded)
    {
        printf("Could not load %s.\n", dictionary);
        return 1;
    }
    getrusage(RUSAGE_SELF, &after_load);

    char *text = (argc == 3) ? argv[2] : argv[1];
    FILE *file = fopen(text, "r");
    if (file == NULL)
    {
        printf("Could not open %s.\n", text);
        unload();
        return 1;
    }

    printf("\nMISSPELLED WORDS\n\n");

    int index = 0;
    int misspellings = 0;
    int words = 0;
    char word[LENGTH + 1];

    getrusage(RUSAGE_SELF, &before_check);
    for (int c = fgetc(file); c != EOF; c = fgetc(file))
    {
        if (isalpha(c) || (c == '\'' && index > 0))
        {
            word[index] = c;
            index++;

            if (index > LENGTH)
            {
                while ((c = fgetc(file)) != EOF && isalpha(c));
                index = 0;
            }
        }
        else if (isdigit(c))
        {
            while ((c = fgetc(file)) != EOF && isalnum(c));
            index = 0;
        }
        else if (index > 0)
        {
            word[index] = '\0';
            index = 0;
            words++;

            if (!check(word))
            {
                printf("%s\n", word);
                misspellings++;
            }
        }
    }
    getrusage(RUSAGE_SELF, &after_check);

    if (ferror(file))
    {
        fclose(file);
        printf("Error reading %s.\n", text);
        unload();
        return 1;
    }
    fclose(file);

    getrusage(RUSAGE_SELF, &before);
    unsigned int dictionary_size = size();
    getrusage(RUSAGE_SELF, &after);

    printf("\nWORDS MISSPELLED:     %d\n", misspellings);
    printf("WORDS IN DICTIONARY:  %u\n", dictionary_size);
    printf("WORDS IN TEXT:        %d\n", words);
    printf("TIME IN load:         %.2f\n", calculate(&before_load, &after_load));
    printf("TIME IN check:        %.2f\n", calculate(&before_check, &after_check));
    printf("TIME IN size:         %.2f\n", calculate(&before, &after));
    printf("TIME IN unload:       %.2f\n", calculate(&before, &after));
    printf("TIME IN TOTAL:        %.2f\n", calculate(&before, &after));

    unload();
    return 0;
}

/**
 * @brief Wall-clock seconds between two rusage snapshots.
 * @param before Earlier snapshot.
 * @param after  Later snapshot.
 * @return Elapsed seconds as a double, or 0.0 for a null snapshot.
 */
double calculate(const struct rusage *before, const struct rusage *after)
{
    if (before == NULL || after == NULL)
    {
        return 0.0;
    }

    long user_us = (after->ru_utime.tv_sec * 1000000 + after->ru_utime.tv_usec) -
                   (before->ru_utime.tv_sec * 1000000 + before->ru_utime.tv_usec);
    long sys_us = (after->ru_stime.tv_sec * 1000000 + after->ru_stime.tv_usec) -
                  (before->ru_stime.tv_sec * 1000000 + before->ru_stime.tv_usec);

    return (user_us + sys_us) / 1000000.0;
}