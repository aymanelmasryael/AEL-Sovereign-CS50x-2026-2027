# AEL CS Encyclopedia™ – Week 3 Algorithms: Engineering Notes (EN)

---

## 🏛️ AEL Sovereign Identity Block

```json
{
  "ael_seal": "AEL CS Encyclopedia — © Ayman Elmasry",
  "owner": "Ayman Elmasry",
  "legal_entities": [
    "Ayman Elmasry LLC (UAE)",
    "Ayman Elmasry Advertising & Marketing (Egypt)"
  ],
  "syllabus_source": "Harvard CS50x 2026-2027",
  "domain": "Week 3: String Comparison & Applied Sorting",
  "document_type": "03_Engineering_Notes (EN)",
  "methodology": "8-Stage Sub-Silicon Execution Paradigm",
  "execution_state": "Active Master Verification"
}
```

---

## 🔬 Section 1: The String Comparison Trap

### 1. Why `==` Fails for Strings in Bare-Metal C
When searching for a scalar integer within an array, developers evaluate `if (numbers[i] == target)`. However, executing `if (strings[i] == "David")` in bare-metal C induces a catastrophic logical defect.
*   **Engineering Deconstruction**: A string variable is fundamentally a pointer holding a memory address (`Memory Address`). The `==` operator compares the physical memory addresses rather than the underlying character stream. Therefore, even if two strings are textually identical, they occupy distinct memory locations, resulting in a false equality evaluation.

```text
===================================================================================
             STRING EQUALITY MEMORY TRAP
===================================================================================

  if (s == t)  ──>  Compares Address 0x100 with Address 0x200 (Result: FALSE!)
  
  if (strcmp(s, t) == 0) ──> Compares character values 'D','A','V','I','D' (Result: TRUE!)

===================================================================================
```

### 2. `<string.h>` and the `strcmp` Routine
To evaluate actual character stream equality, developers must invoke `strcmp`, which returns exactly `0` when both strings match perfectly:
```c
#include <cs50.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    string names[] = {"Bill", "Charlie", "Fred", "George", "Ginny", "Percy", "Ron"};

    for (int i = 0; i < 7; i++)
    {
        if (strcmp(names[i], "Ron") == 0)
        {
            printf("Found\n");
            return 0; // Exit success
        }
    }
    printf("Not found\n");
    return 1; // Exit failure
}
```

---

## 🧠 Section 2: Applied `typedef struct` Implementations

### 1. Declaration within Global Scope
To ensure all function prototypes and subroutines recognize the custom structural declaration, it must be instantiated in the global scope prior to `main`:
```c
#include <cs50.h>
#include <stdio.h>
#include <string.h>

typedef struct
{
    string name;
    string number;
}
person;

int main(void)
{
    person people[2];

    people[0].name = "Carter";
    people[0].number = "+1-617-495-1000";

    people[1].name = "David";
    people[1].number = "+1-949-468-2750";

    for (int i = 0; i < 2; i++)
    {
        if (strcmp(people[i].name, "David") == 0)
        {
            printf("Found %s\n", people[i].number);
            return 0;
        }
    }
    printf("Not found\n");
    return 1;
}
```
*   **Dot Notation**: Developers deploy dot notation (`.name` or `.number`) to directly access internal member fields within the `struct` memory wrapper.

---

## 🧬 Section 3: Bubble Sort Architectural Optimization

### 1. Integrating the `swapped` Monitoring Flag
In unoptimized Bubble Sort implementations, the execution loop continues exhaustive comparisons even if the target buffer achieves sorted equilibrium early. To optimize the best-case runtime down to $\Omega(n)$, developers inject a tracking boolean `bool swapped`:

```c
bool swapped;
do
{
    swapped = false;
    for (int i = 0; i < n - 1; i++)
    {
        if (arr[i] > arr[i + 1])
        {
            int temp = arr[i];
            arr[i] = arr[i + 1];
            arr[i + 1] = temp;
            swapped = true;
        }
    }
}
while (swapped);
```
If a complete inner loop executes without a single swap (`swapped == false`), the execution halts instantly, conserving thousands of processor clock cycles (`CPU Cycles`).

---
**© Ayman Elmasry — AEL Digital Studio™ · All Rights Reserved.**
