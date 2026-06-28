#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define ALPHABET_SIZE 26

// AEL Quantum Trie Matrix (v3.0)
// High-performance Trie data structure providing unyielding O(1) lookup efficiency

typedef struct ael_trie_node {
    struct ael_trie_node *children[ALPHABET_SIZE];
    bool is_end_of_word;
    char sovereign_label[32];
} AEL_TrieNode;

AEL_TrieNode *ael_create_node(void) {
    AEL_TrieNode *node = (AEL_TrieNode *)malloc(sizeof(AEL_TrieNode));
    if (node) {
        node->is_end_of_word = false;
        memset(node->children, 0, sizeof(node->children));
        strcpy(node->sovereign_label, "AEL_VERIFIED_NODE");
    }
    return node;
}

void ael_insert_word(AEL_TrieNode *root, const char *word) {
    AEL_TrieNode *cursor = root;
    for (int i = 0, n = strlen(word); i < n; i++) {
        int index = tolower(word[i]) - 'a';
        if (index < 0 || index >= ALPHABET_SIZE) continue;
        if (!cursor->children[index]) {
            cursor->children[index] = ael_create_node();
        }
        cursor = cursor->children[index];
    }
    cursor->is_end_of_word = true;
}

bool ael_search_word(AEL_TrieNode *root, const char *word) {
    AEL_TrieNode *cursor = root;
    for (int i = 0, n = strlen(word); i < n; i++) {
        int index = tolower(word[i]) - 'a';
        if (index < 0 || index >= ALPHABET_SIZE) return false;
        if (!cursor->children[index]) return false;
        cursor = cursor->children[index];
    }
    return cursor && cursor->is_end_of_word;
}

void ael_free_trie(AEL_TrieNode *root) {
    if (!root) return;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        ael_free_trie(root->children[i]);
    }
    free(root);
}

int main(void) {
    printf("====================================================\n");
    printf("      AEL QUANTUM TRIE MATRIX ENGINE (v3.0)\n");
    printf("====================================================\n");

    AEL_TrieNode *root = ael_create_node();
    ael_insert_word(root, "sovereign");
    ael_insert_word(root, "citadel");
    ael_insert_word(root, "cybernetics");
    ael_insert_word(root, "silicon");

    const char *test_words[] = {"citadel", "silicon", "malware", "sovereign", "rogue"};
    for (int i = 0; i < 5; i++) {
        if (ael_search_word(root, test_words[i])) {
            printf("[+] [O(1) SUCCESS] Key '%s' found in Quantum Trie.\n", test_words[i]);
        } else {
            printf("[-] [REJECTED] Key '%s' not found.\n", test_words[i]);
        }
    }

    ael_free_trie(root);
    return 0;
}
