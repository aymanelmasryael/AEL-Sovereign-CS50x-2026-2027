/**
 * AEL Caesar Cipher — Production-Grade Rotational Encryption Kernel
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
 * Validates that the CLI key argument is strictly numerical
 */
bool is_valid_key(const char *key_str) {
    for (int i = 0, n = strlen(key_str); i < n; i++) {
        if (!isdigit((unsigned char)key_str[i])) {
            return false;
        }
    }
    return true;
}

/**
 * Encrypts buffer in-place using Caesar rotation logic
 */
void encrypt_caesar(char *text, int key) {
    int len = strlen(text);
    for (int i = 0; i < len; i++) {
        if (isupper((unsigned char)text[i])) {
            text[i] = ((text[i] - 'A' + key) % 26) + 'A';
        } else if (islower((unsigned char)text[i])) {
            text[i] = ((text[i] - 'a' + key) % 26) + 'a';
        }
        // Non-alphabetic characters remain pristine
    }
}

int main(int argc, char *argv[]) {
    printf("\n=========================================================================\n");
    printf("             AEL CAESAR CIPHER — ROTATIONAL ENCRYPTION KERNEL\n");
    printf("=========================================================================\n\n");

    // Standard CLI Key Argument Validation
    if (argc != 2 || !is_valid_key(argv[1])) {
        fprintf(stderr, "[ERROR] Usage: ./ael_cipher_caesar key\n");
        fprintf(stderr, "Example: ./ael_cipher_caesar 13\n");
        return 1;
    }

    int key = atoi(argv[1]);
    char plaintext[512];

    printf("[AEL CIPHER] Active Encryption Key Configured : %d\n\n", key);
    printf("Enter plaintext payload: ");
    
    if (fgets(plaintext, sizeof(plaintext), stdin) == NULL) {
        strcpy(plaintext, "SOVEREIGN ARCHITECTURE 2026!");
    }
    plaintext[strcspn(plaintext, "\r\n")] = '\0'; // Strip newline

    printf("\n[AEL MATRIX] --- CIPHERTRANSFORM IN PROGRESS ---\n");
    printf("  --> Plaintext  : %s\n", plaintext);

    // Apply encryption
    encrypt_caesar(plaintext, key);

    printf("  --> Ciphertext : %s\n", plaintext);
    printf("\n[STATUS] Rotational Encryption Successful! ✅\n");
    printf("=========================================================================\n\n");

    return 0;
}
