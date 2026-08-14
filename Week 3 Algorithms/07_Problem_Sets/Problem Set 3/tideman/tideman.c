/**
 * @file tideman.c
 * @brief Tideman ranked-choice election: voters rank every candidate and the
 *        winner is the source of the locked, acyclic "strongest win" graph.
 * @author Ayman Elmasry — AEL Digital Studio
 * @project AEL Sovereign — CS50x 2026-2027, Problem Set 3
 *
 * Algorithm:
 *   1. vote() records one ballot rank at a time into the ranks[] array,
 *      rejecting unknown names or any candidate already ranked on this ballot.
 *   2. record_preferences() turns each completed ballot into pairwise counts.
 *   3. add_pairs() derives one directed "preference edge" per unordered pair.
 *   4. sort_pairs() orders those edges by margin of victory (largest first).
 *   5. lock_pairs() locks edges in that order only when adding the edge cannot
 *      form a cycle (checked with a depth-first path search from loser to
 *      winner through the edges locked so far).
 *   6. print_winner() prints the candidate who is never a loser in the graph.
 *
 * Complexity:
 *   - vote()             : O(C) name scan plus O(rank) duplicate check.
 *   - record_preferences: O(C^2) per ballot.
 *   - add_pairs          : O(C^2).
 *   - sort_pairs         : O(P log P), P <= C(C-1)/2.
 *   - lock_pairs         : O(P * C^2) in the worst case per DFS walk.
 *   - print_winner       : O(C^2).
 */

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 9

typedef struct
{
    string name;
    int votes;
} candidate;

typedef struct
{
    int winner;
    int loser;
} pair;

candidate candidates[MAX];
int candidate_count;
pair pairs[MAX * (MAX - 1) / 2];
int pair_count;
int preferences[MAX][MAX];
bool locked[MAX][MAX];

bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
int pair_margin_compare(const void *a, const void *b);
bool edge_creates_cycle(int winner, int current);

int main(int argc, string argv[])
{
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
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

    pair_count = 0;

    int voter_count = get_int("Number of voters: ");
    if (voter_count < 1)
    {
        printf("Invalid number of voters.\n");
        return 3;
    }

    int ranks[MAX];
    for (int i = 0; i < voter_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            ranks[j] = -1;
        }

        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);
            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 4;
            }
        }

        record_preferences(ranks);
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

/**
 * @brief Records a single ranked choice onto a voter's ballot.
 *
 * The candidate named at rank r is stored in ranks[r] as its roster index,
 * provided the name is on the roster and has not already been assigned an
 * earlier rank on this same ballot (checked by scanning ranks[0..r-1]).
 *
 * @param rank  The current preference position being filled (0-based).
 * @param name  The candidate name offered by the voter.
 * @param ranks Working array in which each filled slot holds a roster index.
 * @return true  if the choice was accepted,
 *         false if the name is unknown or already ranked on this ballot.
 */
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i].name) == 0)
        {
            for (int j = 0; j < rank; j++)
            {
                if (ranks[j] == i)
                {
                    return false;
                }
            }
            ranks[rank] = i;
            candidates[i].votes++;
            return true;
        }
    }
    return false;
}

/**
 * @brief Accumulates pairwise preferences implied by one complete ballot.
 *
 * For every ordered pair (i before j) in ranks[], preferences[i][j] is
 * incremented, reflecting that the voter prefers candidate i over j.
 *
 * @param ranks Completed ballot: ranks[i] holds the roster index at rank i.
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
 * @brief Builds the list of "strong" directed preference edges.
 *
 * For each unordered candidate pair (a, b), the direction of the edge points
 * at the candidate preferred by more voters; exact ties contribute no edge.
 */
void add_pairs(void)
{
    pair_count = 0;
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
            else if (preferences[i][j] < preferences[j][i])
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                pair_count++;
            }
        }
    }
}

/**
 * @brief Comparator for qsort(): orders pairs by margin of victory, descending.
 *
 * @param a First pair to compare.
 * @param b Second pair to compare.
 * @return negative if a should sort after b, zero on equal margins,
 *         positive if a should sort before b.
 */
int pair_margin_compare(const void *a, const void *b)
{
    const pair *pa = (const pair *) a;
    const pair *pb = (const pair *) b;

    int margin_a = preferences[pa->winner][pa->loser] - preferences[pa->loser][pa->winner];
    int margin_b = preferences[pb->winner][pb->loser] - preferences[pb->loser][pb->winner];

    return margin_b - margin_a;
}

/**
 * @brief Sorts all recorded pairs by descending margin of victory.
 */
void sort_pairs(void)
{
    qsort(pairs, pair_count, sizeof(pair), pair_margin_compare);
}

/**
 * @brief Depth-first search: can the locked graph already route "current"
 *        back to "winner"?
 *
 * Starting from the would-be loser, every locked outgoing edge is followed;
 * if the walk ever reaches the would-be winner, locking winner->loser now
 * would complete a cycle.
 *
 * @param winner Candidate index that would gain the new outgoing edge.
 * @param current Node currently being expanded during the search.
 * @return true  if a cycle would be created,
 *         false otherwise.
 */
bool edge_creates_cycle(int winner, int current)
{
    if (locked[current][winner])
    {
        return true;
    }

    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[current][i] && edge_creates_cycle(winner, i))
        {
            return true;
        }
    }

    return false;
}

/**
 * @brief Locks pairs in descending margin order, skipping any that would
 *        introduce a cycle into the graph.
 */
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        if (!edge_creates_cycle(pairs[i].winner, pairs[i].loser))
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }
}

/**
 * @brief Prints the Tideman winner.
 *
 * The winner is the unique source node of the locked graph — a candidate that
 * never appears as the loser of any locked edge.
 */
void print_winner(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        bool ever_defeated = false;
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i])
            {
                ever_defeated = true;
                break;
            }
        }
        if (!ever_defeated)
        {
            printf("%s\n", candidates[i].name);
            return;
        }
    }
}
