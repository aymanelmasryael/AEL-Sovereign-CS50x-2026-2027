# Projects Architectural Design: Week 1 - C (Mario, Cash & Credit Systems)

---

## 🏛️ Architecture of Classic CS50 Problem Sets

The problem sets of Week 1 in `C` represent the student's primary rite of passage into bare-metal, algorithmically intensive software engineering (`Problem-Solving Architecture`). In this wing, we deconstruct the underlying algorithmic paradigms and execution strategies for three seminal projects within the `CS50x` curriculum:

```text
===================================================================================
                       WEEK 1 C - PROJECTS ARCHITECTURE
===================================================================================

  ┌────────────────────────────────────────────────────────┐
  │ 1. MARIO (Less & More Comfortable)                     │ ──> Nested Loops & Alignment
  ├────────────────────────────────────────────────────────┤
  │ 2. CASH (Less Comfortable)                             │ ──> The Greedy Algorithm
  ├────────────────────────────────────────────────────────┤
  │ 3. CREDIT (More Comfortable)                           │ ──> Luhn's Algorithm & Modulo Math
  └────────────────────────────────────────────────────────┘

===================================================================================
```

---

## 🧱 1. The Mario Pyramid Architecture (`Mario - Less & More Comfortable`)

Inspired by the structural brick blocks of `Super Mario Bros`, this project requires students to print ascending and descending text-based pyramids in the console terminal. The core architectural goals are mastering the geometric arithmetic of nested loops (`Nested Loops`) and enforcing strict user input validation (`Input Validation`).

### `Mario - Less Comfortable` (Single Right-Aligned Pyramid)
*   **Engineering Requirement:** Construct a single right-aligned pyramid with a user-defined height spanning between 1 and 8.
*   **Structural Grid Schematic:**
```text
   #  (3 spaces, 1 hash)
  ##  (2 spaces, 2 hashes)
 ###  (1 space,  3 hashes)
####  (0 spaces, 4 hashes)
```
*   **Loop Arithmetic Principles:**
    *   For any zero-indexed row `i` (ranging from `0` to `height - 1`), the required number of leading spaces is `spaces = height - i - 1`.
    *   The required number of brick hashes is `hashes = i + 1`.

### `Mario - More Comfortable` (Double Adjacent Pyramids)
*   **Engineering Requirement:** Construct two adjacent, opposing pyramids separated by a fixed static gap of exactly two spaces `  `.
*   **Structural Grid Schematic:**
```text
   #  #
  ##  ##
 ###  ###
####  ####
```
*   **Bare-Metal Implementation (`mario.c`):**
```c
#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int height;
    // 1. Input Validation (Enforce integer between 1 and 8)
    do
    {
        height = get_int("Height: ");
    }
    while (height < 1 || height > 8);

    // 2. Master Loop for Rows
    for (int i = 0; i < height; i++)
    {
        // Print leading spaces
        for (int spaces = 0; spaces < height - i - 1; spaces++)
        {
            printf(" ");
        }

        // Print left pyramid hashes
        for (int hashes = 0; hashes <= i; hashes++)
        {
            printf("#");
        }

        // Print middle static gap
        printf("  ");

        // Print right pyramid hashes
        for (int hashes = 0; hashes <= i; hashes++)
        {
            printf("#");
        }

        // Move to next row
        printf("\n");
    }
}
```

---

## 🪙 2. The Change Machine & The Greedy Algorithm (`Cash`)

In the `Cash` problem set, engineers architect an optimized coin-dispensation engine calculated to return the absolute minimum number of coins to a customer.

```text
===================================================================================
                       THE GREEDY ALGORITHM PIPELINE
===================================================================================

  [ Cents Input: e.g., 68¢ ]
            │
            ├─► Quarters (25¢) ──> 68 / 25 = 2 (Remainder: 18¢)
            ├─► Dimes (10¢)    ──> 18 / 10 = 1 (Remainder: 8¢)
            ├─► Nickels (5¢)   ──> 8 / 5   = 1 (Remainder: 3¢)
            └─► Pennies (1¢)   ──> 3 / 1   = 3 (Remainder: 0¢)
            
  [ Total Minimum Coins = 2 + 1 + 1 + 3 = 7 Coins ]

===================================================================================
```

### Deconstructing the Greedy Paradigm (`Greedy Algorithm`):
*   **Algorithmic Principle:** A greedy algorithm executes the local optimal decision at each distinct stage of execution, without concern for long-term algorithmic implications. For standard American coin denominations (`25¢, 10¢, 5¢, 1¢`), mathematical proofs demonstrate that this greedy heuristic reliably achieves the global optimum.
*   **Bare-Metal Implementation (`cash.c`):**
```c
#include <cs50.h>
#include <stdio.h>

int calculate_quarters(int cents);
int calculate_dimes(int cents);
int calculate_nickels(int cents);
int calculate_pennies(int cents);

int main(void)
{
    int cents;
    do
    {
        cents = get_int("Change owed: ");
    }
    while (cents < 0);

    int quarters = calculate_quarters(cents);
    cents = cents - (quarters * 25);

    int dimes = calculate_dimes(cents);
    cents = cents - (dimes * 10);

    int nickels = calculate_nickels(cents);
    cents = cents - (nickels * 5);

    int pennies = calculate_pennies(cents);
    cents = cents - (pennies * 1);

    int coins = quarters + dimes + nickels + pennies;
    printf("%i\n", coins);
}

int calculate_quarters(int cents) { return cents / 25; }
int calculate_dimes(int cents)    { return cents / 10; }
int calculate_nickels(int cents)  { return cents / 5; }
int calculate_pennies(int cents)  { return cents / 1; }
```

---

## 💳 3. Verification Engine: Luhn's Algorithm (`Credit`)

The `Credit` problem set represents an advanced engineering challenge requiring the architectural verification of credit card numbers (`Visa, MasterCard, American Express`) utilizing **Luhn's Checksum Algorithm (`Luhn's Algorithm`)**.

```text
===================================================================================
                       LUHN'S CARD VERIFICATION ALGORITHM
===================================================================================

  [ Card Number: e.g., 4 0 1 2 8 8 8 8 8 8 8 8 8 8 8 3 ] (Length 16, Starts 4 -> Visa)
            │
            ├─► 1. Multiply every second digit from right by 2 (Add product digits)
            ├─► 2. Add sum of digits that weren't multiplied
            └─► 3. If total modulo 10 == 0, Card is VALID.

===================================================================================
```

### 1. Standard Enterprise Industry Classifications:
*   **American Express (`AMEX`):** Exactly `15` digits in length; starts with prefixes `34` or `37`.
*   **MasterCard:** Exactly `16` digits in length; starts with prefixes spanning `51` through `55`.
*   **Visa:** Exactly `13` or `16` digits in length; starts with prefix `4`.

### 2. Deconstructing Luhn's Architectural Checksum:
1.  **Stage One:** Commencing from the second-to-last digit and traversing leftward, multiply every second digit by `2`.
    *   *Critical Engineering Detail:* If the resulting product yields a two-digit number (e.g., `6 * 2 = 12`), do not add `12` to the sum. Instead, split the product and sum its individual digits (`1 + 2 = 3`).
2.  **Stage Two:** Sum all remaining digits that were skipped (not multiplied by 2).
3.  **Stage Three:** Combine both sums. If the terminal digit of the aggregate total equals zero (`total_sum % 10 == 0`), the checksum is mathematically validated.

### 3. Modulo Math & Low-Level Extraction (`Modulo & Division Math`):
Because high-level string manipulation is restricted in early C problem sets, engineers must parse card digits directly from numerical types. By executing modulo division `card % 10`, the system extracts the trailing rightmost digit. Executing standard division `card /= 10` permanently truncates the trailing digit. Given the 16-digit magnitude of credit card numbers, developers must allocate 64-bit `long` variables to prevent catastrophic overflow (`Overflow`).

---
**© Ayman Elmasry — AEL Digital Studio™ · All Rights Reserved.**
