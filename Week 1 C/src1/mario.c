#include <stdio.h>

int main(void) {
    int height = 8;
    for (int i = 0; i < height; i++) {
        // Print leading spaces
        for (int j = 0; j < height - i - 1; j++) {
            printf(" ");
        }
        // Print left hashes
        for (int j = 0; j <= i; j++) {
            printf("#");
        }
        // Print gap
        printf("  ");
        // Print right hashes
        for (int j = 0; j <= i; j++) {
            printf("#");
        }
        printf("\n");
    }
    return 0;
}
