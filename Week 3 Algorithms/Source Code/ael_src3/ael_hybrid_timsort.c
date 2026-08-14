#include <stdio.h>
#include <stdlib.h>

#define SOVEREIGN_THRESHOLD 32

// AEL Apex Hybrid Matrix (TimSort-inspired sorting architecture)
// Optimizes CPU cache performance by switching to insertion sort for small subarrays

void ael_insertion_sort(int arr[], int left, int right) {
    for (int i = left + 1; i <= right; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= left && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void ael_merge(int arr[], int left, int mid, int right) {
    int len1 = mid - left + 1, len2 = right - mid;
    int *left_arr = (int *)malloc(len1 * sizeof(int));
    int *right_arr = (int *)malloc(len2 * sizeof(int));

    for (int i = 0; i < len1; i++) left_arr[i] = arr[left + i];
    for (int i = 0; i < len2; i++) right_arr[i] = arr[mid + 1 + i];

    int i = 0, j = 0, k = left;
    while (i < len1 && j < len2) {
        if (left_arr[i] <= right_arr[j]) arr[k++] = left_arr[i++];
        else arr[k++] = right_arr[j++];
    }
    while (i < len1) arr[k++] = left_arr[i++];
    while (j < len2) arr[k++] = right_arr[j++];

    free(left_arr);
    free(right_arr);
}

void ael_hybrid_timsort(int arr[], int left, int right) {
    if (right - left <= SOVEREIGN_THRESHOLD) {
        ael_insertion_sort(arr, left, right);
        return;
    }
    int mid = left + (right - left) / 2;
    ael_hybrid_timsort(arr, left, mid);
    ael_hybrid_timsort(arr, mid + 1, right);
    ael_merge(arr, left, mid, right);
}

int main(void) {
    int data[] = {64, 34, 25, 12, 22, 11, 90, 88, 76, 54, 32, 19, 5, 3, 99, 102, 43, 21, 15, 8};
    int n = sizeof(data) / sizeof(data[0]);

    printf("====================================================\n");
    printf("      AEL APEX HYBRID TIMSORT MATRIX (v3.0)\n");
    printf("====================================================\n");

    printf("[+] Original Array: ");
    for (int i = 0; i < n; i++) printf("%d ", data[i]);
    printf("\n");

    ael_hybrid_timsort(data, 0, n - 1);

    printf("[+] Sorted Array:   ");
    for (int i = 0; i < n; i++) printf("%d ", data[i]);
    printf("\n");

    return 0;
}
