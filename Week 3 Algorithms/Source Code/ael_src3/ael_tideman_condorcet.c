/**
 * AEL Tideman Condorcet — Directed Acyclic Graph (DAG) Voting Kernel
 * Built for CS50x 2026-2027 | AEL CS Encyclopedia
 * 
 * © Ayman Elmasry | AEL Digital Studio™
 * Enforcing the strict directive: 100% Original Proprietary Enterprise Code.
 */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_CANDIDATES 9
#define MAX_VOTERS 100

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX_CANDIDATES][MAX_CANDIDATES];

// locked[i][j] means i is locked in over j (Directed Edge in DAG)
bool locked[MAX_CANDIDATES][MAX_CANDIDATES];

typedef struct {
    int winner;
    int loser;
} Pair;

char candidates[MAX_CANDIDATES][64];
Pair pairs[MAX_CANDIDATES * (MAX_CANDIDATES - 1) / 2];

int candidate_count;
int pair_count;

/**
 * Sweeps preference matrix to isolate winning pairs
 */
void add_pairs(void) {
    pair_count = 0;
    for (int i = 0; i < candidate_count; i++) {
        for (int j = i + 1; j < candidate_count; j++) {
            if (preferences[i][j] > preferences[j][i]) {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            } else if (preferences[i][j] < preferences[j][i]) {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                pair_count++;
            }
        }
    }
}

/**
 * Sorts pairs in decreasing order of victory margin
 */
void sort_pairs(void) {
    for (int i = 0; i < pair_count - 1; i++) {
        int max_idx = i;
        int max_margin = preferences[pairs[i].winner][pairs[i].loser] - preferences[pairs[i].loser][pairs[i].winner];

        for (int j = i + 1; j < pair_count; j++) {
            int margin = preferences[pairs[j].winner][pairs[j].loser] - preferences[pairs[j].loser][pairs[j].winner];
            if (margin > max_margin) {
                max_margin = margin;
                max_idx = j;
            }
        }

        if (max_idx != i) {
            Pair temp = pairs[i];
            pairs[i] = pairs[max_idx];
            pairs[max_idx] = temp;
        }
    }
}

/**
 * Depth-First Search (DFS) helper to verify if an active path connects target back to base (Cycle Detection)
 */
bool has_cycle(int base, int target) {
    if (target == base) {
        return true; // Cycle detected
    }

    for (int i = 0; i < candidate_count; i++) {
        if (locked[target][i]) {
            if (has_cycle(base, i)) {
                return true;
            }
        }
    }
    return false;
}

/**
 * Locks pairs into Directed Acyclic Graph (DAG), actively preventing cycle hazards
 */
void lock_pairs(void) {
    for (int i = 0; i < pair_count; i++) {
        int win = pairs[i].winner;
        int lose = pairs[i].loser;

        // Verify if locking this edge creates a closed loop
        if (!has_cycle(win, lose)) {
            locked[win][lose] = true;
            printf("[AEL DAG MATRIX] Locked Directed Edge : %s --> %s ✅\n", candidates[win], candidates[lose]);
        } else {
            printf("[AEL CYCLE HAZARD REJECTED] Skipped Edge : %s --> %s (Forces closed cycle) ❌\n", candidates[win], candidates[lose]);
        }
    }
}

/**
 * Identifies the absolute source of the DAG (The Condorcet Winner with 0 incoming edges)
 */
void print_winner(void) {
    for (int i = 0; i < candidate_count; i++) {
        bool incoming_edges = false;
        for (int j = 0; j < candidate_count; j++) {
            if (locked[j][i]) {
                incoming_edges = true;
                break;
            }
        }

        if (!incoming_edges) {
            printf("\n[AEL CONDORCET CHAMPION] : %s 🏆 (DAG Source - Undefeatable in Head-to-Head)\n", candidates[i]);
            return;
        }
    }
}

int main(void) {
    printf("\n=========================================================================\n");
    printf("             AEL TIDEMAN CONDORCET — DIRECTED ACYCLIC GRAPH KERNEL\n");
    printf("=========================================================================\n\n");

    // Initialize slate
    candidate_count = 3;
    strcpy(candidates[0], "Ayman");
    strcpy(candidates[1], "Bob");
    strcpy(candidates[2], "Alice");

    // Clear locked graph
    for (int i = 0; i < candidate_count; i++) {
        for (int j = 0; j < candidate_count; j++) {
            locked[i][j] = false;
            preferences[i][j] = 0;
        }
    }

    // Configure mock preferences to simulate a cycle hazard
    // Ayman > Bob (7-3)
    preferences[0][1] = 7; preferences[1][0] = 3;
    // Bob > Alice (6-4)
    preferences[1][2] = 6; preferences[2][1] = 4;
    // Alice > Ayman (5-5 tie break or 6-4 to force cycle check)
    preferences[2][0] = 6; preferences[0][2] = 4;

    printf("[TABULATION PIPELINE] Identifying pairs & margins...\n\n");
    add_pairs();
    sort_pairs();

    printf("[LOCKING PHASE] Executing Depth-First Search (DFS) Cycle Defense...\n");
    lock_pairs();

    // Broadcast Condorcet Winner
    print_winner();

    printf("=========================================================================\n\n");
    return 0;
}
