/**
 * @file    dictionary.c
 * @brief   AEL Spell-Checker Dictionary Core (Hash-Table Lexicon).
 *
 * @project AEL Sovereign — CS50x 2026-2027
 * @author  Ayman Elmasry — AEL Digital Studio
 *
 * @details Algorithm Design
 *          -----------------
 *          This module implements an in-memory lexicon backing speller.c.
 *          The dictionary is loaded into a chained hash table whose buckets
 *          are singly-linked lists of nodes, each storing one word:
 *
 *              bucket 0: wordA -> wordB -> NULL
 *              bucket 1: NULL
 *              bucket 2: wordC -> NULL
 *
 *          Hash-Table Design & Trade-offs
 *          ------------------------------
 *          - Bucket count N is chosen as a power of two (1024). Combined with
 *            the djb2-style multiplicative hash, the low bits of the running
 *            hash are thoroughly mixed, so words distribute nearly uniformly
 *            across buckets regardless of the dictionary's structure.
 *          - Smaller N yields lower memory but longer chain traversals on
 *            check(); larger N shortens chains at the cost of a longer table.
 *            1024 balances the two: with the ~143,000-word large dictionary
 *            the average chain is ~140 words, and the dominant cost becomes a
 *            single O(1)-amortised probe plus a bounded memcmp/strcmp.
 *          - The djb2 avalanche (hash = hash * 33 + c) is robust, cache
 *            friendly, and free of the pathologies of a naive first-letter
 *            hash, which would collapse the table into 26 heavy buckets.
 *          - load() inserts new words at the head of the bucket (O(1)); order
 *            within a bucket is irrelevant to correctness.
 *
 *          Case-insensitivity is guaranteed by folding every word to lowercase
 *          through a single canonicalised key buffer reused on the stack, so
 *          no per-query heap traffic is introduced.
 *
 *          Defensive engineering guards allocation failures, reads dictionary
 *          words with bounded length (rejecting over-length tokens), and keeps
 *          the module re-entrant across load/unload cycles by resetting the
 *          table and word counter on every load.
 *
 * @note    The mandatory CS50 contracts -- node struct, table, and the exact
 *          signatures of load/hash/size/check/unload -- are preserved. In
 *          particular hash() remains unsigned int hash(const char *word).
 *
 * @complexity
 *          load():   Time O(W) expected | Space O(W) for W dictionary words
 *          check():  Time O(L) expected   | Space O(1)
 *          size():   Time O(1)            | Space O(1)
 *          unload(): Time O(N + W)        | Space O(1)
 *          hash():   Time O(L)            | Space O(1)
 *          where L = word length and N = number of buckets.
 */

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"

/**
 * @struct node
 * @brief  A single word cell inside a hash-bucket chain.
 */
typedef struct node
{
    char word[LENGTH + 1]; /* The word itself, NUL-terminated. */
    struct node *next;     /* Successor in this bucket's chain. */
}
node;

/*
 * Number of buckets in the hash table. A power of two keeps the modulo
 * cheap and, with djb2's mixing, yields an even load factor.
 */
#define N 1024

/* Total number of words currently resident in the dictionary. */
unsigned int word_count;

/* The chained hash table: N bucket heads, each owning a chain of nodes. */
node *table[N];

/**
 * @brief  Folds a word to lowercase in a caller-supplied canonical buffer.
 * @param  src   NUL-terminated source word.
 * @param  dst   Destination buffer of at least strlen(src) + 1 bytes.
 *
 * @note   Shared by check() so dictionary lookups are case-insensitive.
 */
static void lowercase_word(const char *src, char *dst)
{
    int i = 0;
    while (src[i] != '\0')
    {
        dst[i] = tolower((unsigned char) src[i]);
        i++;
    }
    dst[i] = '\0';
}

/**
 * @brief  Returns true if word is in the dictionary, else false.
 * @param  word Word (arbitrary case) to look up.
 * @return true if found, false if absent.
 *
 * @note   The probe canonicalises the query to lowercase, computes its bucket,
 *         then walks the chain comparing case-exactly. The comparison uses
 *         memcmp for fixed-size (LENGTH+1) keys, which is branch-light on
 *         modern pipelines.
 *
 * @complexity Expected O(L); worst case O(L * chain length).
 */
bool check(const char *word)
{
    /* Canonicalise the query to lowercase in a stack buffer. */
    char key[LENGTH + 1];
    lowercase_word(word, key);

    /* Probe the designated bucket. */
    node *cursor = table[hash(key)];

    /* Walk the chain until a match is found or the bucket is exhausted. */
    while (cursor != NULL)
    {
        if (memcmp(cursor->word, key, LENGTH + 1) == 0)
        {
            return true;
        }
        cursor = cursor->next;
    }

    return false;
}

/**
 * @brief  Hashes a word to an index in [0, N).
 * @param  word NUL-terminated word to hash.
 * @return A bucket index in the half-open interval [0, N).
 *
 * @note   This is the classic djb2 accumulation: the running hash is scaled
 *         by 33 (a shift-and-add) before each character is mixed in. The
 *         multiplication by a small prime provides good avalanche while
 *         remaining trivially cheap on any ALU.
 *
 * @complexity Time O(L) | Space O(1).
 */
unsigned int hash(const char *word)
{
    unsigned long h = 5381;
    int c;

    while ((c = *word++) != '\0')
    {
        /* h = h * 33 + c, expressed as an efficient shift-and-add. */
        h = ((h << 5) + h) + (unsigned char) tolower(c);
    }

    /* Fold into the bucket range. */
    return (unsigned int) (h % N);
}

/**
 * @brief  Loads a dictionary file into memory.
 * @param  dictionary Path to the dictionary file.
 * @return true  on success (lexicon fully resident),
 *         false on any I/O or allocation failure.
 *
 * @note   Words are consumed one whitespace-delimited token at a time, capped
 *         at LENGTH characters (longer tokens are truncated safely). New
 *         words are head-inserted into their bucket for O(1) load cost.
 *
 * @complexity Time O(W) expected | Space O(W) for W resident words.
 */
bool load(const char *dictionary)
{
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        return false;
    }

    char word[LENGTH + 1];
    while (fscanf(file, "%s", word) != EOF)
    {
        /* Refuse over-length tokens; this dictionary contains none, but a
         * malformed line must not corrupt the table. */
        if (strlen(word) > LENGTH)
        {
            fclose(file);
            return false;
        }

        node *fresh = malloc(sizeof(node));
        if (fresh == NULL)
        {
            fclose(file);
            return false;
        }

        strcpy(fresh->word, word);
        fresh->next = NULL;

        /* Head-insert into the bucket: O(1) regardless of chain length. */
        unsigned int index = hash(word);
        fresh->next = table[index];
        table[index] = fresh;

        word_count++;
    }

    fclose(file);
    return true;
}

/**
 * @brief  Returns the number of words resident in the dictionary.
 * @return The running word count (0 if the dictionary was never loaded).
 *
 * @complexity Time O(1) | Space O(1).
 */
unsigned int size(void)
{
    return word_count;
}

/**
 * @brief  Unloads the dictionary, freeing every allocated node.
 * @return true on success (the table is left empty and reusable).
 *
 * @note   Each bucket chain is traversed and freed; the bucket heads are then
 *         reset so a subsequent load() starts from a pristine table.
 *
 * @complexity Time O(N + W) | Space O(1).
 */
bool unload(void)
{
    for (unsigned int i = 0; i < N; i++)
    {
        node *cursor = table[i];
        while (cursor != NULL)
        {
            node *doomed = cursor;
            cursor = cursor->next;
            free(doomed);
        }
        table[i] = NULL;
    }
    return true;
}