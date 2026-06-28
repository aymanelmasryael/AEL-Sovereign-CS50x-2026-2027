#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define LENGTH 45
#define N 65536

typedef struct node {
    char word[LENGTH + 1];
    struct node *next;
} node;

node *table[N];
unsigned int word_count = 0;

unsigned int hash(const char *word) {
    unsigned int hash_val = 0;
    for (int i = 0, n = strlen(word); i < n; i++) {
        hash_val = (hash_val << 2) ^ toupper(word[i]);
    }
    return hash_val % N;
}

bool load(const char *dictionary) {
    FILE *file = fopen(dictionary, "r");
    if (file == NULL) return false;

    char word[LENGTH + 1];
    while (fscanf(file, "%45s", word) != EOF) {
        node *new_node = malloc(sizeof(node));
        if (new_node == NULL) return false;
        strcpy(new_node->word, word);
        unsigned int index = hash(word);
        new_node->next = table[index];
        table[index] = new_node;
        word_count++;
    }
    fclose(file);
    return true;
}

bool check(const char *word) {
    char lower_word[LENGTH + 1];
    int n = strlen(word);
    for (int i = 0; i < n; i++) lower_word[i] = tolower(word[i]);
    lower_word[n] = '\0';

    unsigned int index = hash(lower_word);
    node *cursor = table[index];
    while (cursor != NULL) {
        if (strcasecmp(cursor->word, word) == 0) return true;
        cursor = cursor->next;
    }
    return false;
}

unsigned int size(void) {
    return word_count;
}

bool unload(void) {
    for (int i = 0; i < N; i++) {
        node *cursor = table[i];
        while (cursor != NULL) {
            node *tmp = cursor;
            cursor = cursor->next;
            free(tmp);
        }
    }
    return true;
}
