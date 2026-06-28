# AEL CS Encyclopedia™ – Week 3 Algorithms: Projects Overview (EN)

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
  "domain": "Week 3: Algorithm Implementation & Graph Verification",
  "document_type": "06_Projects (EN)",
  "methodology": "8-Stage Sub-Silicon Execution Paradigm",
  "execution_state": "Active Master Verification"
}
```

---

## 🚀 Overview of Week 3 Projects

Week 3 contains several of the most intellectually rigorous algorithmic challenges in the computer science curriculum, spanning empirical binary forensics to high-complexity voting protocols and directed acyclic graphs.

---

## 🛡️ Project 1: Opaque Binary Sorting Forensics (`Sort`)

### 1. Architectural Objective
This exercise trains the software engineer in empirical behavioral forensics (`Software Forensics`). The student is provided three pre-compiled executables (`sort1, sort2, sort3`) and tasked with deducing the exact internal sorting algorithm (Bubble, Selection, or Merge Sort) by monitoring runtime execution complexity (`Time Complexity`) across randomized, sorted, and reversed datasets.

```text
===================================================================================
             SORT PROBLEM ASYMPTOTIC FORENSICS LOGIC
===================================================================================

  [ Run Time Experiment ] ──> Observe best/worst case speed ──> Categorize Algorithm
  (Random / Reversed / Sorted buffers)

===================================================================================
```

---

## 🔬 Project 2: Majoritarian & Instant-Runoff Voting (`Plurality & Runoff`)

### 1. Plurality Architecture (First-Past-The-Post)
*   **Design**: Declaration of a custom `candidate` struct maintaining a string identifier and an integer vote accumulator.
*   **Challenge**: Executing a linear search during ballot ingestion, followed by a global maximum search to print the winner (or multiple tied winners).

### 2. Runoff Architecture (Ranked-Choice Voting)
*   **Design**: Ingestion of complete voter preferences into a two-dimensional matrix `preferences[i][j]`.
*   **Challenge**: If no single candidate secures strict majority (>50%), the system isolates the candidate with the lowest tally (`eliminate`) and rolls their ballots over to the voter's subsequent valid preference.

---

## 🧬 Project 3: The Absolute Apex Challenge – `Tideman`

### 1. Architectural Paradigm
Tideman overcomes the mathematical vulnerabilities of conventional voting systems by isolating the "Condorcet Winner"—the single candidate who would decisively win a head-to-head election against every other candidate in the field.

```text
===================================================================================
             TIDEMAN GRAPH LOCKING PIPELINE (ACYCLIC DIRECTED GRAPH)
===================================================================================

  [ Pair Tabulation ] ──> [ Sort by Victory Margin ] ──> [ Lock Pairs (No Cycles!) ]

===================================================================================
```

### 2. The Apex Algorithmic Hurdle (`lock_pairs`)
To prevent the formation of a closed circular graph (`Cycle`) which would render the election unwinnable, developers must implement a Depth-First Search (`DFS`) recursive graph traversal algorithm to verify that no transitive path connects the prospective loser back to the winner before locking the directed edge inside the `locked[][]` matrix.

---
**© Ayman Elmasry — AEL Digital Studio™ · All Rights Reserved.**
