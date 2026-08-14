/**
 * AEL Scrabble Match — Enterprise Word Tabulation Engine
 * Built for CS50x 2026-2027 | AEL CS Encyclopedia
 * 
 * © Ayman Elmasry | AEL Digital Studio™
 * Enforcing the strict directive: 100% Original Proprietary Enterprise Code.
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Standard Scrabble point weights for letters A-Z
const int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

/**
 * Computes the official Scrabble point total for a given string buffer
 */
int compute_score(const char *word) {
    int score = 0;
    int len = strlen(word);

    for (int i = 0; i < len; i++) {
        if (isalpha((unsigned char)word[i])) {
            // Normalize to uppercase index (0-25)
            int index = toupper((unsigned char)word[i]) - 'A';
            score += POINTS[index];
        }
    }

    return score;
}

int main(void) {
    printf("\n=========================================================================\n");
    printf("             AEL SCRABBLE MATCH — ENTERPRISE EVALUATION KERNEL\n");
    printf("=========================================================================\n\n");

    char word1[256];
    char word2[256];

    printf("Player 1, enter sovereign token: ");
    if (fgets(word1, sizeof(word1), stdin) == NULL) {
        // Fallback simulated input if non-interactive
        strcpy(word1, "SOVEREIGN");
    }
    word1[strcspn(word1, "\r\n")] = '\0'; // Strip newline

    printf("Player 2, enter sovereign token: ");
    if (fgets(word2, sizeof(word2), stdin) == NULL) {
        // Fallback simulated input if non-interactive
        strcpy(word2, "ENCYCLOPEDIA");
    }
    word2[strcspn(word2, "\r\n")] = '\0'; // Strip newline

    // Compute scores
    int score1 = compute_score(word1);
    int score2 = compute_score(word2);

    printf("\n[AEL TABULATION] --- SCORING MATRIX ---\n");
    printf("  --> Player 1 Score : %d points (%s)\n", score1, word1);
    printf("  --> Player 2 Score : %d points (%s)\n", score2, word2);

    printf("\n[VERIFICATION STATUS] ");
    if (score1 > score2) {
        printf("Player 1 wins! 🏆\n");
    } else if (score1 < score2) {
        printf("Player 2 wins! 🏆\n");
    } else {
        printf("Tie! 🤝\n");
    }
    printf("=========================================================================\n\n");

    return 0;
}
