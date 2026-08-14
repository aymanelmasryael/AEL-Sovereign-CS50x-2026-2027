# AEL CS Encyclopedia™ – Week 4 Memory: Computer Science Notes (EN)

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
  "domain": "Week 4 Memory: Theoretical Foundations & Memory Math",
  "document_type": "Notes (EN)",
  "methodology": "8-Stage Sub-Silicon Execution Paradigm",
  "execution_state": "Active Master Verification"
}
```

---

## 🚀 Theoretical Core: Pointer Algebra & Word Alignment

In theoretical computer science, memory is modeled not as disparate variables, but as a continuous **addressable vector space (`Memory Address Space`)**. Week 4 explores the absolute mathematical laws governing how execution engines traverse and manipulate these discrete address cells.

---

## 🛡️ Theoretical CS Pillars of Memory

### 1. Pointer Arithmetic
*   **Mathematical Formula**: Adding an integer $K$ to a pointer $P$ of type $T$ (`T *P`) instructs the underlying ALU to advance the base address by $K \times \text{sizeof}(T)$ bytes.
*   **Proof of Execution**: Given `int *p = 0x1000`, the expression `p + 1` increments the memory register to `0x1004` (as a standard 32-bit integer spans 4 bytes).

```text
===================================================================================
             POINTER ARITHMETIC MULTIPLICATION MATRIX
===================================================================================

  [ Base Address: 0x1000 ] (int *p)
   │
   ├─► p + 1 ──> 0x1000 + (1 * 4 bytes) ──> 0x1004
   ├─► p + 2 ──> 0x1000 + (2 * 4 bytes) ──> 0x1008
   └─► p + 3 ──> 0x1000 + (3 * 4 bytes) ──> 0x100C

===================================================================================
```

---

### 2. Struct Padding & Data Alignment
*   **Hardware Mechanics**: Modern 64-bit microprocessors fetch memory in uniform 8-byte chunks (`Word Boundaries`). When a 1-byte character is declared adjacent to a 4-byte integer within a `struct`, the compiler actively injects uninitialized `Padding Bytes` to ensure natural word alignment and prevent CPU cycle penalties.

---

### 3. Asymptotic Boundary Violations (`Stack vs. Buffer Overflow`)
*   **`Stack Overflow`**: Triggered when unbounded recursive calls exhaust the fixed stack allocation frame, causing the stack pointer to collide directly with the active heap space.
*   **`Buffer Overflow`**: Triggered when unchecked input exceeds the provisioned byte length of a buffer, corrupting adjacent stack memory and overwriting the function's base return address pointer.

---
**© Ayman Elmasry — AEL Digital Studio™ · All Rights Reserved.**
