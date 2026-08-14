# Computer Science Notes: Week 1 - C (Core Syntax, Data Types & Flow Control)

---

## 🏛️ Anatomy of C & `src1` Inspection

`C` is a procedural, general-purpose language that affords direct memory access and low-level architectural control. In this pillar, we delve into the rigorous academic foundations of computer science, referencing the invaluable code archive `src1` and `lecture1.pdf`.

```text
===================================================================================
                       THE ANATOMY OF A C PROGRAM (hello2.c)
===================================================================================

  #include <cs50.h>    ──> Preprocessor Directive (CS50 Library)
  #include <stdio.h>   ──> Preprocessor Directive (Standard Input/Output)

  int main(void)       ──> Main Entry Point (Returns int 0 on success, takes void)
  {
      string answer = get_string("What's your name? "); ──> Variable Instantiation
      printf("hello, %s\n", answer);                    ──> Formatted Output (%s)
  }

===================================================================================
```

---

## 📦 Data Types & Format Specifiers

C is a statically typed language. Every variable must be declared with an explicit data type, defining both its exact memory footprint in bytes and the semantic interpretation of its underlying binary sequences.

### 1. Primitive Data Types (`Primitive Types`)
*   `int`: Integers. Typically occupies 4 bytes (`32 bits`) of memory, representing values from `-2,147,483,648` to `2,147,483,647`. (Directly referenced in `calculator0.c`).
*   `char`: Single character. Occupies 1 byte (`8 bits`) and is mapped to numeric integer equivalents via the `ASCII` standard table. (Referenced in `agree0.c`).
*   `float`: Single-precision floating-point numbers. Occupies 4 bytes. Susceptible to floating-point imprecision due to limited fractional bits. (Referenced in `calculator2.c`).
*   `double`: Double-precision floating-point numbers. Occupies 8 bytes (`64 bits`), affording expanded mantissa precision for rigorous computational calculations.
*   `bool`: Boolean values (`true / false`). Accessible via the `<cs50.h>` library or standard `<stdbool.h>`.
*   `string`: Character strings. Structurally represented as an array of characters (`Array of chars`) terminated by a null byte (`\0`).

### 2. Standard Output Format Specifiers (`Format Specifiers for printf`)
| Format Specifier | Associated Data Type | Explicit Architectural Utility |
| :--- | :--- | :--- |
| `%i` or `%d` | `int` | Printing standard integer values |
| `%c` | `char` | Printing singular characters |
| `%f` | `float` or `double` | Printing floating-point numbers (e.g., `%.2f` for two decimal places) |
| `%s` | `string` | Printing null-terminated character arrays |

---

## 🔀 Control Flow & Logical Evaluation

Control flow constructs empower programs to make dynamic runtime decisions based on evaluating boolean conditions. Here we examine paradigms extracted from `compare` and `agree` codebases.

### 1. Conditional Branching (`Conditionals - compare0.c`)
```c
if (x < y)
{
    printf("x is less than y\n");
}
else if (x > y)
{
    printf("x is greater than y\n");
}
else
{
    printf("x is equal to y\n");
}
```

### 2. Boolean Logical Operators (`Logical Operators - agree2.c`)
*   `&&` (AND): Evaluates to true if and only if both operands are true.
*   `||` (OR): Evaluates to true if at least one operand is true.
*   `!` (NOT): Unary negation operator; inverts the boolean state of its operand.

```c
if (c == 'y' || c == 'Y')
{
    printf("Agreed.\n");
}
```

---

## 🔄 Iteration Constructs & Modular Design

Repetition is the bedrock of computational efficiency. Rather than duplicating logic, we leverage loop constructs and function abstraction, as demonstrated in `cat` and `mario`.

### 1. Iteration Paradigms (`Iteration Paradigms - cat15.c & mario5.c`)
*   `while`: Condition-controlled loops. Optimal when the exact iteration count is unknown prior to runtime.
*   `for`: Counter-controlled loops featuring explicit initialization, condition evaluation, and update clauses (`Initialization; Condition; Update`).
*   `do-while`: Post-test loops. Guaranteed to execute the inner block at least once prior to evaluating the exit condition. **Representing the gold standard for robust user input validation (`Input Validation`)**.

```c
// Input Validation using do-while (mario5.c)
int n;
do
{
    n = get_int("Size: ");
}
while (n < 1);
```

### 2. Modular Design & Function Prototypes (`Prototypes & Custom Functions`)
In `cat15.c`, Malan elucidates a foundational rule of C compilers: source files are parsed sequentially from top to bottom. Calling a custom function like `meow(3)` before it is explicitly declared results in a fatal compiler error.
*   **Architectural Solution (`Function Prototype`):** Declaring the function signature at the top of the file (`void meow(int n);`), while housing the actual implementation logic below the `main` entry point.

```c
#include <stdio.h>

void meow(int n); // Prototype Declaration

int main(void)
{
    meow(3);
}

void meow(int n) // Internal Implementation
{
    for (int i = 0; i < n; i++)
    {
        printf("meow\n");
    }
}
```

---
**© Ayman Elmasry — AEL Digital Studio™ · All Rights Reserved.**
