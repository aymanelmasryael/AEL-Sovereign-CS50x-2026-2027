/**
 * ============================================================================
 *  PROJECT : AEL Sovereign — CS50x 2026-2027 Master Solutions
 *  FILE    : mario.c
 *  AUTHOR  : Ayman Elmasry — AEL Digital Studio
 *  ---------------------------------------------------------------------------
 *  DESCRIPTION
 *    Renders the classic Super Mario "more comfortable" twin pyramid: a
 *    left-aligned right-triangle of bricks mirrored across a two-space gutter
 *    into an identical right triangle. Satisfies the "More" problem set by
 *    producing horizontal symmetry with a gap, built row by row.
 *
 *  ALGORITHM (STEP BY STEP)
 *    1. Prompt for a height; accept only integers in the inclusive range
 *       [1, 8], looping until a compliant value is entered.
 *    2. For each row r (1-based) from 1..height:
 *         a. Print (height - r) leading spaces, right-aligning the left block.
 *         b. Print r '#', forming the left-aligned ascending staircase.
 *         c. Print the canonical two-space gutter separating the halves.
 *         d. Print r '#' again, forming the mirrored descending staircase.
 *         e. Terminate the row with a newline.
 *
 *  INDUCTIVE COMPLEXITY
 *    Row r emits (height - r) + r + 2 + r = height + r + 2 characters, so the
 *    total work telescopes to Theta(height^2) — inherent to writing every
 *    character of the output, hence asymptotically optimal for this task.
 *
 *  COMPLEXITY
 *    Time  : O(height^2)   — the output grid itself is Theta(height^2) chars.
 *    Space : O(1)          — only scalar loop counters are retained.
 *
 *  COMPILE  : gcc -o mario mario.c -lcs50
 * ============================================================================
 */

#include <cs50.h>
#include <stdio.h>

/* Problem-set-specified bounds for the pyramid height. */
#define MIN_HEIGHT 1
#define MAX_HEIGHT 8

/* The fixed-width gutter separating the two mirrored half-pyramids. */
#define GUTTER_WIDTH 2

/* Forward declarations for a modular rendering pipeline. */
int get_height(void);
void render_pyramid(int height);
void render_leaning_tower(const int height, int row);

int main(void)
{
    int height = get_height();
    render_pyramid(height);
    return 0;
}

/**
 * Prompts the user and returns a height within the permitted range.
 * The do/while guarantees at least one prompt and re-asks until valid.
 */
int get_height(void)
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
 * Orchestrates the full masonry. Height-informed arts: for each row we
 * delegate the construction of the ascending staircase to the left and
 * emit the gutter plus the descending staircase directly.
 */
void render_pyramid(int height)
{
    for (int row = 1; row <= height; row++)
    {
        /* Left half: leading spaces then `row` bricks. */
        render_leaning_tower(height, row);

        /* Central gutter of two spaces. */
        for (int g = 0; g < GUTTER_WIDTH; g++)
        {
            printf(" ");
        }

        /* Right half: `row` bricks with no trailing spaces. */
        for (int b = 0; b < row; b++)
        {
            printf("#");
        }

        printf("\n");
    }
}

/**
 * Emits the left-aligned ascending staircase for a given row.
 *
 * For row `row` the number of required prefixes is (height - row) spaces,
 * followed by exactly `row` bricks. Row height is `const` to signal that the
 * rendering decisions derive from the immutable total height only.
 */
void render_leaning_tower(const int height, int row)
{
    /* Padding: keep the staircase visually right-justified to the gutter. */
    for (int space = 0; space < height - row; space++)
    {
        printf(" ");
    }

    /* Bricks: the working run-length for this row. */
    for (int brick = 0; brick < row; brick++)
    {
        printf("#");
    }
}
