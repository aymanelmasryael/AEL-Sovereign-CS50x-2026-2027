/**
 * AEL Readability Index — Coleman-Liau Academic Readability Engine
 * Built for CS50x 2026-2027 | AEL CS Encyclopedia
 * 
 * © Ayman Elmasry | AEL Digital Studio™
 * Enforcing the strict directive: 100% Original Proprietary Enterprise Code.
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

/**
 * Executes Coleman-Liau grading math on arbitrary text buffers
 */
void audit_readability(const char *text) {
    int letters = 0;
    int words = 1; // Count starts at 1 to capture the final word
    int sentences = 0;
    int len = strlen(text);

    if (len == 0) {
        printf("[STATUS] Empty buffer. Grade 1\n");
        return;
    }

    for (int i = 0; i < len; i++) {
        if (isalpha((unsigned char)text[i])) {
            letters++;
        } else if (isspace((unsigned char)text[i])) {
            words++;
        } else if (text[i] == '.' || text[i] == '!' || text[i] == '?') {
            sentences++;
        }
    }

    // Protection against integer division hazards
    float L = ((float)letters / words) * 100.0f;
    float S = ((float)sentences / words) * 100.0f;
    float index = 0.0588f * L - 0.296f * S - 15.8f;
    int grade = (int)round(index);

    printf("\n[AEL DIAGNOSTIC] --- TEXT PARSING MATRIX ---\n");
    printf("  --> Total Letters   : %d\n", letters);
    printf("  --> Total Words     : %d\n", words);
    printf("  --> Total Sentences : %d\n", sentences);
    printf("  --> Coleman Index   : %.2f\n", index);

    printf("\n[ACADEMIC GRADE LEVEL] ");
    if (grade >= 16) {
        printf("Grade 16+ 🎓 (Advanced Academic/Professional)\n");
    } else if (grade < 1) {
        printf("Before Grade 1 👶 (Initial Elementary)\n");
    } else {
        printf("Grade %d 🏫\n", grade);
    }
}

int main(void) {
    printf("\n=========================================================================\n");
    printf("             AEL READABILITY INDEX — ACADEMIC VERIFICATION ENGINE\n");
    printf("=========================================================================\n\n");

    char text[1024];
    printf("Enter sovereign text payload for academic grading: ");
    
    if (fgets(text, sizeof(text), stdin) == NULL) {
        // Fallback simulation text if non-interactive
        strcpy(text, "Sovereign AI systems architecture requires strict verification matrix standards. Each micro-kernel must execute with perfect memory cleanliness and high academic grading accuracy!");
    }
    text[strcspn(text, "\r\n")] = '\0'; // Strip newline

    audit_readability(text);

    printf("=========================================================================\n\n");
    return 0;
}
