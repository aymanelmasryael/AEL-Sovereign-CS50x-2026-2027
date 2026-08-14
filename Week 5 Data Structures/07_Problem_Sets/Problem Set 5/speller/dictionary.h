/**
 * @file dictionary.h
 * @brief Public interface of the speller dictionary module.
 * @author Ayman Elmasry — AEL Digital Studio
 * @project AEL Sovereign — CS50x 2026-2027, Problem Set 5
 *
 * The exact interface (including the node struct and LENGTH) is part of the
 * problem's distribution contract and must not change: speller.c and check50
 * both compile against it.
 */

#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stdbool.h>
#include <stddef.h>

#define LENGTH 45

typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

bool check(const char *word);
unsigned int hash(const char *word);
bool load(const char *dictionary);
unsigned int size(void);
bool unload(void);

#endif
