/**
 * AEL Plurality System — Winner-Take-All Linear Search Voting Engine
 * Built for CS50x 2026-2027 | AEL CS Encyclopedia
 * 
 * © Ayman Elmasry | AEL Digital Studio™
 * Enforcing the strict directive: 100% Original Proprietary Enterprise Code.
 */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_CANDIDATES 9

typedef struct {
    char name[64];
    int votes;
} Candidate;

Candidate candidates[MAX_CANDIDATES];
int candidate_count = 0;

/**
 * Executes a fast linear search across candidate array to register valid ballots
 */
bool vote(const char *name) {
    for (int i = 0; i < candidate_count; i++) {
        if (strcmp(candidates[i].name, name) == 0) {
            candidates[i].votes++;
            return true;
        }
    }
    return false;
}

/**
 * Sweeps array to identify maximum vote peak and prints champions
 */
void print_winner(void) {
    int max_votes = 0;
    for (int i = 0; i < candidate_count; i++) {
        if (candidates[i].votes > max_votes) {
            max_votes = candidates[i].votes;
        }
    }

    printf("\n[AEL ELECTORAL CHAMPION(S)] : ");
    for (int i = 0; i < candidate_count; i++) {
        if (candidates[i].votes == max_votes) {
            printf("%s ", candidates[i].name);
        }
    }
    printf("🏆 (Max Votes: %d)\n", max_votes);
}

int main(int argc, char *argv[]) {
    printf("\n=========================================================================\n");
    printf("             AEL PLURALITY SYSTEM — ENTERPRISE VOTING KERNEL\n");
    printf("=========================================================================\n\n");

    if (argc < 2) {
        printf("[FALLBACK] No CLI candidates provided. Initializing default sovereign slate...\n\n");
        strcpy(candidates[0].name, "Ayman"); candidates[0].votes = 0;
        strcpy(candidates[1].name, "SovereignAI"); candidates[1].votes = 0;
        strcpy(candidates[2].name, "Antigravity"); candidates[2].votes = 0;
        candidate_count = 3;
    } else {
        candidate_count = argc - 1;
        if (candidate_count > MAX_CANDIDATES) {
            fprintf(stderr, "[FATAL] Maximum candidate limit exceeded (%d).\n", MAX_CANDIDATES);
            return 1;
        }
        for (int i = 0; i < candidate_count; i++) {
            strcpy(candidates[i].name, argv[i + 1]);
            candidates[i].votes = 0;
        }
    }

    printf("[SLATE REGISTERED] : %d active candidates.\n\n", candidate_count);

    // Simulate Voting Process
    const char *simulated_ballots[] = {"Ayman", "SovereignAI", "Ayman", "Antigravity", "Ayman"};
    int ballot_count = 5;

    printf("[TABULATION MATRIX] Processing incoming ballots...\n");
    for (int i = 0; i < ballot_count; i++) {
        const char *cast = simulated_ballots[i];
        if (vote(cast)) {
            printf("  --> Valid Ballot Accepted : %s ✅\n", cast);
        } else {
            printf("  --> Invalid Ballot Rejected : %s ❌\n", cast);
        }
    }

    // Output Winners
    print_winner();

    printf("=========================================================================\n\n");
    return 0;
}
