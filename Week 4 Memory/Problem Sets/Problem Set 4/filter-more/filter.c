/**
 * @file filter.c
 * @brief Command-line driver for the problem-set image filters.
 * @author Ayman Elmasry — AEL Digital Studio
 * @project AEL Sovereign — CS50x 2026-2027, Problem Set 4
 *
 * Usage: ./filter [flag] infile outfile
 *   -g  grayscale
 *   -b  blur (applied `amount` times)
 *   -e  edges (applied `amount` times)
 *   -r  reflect
 *
 * The file is validated as a 24-bit, uncompressed, little-endian BMP before
 * its pixel array (including per-row padding) is read into a flat RGBTRIPLE
 * buffer. After the chosen filter runs, the buffer is written back out with
 * its padding restored.
 */

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#include <cs50.h>

#include "helpers.h"

int main(int argc, char *argv[])
{
    char *filters = "begr";
    char filter = getopt(argc, argv, filters);

    if (filter == '?')
    {
        printf("Invalid filter.\n");
        return 1;
    }

    if (getopt(argc, argv, filters) != -1)
    {
        printf("Only one filter allowed.\n");
        return 2;
    }

    if (argc != optind + 2)
    {
        printf("Usage: ./filter [flag] infile outfile\n");
        return 3;
    }

    char *infile = argv[optind];
    char *outfile = argv[optind + 1];

    FILE *inptr = fopen(infile, "rb");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 4;
    }

    FILE *outptr = fopen(outfile, "wb");
    if (outptr == NULL)
    {
        printf("Could not open %s.\n", outfile);
        fclose(inptr);
        return 4;
    }

    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || bi.biBitCount != 24 ||
        bi.biCompression != 0)
    {
        printf("Unsupported file format.\n");
        fclose(inptr);
        fclose(outptr);
        return 4;
    }

    int height = abs(bi.biHeight);
    int width = bi.biWidth;
    int padding = (4 - (width * sizeof(RGBTRIPLE)) % 4) % 4;

    /* Allocate the raster as a pointer-to-array so the flat buffer can be
     * passed directly to the helpers, which expect 2-D array parameters. */
    RGBTRIPLE (*image)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    if (image == NULL)
    {
        printf("Unable to allocate memory.\n");
        fclose(inptr);
        fclose(outptr);
        return 4;
    }

    for (int i = 0; i < height; i++)
    {
        fread(&image[i][0], sizeof(RGBTRIPLE), width, inptr);
        fseek(inptr, padding, SEEK_CUR);
    }

    switch (filter)
    {
        case 'g':
            grayscale(height, width, image);
            break;

        case 'r':
            reflect(height, width, image);
            break;

        case 'b':
        {
            int amount = get_int("Blur amount: ");
            for (int i = 0; i < amount; i++)
            {
                blur(height, width, image);
            }
            break;
        }

        case 'e':
        {
            int amount = get_int("Edge amount: ");
            for (int i = 0; i < amount; i++)
            {
                edges(height, width, image);
            }
            break;
        }
    }

    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    for (int i = 0; i < height; i++)
    {
        fwrite(&image[i][0], sizeof(RGBTRIPLE), width, outptr);
        for (int k = 0; k < padding; k++)
        {
            fputc(0x00, outptr);
        }
    }

    free(image);
    fclose(inptr);
    fclose(outptr);
    return 0;
}