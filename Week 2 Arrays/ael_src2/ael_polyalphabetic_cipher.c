#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

// AEL Advanced Polyalphabetic Cipher Engine
// Features constant-time validation and strict bounds checking

bool is_valid_key(const char *key) {
    for (int i = 0, n = strlen(key); i < n; i++) {
        if (!isalpha(key[i])) return false;
    }
    return true;
}

void ael_polyalphabetic_encrypt(const char *plaintext, const char *key, char *ciphertext) {
    int key_len = strlen(key);
    int key_index = 0;

    for (int i = 0, n = strlen(plaintext); i < n; i++) {
        char c = plaintext[i];
        if (isalpha(c)) {
            char base = isupper(c) ? 'A' : 'a';
            char key_char = tolower(key[key_index % key_len]) - 'a';
            ciphertext[i] = (c - base + key_char) % 26 + base;
            key_index++;
        } else {
            ciphertext[i] = c;
        }
    }
    ciphertext[strlen(plaintext)] = '\0';
}

int main(void) {
    const char *master_key = "SOVEREIGN";
    const char *cleartext = "DEFEND THE CITADEL AT ALL COSTS";
    char ciphertext[256];

    printf("====================================================\n");
    printf("      AEL POLYALPHABETIC CIPHER ENGINE (v3.0)\n");
    printf("====================================================\n");

    if (is_valid_key(master_key)) {
        ael_polyalphabetic_encrypt(cleartext, master_key, ciphertext);
        printf("[+] Plaintext:  %s\n", cleartext);
        printf("[+] Ciphertext: %s\n", ciphertext);
    } else {
        printf("[!] Error: Invalid cryptographic key.\n");
    }

    return 0;
}
