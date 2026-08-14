/**
 * @file helpers.c
 * @brief Implementation of grayscale, reflect, blur and Sobel edge detection.
 * @author Ayman Elmasry — AEL Digital Studio
 * @project AEL Sovereign — CS50x 2026-2027, Problem Set 4 (Filter More)
 *
 * Design notes:
 *   - All rounding uses round() from math.h so per-channel averages and Sobel
 *     magnitudes land on the mathematically nearest integer.
 *   - blur() and edges() read exclusively from a pristine byte-for-byte copy
 *     of the image; the in-place write never contaminates neighbouring reads.
 *   - Sobel magnitude is clamped to 255, the ceiling of a BYTE channel.
 *
 * Complexity (W x H image):
 *   - grayscale / reflect: O(W*H).
 *   - blur:                O(W*H) with a fixed 3x3 neighbour window.
 *   - edges:               O(W*H) with a fixed 3x3 convolution window.
 */

#include "helpers.h"

#include <math.h>

/**
 * @brief Converts every pixel to its luminance, written to all three channels.
 *
 * The average is computed in floating point and rounded to the nearest
 * integer before being stored in the blue, green and red BYTE fields.
 */
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int luminance =
                (int) round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0);

            image[i][j].rgbtBlue = luminance;
            image[i][j].rgbtGreen = luminance;
            image[i][j].rgbtRed = luminance;
        }
    }
}

/**
 * @brief Mirrors the image left-to-right.
 *
 * Each row is folded around its vertical centre; the outer half of the row is
 * walked and each left pixel is swapped with its right counterpart, so the
 * symmetry swap is performed exactly once per pixel pair.
 */
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            RGBTRIPLE swap = image[i][j];
            image[i][j] = image[i][width - 1 - j];
            image[i][width - 1 - j] = swap;
        }
    }
}

/**
 * @brief Applies a 3x3 box blur.
 *
 * Each output pixel is the average of every channel value in the up-to-9 tile
 * centred on it, with out-of-bounds neighbours excluded. The image is first
 * copied so a pixel averages its original neighbours rather than previously
 * blurred values.
 */
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE snapshot[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            snapshot[i][j] = image[i][j];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int blue_sum = 0;
            int green_sum = 0;
            int red_sum = 0;
            int samples = 0;

            for (int di = -1; di <= 1; di++)
            {
                for (int dj = -1; dj <= 1; dj++)
                {
                    int ni = i + di;
                    int nj = j + dj;
                    if (ni >= 0 && ni < height && nj >= 0 && nj < width)
                    {
                        blue_sum += snapshot[ni][nj].rgbtBlue;
                        green_sum += snapshot[ni][nj].rgbtGreen;
                        red_sum += snapshot[ni][nj].rgbtRed;
                        samples++;
                    }
                }
            }

            image[i][j].rgbtBlue = (int) round((float) blue_sum / samples);
            image[i][j].rgbtGreen = (int) round((float) green_sum / samples);
            image[i][j].rgbtRed = (int) round((float) red_sum / samples);
        }
    }
}

/**
 * @brief Detects edges using the Sobel operator.
 *
 * Two 3x3 kernels — Gx for horizontal change, Gy for vertical change — are
 * convolved with the pristine copy of the image. The new channel value is
 * round(sqrt(Gx^2 + Gy^2)), capped at 255 so it fits a BYTE.
 */
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    static const int GX[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    static const int GY[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    RGBTRIPLE snapshot[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            snapshot[i][j] = image[i][j];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int gx_blue = 0, gx_green = 0, gx_red = 0;
            int gy_blue = 0, gy_green = 0, gy_red = 0;

            for (int di = -1; di <= 1; di++)
            {
                for (int dj = -1; dj <= 1; dj++)
                {
                    int ni = i + di;
                    int nj = j + dj;
                    if (ni < 0 || ni >= height || nj < 0 || nj >= width)
                    {
                        continue;
                    }

                    gx_blue += GX[di + 1][dj + 1] * snapshot[ni][nj].rgbtBlue;
                    gx_green += GX[di + 1][dj + 1] * snapshot[ni][nj].rgbtGreen;
                    gx_red += GX[di + 1][dj + 1] * snapshot[ni][nj].rgbtRed;

                    gy_blue += GY[di + 1][dj + 1] * snapshot[ni][nj].rgbtBlue;
                    gy_green += GY[di + 1][dj + 1] * snapshot[ni][nj].rgbtGreen;
                    gy_red += GY[di + 1][dj + 1] * snapshot[ni][nj].rgbtRed;
                }
            }

            int blue = (int) round(sqrt((double) (gx_blue * gx_blue + gy_blue * gy_blue)));
            int green = (int) round(sqrt((double) (gx_green * gx_green + gy_green * gy_green)));
            int red = (int) round(sqrt((double) (gx_red * gx_red + gy_red * gy_red)));

            image[i][j].rgbtBlue = (blue > 255) ? 255 : blue;
            image[i][j].rgbtGreen = (green > 255) ? 255 : green;
            image[i][j].rgbtRed = (red > 255) ? 255 : red;
        }
    }
}