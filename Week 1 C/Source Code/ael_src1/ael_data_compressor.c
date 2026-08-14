/**
 * AEL Sovereign Systems — Custom CS50 Masterpiece (ael_src1)
 * Project: Run-Length Encoding (RLE) Data Compression Engine (ael_data_compressor.c)
 * Description: Demonstrates algorithmic compression in bare C, string indexing, buffer
 *              handling, char typecasting, and practical memory size reduction.
 */

#include <cs50.h>
#include <stdio.h>
#include <string.h>

void compress_rle(string input);

int main(void)
{
    printf("====================================================\n");
    printf("     AEL RUN-LENGTH ENCODING (RLE) COMPRESSOR\n");
    printf("====================================================\n");

    // Static uncompressed sample string
    string sample = "AAAAABBBCCDAA";
    printf("Sample Input:   %s (Size: %lu bytes)\n", sample, strlen(sample));
    printf("Compressed Output: ");
    compress_rle(sample);

    printf("\n====================================================\n");

    // Interactive Prompt
    string custom = get_string("Enter Custom Text for RLE Compression (e.g., AAAAAAABBBBCC): ");
    printf("Compressed Output: ");
    compress_rle(custom);

    printf("\n====================================================\n");
    return 0;
}

/**
 * Executes Run-Length Encoding (RLE) string compression algorithm
 */
void compress_rle(string input)
{
    int n = strlen(input);
    if (n == 0)
    {
        printf("[EMPTY]");
        return;
    }

    for (int i = 0; i < n; i++)
    {
        // Count occurrences of current character
        int count = 1;
        while (i < n - 1 && input[i] == input[i + 1])
        {
            count++;
            i++;
        }

        // Print character followed by run count
        printf("%c%d", input[i], count);
    }
}
