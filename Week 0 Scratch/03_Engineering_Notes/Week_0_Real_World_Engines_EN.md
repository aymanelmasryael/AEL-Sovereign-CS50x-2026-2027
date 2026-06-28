# AEL Engineering Notes
## Week 0: Architectural Complexity, Real-World Systems & The C/Scratch Rosetta Stone

---

## 📐 Unit 1: Asymptotic Time Complexity Analysis (Big O Curves)

In slides 94 through 99 of `lecture0.pdf`, David Malan introduces the visual graphs that form the bedrock of algorithmic comparison in computer science. In enterprise systems engineering, we define this rigorous evaluation as **Asymptotic Computational Complexity**.

```text
Time to Solve
  ^
  |   / n (Linear - 1 by 1)
  |  /  n/2 (Linear - 2 by 2)
  | /
  |/_______________________ log2(n) (Logarithmic Search)
  +---------------------------------------------> Size of Problem (n)
```

### 🔬 Architectural Deconstruction of the Three Curves:

#### 1. $O(n)$ Curve - Step-by-Step Linear Search:
*   **Lecture Mechanism:** Flipping through a phone book page by page from beginning to end.
*   **Engineering Evaluation:** Represents a pure 1:1 linear correlation. If the data set contains $N=1000$ records, the processor requires exactly 1000 operations in the worst-case scenario. Architecturally unsustainable for massive enterprise databases.

#### 2. $O(n/2)$ Curve - Two-by-Two Page Skipping:
*   **Lecture Mechanism:** Flipping through the phone book two pages at a time.
*   **Engineering Evaluation:** While the physical curve appears slightly steeper (solving the problem in 500 steps instead of 1000), mathematically and architecturally, the growth order remains linear $O(n)$. It also introduces the engineering risk of overshooting the target and requiring a rollback step.

#### 3. $O(\log_2 n)$ Curve - Binary Search:
*   **Lecture Mechanism:** Opening the phone book directly in the middle, evaluating the target half, tearing the incorrect half away, and repeating the process on the remainder.
*   **Engineering Pinnacle:** This is the "Holy Grail" of computer science. No matter how massively the data set $n$ scales into billions of records, the time curve remains nearly horizontal. For 4 billion pages, the processor locates the precise target in a mere 32 execution steps!

---

## 🏛️ Unit 2: The C vs Scratch Rosetta Stone

In slides 110 and 111, Malan provides a direct historical comparison—a computational "Rosetta Stone"—translating visual Scratch execution blocks directly into bare-metal C syntax.

```c
#include <stdio.h>

int main(void)
{
    printf("hello, world\n");
}
```

```text
[ Scratch Representation ]
( when [Green Flag] clicked )
( say [hello, world]        )
```

### 🔬 Under-the-Hood Comparative Analysis:

| Architectural Element | Bare-Metal C Implementation | Scratch Visual Engine |
| :--- | :--- | :--- |
| **System Initialization** | `int main(void)`: Explicitly defines the primary execution entry point for the CPU and initializes the call stack. | `when green flag clicked`: An event-driven listener managed by the underlying JavaScript runtime engine. |
| **Library Abstraction** | `#include <stdio.h>`: Direct preprocessor directive instructing the compiler to link standard I/O byte streams into the binary. | Fully abstracted; the runtime inherits default rendering engines automatically without explicit linking. |
| **Output Side Effect** | `printf(...)`: Dispatches a formatted byte stream directly to standard terminal output (`stdout`). | `say [...]`: An abstraction layer that updates the VRAM pixel buffer to draw a graphical speech bubble on the canvas. |

---
**© Ayman Elmasry — AEL Digital Studio™ · All Rights Reserved.**
