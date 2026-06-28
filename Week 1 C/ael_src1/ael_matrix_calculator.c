/**
 * AEL Sovereign Systems — Custom CS50 Masterpiece (ael_src1)
 * Project: Multi-Dimensional Matrix Calculator & Array Transformation (ael_matrix_calculator.c)
 * Description: Demonstrates 2D array representation in stack memory, nested iteration
 *              structures, and basic matrix multiplication algorithms in bare C.
 */

#include <cs50.h>
#include <stdio.h>

#define SIZE 3

void print_matrix(string title, int matrix[SIZE][SIZE]);
void multiply_matrices(int m1[SIZE][SIZE], int m2[SIZE][SIZE], int result[SIZE][SIZE]);

int main(void)
{
    printf("====================================================\n");
    printf("     AEL COGNITIVE MATRIX MULTIPLICATION ENGINE\n");
    printf("====================================================\n");

    // Sample Matrix Alpha (Static stack allocation)
    int alpha[SIZE][SIZE] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };

    // Sample Matrix Beta (Static stack allocation)
    int beta[SIZE][SIZE] = {
        {9, 8, 7},
        {6, 5, 4},
        {3, 2, 1}
    };

    // Uninitialized Output Matrix
    int gamma[SIZE][SIZE];

    print_matrix("Matrix Alpha (A)", alpha);
    print_matrix("Matrix Beta (B)", beta);

    printf("⚡ Executing O(N^3) Matrix Multiplication Algorithm...\n");
    multiply_matrices(alpha, beta, gamma);

    print_matrix("Result Matrix Gamma (A x B)", gamma);

    printf("====================================================\n");
    return 0;
}

/**
 * Renders a 2D matrix in the console terminal with professional formatting
 */
void print_matrix(string title, int matrix[SIZE][SIZE])
{
    printf("\n--- %s ---\n", title);
    for (int i = 0; i < SIZE; i++)
    {
        printf("[ ");
        for (int j = 0; j < SIZE; j++)
        {
            printf("%3d ", matrix[i][j]);
        }
        printf("]\n");
    }
    printf("\n");
}

/**
 * Computes the matrix dot product using triple nested loops
 */
void multiply_matrices(int m1[SIZE][SIZE], int m2[SIZE][SIZE], int result[SIZE][SIZE])
{
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            result[i][j] = 0; // Initialize accumulator
            for (int k = 0; k < SIZE; k++)
            {
                result[i][j] += m1[i][k] * m2[k][j];
            }
        }
    }
}
