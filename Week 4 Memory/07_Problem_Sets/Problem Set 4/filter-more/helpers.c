#include <math.h>
#include <string.h>
#include "helpers.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            int average = round((float)(image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3);
            // Set RGB values to average
            image[i][j].rgbtRed = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtBlue = average;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width / 2; ++j)
        {
            // swap
            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][width - j - 1];
            image[i][width - j - 1] = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int total_red = 0;
            int total_green = 0;
            int total_blue = 0;
            int cell_count = 0;  // neighbour box count

            for (int r = -1; r < 2; r++)
            {
                int row = i + r;
                if (row < 0 || row >= height)  // pass
                {
                    continue;
                }

                for (int c = -1; c < 2; c++)
                {
                    int col = j + c;
                    if (col < 0 || col >= width)  // pass
                    {
                        continue;
                    }

                    total_red += image[row][col].rgbtRed;
                    total_green += image[row][col].rgbtGreen;
                    total_blue += image[row][col].rgbtBlue;
                    cell_count++;
                }
            }
            // equalize RGB to average RGB values
            temp[i][j].rgbtRed = round((float)total_red / cell_count);
            temp[i][j].rgbtGreen = round((float)total_green / cell_count);
            temp[i][j].rgbtBlue = round((float)total_blue / cell_count);
        }
    }

    memcpy(image, temp, sizeof(RGBTRIPLE) * height * width);
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width];
    // for detecting the borders
    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int x_red = 0;
            int x_green = 0;
            int x_blue = 0;
            int y_red = 0;
            int y_green = 0;
            int y_blue = 0;
            for (int r = -1; r < 2; r++)
            {
                int row = i + r;
                if (row < 0 || row >= height)  // pass
                {
                    continue;
                }
                for (int c = -1; c < 2; c++)
                {
                    int col = j + c;
                    if (col < 0 || col >= width)  // pass
                    {
                        continue;
                    }
                    // multiply with Gx and Gy
                    x_red += image[row][col].rgbtRed * Gx[r + 1][c + 1];
                    x_green += image[row][col].rgbtGreen * Gx[r + 1][c + 1];
                    x_blue += image[row][col].rgbtBlue * Gx[r + 1][c + 1];
                    y_red += image[row][col].rgbtRed * Gy[r + 1][c + 1];
                    y_green += image[row][col].rgbtGreen * Gy[r + 1][c + 1];
                    y_blue += image[row][col].rgbtBlue * Gy[r + 1][c + 1];
                }
            }
            int red = round(sqrt(x_red * x_red + y_red * y_red));
            int green = round(sqrt(x_green * x_green + y_green * y_green));
            int blue = round(sqrt(x_blue * x_blue + y_blue * y_blue));
            // values can not be greater than 255
            if (red > 255)
            {
                red = 255;
            }
            if (green > 255)
            {
                green = 255;
            }
            if (blue > 255)
            {
                blue = 255;
            }
            temp[i][j].rgbtRed = red;
            temp[i][j].rgbtGreen = green;
            temp[i][j].rgbtBlue = blue;
        }
    }
    memcpy(image, temp, sizeof(RGBTRIPLE) * height * width);
    return;
}
