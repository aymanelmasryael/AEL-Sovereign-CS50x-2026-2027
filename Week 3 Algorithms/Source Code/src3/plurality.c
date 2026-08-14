#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_CANDIDATES 9

typedef struct {
    char name[64];
    int votes;
} Candidate;

Candidate candidates[MAX_CANDIDATES];
int candidate_count = 3;

bool vote(const char *name) {
    for (int i = 0; i < candidate_count; i++) {
        if (strcmp(candidates[i].name, name) == 0) {
            candidates[i].votes++;
            return true;
        }
    }
    return false;
}

void print_winner(void) {
    int max_votes = 0;
    for (int i = 0; i < candidate_count; i++) {
        if (candidates[i].votes > max_votes) {
            max_votes = candidates[i].votes;
        }
    }
    for (int i = 0; i < candidate_count; i++) {
        if (candidates[i].votes == max_votes) {
            printf("%s\n", candidates[i].name);
        }
    }
}

int main(void) {
    strcpy(candidates[0].name, "Alice");
    candidates[0].votes = 0;
    strcpy(candidates[1].name, "Bob");
    candidates[1].votes = 0;
    strcpy(candidates[2].name, "Charlie");
    candidates[2].votes = 0;

    vote("Alice");
    vote("Bob");
    vote("Alice");

    printf("Winner: ");
    print_winner();
    return 0;
}
