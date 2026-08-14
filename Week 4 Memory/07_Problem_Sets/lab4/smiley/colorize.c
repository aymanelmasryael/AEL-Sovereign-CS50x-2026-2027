/**
 * @file colorize.c
 * @brief BMP driver for the smiley colorization lab.
 * @author Ayman Elmasry — AEL Digital Studio
 * @project AEL Sovereign — CS50x 2026-2027, Lab 4
 *
 * Reads a 24-bit BMP, hands its pixel array to colorize() and writes the
 * recoloured result to outfile.bmp, preserving per-row padding bytes.
 */

#include <stdio.h>
#include <stdlib.h>

#include "helpers.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: colorize infile outfile\n");
        return 1;
    }

    FILE *inptr = fopen(argv[1], "rb");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", argv[1]);
        return 4;
    }

    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || bi.biBitCount != 24 ||
        bi.biCompression != 0)
    {
        fclose(inptr);
        printf("Unsupported file format.\n");
        return 4;
    }

    int height = abs(bi.biHeight);
    int width = bi.biWidth;
    int padding = (4 - (width * sizeof(RGBTRIPLE)) % 4) % 4;

    /* Allocate the raster as a pointer-to-array so the flat buffer can be
     * passed directly to colorize(), which expects a 2-D array parameter. */
    RGBTRIPLE (*image)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    if (image == NULL)
    {
        fclose(inptr);
        printf("Not enough memory to store image.\n");
        return 4;
    }

    for (int i = 0; i < height; i++)
    {
        fread(&image[i][0], sizeof(RGBTRIPLE), width, inptr);
        fseek(inptr, padding, SEEK_CUR);
    }

    colorize(height, width, image);

    FILE *outptr = fopen("outfile.bmp", "wb");
    if (outptr == NULL)
    {
        fclose(inptr);
        free(image);
        printf("Could not open outfile.bmp.\n");
        return 4;
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

    fclose(inptr);
    fclose(outptr);
    free(image);
    return 0;
}