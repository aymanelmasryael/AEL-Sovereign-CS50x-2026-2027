/**
 * @file    inheritance.c
 * @brief   Genetic Blood-Type Inheritance Simulator (Recursive Family Tree).
 *
 * @project AEL Sovereign — CS50x 2026-2027
 * @author  Ayman Elmasry — AEL Digital Studio
 *
 * @details Algorithm Design
 *          -----------------
 *          This program simulates the Mendelian inheritance of human blood
 *          types across a fixed number of generations. A person's genotype
 *          consists of two alleles drawn from {A, B, O}; each child receives
 *          one allele from each parent. Skipping a generation means a person's
 *          parents are fabricated with fully random alleles.
 *
 *          The family tree is a full binary tree of height GENERATIONS:
 *
 *                           [child: 1 allele from each parent]
 *                          /                                  \
 *                  [parent0]                              [parent1]
 *                   /    \                                  /    \
 *           [g1][g2]     [g1][g2]                   [g1][g2]     [g1][g2]
 *
 *          Recursion Tree & Design
 *          -----------------------
 *          create_family(generations) builds the tree top-down:
 *            - Base case  (generations == 1): a founder with two independent
 *              random alleles and NULL parents.
 *            - Recursive case (generations > 1): spawns two subfamilies with
 *              generations - 1, then samples one random allele from each
 *              parent with uniform coin flips -- exactly one allele per side,
 *              mirroring real meiosis.
 *
 *          The tree is then traversed pre-order for display (indentation
 *          doubling per generation) and post-order for teardown (children
 *          released before their parent, so no pointer outlives its target).
 *
 *          Defensive engineering checks every malloc, seeds the PRNG at most
 *          once from the wall clock, and makes free/print null-safe so no
 *          buffer is ever double-released or dereferenced after death.
 *
 * @note    The mandatory CS50 contracts -- the Person struct, the constants,
 *          and the exact signatures of create_family / print_family /
 *          free_family (plus the supporting random_allele helper) -- are
 *          preserved verbatim.
 *
 * @complexity
 *          create_family(n): Time O(2^n) | Space O(n) call-stack depth,
 *                            O(2^n) heap for the full tree.
 *          free_family(p):   Time O(2^n) | Space O(n) call stack.
 *          print_family(...):Time O(2^n) | Space O(n) call stack.
 *          where n = GENERATIONS.
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/**
 * @struct person
 * @brief  A single family member: references to both parents and two alleles.
 */
typedef struct person
{
    struct person *parents[2];  /* Biological parents (NULL for founders). */
    char alleles[2];            /* Two of {A, B, O}.                      */
}
person;

/* Depth of the simulated pedigree (child + parents + grandparents...). */
const int GENERATIONS = 3;

/* Number of spaces printed per generation of indentation. */
const int INDENT_LENGTH = 4;

/* Function prototypes mandated by the CS50 problem specification. */
person *create_family(int generations);
void print_family(person *p, int generation);
void free_family(person *p);
char random_allele(void);

/**
 * @brief  Entry point: seeds the generator, grows the tree, prints it,
 *         then tears it down completely.
 */
int main(void)
{
    /* Seed the pseudo-random number generator from the wall clock once. */
    srand(time(0));

    /* Grow the full pedigree from the root generation down. */
    person *p = create_family(GENERATIONS);

    /* Display the family tree, generation labels, and blood types. */
    print_family(p, 0);

    /* Release every node allocated across the entire tree. */
    free_family(p);
}

/**
 * @brief  Creates a new generation of a family tree via recursion.
 * @param  generations Number of generations this subtree must contain.
 * @return Pointer to the newly created person (the subtree root).
 *
 * @note   Reach generations == 1, the person is a founder whose alleles are
 *         drawn uniformly from {A, B, O}; otherwise the person is a child who
 *         inherits one random allele from each fabricated parent.
 *
 * @complexity Time O(2^n) | Space O(2^n) heap, O(n) call stack.
 */
person *create_family(int generations)
{
    /* Allocate the new individual; abort cleanly if memory is unavailable. */
    person *new_person = malloc(sizeof(person));
    if (new_person == NULL)
    {
        fprintf(stderr, "Out of memory while building family tree.\n");
        exit(1);
    }

    /* Recursive case: there are ancestor generations left to construct. */
    if (generations > 1)
    {
        /* Grow the two parental subtrees, one generation shallower. */
        person *parent0 = create_family(generations - 1);
        person *parent1 = create_family(generations - 1);

        /* Wire this child to its fictional parents. */
        new_person->parents[0] = parent0;
        new_person->parents[1] = parent1;

        /*
         * Meiosis in miniature: the child takes one uniformly random allele
         * from each parent -- never two from the same side.
         */
        new_person->alleles[0] = parent0->alleles[rand() % 2];
        new_person->alleles[1] = parent1->alleles[rand() % 2];
    }

    /* Base case: a founder, born of no one, with two random alleles. */
    else
    {
        new_person->parents[0] = NULL;
        new_person->parents[1] = NULL;

        new_person->alleles[0] = random_allele();
        new_person->alleles[1] = random_allele();
    }

    return new_person;
}

/**
 * @brief  Frees the given person and every ancestor (post-order teardown).
 * @param  p The subtree root to release.
 *
 * @note   Children are freed before their parent so the parent's pointer
 *         fields never reference released memory. Null-safe.
 *
 * @complexity Time O(2^n) | Space O(n) call stack.
 */
void free_family(person *p)
{
    /* Base case: a NULL pointer (founder child slot) is trivially done. */
    if (p == NULL)
    {
        return;
    }

    /* Recurse first -- release both subtrees entirely. */
    free_family(p->parents[0]);
    free_family(p->parents[1]);

    /* Only now is it safe to release the parent itself. */
    free(p);
}

/**
 * @brief  Prints a family tree, labelling each individual and blood type.
 * @param  p          The current person to render.
 * @param  generation The person's depth below the child (0 = child).
 *
 * @note   Pre-order traversal with indentation proportional to generation
 *         depth; the genealogical label (Child/Parent/Grandparent/
 *         Great-Grandparent...) is derived arithmetically.
 *
 * @complexity Time O(2^n) | Space O(n) call stack.
 */
void print_family(person *p, int generation)
{
    /* Base case: an empty spouse/parent slot renders nothing. */
    if (p == NULL)
    {
        return;
    }

    /* Indent proportional to the generation depth. */
    for (int i = 0; i < generation * INDENT_LENGTH; i++)
    {
        printf(" ");
    }

    /* Derive the genealogical title from the generation index. */
    if (generation == 0)
    {
        printf("Child (Generation %i): blood type %c%c\n",
               generation, p->alleles[0], p->alleles[1]);
    }
    else if (generation == 1)
    {
        printf("Parent (Generation %i): blood type %c%c\n",
               generation, p->alleles[0], p->alleles[1]);
    }
    else
    {
        for (int i = 0; i < generation - 2; i++)
        {
            printf("Great-");
        }
        printf("Grandparent (Generation %i): blood type %c%c\n",
               generation, p->alleles[0], p->alleles[1]);
    }

    /* Visit both parental subtrees next (pre-order). */
    print_family(p->parents[0], generation + 1);
    print_family(p->parents[1], generation + 1);
}

/**
 * @brief  Draws a uniformly random blood-type allele from {A, B, O}.
 * @return One of the characters 'A', 'B', or 'O'.
 *
 * @note   Converts rand() % 3 into one of the three ABO alleles.
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
    else
    {
        return 'O';
    }
}