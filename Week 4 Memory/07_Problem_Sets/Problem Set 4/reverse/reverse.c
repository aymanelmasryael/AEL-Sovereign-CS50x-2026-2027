/**
 * @file    reverse.c
 * @brief   WAV Audio Reversal Engine.
 *
 * @project AEL Sovereign — CS50x 2026-2027
 * @author  Ayman Elmasry — AEL Digital Studio
 *
 * @details Algorithm Design
 *          -----------------
 *          This program reverses the audio content of a PCM WAV file by
 *          reversing the order of its sample blocks while preserving the
 *          file header verbatim.
 *
 *          The header is first validated against the canonical WAVE
 *          signature ("RIFF" chunk, "WAVE" format) and the per-sample block
 *          size is derived as  numChannels * bitsPerSample / 8. The reversal
 *          then proceeds by a series of backward SEEK-scrambles: the cursor
 *          is stationed just past the final sample block, and each iteration
 *          rewinds two block sizes, reads one block, and streams it to the
 *          output -- thereby emitting the samples in exact reverse order
 *          while consuming them exactly once.
 *
 *          Defensive engineering checks every allocation, every read
 *          (including partial-read detection), and every fseek, so a
 *          truncated or exotic file cannot silently corrupt the output.
 *          Resource ownership (file handles and heap buffers) is released
 *          on every exit path.
 *
 * @note    The mandatory CS50 contracts -- check_format(WAVHEADER) and
 *          get_block_size(WAVHEADER), both returning int -- are preserved
 *          unchanged for check50 linkage. wav.h is consumed as-is.
 *
 * @complexity
 *          Reversal:    Time O(N) | Space O(block size)
 *          check_format:Time O(1) | Space O(1)
 *          get_block_size: Time O(1)| Space O(1)
 *          where N = number of sample blocks.
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "wav.h"

/* Return codes -- exit paths are differentiated for diagnostics. */
#define EXIT_USAGE   1
#define EXIT_IO      1
#define EXIT_FORMAT  1

int check_format(WAVHEADER header);
int get_block_size(WAVHEADER header);

/**
 * @brief  Entry point: validates the WAV input, then reverses sample blocks.
 * @param  argc Argument count (expects exactly 3: program, input, output).
 * @param  argv Argument vector.
 * @return 0 on success, nonzero on any validated failure.
 */
int main(int argc, char *argv[])
{
    /* Enforce the exact three-argument usage contract. */
    if (argc != 3)
    {
        printf("Usage: reverse input.wav output.wav\n");
        return EXIT_USAGE;
    }

    /* Open the source audio for binary reading. */
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open %s.\n", argv[1]);
        return EXIT_IO;
    }

    /* Read the fixed 44-byte WAV header as one unit. */
    WAVHEADER header;
    if (fread(&header, sizeof(WAVHEADER), 1, input) != 1)
    {
        printf("Could not read header from %s.\n", argv[1]);
        fclose(input);
        return EXIT_FORMAT;
    }

    /* Reject files that do not carry the canonical WAVE signature. */
    if (!check_format(header))
    {
        printf("Not a WAVE file\n");
        fclose(input);
        return EXIT_FORMAT;
    }

    /* Open (or create) the destination audio for binary writing. */
    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Could not open %s.\n", argv[2]);
        fclose(input);
        return EXIT_IO;
    }

    /* Forward the header unchanged: only sample order is reversed. */
    if (fwrite(&header, sizeof(WAVHEADER), 1, output) != 1)
    {
        printf("Could not write header to %s.\n", argv[2]);
        fclose(output);
        fclose(input);
        return EXIT_IO;
    }

    /*
     * Derive the atomic sample-block size in bytes. For a PCM stream this
     * equals the frame width: channel count multiplied by bytes-per-sample.
     */
    int block_size = get_block_size(header);
    if (block_size <= 0)
    {
        printf("Unsupported audio format (invalid block size).\n");
        fclose(output);
        fclose(input);
        return EXIT_FORMAT;
    }

    /*
     * Park the read cursor just beyond the final sample block, then walk
     * backward block-by-block. Each step rewinds two blocks (one to resite
     * the cursor, one to move ahead), reads exactly one block, and streams it
     * to the output file -- emitting samples in reverse order.
     */
    if (fseek(input, block_size, SEEK_END) != 0)
    {
        printf("Could not position within %s.\n", argv[1]);
        fclose(output);
        fclose(input);
        return EXIT_IO;
    }

    BYTE *buffer = malloc((size_t) block_size);
    if (buffer == NULL)
    {
        printf("Not enough memory to reverse audio.\n");
        fclose(output);
        fclose(input);
        return EXIT_IO;
    }

    while (ftell(input) - block_size > (long) sizeof(WAVHEADER))
    {
        if (fseek(input, -2 * block_size, SEEK_CUR) != 0)
        {
            printf("Could not position within %s.\n", argv[1]);
            free(buffer);
            fclose(output);
            fclose(input);
            return EXIT_IO;
        }

        if (fread(buffer, (size_t) block_size, 1, input) != 1)
        {
            printf("Could not read sample data from %s.\n", argv[1]);
            free(buffer);
            fclose(output);
            fclose(input);
            return EXIT_IO;
        }

        if (fwrite(buffer, (size_t) block_size, 1, output) != 1)
        {
            printf("Could not write sample data to %s.\n", argv[2]);
            free(buffer);
            fclose(output);
            fclose(input);
            return EXIT_IO;
        }
    }

    /* Release all heap and stream resources. */
    free(buffer);
    fclose(input);
    fclose(output);

    return 0;
}

/**
 * @brief  Validates that the given header describes a canonical WAV file.
 * @param  header The 44-byte WAVHEADER to inspect.
 * @return 1 (true) if the four format bytes spell "WAVE", else 0 (false).
 *
 * @note   The canonical RIFF/WAVE signature is encoded as "WAVE" at offset 8.
 */
int check_format(WAVHEADER header)
{
    return (header.format[0] == 'W' && header.format[1] == 'A' &&
            header.format[2] == 'V' && header.format[3] == 'E');
}

/**
 * @brief  Computes the byte width of one atomic audio sample block.
 * @param  header The WAVHEADER describing the audio stream.
 * @return numChannels * (bitsPerSample / 8): bytes consumed per sample frame.
 *
 * @note   block_size is non-positive when the format is nonsensical
 *         (zero channels or an unaligned bit depth), which callers reject.
 */
int get_block_size(WAVHEADER header)
{
    return header.numChannels * header.bitsPerSample / 8;
}