/**
 * @file inheritance.c
 * @brief Simulates blood-type inheritance across three generations.
 * @author Ayman Elmasry — AEL Digital Studio
 * @project AEL Sovereign — CS50x 2026-2027, Lab 5
 *
 * Algorithm:
 *   - create_family() builds the family tree recursively. A leaf person (the
 *     oldest generation) has no parents and two randomly drawn alleles from
 *     {'A','B','O'}. Every non-leaf child is created with two parents created
 *     one generation earlier, then inherits one randomly selected allele from
 *     each parent (mirroring Mendelian transmission).
 *   - print_family() walks the tree in pre-order, indenting each generation
 *     deeper by INDENT_LENGTH spaces.
 *   - free_family() releases memory in post-order so children are always
 *     freed before their parents.
 *
 * Complexity:
 *   - create_family: O(2^g) node creations for g generations.
 *   - print_family : O(2^g) node visits.
 *   - free_family  : O(2^g) node frees.
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct person
{
    struct person *parents[2];
    char alleles[2];
} person;

const int GENERATIONS = 3;
const int INDENT_LENGTH = 4;

person *create_family(int generations);
void print_family(person *p, int generation);
void free_family(person *p);
char random_allele(void);

int main(void)
{
    srand(time(0));

    person *root = create_family(GENERATIONS);
    print_family(root, 0);
    free_family(root);

    return 0;
}

/**
 * @brief Builds a family of the given depth.
 *
 * @param generations Remaining depth; 1 produces an allele-carrying leaf,
 *                    anything larger produces a child with two parent subtrees.
 * @return Pointer to the newly allocated root of the subtree, or NULL if
 *         memory allocation failed anywhere along the way.
 */
person *create_family(int generations)
{
    person *child = malloc(sizeof(person));
    if (child == NULL)
    {
        return NULL;
    }

    if (generations > 1)
    {
        person *parent0 = create_family(generations - 1);
        person *parent1 = create_family(generations - 1);
        if (parent0 == NULL || parent1 == NULL)
        {
            free_family(parent0);
            free_family(parent1);
            free(child);
            return NULL;
        }

        child->parents[0] = parent0;
        child->parents[1] = parent1;

        child->alleles[0] = parent0->alleles[rand() % 2];
        child->alleles[1] = parent1->alleles[rand() % 2];
    }
    else
    {
        child->parents[0] = NULL;
        child->parents[1] = NULL;

        child->alleles[0] = random_allele();
        child->alleles[1] = random_allele();
    }

    return child;
}

/**
 * @brief Draws one blood-type allele uniformly from {'A','B','O'}.
 * @return The chosen allele character.
 */
char random_allele(void)
{
    int r = rand() % 3;
    if (r == 0)
    {
        return 'A';
    }
    else if (r == 1)
    {
        return 'B';
    }
    return 'O';
}

/**
 * @brief Prints a family tree with one indented line per person.
 *
 * @param p          Current node, or NULL for an absent parent.
 * @param generation Depth of this node (0 = child).
 */
void print_family(person *p, int generation)
{
    if (p == NULL)
    {
        return;
    }

    for (int i = 0; i < generation * INDENT_LENGTH; i++)
    {
        printf(" ");
    }

    if (generation == 0)
    {
        printf("Child (Generation %i): blood type %c%c\n", generation, p->alleles[0], p->alleles[1]);
    }
    else if (generation == 1)
    {
        printf("Parent (Generation %i): blood type %c%c\n", generation, p->alleles[0],
               p->alleles[1]);
    }
    else
    {
        for (int i = 0; i < generation - 2; i++)
        {
            printf("Great-");
        }
        printf("Grandparent (Generation %i): blood type %c%c\n", generation, p->alleles[0],
               p->alleles[1]);
    }

    print_family(p->parents[0], generation + 1);
    print_family(p->parents[1], generation + 1);
}

/**
 * @brief Frees a family tree in post-order.
 *
 * @param p Current node, or NULL (base case).
 */
void free_family(person *p)
{
    if (p == NULL)
    {
        return;
    }

    free_family(p->parents[0]);
    free_family(p->parents[1]);
    free(p);
}