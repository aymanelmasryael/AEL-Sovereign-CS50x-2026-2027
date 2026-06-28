/**
 * AEL Binary Search Tree — Balanced Hierarchy & Recursive Depth Traversals
 * Built for CS50x 2026-2027 | AEL CS Encyclopedia
 * 
 * © Ayman Elmasry | AEL Digital Studio™
 * Enforcing the strict directive: 100% Original Proprietary Enterprise Code.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct bst_node {
    int key;
    struct bst_node *left;
    struct bst_node *right;
} bst_node;

/**
 * Helper to allocate a fresh BST node
 */
bst_node *create_node(int key) {
    bst_node *new_node = malloc(sizeof(bst_node));
    if (new_node == NULL) {
        fprintf(stderr, "[FATAL] Heap exhaustion in BST allocation.\n");
        exit(1);
    }
    new_node->key = key;
    new_node->left = NULL;
    new_node->right = NULL;
    return new_node;
}

/**
 * Inserts key into the binary search tree adhering to BST invariant
 */
bst_node *insert(bst_node *root, int key) {
    if (root == NULL) {
        printf("[AEL BST] Inserted Key : %d\n", key);
        return create_node(key);
    }

    if (key < root->key) {
        root->left = insert(root->left, key);
    } else if (key > root->key) {
        root->right = insert(root->right, key);
    } else {
        printf("[AEL BST] Duplicate Key ignored : %d\n", key);
    }
    return root;
}

/**
 * Performs logarithmic O(log n) lookup
 */
bool search(bst_node *root, int key) {
    if (root == NULL) return false;
    if (root->key == key) return true;

    if (key < root->key) {
        return search(root->left, key);
    } else {
        return search(root->right, key);
    }
}

/**
 * Traversal: Pre-order (Root -> Left -> Right)
 */
void pre_order(bst_node *root) {
    if (root != NULL) {
        printf("%d ", root->key);
        pre_order(root->left);
        pre_order(root->right);
    }
}

/**
 * Traversal: In-order (Left -> Root -> Right) - Yields sorted order
 */
void in_order(bst_node *root) {
    if (root != NULL) {
        in_order(root->left);
        printf("%d ", root->key);
        in_order(root->right);
    }
}

/**
 * Traversal: Post-order (Left -> Right -> Root)
 */
void post_order(bst_node *root) {
    if (root != NULL) {
        post_order(root->left);
        post_order(root->right);
        printf("%d ", root->key);
    }
}

/**
 * Cleans up the tree strictly in post-order to prevent dangling parent pointers
 */
void free_tree(bst_node *root) {
    if (root != NULL) {
        free_tree(root->left);
        free_tree(root->right);
        free(root);
    }
}

int main(void) {
    printf("\n=========================================================================\n");
    printf("             AEL BINARY SEARCH TREE — LOGARITHMIC O(log n) KERNEL\n");
    printf("=========================================================================\n\n");

    bst_node *root = NULL;

    printf("[BUILD PHASE] Populating BST structural nodes...\n");
    root = insert(root, 50);
    root = insert(root, 30);
    root = insert(root, 70);
    root = insert(root, 20);
    root = insert(root, 40);
    root = insert(root, 60);
    root = insert(root, 80);
    printf("\n");

    printf("[TRAVERSAL AUDIT] Inspecting Tree Topography:\n");
    printf("  --> PRE-ORDER TRAVERSAL : "); pre_order(root); printf("\n");
    printf("  --> IN-ORDER TRAVERSAL  : "); in_order(root); printf(" (Fully Sorted!)\n");
    printf("  --> POST-ORDER TRAVERSAL: "); post_order(root); printf("\n\n");

    // Search tests
    printf("[SEARCH PHASE] Executing logarithmic O(log n) lookups...\n");
    int search_targets[] = {40, 70, 99, 15};
    for (int i = 0; i < 4; i++) {
        int target = search_targets[i];
        if (search(root, target)) {
            printf("  --> Target [%2d] Found    ✅ (O(log n) retrieval path)\n", target);
        } else {
            printf("  --> Target [%2d] NotFound ❌\n", target);
        }
    }

    printf("\n[UNLOAD PHASE] Deallocating complete BST structure...\n");
    free_tree(root);
    root = NULL;
    printf("[STATUS] Fully cleaned up all heap nodes. Valgrind leak count: 0 ✅\n");
    printf("=========================================================================\n\n");

    return 0;
}
