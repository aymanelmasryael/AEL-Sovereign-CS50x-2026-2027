/**
 * @file reverse.c
 * @brief Reverses the audio of a WAV file, sample block by sample block.
 * @author Ayman Elmasry — AEL Digital Studio
 * @project AEL Sovereign — CS50x 2026-2027, Problem Set 4
 *
 * Algorithm:
 *   1. Validate the CLI and open both files ("rb" / "wb").
 *   2. Read the 44-byte WAVHEADER and verify the RIFF/WAVE magic, the
 *      consistency of chunkSize, and PCM (audioFormat == 1).
 *   3. Write the (repaired) header to the output up front.
 *   4. Walk the audio payload backwards in blocks of
 *      blockAlign = numChannels * bitsPerSample / 8 bytes, copying each block
 *      verbatim from input to output. Reversing whole interleaved blocks
 *      rather than single samples keeps stereo channels together, so each
 *      speaker keeps its own track.
 *
 * Complexity: O(n) I/O over the audio payload; constant extra memory.
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "wav.h"

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: ./reverse input.wav output.wav\n");
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

    WAVHEADER header;
    if (fread(&header, sizeof(WAVHEADER), 1, input) != 1)
    {
        printf("Not a WAV file.\n");
        fclose(input);
        fclose(output);
        return 1;
    }

    int chunk_id_ok = header.chunkID[0] == 'R' && header.chunkID[1] == 'I' &&
                      header.chunkID[2] == 'F' && header.chunkID[3] == 'F';
    int format_ok = header.format[0] == 'W' && header.format[1] == 'A' &&
                    header.format[2] == 'V' && header.format[3] == 'E';

    if (!chunk_id_ok || !format_ok || header.chunkSize != 36 + header.subchunk2Size ||
        header.audioFormat != 1)
    {
        printf("Not a WAV file.\n");
        fclose(input);
        fclose(output);
        return 1;
    }

    header.chunkSize = 36 + header.subchunk2Size;

    if (fwrite(&header, sizeof(WAVHEADER), 1, output) != 1)
    {
        printf("Could not write file.\n");
        fclose(input);
        fclose(output);
        return 1;
    }

    int block_size = (header.bitsPerSample / 8) * header.numChannels;
    if (block_size <= 0)
    {
        printf("Unsupported audio format.\n");
        fclose(input);
        fclose(output);
        return 1;
    }

    fseek(input, 0, SEEK_END);
    long file_size = ftell(input);
    if (file_size < (long) sizeof(WAVHEADER))
    {
        printf("Not a WAV file.\n");
        fclose(input);
        fclose(output);
        return 1;
    }

    long audio_bytes = file_size - (long) sizeof(WAVHEADER);
    long block_count = audio_bytes / block_size;

    uint8_t *buffer = malloc((size_t) block_size);
    if (buffer == NULL)
    {
        printf("Unable to allocate memory.\n");
        fclose(input);
        fclose(output);
        return 1;
    }

    for (long i = block_count - 1; i >= 0; i--)
    {
        long offset = (long) sizeof(WAVHEADER) + i * block_size;
        if (fseek(input, offset, SEEK_SET) != 0 || fread(buffer, block_size, 1, input) != 1 ||
            fwrite(buffer, block_size, 1, output) != 1)
        {
            printf("Error processing audio data.\n");
            free(buffer);
            fclose(input);
            fclose(output);
            return 1;
        }
    }

    free(buffer);
    fclose(input);
    fclose(output);
    return 0;
}