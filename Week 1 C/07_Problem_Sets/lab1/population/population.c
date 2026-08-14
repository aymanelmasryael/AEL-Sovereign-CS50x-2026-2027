/**
 * @file population.c
 * @brief CS50x Lab 1 — Population Growth: llama herd simulator.
 *
 * @author Ayman Elmasry — AEL Digital Studio
 * @project AEL Sovereign — CS50x 2026-2027
 *
 * @details
 *   Simulates the year-over-year growth of a llama population under the
 *   course's simple demographic model: each year, new llamas equal the
 *   integer part of n/3, and departed llamas equal the integer part of
 *   n/4 (both using truncating integer division). The program reports how
 *   many years are required for the population to reach or exceed a
 *   target end size.
 *
 * Algorithm:
 *   1. Prompt for a start size, insisting it is at least 9 (MIN).
 *   2. Prompt for an end size, insisting it is at least the start size.
 *   3. Simulate years, applying n = n + (n/3) - (n/4) until n >= end.
 *   4. Report the number of years with "Years: <years>".
 *
 * Complexity:
 *   Time  — O(y): y simulated years; each is a constant-time update.
 *   Space — O(1): only scalar population and year counters.
 */

#include <cs50.h>
#include <stdio.h>

/* Minimum viable start population mandated by the lab specification. */
#define MIN_START_SIZE 9

/**
 * @brief Prompt until a valid starting population is supplied.
 *
 * @return The validated start size (at least MIN_START_SIZE).
 */
static int prompt_start_size(void)
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
 * @brief Prompt until an end population at least as large as start.
 *
 * @param start The already-validated start size.
 * @return The validated end size (greater than or equal to start).
 */
static int prompt_end_size(int start)
{
    int end;

    do
    {
        end = get_int("End size: ");
    }
    while (end < start);

    return end;
}

/**
 * @brief Count the years until the population reaches the end target.
 *
 * @param start The initial population.
 * @param end   The target population.
 * @return The number of simulated years required.
 */
static int years_to_reach(int start, int end)
{
    int years = 0;
    int population = start;

    /* Each iteration is one full year of births and deaths. Integer
       division is deliberately truncating, matching the lab's contract. */
    while (population < end)
    {
        population = population + population / 3 - population / 4;
        years++;
    }

    return years;
}

int main(void)
{
    int start = prompt_start_size();
    int end = prompt_end_size(start);

    printf("Years: %i\n", years_to_reach(start, end));

    return 0;
}
