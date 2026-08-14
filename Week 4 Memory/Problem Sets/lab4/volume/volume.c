/**
 * @file volume.c
 * @brief Scales the amplitude of every PCM sample in a WAV file.
 * @author Ayman Elmasry — AEL Digital Studio
 * @project AEL Sovereign — CS50x 2026-2027, Lab 4
 *
 * Algorithm:
 *   1. Require exactly three arguments: input, output and a float factor.
 *   2. Copy the 44-byte header verbatim, then stream the 16-bit sample
 *      payload, multiplying each sample by the factor as it passes through.
 *
 * Complexity: O(n) time over the sample stream with O(1) buffer memory.
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

const int HEADER_SIZE = 44;

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    FILE *input = fopen(argv[1], "rb");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    FILE *output = fopen(argv[2], "wb");
    if (output == NULL)
    {
        printf("Could not open file.\n");
        fclose(input);
        return 1;
    }

    float factor = atof(argv[3]);

    uint8_t header[HEADER_SIZE];
    if (fread(header, HEADER_SIZE, 1, input) != 1)
    {
        printf("Could not read file.\n");
        fclose(input);
        fclose(output);
        return 1;
    }
    if (fwrite(header, HEADER_SIZE, 1, output) != 1)
    {
        printf("Could not write file.\n");
        fclose(input);
        fclose(output);
        return 1;
    }

    int16_t sample;
    while (fread(&sample, sizeof(int16_t), 1, input))
    {
        sample = (int16_t) (sample * factor);

        if (fwrite(&sample, sizeof(int16_t), 1, output) != 1)
        {
            printf("Could not write file.\n");
            fclose(input);
            fclose(output);
            return 1;
        }
    }

    if (ferror(input))
    {
        printf("Could not read file.\n");
        fclose(input);
        fclose(output);
        return 1;
    }

    fclose(input);
    fclose(output);
    return 0;
}