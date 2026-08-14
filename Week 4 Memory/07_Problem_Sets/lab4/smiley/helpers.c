/**
 * @file helpers.c
 * @brief Implementation of the black-to-teal colorization filter.
 * @author Ayman Elmasry — AEL Digital Studio
 * @project AEL Sovereign — CS50x 2026-2027, Lab 4
 *
 * Algorithm:
 *   Every pixel whose blue, green and red channels are all 0x00 (pure black)
 *   is recoloured to a single teal tone {0x00, 0xff, 0x00}. Pixels with any
 *   nonzero channel are left untouched, preserving the drawing's outlines.
 *
 * Complexity: O(W*H) time over the pixel grid; in-place, no extra memory.
 */

#include "helpers.h"

void colorize(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE teal = {0x00, 0xff, 0x00};

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (image[i][j].rgbtBlue == 0x00 && image[i][j].rgbtGreen == 0x00 &&
                image[i][j].rgbtRed == 0x00)
            {
                image[i][j] = teal;
            }
        }
    }
}