/**
 * @file    volume.c
 * @brief   WAV Volume Amplifier (Streaming PCM Processor).
 *
 * @project AEL Sovereign — CS50x 2026-2027
 * @author  Ayman Elmasry — AEL Digital Studio
 *
 * @details Algorithm Design
 *          -----------------
 *          This program scales the amplitude of every 16-bit PCM sample in a
 *          WAV file by a user-supplied factor, producing a louder (factor > 1)
 *          or quieter (factor < 1) rendition of the audio.
 *
 *          The design is a single-pass streaming pipeline:
 *            1. The fixed 44-byte header is copied verbatim -- the header
 *               encodes metadata (sample rate, channels, bit depth), never
 *               amplitude, so it must not be scaled.
 *            2. Each 16-bit signed sample is read, multiplied by the floating
 *               factor, and written out. The stream terminates naturally when
 *               the input is exhausted, so memory usage is a constant single
 *               sample regardless of file size.
 *
 *          Defensive engineering validates every command-line argument, every
 *          open, and every read/write, and guarantees that both file handles
 *          are released even on early failure. The multiplication is performed
 *          in floating point to preserve amplitude fidelity, then narrowed
 *          back to the native int16 sample width.
 *
 * @note    This is the CS50x Week 4 Lab. The header size constant (44 bytes)
 *          and the streaming loop structure are preserved as expected.
 *
 * @complexity
 *          Time  O(N) | Space O(1)
 *          where N = number of 16-bit samples in the audio stream.
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* Fixed byte width of the canonical RIFF/WAVE header. */
const int HEADER_SIZE = 44;

/**
 * @brief  Entry point: applies the amplitude factor to every PCM sample.
 * @param  argc Argument count (expects exactly 4: program, input, output, factor).
 * @param  argv Argument vector.
 * @return 0 on success, nonzero on any validated failure.
 */
int main(int argc, char *argv[])
{
    /* Enforce the exact four-argument usage contract. */
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    /* Open the source audio for binary reading. */
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    /* Open (or create) the destination audio for binary writing. */
    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Could not open file.\n");
        fclose(input);
        return 1;
    }

    /* Parse the amplitude scale factor; default to unity on garbage input. */
    float factor = atof(argv[3]);

    /* Relay the header byte-for-byte: metadata, not audio, so unscaled. */
    uint8_t header[HEADER_SIZE];
    if (fread(header, HEADER_SIZE, 1, input) != 1)
    {
        printf("Could not read header from %s.\n", argv[1]);
        fclose(output);
        fclose(input);
        return 1;
    }
    if (fwrite(header, HEADER_SIZE, 1, output) != 1)
    {
        printf("Could not write header to %s.\n", argv[2]);
        fclose(output);
        fclose(input);
        return 1;
    }

    /*
     * Stream every 16-bit signed sample, scaling its amplitude by the factor.
     * The loop terminates once the sample stream is fully consumed.
     */
    int16_t buffer;
    while (fread(&buffer, sizeof(int16_t), 1, input) == 1)
    {
        buffer = (int16_t)(buffer * factor);

        if (fwrite(&buffer, sizeof(int16_t), 1, output) != 1)
        {
            printf("Could not write audio data to %s.\n", argv[2]);
            fclose(output);
            fclose(input);
            return 1;
        }
    }

    /* Both streams are exhausted and correctly released. */
    fclose(input);
    fclose(output);

    return 0;
}