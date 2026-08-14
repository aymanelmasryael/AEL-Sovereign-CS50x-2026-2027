/**
 * @file    plurality.c
 * @brief   Plurality (First-Past-The-Post) Election Tabulation Engine.
 *
 * @project AEL Sovereign — CS50x 2026-2027
 * @author  Ayman Elmasry — AEL Digital Studio
 *
 * @details Algorithm Design
 *          -----------------
 *          This program implements a plurality, or "first-past-the-post",
 *          election. Each voter casts a single vote for exactly one candidate;
 *          the candidate who accumulates the greatest number of votes wins.
 *          In the event of a tie, every candidate sharing the top vote count
 *          is declared a winner.
 *
 *          The design centres on a fixed-capacity candidate registry (MAX) that
 *          is populated directly from the command line. Two linear sweeps drive
 *          the core logic:
 *            1. A single O(n) scan per ballot in vote() resolves a voter's
 *               name against the registry via case-exact string comparison.
 *            2. A two-pass O(n) traversal in print_winner() first determines
 *               the maximum vote count, then emits every candidate whose tally
 *               equals that maximum (thereby handling multi-way ties cleanly).
 *
 *          Defensive engineering guards every externally supplied input:
 *          argument count, candidate-list overflow, and unrecognised ballot
 *          names are all rejected with explicit diagnostics rather than
 *          silently tolerated.
 *
 * @note    The mandatory CS50 contracts -- the candidate struct, the global
 *          candidates array, candidate_count, vote() and print_winner() -- are
 *          preserved verbatim so that check50 can link against this unit.
 *
 * @complexity
 *          vote():        Time O(n)   | Space O(1)
 *          print_winner():Time O(n)   | Space O(1)
 *          main():        Time O(v*n) | Space O(n)
 *          where n = candidate_count and v = voter_count.
 */

#include <cs50.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

/* Maximum number of candidates permitted in a single election. */
#define MAX 9

/**
 * @struct candidate
 * @brief  A single contestant bearing a display name and an accumulator of
 *         ballots received.
 */
typedef struct
{
    string name;   /* Candidate's registered voting name. */
    int    votes;  /* Running tally of cast votes.       */
}
candidate;

/* Global registry of all registered candidates. */
candidate candidates[MAX];

/* Number of candidates currently registered for this election. */
int candidate_count;

/* Function prototypes mandated by the CS50 problem specification. */
bool vote(string name);
void print_winner(void);

/**
 * @brief  Entry point: validates arguments, registers candidates, collects
 *         all ballots, and proclaims the winner(s).
 * @param  argc Number of command-line arguments (program name included).
 * @param  argv Vector of argument strings.
 * @return 0 on success, 1 for usage error, 2 for candidate overflow.
 */
int main(int argc, string argv[])
{
    /* Reject an invocaton that supplies no candidate names. */
    if (argc < 2)
    {
        printf("Usage: plurality [candidate ...]\n");
        return 1;
    }

    /*
     * Register every command-line argument as a candidate, enforcing the
     * hard ceiling imposed by the fixed-size registry.
     */
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name  = argv[i + 1];
        candidates[i].votes = 0;   /* All ballots begin at zero. */
    }

    /* Query the electorate's size, then collect every vote. */
    int voter_count = get_int("Number of voters: ");
    for (int i = 0; i < voter_count; i++)
    {
        string name = get_string("Vote: ");

        /* A name that matches no registered candidate is an invalid ballot. */
        if (!vote(name))
        {
            printf("Invalid vote.\n");
        }
    }

    /* Tabulate and broadcast the result. */
    print_winner();
}

/**
 * @brief  Registers a single ballot for the candidate whose name matches.
 * @param  name The candidate name written on the ballot.
 * @return true  if the ballot was accepted,
 *         false if no registered candidate matches (invalid vote).
 *
 * @note   This performs a linear, case-sensitive scan of the candidate
 *         registry; the first exact match is credited with the vote.
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
 * @brief  Prints the name(s) of the candidate(s) with the greatest tally.
 *
 * @note   A first pass computes the peak vote total; a second pass prints
 *         every candidate reaching that peak, elegantly accommodating ties.
 */
void print_winner(void)
{
    /* Locate the maximum number of votes cast for any single candidate. */
    int maximum = 0;
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes > maximum)
        {
            maximum = candidates[i].votes;
        }
    }

    /* Emit every candidate whose tally equals the computed maximum. */
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes == maximum)
        {
            printf("%s\n", candidates[i].name);
        }
    }
}
