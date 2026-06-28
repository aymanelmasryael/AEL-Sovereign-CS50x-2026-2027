/**
 * AEL Buffer Guardian — Bare-Metal Memory Allocator & Leak Detector Wrapper
 * Built for CS50x 2026-2027 | AEL CS Encyclopedia
 * 
 * © Ayman Elmasry | AEL Digital Studio™
 * Enforcing the strict directive: 100% Original Proprietary Enterprise Code.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_ALLOCATIONS 1024

typedef struct {
    void *address;
    size_t size;
    bool active;
    const char *allocation_label;
} MemoryBlock;

typedef struct {
    MemoryBlock blocks[MAX_ALLOCATIONS];
    size_t active_count;
    size_t total_allocated_bytes;
} MemoryGuardian;

// Global Guardian Instance
MemoryGuardian guardian = { .active_count = 0, .total_allocated_bytes = 0 };

/**
 * Custom memory allocator wrapper with forensic tracking
 */
void *ael_malloc(size_t size, const char *label) {
    void *ptr = malloc(size);
    if (ptr == NULL) {
        fprintf(stderr, "[FATAL ERROR] Bare-Metal Silicon Allocation Failed for %s\n", label);
        return NULL;
    }

    // Record allocation in Guardian Table
    for (int i = 0; i < MAX_ALLOCATIONS; i++) {
        if (!guardian.blocks[i].active) {
            guardian.blocks[i].address = ptr;
            guardian.blocks[i].size = size;
            guardian.blocks[i].active = true;
            guardian.blocks[i].allocation_label = label;
            guardian.active_count++;
            guardian.total_allocated_bytes += size;
            printf("[AEL GUARDIAN] Allocated %zu bytes at [0x%p] (%s)\n", size, ptr, label);
            return ptr;
        }
    }

    fprintf(stderr, "[WARNING] Guardian Tracking Table Full! Allocation unmonitored.\n");
    return ptr;
}

/**
 * Custom deallocator wrapper preventing Double Free and tracking active handles
 */
void ael_free(void *ptr) {
    if (ptr == NULL) {
        fprintf(stderr, "[AEL GUARDIAN WARNING] Attempted to free a NULL pointer! Ignored to prevent SIGSEGV.\n");
        return;
    }

    for (int i = 0; i < MAX_ALLOCATIONS; i++) {
        if (guardian.blocks[i].active && guardian.blocks[i].address == ptr) {
            free(ptr);
            guardian.blocks[i].active = false;
            guardian.active_count--;
            guardian.total_allocated_bytes -= guardian.blocks[i].size;
            printf("[AEL GUARDIAN] Freed block at [0x%p] (%s)\n", ptr, guardian.blocks[i].allocation_label);
            return;
        }
    }

    fprintf(stderr, "[AEL GUARDIAN FATAL] Untracked or Double Free detected at [0x%p]! Execution compromised.\n", ptr);
}

/**
 * Valgrind-style diagnostic audit dump
 */
void ael_guardian_audit() {
    printf("\n=========================================================================\n");
    printf("             AEL BUFFER GUARDIAN RUNTIME DIAGNOSTIC AUDIT\n");
    printf("=========================================================================\n");
    printf("Active Allocations Remaining : %zu\n", guardian.active_count);
    printf("Net Allocated Active Bytes   : %zu bytes\n", guardian.total_allocated_bytes);

    if (guardian.active_count == 0) {
        printf("\n[STATUS] PERFECT SILICON CLEANLINESS! No Memory Leaks Detected. ✅\n");
    } else {
        printf("\n[WARNING] MEMORY LEAKS DETECTED! Unfreed Blocks Listed Below: ❌\n");
        for (int i = 0; i < MAX_ALLOCATIONS; i++) {
            if (guardian.blocks[i].active) {
                printf("  --> [0x%p] : %zu bytes (%s)\n", guardian.blocks[i].address, guardian.blocks[i].size, guardian.blocks[i].allocation_label);
            }
        }
    }
    printf("=========================================================================\n\n");
}

int main(void) {
    printf("Initializing AEL Buffer Guardian Verification Matrix...\n\n");

    // Simulated Execution 1: Safe dynamic allocation and deallocation
    int *cipher_matrix = (int *)ael_malloc(256 * sizeof(int), "Encryption Kernel Matrix");
    ael_free(cipher_matrix);
    cipher_matrix = NULL; // Best Engineering Practice

    // Simulated Execution 2: Intentionally triggering a Memory Leak for forensic verification
    char *user_session = (char *)ael_malloc(1024 * sizeof(char), "Sovereign User Auth Token");
    // Omitting ael_free(user_session) intentionally to showcase Valgrind-like leak reporting

    // Simulated Execution 3: Handling a NULL pointer free gracefully
    int *null_ptr = NULL;
    ael_free(null_ptr);

    // Final Memory System Audit
    ael_guardian_audit();

    // Clean up leak for pure termination
    ael_free(user_session);
    printf("Simulated remediation complete. Have a sovereign day.\n");

    return 0;
}
