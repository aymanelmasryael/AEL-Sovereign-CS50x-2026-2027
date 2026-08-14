/**
 * AEL Substitution Cipher — Professional Full-Alphabet Substitution Engine
 * Built for CS50x 2026-2027 | AEL CS Encyclopedia
 * 
 * © Ayman Elmasry | AEL Digital Studio™
 * Enforcing the strict directive: 100% Original Proprietary Enterprise Code.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

/**
 * Validates that the key contains precisely 26 unique alphabetical characters
 */
bool is_valid_substitution_key(const char *key) {
    if (strlen(key) != 26) {
        return false;
    }

    bool seen[26] = { false };
    for (int i = 0; i < 26; i++) {
        if (!isalpha((unsigned char)key[i])) {
            return false;
        }

        int index = toupper((unsigned char)key[i]) - 'A';
        if (seen[index]) {
            return false; // Duplicate letter detected
        }
        seen[index] = true;
    }

    return true;
}

/**
 * Encrypts plaintext in-place using a 26-character key array
 */
void encrypt_substitution(char *text, const char *key) {
    int len = strlen(text);
    for (int i = 0; i < len; i++) {
        if (isupper((unsigned char)text[i])) {
            int index = text[i] - 'A';
            text[i] = toupper((unsigned char)key[index]);
        } else if (islower((unsigned char)text[i])) {
            int index = text[i] - 'a';
            text[i] = tolower((unsigned char)key[index]);
        }
        // Non-alphabetic characters remain pristine
    }
}

int main(int argc, char *argv[]) {
    printf("\n=========================================================================\n");
    printf("             AEL SUBSTITUTION CIPHER — PROFESSIONAL ENCRYPTION KERNEL\n");
    printf("=========================================================================\n\n");

    if (argc != 2) {
        fprintf(stderr, "[ERROR] Usage: ./ael_cipher_substitution key\n");
        return 1;
    }

    if (!is_valid_substitution_key(argv[1])) {
        fprintf(stderr, "[ERROR] Key must contain exactly 26 unique alphabetical characters.\n");
        return 1;
    }

    const char *key = argv[1];
    char plaintext[512];

    printf("[AEL CIPHER] Active 26-Char Key Configured : %s\n\n", key);
    printf("Enter plaintext payload: ");

    if (fgets(plaintext, sizeof(plaintext), stdin) == NULL) {
        strcpy(plaintext, "SOVEREIGN DATA SECURED!");
    }
    plaintext[strcspn(plaintext, "\r\n")] = '\0'; // Strip newline

    printf("\n[AEL MATRIX] --- CIPHERTRANSFORM IN PROGRESS ---\n");
    printf("  --> Plaintext  : %s\n", plaintext);

    // Apply encryption
    encrypt_substitution(plaintext, key);

    printf("  --> Ciphertext : %s\n", plaintext);
    printf("\n[STATUS] Full Substitution Encryption Successful! ✅\n");
    printf("=========================================================================\n\n");

    return 0;
}
