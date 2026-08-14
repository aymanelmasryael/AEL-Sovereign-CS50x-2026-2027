# AEL CS Encyclopedia™ – Week 3 Algorithms: Computer Science Notes (EN)

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
  "domain": "Week 3: Data Structs & Computational Theory",
  "document_type": "02_Computer_Science_Notes (EN)",
  "methodology": "8-Stage Sub-Silicon Execution Paradigm",
  "execution_state": "Active Master Verification"
}
```

---

## 🔬 Section 1: Memory Encapsulation & `Struct` Architecture

### 1. The Architectural Limitation of Parallel Arrays
During initial iterations of phone book construction, developers are forced to instantiate decoupled parallel buffers (`names[]` and `numbers[]`).
*   **Engineering Vulnerability**: There exists zero physical memory correlation between `names[0]` and `numbers[0]`. If a sorting algorithm mutates the names array without perfectly shadowing index swaps across the numbers array, the relational integrity of the database is irrevocably corrupted.

### 2. Custom Structural Instantiation (`typedef struct`)
To resolve this architectural deficiency, bare-metal C provides an explicit mechanism to declare custom composite data types that encapsulate multiple variables inside a unified memory block:
```c
typedef struct
{
    string name;
    string number;
}
person;

// Memory instantiation
person people[2];
people[0].name = "David";
people[0].number = "+1-949-468-2750";
```

```text
===================================================================================
             CUSTOM STRUCT MEMORY ENCAPSULATION
===================================================================================

  [ person: people[0] ]
  ├── name   ──> Pointer to "David"
  └── number ──> Pointer to "+1-949-468-2750"

===================================================================================
```

---

## 🧠 Section 2: Mathematical Sorting Benchmarks

### 1. Comprehensive Asymptotic Benchmark Matrix

| Algorithm Name | Worst Case (`Big O`) | Best Case (`Big Omega`) | Core Paradigm |
| :--- | :--- | :--- | :--- |
| **Bubble Sort** | $O(n^2)$ | $\Omega(n)$ | Monotonic pairwise adjacent comparison and swapping. |
| **Selection Sort** | $O(n^2)$ | $\Omega(n^2)$ | Search for absolute minimum element and index swapping. |
| **Merge Sort** | $O(n \log n)$ | $\Omega(n \log n)$ | Recursive buffer bisection and sorted recombination. |
| **Linear Search** | $O(n)$ | $\Omega(1)$ | Sequential element-by-element linear buffer traversal. |
| **Binary Search** | $O(\log n)$ | $\Omega(1)$ | Halving sorted target partitions recursively. |

### 2. Architectural Understanding of $\Omega(1)$
In search heuristics, $\Omega(1)$ represents the optimal asymptotic boundary: discovering the target payload on the absolute initial inspection (e.g., index `0` in Linear Search, or the exact bisection midpoint in Binary Search).

---

## 🧬 Section 3: Recursive Stack Mechanics & Activation Records

### 1. Stack Frames (`Activation Records`)
When a routine self-invokes (e.g., mathematical `factorial` evaluation or pyramid generation via `draw`), the operating system kernel allocates a distinct activation record (`Stack Frame`) within the execution call stack (`Call Stack`).
*   **The Base Case Hazard**: If a developer omits a definitive base case condition, self-invoking frames accumulate indefinitely until exhausting available stack space, triggering a fatal segmentation fault known canonically as a `Stack Overflow`.

```text
===================================================================================
             RECURSIVE CALL STACK ACCUMULATION (PYRAMID)
===================================================================================

  [ Stack Top: draw(1) ] ──> Base Case Hit (Prints #, returns)
  [ Stack Mid: draw(2) ] ──> Waits for draw(1) to finish, prints ##
  [ Stack Bot: draw(3) ] ──> Waits for draw(2) to finish, prints ###
  [ main()             ] ──> Root execution frame

===================================================================================
```

---
**© Ayman Elmasry — AEL Digital Studio™ · All Rights Reserved.**
