/**
 * @file mario.c
 * @brief CS50x Problem Set 1 — Mario (More): twin brick pyramids.
 *
 * @author Ayman Elmasry — AEL Digital Studio
 * @project AEL Sovereign — CS50x 2026-2027
 *
 * @details
 *   Recreates the two "half-pyramids" from the original Super Mario Bros.
 *   level, rendered entirely from '#'. The user is prompted for a height
 *   restricted to the inclusive range [1, 8]; any other value is rejected
 *   and the prompt is repeated. The left pyramid is right-aligned via
 *   leading spaces, and the right pyramid is its horizontal mirror image,
 *   separated by exactly two spaces of gutter.
 *
 * Algorithm:
 *   1. Prompt (and re-prompt) until a valid height h in [1, 8] is given.
 *   2. For each row r in 1..h:
 *        - print (h - r) leading spaces (right-alignment),
 *        - print r hashes for the left pyramid,
 *        - print a fixed two-space gap,
 *        - print r hashes for the right pyramid,
 *        - print a newline.
 *
 * Complexity:
 *   Time  — O(h^2): each row emits O(h) characters, so the total output
 *           is bounded by O(h^2).
 *   Space — O(1): only scalar state; output is streamed, never buffered.
 */

#include <cs50.h>
#include <stdio.h>

/* Contract constraints from the problem specification. */
#define MIN_HEIGHT 1
#define MAX_HEIGHT 8
#define PYRAMID_GAP 2

/**
 * @brief Prompt the user until a height within [MIN_HEIGHT, MAX_HEIGHT].
 *
 * @return The validated height entered by the user.
 */
static int prompt_height(void)
{
    int height;

    do
    {
        height = get_int("Height: ");
    }
    while (height < MIN_HEIGHT || height > MAX_HEIGHT);

    return height;
}

/**
 * @brief Print one row of both pyramids for the given height.
 *
 * @param row    The 1-based row number being rendered.
 * @param height The total number of rows in the pyramids.
 */
static void print_row(int row, int height)
{
    /* Leading spaces push the left pyramid flush against the gutter. */
    for (int spaces = 0; spaces < height - row; spaces++)
    {
        printf(" ");
    }

    /* Left pyramid bricks. */
    for (int bricks = 0; bricks < row; bricks++)
    {
        printf("#");
    }

    /* The fixed gutter that separates the two pyramids. */
    for (int gap = 0; gap < PYRAMID_GAP; gap++)
    {
        printf(" ");
    }

    /* Right pyramid bricks; no leading spaces are required here. */
    for (int bricks = 0; bricks < row; bricks++)
    {
        printf("#");
    }

    printf("\n");
}

int main(void)
{
    int height = prompt_height();

    for (int row = 1; row <= height; row++)
    {
        print_row(row, height);
    }

    return 0;
}
