# AEL CS Encyclopedia™ – Week 3 Algorithms: Deep Internal Analysis (EN)

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
  "domain": "Week 3: Mathematical Recurrences & Memory Padding",
  "document_type": "04_Deep_Internal_Analysis (EN)",
  "methodology": "8-Stage Sub-Silicon Execution Paradigm",
  "execution_state": "Active Master Verification"
}
```

---

## 🔬 Section 1: Mathematical Proof of Merge Sort Asymptotics

### 1. The Recursion Tree Method
To definitively substantiate how Merge Sort achieves an asymptotic runtime leap from $O(n^2)$ down to $O(n \log n)$, we deconstruct its formal recurrence relation (`Recurrence Relation`):
$$T(n) = 2T\left(\frac{n}{2}\right) + O(n)$$
*   **Mathematical Deconstruction**:
    *   Bisecting the array into halves requires bounded constant time $O(1)$.
    *   Self-invoking the sorting subroutine upon each sub-partition yields $2T(n/2)$.
    *   The merge operation demands linear inspection across every discrete element, incurring $O(n)$ overhead.

```text
===================================================================================
             MERGE SORT RECURSION TREE DECONSTRUCTION
===================================================================================

  Level 0:                     [ n ]                     ──> Work: O(n)
                             /       \
  Level 1:             [ n/2 ]       [ n/2 ]             ──> Work: 2 * (n/2) = O(n)
                      /      \       /      \
  Level 2:        [ n/4 ] [ n/4 ] [ n/4 ] [ n/4 ]        ──> Work: 4 * (n/4) = O(n)
  
  Total Depth = log_2(n)  ──> Total Execution Cost: O(n * log n)

===================================================================================
```

---

## 🧠 Section 2: Memory Alignment & Struct Padding

### 1. Physical Memory Alignment Boundaries
When instantiating a custom `struct` within bare-metal C, developers intuitively assume the total byte allocation equals the sum of its primitive members. However, physical central processing units enforce strict structural alignment constraints (`Memory Alignment`).
*   **The Padding Phenomenon**: CPUs fetch memory blocks along 4-byte or 8-byte word boundaries. If a `struct` contains a `char` (1 Byte) followed by an `int` (4 Bytes), the bare-metal compiler automatically injects 3 unused padding bytes (`Padding Bytes`) to preserve word alignment for the integer payload.

```text
===================================================================================
             PHYSICAL STRUCT MEMORY PADDING (64-BIT ARCH)
===================================================================================

  struct { char c; int i; };
  
  [ 0x100: char c (1B) ] [ 0x101-0x103: PADDING (3B) ] [ 0x104-0x107: int i (4B) ]
  
  Total Size in RAM = 8 Bytes (not 5 Bytes!)

===================================================================================
```

---

## 🧬 Section 3: Activation Record Dynamics during Deep Recursion

### 1. Anatomy of a Stack Frame (`Activation Record`)
Every discrete invocation of a recursive subroutine forces the runtime kernel to push an activation record containing:
1. **Return Address**: The instruction pointer indicating where execution resumes post-termination.
2. **Arguments**: Parameter values passed directly into the frame.
3. **Local Variables**: Stack-allocated temporary variables.

If the cumulative recursive call depth breaches the allocated stack bound (typically restricted to several megabytes across modern OS architectures), the Memory Management Unit (`MMU`) intercepts the hardware trap and dispatches a fatal `SIGSEGV` segmentation fault to immediately terminate the process.

---
**© Ayman Elmasry — AEL Digital Studio™ · All Rights Reserved.**
