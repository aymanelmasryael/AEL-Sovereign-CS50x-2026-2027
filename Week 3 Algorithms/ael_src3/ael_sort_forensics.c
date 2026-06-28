/**
 * AEL Sort Forensics — Asymptotic Benchmark & Diagnostic Engine
 * Built for CS50x 2026-2027 | AEL CS Encyclopedia
 * 
 * © Ayman Elmasry | AEL Digital Studio™
 * Enforcing the strict directive: 100% Original Proprietary Enterprise Code.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define BUFFER_SIZE 5000

// Helper to copy data buffers
void clone_buffer(const int *source, int *dest, int size) {
    memcpy(dest, source, size * sizeof(int));
}

// Bubble Sort Heuristic
void bubble_sort(int *arr, int n) {
    for (int i = 0; i < n - 1; i++) {
        int swapped = 0;
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j]; arr[j] = arr[j + 1]; arr[j + 1] = temp;
                swapped = 1;
            }
        }
        if (!swapped) break;
    }
}

// Selection Sort Heuristic
void selection_sort(int *arr, int n) {
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[min_idx]) min_idx = j;
        }
        int temp = arr[i]; arr[i] = arr[min_idx]; arr[min_idx] = temp;
    }
}

// Merge Sort Heuristic Kernel
void merge(int *arr, int l, int m, int r) {
    int n1 = m - l + 1; int n2 = r - m;
    int *L = malloc(n1 * sizeof(int)); int *R = malloc(n2 * sizeof(int));
    for (int i = 0; i < n1; i++) L[i] = arr[l + i];
    for (int j = 0; j < n2; j++) R[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) arr[k++] = L[i++];
        else arr[k++] = R[j++];
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
    free(L); free(R);
}

void merge_sort(int *arr, int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        merge_sort(arr, l, m);
        merge_sort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

/**
 * Forensically profiles algorithms across data states
 */
void audit_benchmark_suite(const char *state_label, const int *master_buffer) {
    int work_buffer[BUFFER_SIZE];
    clock_t start, end;
    double cpu_time_used;

    printf("\n[DATA MATRIX STATE] : %s (%d elements)\n", state_label, BUFFER_SIZE);
    printf("-------------------------------------------------------------------------\n");

    // Profile Bubble Sort
    clone_buffer(master_buffer, work_buffer, BUFFER_SIZE);
    start = clock(); bubble_sort(work_buffer, BUFFER_SIZE); end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("  --> Bubble Sort    [O(n^2)]      : %f seconds\n", cpu_time_used);

    // Profile Selection Sort
    clone_buffer(master_buffer, work_buffer, BUFFER_SIZE);
    start = clock(); selection_sort(work_buffer, BUFFER_SIZE); end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("  --> Selection Sort [O(n^2)]      : %f seconds\n", cpu_time_used);

    // Profile Merge Sort
    clone_buffer(master_buffer, work_buffer, BUFFER_SIZE);
    start = clock(); merge_sort(work_buffer, 0, BUFFER_SIZE - 1); end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("  --> Merge Sort     [O(n log n)]  : %f seconds\n", cpu_time_used);
    printf("-------------------------------------------------------------------------\n");
}

int main(void) {
    printf("\n=========================================================================\n");
    printf("             AEL SORT FORENSICS — ASYMPTOTIC BENCHMARK KERNEL\n");
    printf("=========================================================================\n\n");

    int master_random[BUFFER_SIZE];
    int master_sorted[BUFFER_SIZE];
    int master_reversed[BUFFER_SIZE];

    srand(time(NULL));

    // Initialize master matrix arrays
    for (int i = 0; i < BUFFER_SIZE; i++) {
        master_random[i] = rand() % 100000;
        master_sorted[i] = i;
        master_reversed[i] = BUFFER_SIZE - i;
    }

    audit_benchmark_suite("RANDOMIZED BUFFER PAYLOAD", master_random);
    audit_benchmark_suite("PERFECTLY SORTED BUFFER", master_sorted);
    audit_benchmark_suite("INVERTED REVERSED BUFFER", master_reversed);

    printf("\n[FORENSIC CONCLUSION] Merge Sort exhibits vastly superior performance on random/reversed datasets, while Bubble Sort dominates pre-sorted arrays due to its adaptive exit switch! ✅\n");
    printf("=========================================================================\n\n");

    return 0;
}
