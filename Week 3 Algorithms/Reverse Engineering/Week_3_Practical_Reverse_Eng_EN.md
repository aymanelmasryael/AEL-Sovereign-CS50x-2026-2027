# AEL CS Encyclopedia™ – Week 3 Algorithms: Practical Reverse Engineering (EN)

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
  "domain": "Week 3: Binary Forensics & Voting Mechanics",
  "document_type": "05_Practical_Reverse_Eng (EN)",
  "methodology": "8-Stage Sub-Silicon Execution Paradigm",
  "execution_state": "Active Master Verification"
}
```

---

## 🔬 Section 1: Forensic Deconstruction of Opaque `Sort` Binaries

### 1. Decoding `sort1`, `sort2`, and `sort3` via the `time` Routine
In the `Sort` problem set, students are provisioned three compiled opaque binaries and tasked with identifying which deploys Bubble, Selection, or Merge sort.
*   **Behavioral Forensics Methodology**: Developers feed massive dataset buffers (`random50000.txt`, `sorted50000.txt`, `reversed50000.txt`) into the executables while profiling execution times via the CLI:
    ```bash
    time ./sort1 sorted50000.txt
    time ./sort2 sorted50000.txt
    time ./sort3 sorted50000.txt
    ```

```text
===================================================================================
             RUNTIME BEHAVIORAL FORENSICS (TIMING ANALYSIS)
===================================================================================

  [ Binary A ] ──> Fast on sorted, VERY slow on reversed ──> BUBBLE SORT (Omega(n))
  [ Binary B ] ──> Slow on sorted, slow on reversed      ──> SELECTION SORT (Theta(n^2))
  [ Binary C ] ──> Blazing fast on ALL inputs            ──> MERGE SORT (O(n log n))

===================================================================================
```

---

## 🧠 Section 2: Architectural Deconstruction of Voting Systems (`Plurality & Runoff`)

### 1. Deconstructing `Plurality` (First-Past-The-Post)
*   **Architectural Concept**: Each voter selects exactly one candidate. Developers maintain a static array of custom structs `candidates[]`.
*   **Subroutine Logic**: The `vote(string name)` routine inspects candidate validity and increments `candidates[i].votes++`. The `print_winner` routine establishes the absolute global maximum vote count and prints all candidates tied at that boundary.

### 2. Deconstructing `Runoff` (Instant-Runoff Voting)
*   **Two-Dimensional Preference Table (`preferences[i][j]`)**: Directly records voter `i`'s ranked preference at index `j`.
*   **Elimination Architecture**: If no single candidate achieves strict mathematical majority (>50%), the system isolates the candidate holding the lowest vote total and mutates their state flag to `eliminated = true`. During the subsequent tabulation loop, ballots assigned to eliminated candidates automatically roll over to the voter's next highest valid preference.

---

## 🧬 Section 3: The Apex CS50x Challenge – `Tideman` Architecture

### 1. The Condorcet Paradigm
The `Tideman` project stands canonically as the most formidable algorithmic test in CS50x. Its explicit goal is isolating the candidate who achieves decisive head-to-head pairwise victory against all alternate contenders.

```text
===================================================================================
             TIDEMAN 3-STAGE GRAPH EXECUTION PIPELINE
===================================================================================

  [ TALLY ]  ──> Record pairwise victory margins in pairs[] array
     │
  [ SORT ]   ──> Sort pairs by victory strength (highest margin first)
     │
  [ LOCK ]   ──> Lock edges in locked[][] graph ONLY IF no cycle is created (DFS)

===================================================================================
```

### 2. Cycle Detection Mechanics (`lock_pairs`)
The core architectural complexity resides inside `lock_pairs`. Before locking a directed edge (`locked[winner][loser] = true`), the engine must verify that no transitive graph path routes the loser back to the initial winner (`Cycle`).
*   **Depth-First Search (`DFS Recursion`)**: Developers construct a recursive structural verification routine traversing the `locked` adjacency matrix. If a prospective edge path routes back to the root node, the lock operation is explicitly aborted to guarantee the existence of an acyclic graph source (`Source`).

---
**© Ayman Elmasry — AEL Digital Studio™ · All Rights Reserved.**
