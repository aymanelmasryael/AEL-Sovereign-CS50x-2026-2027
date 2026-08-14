/**
 * ============================================================================
 *  PROJECT : AEL Sovereign — CS50x 2026-2027 Master Solutions
 *  FILE    : population.c
 *  AUTHOR  : Ayman Elmasry — AEL Digital Studio
 *  ---------------------------------------------------------------------------
 *  DESCRIPTION
 *    Models exponential llama population growth as described by CS50's Lab 1.
 *    Each simulated year every member can birth one llama (population / 3),
 *    and every fourth llama is lost to natural attrition (population / 4).
 *    The task is to compute how many full years elapse before the population
 *    reaches or strictly exceeds a user-supplied target size.
 *
 *  ALGORITHM (STEP BY STEP)
 *    1. Prompt for start size; reject any value below 9 (a vigorous colony
 *       cannot survive below this threshold — a biological feasibility lower
 *       bound hard-coded by the problem specification).
 *    2. Prompt for end size; reject any value not strictly greater than the
 *       start size, guaranteeing the simulation always terminates.
 *    3. While the current population remains strictly below the target:
 *         a. Record births  = population / 3   (integer division).
 *         b. Record deaths  = population / 4   (integer division).
 *         c. population += births - deaths
 *         d. Advance the years counter by one.
 *    4. Report the elapsed years.
 *
 *  INTEGER-DIVISION & LOOP-INVARIANT NOTES
 *    Integer division intentionally truncates toward zero, mirroring "whole
 *    llamas only" population accounting. The loop structure guarantees the
 *    years counter increments exactly once per simulated calendar year, so
 *    the invariant "years == number of completed growth cycles" always holds.
 *
 *  COMPLEXITY
 *    Time  : O(log endSize) — growth is roughly geometric (net ~7.1% per
 *                             year ~ +1/12), so years grow logarithmically.
 *    Space : O(1) — only scalar bookkeeping variables.
 *
 *  COMPILE  : gcc -o population population.c -lcs50
 * ============================================================================
 */

#include <cs50.h>
#include <stdio.h>

/* Minimum viable colony size mandated by the problem specification. */
#define MIN_START_SIZE 9

/* Forward declarations for a modular, self-documenting design. */
int get_start_size(void);
int get_end_size(int start);
int years_until_target(int start, int target);

int main(void)
{
    int start = get_start_size();
    int target = get_end_size(start);

    int years = years_until_target(start, target);
    printf("Years: %d\n", years);

    return 0;
}

/**
 * Prompts the user for a valid starting population.
 * Rejects any value below MIN_START_SIZE until one is supplied.
 */
int get_start_size(void)
{
    int start;
    do
    {
        start = get_int("Start size: ");
    }
    while (start < MIN_START_SIZE);
    return start;
}

/**
 * Prompts the user for a valid end (target) population.
 * The target must strictly exceed the starting population so that the
 * growth simulation has a well-defined, terminating objective.
 */
int get_end_size(int start)
{
    int target;
    do
    {
        target = get_int("End size: ");
    }
    while (target < start);
    return target;
}

/**
 * Simulates annual llama growth cycles and returns the number of whole
 * years required for the population to reach the target size.
 *
 * LOOP INVARIANT: at the top of every iteration, `current` reflects the
 * population at the beginning of the current un-simulated year, and
 * `years` equals the number of completed cycles so far.
 */
int years_until_target(int start, int target)
{
    int current = start;
    int years = 0;

    while (current < target)
    {
        int births = current / 3;      /* one-third of the herd is born  */
        int deaths = current / 4;      /* one-quarter of the herd is lost */

        current = current + births - deaths;
        years++;                       /* one full calendar year elapsed */
    }

    return years;
}
