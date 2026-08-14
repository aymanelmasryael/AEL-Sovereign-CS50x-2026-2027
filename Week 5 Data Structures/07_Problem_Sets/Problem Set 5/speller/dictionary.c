/**
 * @file dictionary.c
 * @brief Hash-table-backed dictionary for the speller problem set.
 * @author Ayman Elmasry — AEL Digital Studio
 * @project AEL Sovereign — CS50x 2026-2027, Problem Set 5
 *
 * Data structure:
 *   A chained hash table with a fixed number of buckets (N). Each bucket is a
 *   singly linked list of node structs, one per dictionary word.
 *
 * Algorithm:
 *   - hash(): djb2 — a well-distributed, byte-oriented string hash. Every
 *     character is lowercased first, which makes the function
 *     case-insensitive while keeping ASCII words in a tight bucket range.
 *   - load(): streams words with fscanf("%s"), hashes each one and prepends a
 *     freshly allocated node to the head of its bucket (O(1) per insertion),
 *     while tallying the total word count.
 *   - check(): hashes the input word, then walks the bucket comparing with a
 *     case-insensitive match so "CaTs" equals "cats".
 *   - size(): returns the running word count maintained during load().
 *   - unload(): frees every bucket's entire chain in a post-order sweep.
 *
 * Complexity (m = number of dictionary words, b = bucket count):
 *   - hash()   : O(L), L = word length.
 *   - load()   : O(m * L) total.
 *   - check()  : O(L + chain length); chain length is ~m/b on average, so
 *                with a good hash it behaves near-constant for real corpora.
 *   - unload() : O(m) node frees.
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"

/* Number of buckets in the chained hash table. A prime keeps the modulo
 * distribution even when the low bits of djb2 products are biased. */
#define TABLE_BUCKETS 10000

/* Root pointer of every chain; NULL marks an empty bucket. */
static node *table[TABLE_BUCKETS];

/* Number of words loaded so far, cached so size() is O(1). */
static unsigned int word_count = 0;

/**
 * @brief Maps a word to a bucket index in [0, TABLE_BUCKETS).
 *
 * Implements the djb2 variant hash = hash * 33 + c over the lowercased
 * characters of the word. Lowercasing before accumulation guarantees that
 * spelling checks and dictionary insertions collide identically regardless of
 * the case used in the text being spell-checked.
 *
 * @param word NUL-terminated word to hash.
 * @return Bucket index for the word.
 */
unsigned int hash(const char *word)
{
    unsigned long digest = 5381;
    int c;

    while ((c = *word++) != '\0')
    {
        /* Cast to unsigned char keeps tolower() well-defined for every
         * byte value a dictionary word may contain. */
        c = tolower((unsigned char) c);
        digest = ((digest << 5) + digest) + c;
    }

    return (unsigned int) (digest % TABLE_BUCKETS);
}

/**
 * @brief Determines whether a word is in the dictionary.
 *
 * The word is hashed, its bucket chain is walked, and each node's stored word
 * is compared case-insensitively against the query.
 *
 * @param word The word to look up.
 * @return true if found, false otherwise.
 */
bool check(const char *word)
{
    unsigned int bucket = hash(word);

    for (node *cursor = table[bucket]; cursor != NULL; cursor = cursor->next)
    {
        if (strcasecmp(cursor->word, word) == 0)
        {
            return true;
        }
    }

    return false;
}

/**
 * @brief Loads a dictionary file into the hash table.
 *
 * Each whitespace-delimited token is read with a width-limited format string
 * (no longer than LENGTH), hashed and prepended to its bucket. Any failure —
 * missing file, allocation error — rolls back cleanly with a false return.
 *
 * @param dictionary Path to the dictionary file.
 * @return true on success, false on any failure.
 */
bool load(const char *dictionary)
{
    FILE *source = fopen(dictionary, "r");
    if (source == NULL)
    {
        return false;
    }

    char buffer[LENGTH + 1];

    while (fscanf(source, "%45s", buffer) == 1)
    {
        node *fresh = malloc(sizeof(node));
        if (fresh == NULL)
        {
            fclose(source);
            return false;
        }

        strcpy(fresh->word, buffer);

        unsigned int bucket = hash(fresh->word);
        fresh->next = table[bucket];
        table[bucket] = fresh;

        word_count++;
    }

    fclose(source);
    return true;
}

/**
 * @brief Returns the number of words currently loaded.
 * @return The cached word counter, O(1).
 */
unsigned int size(void)
{
    return word_count;
}

/**
 * @brief Releases every node in every bucket of the table.
 *
 * Each chain is traversed with a "current / next" pair so the current node is
 * free()d before the traversal advances — no node is ever touched after being
 * freed. The table is left fully reset and ready to reload.
 *
 * @return Always true.
 */
bool unload(void)
{
    for (int i = 0; i < TABLE_BUCKETS; i++)
    {
        node *cursor = table[i];
        while (cursor != NULL)
        {
            node *victim = cursor;
            cursor = cursor->next;
            free(victim);
        }
        table[i] = NULL;
    }

    word_count = 0;
    return true;
}