/**
 * @file helpers.h
 * @brief Public interface of the image-processing filters for Problem Set 4.
 * @author Ayman Elmasry — AEL Digital Studio
 * @project AEL Sovereign — CS50x 2026-2027, Problem Set 4
 *
 * Each filter operates in place on a 2-D array of RGBTRIPLE pixels and never
 * changes the dimensions of the image.
 */

#ifndef HELPERS_H
#define HELPERS_H

#include "bmp.h"

void grayscale(int height, int width, RGBTRIPLE image[height][width]);
void reflect(int height, int width, RGBTRIPLE image[height][width]);
void blur(int height, int width, RGBTRIPLE image[height][width]);
void edges(int height, int width, RGBTRIPLE image[height][width]);

#endif
