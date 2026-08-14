/**
 * AEL Runoff Engine — Two-Dimensional Preference Elimination Kernel
 * Built for CS50x 2026-2027 | AEL CS Encyclopedia
 * 
 * © Ayman Elmasry | AEL Digital Studio™
 * Enforcing the strict directive: 100% Original Proprietary Enterprise Code.
 */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_VOTERS 100
#define MAX_CANDIDATES 9

// preferences[i][j] is jth preference for voter i
int preferences[MAX_VOTERS][MAX_CANDIDATES];

typedef struct {
    char name[64];
    int votes;
    bool eliminated;
} Candidate;

Candidate candidates[MAX_CANDIDATES];
int voter_count;
int candidate_count;

/**
 * Registers voter preferences in the 2D matrix
 */
bool vote(int voter, int rank, const char *name) {
    for (int i = 0; i < candidate_count; i++) {
        if (strcmp(candidates[i].name, name) == 0) {
            preferences[voter][rank] = i;
            return true;
        }
    }
    return false;
}

/**
 * Tabulates active top preferences for non-eliminated candidates
 */
void tabulate(void) {
    for (int i = 0; i < voter_count; i++) {
        for (int j = 0; j < candidate_count; j++) {
            int candidate_index = preferences[i][j];
            if (!candidates[candidate_index].eliminated) {
                candidates[candidate_index].votes++;
                break;
            }
        }
    }
}

/**
 * Verifies if any active candidate has secured > 50% absolute majority
 */
bool print_winner(void) {
    int majority = voter_count / 2;
    for (int i = 0; i < candidate_count; i++) {
        if (candidates[i].votes > majority) {
            printf("\n[AEL RUNOFF CHAMPION] : %s 🏆 (Absolute Majority: %d/%d votes)\n", candidates[i].name, candidates[i].votes, voter_count);
            return true;
        }
    }
    return false;
}

/**
 * Finds minimum votes among active candidates
 */
int find_min(void) {
    int min_votes = voter_count + 1;
    for (int i = 0; i < candidate_count; i++) {
        if (!candidates[i].eliminated && candidates[i].votes < min_votes) {
            min_votes = candidates[i].votes;
        }
    }
    return min_votes;
}

/**
 * Verifies if active pool is completely tied
 */
bool is_tie(int min_votes) {
    for (int i = 0; i < candidate_count; i++) {
        if (!candidates[i].eliminated && candidates[i].votes != min_votes) {
            return false;
        }
    }
    return true;
}

/**
 * Eliminates all bottom-tier candidates with minimum vote floor
 */
void eliminate(int min_votes) {
    for (int i = 0; i < candidate_count; i++) {
        if (!candidates[i].eliminated && candidates[i].votes == min_votes) {
            candidates[i].eliminated = true;
            printf("[AEL ELIMINATION PHASE] Candidate eliminated : %s ❌\n", candidates[i].name);
        }
    }
}

int main(void) {
    printf("\n=========================================================================\n");
    printf("             AEL RUNOFF ENGINE — TWO-DIMENSIONAL ELIMINATION MATRIX\n");
    printf("=========================================================================\n\n");

    // Initialize mock candidate slate
    candidate_count = 3;
    strcpy(candidates[0].name, "Ayman"); candidates[0].votes = 0; candidates[0].eliminated = false;
    strcpy(candidates[1].name, "Bob");   candidates[1].votes = 0; candidates[1].eliminated = false;
    strcpy(candidates[2].name, "Alice"); candidates[2].votes = 0; candidates[2].eliminated = false;

    // Simulate 5 voters with distinct rank matrices
    voter_count = 5;
    vote(0, 0, "Bob");   vote(0, 1, "Ayman"); vote(0, 2, "Alice");
    vote(1, 0, "Alice"); vote(1, 1, "Ayman"); vote(1, 2, "Bob");
    vote(2, 0, "Ayman"); vote(2, 1, "Bob");   vote(2, 2, "Alice");
    vote(3, 0, "Ayman"); vote(3, 1, "Alice"); vote(3, 2, "Bob");
    vote(4, 0, "Bob");   vote(4, 1, "Ayman"); vote(4, 2, "Alice");

    printf("[BALLOTS CONFIGURED] : 5 Voters registered in 2D preference array.\n\n");

    // Executing instant runoff loop
    int round = 1;
    while (true) {
        printf("--- ELECTION ROUND %d ---\n", round++);
        
        // Reset vote tallies
        for (int i = 0; i < candidate_count; i++) candidates[i].votes = 0;

        tabulate();

        for (int i = 0; i < candidate_count; i++) {
            if (!candidates[i].eliminated) {
                printf("  --> %s : %d active votes\n", candidates[i].name, candidates[i].votes);
            }
        }

        if (print_winner()) break;

        int min = find_min();
        if (is_tie(min)) {
            printf("\n[AEL ELECTORAL TIE] Complete tie declared among remaining candidates! 🤝\n");
            break;
        }

        eliminate(min);
        printf("\n");
    }

    printf("=========================================================================\n\n");
    return 0;
}
