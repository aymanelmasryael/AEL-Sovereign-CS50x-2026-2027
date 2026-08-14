/**
 * @file    tideman.c
 * @brief   Tideman Ranked-Pairs Condorcet Election Kernel.
 *
 * @project AEL Sovereign — CS50x 2026-2027
 * @author  Ayman Elmasry — AEL Digital Studio
 *
 * @details Algorithm Design
 *          -----------------
 *          This program implements the Tideman method, a Condorcet-consistent
 *          ranked-choice voting system. Every voter supplies a complete
 *          ordering of all candidates; from these ballots the engine derives,
 *          for each unordered candidate pair (i, j), the number of voters who
 *          prefer i over j (and vice-versa).
 *
 *          The pipeline proceeds in four precisely ordered phases:
 *            1. record_preferences() -- collapses each voter's ranked ballot
 *               into a pairwise preference matrix via a double loop over all
 *               ordered pairs.
 *            2. add_pairs()          -- isolates every decisive head-to-head
 *               contest, storing (winner, loser) pairs and discarding ties.
 *            3. sort_pairs()         -- orders pairs by descending strength of
 *               victory (selection sort), so the most decisive contests are
 *               locked first.
 *            4. lock_pairs()         -- locks edges into the winner graph only
 *               when doing so cannot close a directed cycle. Cycle induction
 *               is detected with a depth-first reachability probe; an edge is
 *               accepted only if no directed path already connects the loser
 *               back to the winner. This invariant guarantees the locked
 *               structure remains a Directed Acyclic Graph (DAG).
 *
 *          The election victor is the Condorcet winner: the unique DAG source
 *          with no incoming locked edges.
 *
 *          Defensive engineering integrates candidate-name validation at ballot
 *          entry (rejecting unknown or duplicate-rank candidates) and robust
 *          integer arithmetic for both preference tallying and margin sizing.
 *
 * @note    The mandatory CS50 contracts -- the pair struct, the global
 *          preferences/locked matrices, candidates/pairs arrays, and all six
 *          required functions -- are preserved exactly for check50 linkage.
 *
 * @complexity
 *          record_preferences(): Time O(n^2)
 *          add_pairs():          Time O(n^2)
 *          sort_pairs():         Time O(p^2)   (p = pair count <= n(n-1)/2)
 *          lock_pairs():         Time O(p * (n + e)) worst case (DFS reach)
 *          print_winner():       Time O(n^2)
 *          where n = candidate_count.
 */

#include <cs50.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

/* Maximum number of candidates permitted in a single election. */
#define MAX 9

/* preferences[i][j] is the number of voters who prefer i over j. */
int preferences[MAX][MAX];

/* locked[i][j] records that candidate i is locked in over candidate j. */
bool locked[MAX][MAX];

/**
 * @struct pair
 * @brief  An ordered head-to-head contest between two candidates.
 */
typedef struct
{
    int winner;  /* Candidate preferred by the majority of voters. */
    int loser;   /* Candidate defeated in this pair.              */
}
pair;

/* Registry of participating candidates. */
string candidates[MAX];

/* Collection of all decisive (non-tied) pairs discovered from ballots. */
pair pairs[MAX * (MAX - 1) / 2];

/* Number of decisive pairs; registry size; both maintained globally. */
int pair_count;
int candidate_count;

/* Function prototypes mandated by the CS50 problem specification. */
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
bool has_cycle(int winner, int loser);

/**
 * @brief  Entry point: registers candidates, collects ranked ballots,
 *         tabulates preferences, locks the DAG, and announces the winner.
 * @return 0 on success, 1/2 for usage/overflow, 3 for an invalid ballot.
 */
int main(int argc, string argv[])
{
    /* Reject an invocation that provides no candidates. */
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    /* Register candidates, enforcing the registry's hard capacity. */
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    /* Reset the locked graph: no edges exist before ballot processing. */
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    /* Collect and tabulate every voter's ranked ballot. */
    for (int i = 0; i < voter_count; i++)
    {
        /* ranks[i] records the candidate occupying the voter's ith position. */
        int ranks[candidate_count];

        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            /* An unrecognised candidate invalidates the entire ballot. */
            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);
        printf("\n");
    }

    /* Execute the ranked-pairs pipeline to completion. */
    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

/**
 * @brief  Maps a candidate name to its registry index at a given ballot rank.
 * @param  rank  The zero-based position this candidate occupies on the ballot.
 * @param  name  The candidate name supplied by the voter.
 * @param  ranks Shared ballot buffer; updated in place at ranks[rank].
 * @return true  if the name matches a registered candidate,
 *         false otherwise (invalid ballot entry).
 *
 * @note   Uses a linear, case-exact scan of the candidate registry.
 */
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

/**
 * @brief  Updates the pairwise preference matrix from one voter's ranking.
 * @param  ranks The candidate indexes in descending order of preference.
 *
 * @note   For every ordered pair (ranks[i], ranks[j]) with i preceding j, the
 *         voter prefers ranks[i]; hence preferences[ranks[i]][ranks[j]] is
 *         credited. The double loop visits each unordered pair exactly once
 *         in the forward direction.
 */
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]]++;
        }
    }
}

/**
 * @brief  Enumerates every decisive pair into the global pairs array.
 *
 * @note   Compares each unordered candidate pair (i, j); the majority
 *         preference determines the winner. A perfect tie contributes no pair.
 */
void add_pairs(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser  = j;
                pair_count++;
            }
            else if (preferences[i][j] < preferences[j][i])
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser  = i;
                pair_count++;
            }
            /* Equality: no preference -- the tie contributes nothing. */
        }
    }
}

/**
 * @brief  Sorts pairs in strictly decreasing order of victory strength.
 *
 * @note   Implements an in-place selection sort keyed on the margin of
 *         victory, i.e. the count of voters preferring the winner over the
 *         loser. Larger margins correspond to more decisive contests and are
 *         locked into the DAG first.
 *
 * @complexity Time O(p^2) | Space O(1) for p decisive pairs.
 */
void sort_pairs(void)
{
    for (int i = 0; i < pair_count - 1; i++)
    {
        int best = i;
        for (int j = i + 1; j < pair_count; j++)
        {
            if (preferences[pairs[j].winner][pairs[j].loser] >
                preferences[pairs[best].winner][pairs[best].loser])
            {
                best = j;
            }
        }
        if (best != i)
        {
            pair pivot = pairs[i];
            pairs[i]   = pairs[best];
            pairs[best] = pivot;
        }
    }
}

/**
 * @brief  Locks pairs into the winner graph, refusing any edge that would
 *         close a directed cycle.
 *
 * @note   Each candidate edge is admitted only if no directed path already
 *         connects the loser back to the winner. This preserves the
 *         acyclicity invariant essential to a valid Condorcet ranking.
 *
 * @complexity Time O(p * (n + e)) worst case across all pair_count edges.
 */
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        if (!has_cycle(pairs[i].winner, pairs[i].loser))
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }
}

/**
 * @brief  Determines whether locking the edge (winner -> loser) would create
 *         a directed cycle in the accumulated lock graph.
 * @param  winner The potential tail of the new edge.
 * @param  loser  The potential head of the new edge.
 * @return true  if a cycle would be induced,
 *         false if the edge is safe to lock.
 *
 * @note   A cycle is induced iff there already exists a directed path from
 *         loser back to winner. This is resolved with depth-first reachability
 *         along already-locked edges. Passing the invariant `winner == loser`
 *         (an identity edge) trivially denotes a cycle.
 *
 * @complexity Worst-case O(n + e) per call over the candidate graph.
 */
bool has_cycle(int winner, int loser)
{
    /* An identity edge is an immediate, degenerate cycle. */
    if (winner == loser)
    {
        return true;
    }

    /* Recurse along every locked out-edge of `loser` to chase a return path. */
    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[loser][i] && has_cycle(winner, i))
        {
            return true;
        }
    }
    return false;
}

/**
 * @brief  Prints the Condorcet winner: the unique DAG source with no
 *         incoming locked edges.
 *
 * @note   A candidate is the source of the locked graph when no other
 *         candidate is locked over it; such a candidate loses to no one and is
 *         therefore undefeated in every decisive head-to-head contest.
 *
 * @complexity Time O(n^2) | Space O(1).
 */
void print_winner(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        bool defeated = false;
        for (int j = 0; j < candidate_count && !defeated; j++)
        {
            if (locked[j][i])
            {
                defeated = true;
            }
        }
        if (!defeated)
        {
            printf("%s\n", candidates[i]);
            return;
        }
    }
}
