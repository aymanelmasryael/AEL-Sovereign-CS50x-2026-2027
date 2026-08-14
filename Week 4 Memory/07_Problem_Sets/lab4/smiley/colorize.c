/**
 * @file    colorize.c
 * @brief   BMP Colour-Key Recoloriser (Black-to-Teal Transformation).
 *
 * @project AEL Sovereign — CS50x 2026-2027
 * @author  Ayman Elmasry — AEL Digital Studio
 *
 * @details Algorithm Design
 *          -----------------
 *          This program recolours the "ink" of a black-and-white BMP image.
 *          It loads a 24-bit uncompressed BMP 4.0, walks every pixel, and
 *          replaces pixels that are exactly black (0x00 on every channel)
 *          with a signature AEL palette colour; all other pixels are left
 *          untouched.
 *
 *          The transformation is a classic colour-key operation: an O(h*w)
 *          pass that reads each pixel once, tests the three channel bytes for
 *          the key colour, and writes the replacement. Because the source
 *          bitmap is a solid black-and-white sprite, testing for exact black
 *          cleanly isolates the ink from the white background.
 *
 *          Defensive engineering validates the BMP signature, the 24-bit
 *          uncompressed format (biBitCount == 24, biCompression == 0), the
 *          header offsets, the heap allocation, and every file operation --
 *          with all acquired resources released on every exit path. Padding
 *          is honoured on both the read and write passes so the file remains
 *          byte-identical in structure, differing only in the colour data.
 *
 * @note    The mandatory contract -- colorize(int height, int width,
 *          RGBTRIPLE image[height][width]) -- is preserved; the helpers module
 *          drives the actual pixel loop.
 *
 * @complexity
 *          Recolouring: Time O(h*w) | Space O(1)
 *          I/O staging: Time O(h*w) | Space O(h*w) for the loaded frame.
 */

#include <stdio.h>
#include <stdlib.h>

#include "helpers.h"

/**
 * @brief  Entry point: loads a BMP, recolours black pixels, saves the result.
 * @param  argc Argument count (expects exactly 3: program, input, output).
 * @param  argv Argument vector.
 * @return 0 on success, nonzero on any validated failure.
 */
int main(int argc, char *argv[])
{
    /* Enforce the exact three-argument usage contract. */
    if (argc != 3)
    {
        printf("Usage: colorize infile outfile\n");
        return 1;
    }

    /* Remember the input and output filenames. */
    char *infile = argv[1];
    char *outfile = argv[2];

    /* Open the source bitmap for binary reading. */
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 4;
    }

    /* Open (or create) the destination bitmap for binary writing. */
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        printf("Could not create %s.\n", outfile);
        return 5;
    }

    /* Read the 14-byte BMP file header. */
    BITMAPFILEHEADER bf;
    if (fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr) != 1)
    {
        printf("Could not read file header from %s.\n", infile);
        fclose(outptr);
        fclose(inptr);
        return 6;
    }

    /* Read the 40-byte BMP info header. */
    BITMAPINFOHEADER bi;
    if (fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr) != 1)
    {
        printf("Could not read info header from %s.\n", infile);
        fclose(outptr);
        fclose(inptr);
        return 6;
    }

    /*
     * Accept only canonical 24-bit uncompressed BMP 4.0 files: the "BM"
     * type marker, a 54-byte pixel offset, a 40-byte info header, 24 bits per
     * pixel, and zero compression.
     */
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        printf("Unsupported file format.\n");
        return 6;
    }

    /* Derive the image dimensions from the info header. */
    int height = abs(bi.biHeight);
    int width  = bi.biWidth;

    /* Allocate a single contiguous block holding the whole scanline array. */
    RGBTRIPLE (*image)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    if (image == NULL)
    {
        printf("Not enough memory to store image.\n");
        fclose(outptr);
        fclose(inptr);
        return 7;
    }

    /* Scanlines are padded to a multiple of 4 bytes. */
    int padding = (4 - (width * sizeof(RGBTRIPLE)) % 4) % 4;

    /* Stream each scanline into memory, skipping the inter-row padding. */
    for (int i = 0; i < height; i++)
    {
        if (fread(image[i], sizeof(RGBTRIPLE), width, inptr) != (size_t) width)
        {
            printf("Could not read pixel data from %s.\n", infile);
            free(image);
            fclose(outptr);
            fclose(inptr);
            return 7;
        }
        fseek(inptr, padding, SEEK_CUR);
    }

    /* Perform the black-to-palette recolouring pass. */
    colorize(height, width, image);

    /* Relay the two headers verbatim. */
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    /* Persist the recoloured scanlines, re-appending row padding. */
    for (int i = 0; i < height; i++)
    {
        fwrite(image[i], sizeof(RGBTRIPLE), width, outptr);
        for (int k = 0; k < padding; k++)
        {
            fputc(0x00, outptr);
        }
    }

    /* Release the frame buffer and both stream handles. */
    free(image);
    fclose(inptr);
    fclose(outptr);

    return 0;
}