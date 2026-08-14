/**
 * @file plurality.c
 * @brief Plurality election tally: every voter names a single favourite candidate
 *        and the candidate with the most votes wins.
 * @author Ayman Elmasry — AEL Digital Studio
 * @project AEL Sovereign — CS50x 2026-2027, Problem Set 3
 *
 * Algorithm:
 *   1. Load the candidate roster from the command line (2..9 names).
 *   2. Ask for the number of voters, then repeatedly read one "Vote: " line.
 *      Each vote is validated against the roster (case-sensitive match) before
 *      the matching candidate's tally is incremented.
 *   3. Compute the winning threshold and print every candidate tied at it.
 *
 * Complexity:
 *   - vote()        : O(C) per ballot, C = candidate count (linear scan).
 *   - print_winner(): O(C) after a single linear pass for the maximum.
 *   - Overall       : O(V * C) where V = voters, C <= 9.
 */

#include <cs50.h>
#include <stdio.h>
#include <string.h>

#define MAX 9

typedef struct
{
    string name;
    int votes;
} candidate;

candidate candidates[MAX];
int candidate_count;

bool vote(string name);
void print_winner(void);

int main(int argc, string argv[])
{
    if (argc < 2)
    {
        printf("Usage: plurality [candidate ...]\n");
        return 1;
    }

    if (argc - 1 > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }

    candidate_count = argc - 1;
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
    }

    int voter_count = get_int("Number of voters: ");
    if (voter_count < 1)
    {
        printf("Invalid number of voters.\n");
        return 3;
    }

    for (int i = 0; i < voter_count; i++)
    {
        string name = get_string("Vote: ");
        if (!vote(name))
        {
            printf("Invalid vote.\n");
        }
    }

    print_winner();
    return 0;
}

/**
 * @brief Records a single vote for the named candidate if one exists.
 *
 * Performs a case-sensitive scan over the candidate roster. On a hit the
 * candidate's tally is incremented; otherwise the ballot is rejected.
 *
 * @param name The candidate name as typed by the voter.
 * @return true  if the name matched a roster entry and the vote was counted,
 *         false otherwise.
 */
bool vote(string name)
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i].name) == 0)
        {
            candidates[i].votes++;
            return true;
        }
    }
    return false;
}

/**
 * @brief Prints the name of each candidate holding the highest tally.
 *
 * The winning count is determined in one linear pass; a second pass emits
 * every candidate whose tally equals that maximum so tied races print all
 * winners, one per line.
 */
void print_winner(void)
{
    int top_votes = 0;

    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes > top_votes)
        {
            top_votes = candidates[i].votes;
        }
    }

    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes == top_votes)
        {
            printf("%s\n", candidates[i].name);
        }
    }
}
