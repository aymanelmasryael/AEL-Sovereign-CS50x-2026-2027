# AEL CS Encyclopedia™ – Week 5 Data Structures: CS Notes (EN)

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
  "domain": "Week 5 Data Structures: Abstract Data Types & Asymptotic Topology",
  "document_type": "02_Computer_Science_Notes (EN)",
  "methodology": "8-Stage Sub-Silicon Execution Paradigm",
  "execution_state": "Active Master Verification"
}
```

---

## 🔬 Section 1: Abstract Data Types (ADTs)

### 1. The Power of Structural Abstraction
Abstract Data Types (`ADTs`), such as Queues and Stacks, represent standardized behavioral interfaces completely decoupled from their underlying silicon memory layout. A Stack (`LIFO: Last In, First Out`) or a Queue (`FIFO: First In, First Out`) can be physically engineered using either contiguous static arrays or dynamic pointer-backed linked lists.

```text
===================================================================================
             STACK (LIFO) VS QUEUE (FIFO) EXECUTION
===================================================================================

  STACK (LIFO)                            QUEUE (FIFO)
  ┌──────────┐                            ┌─────┬─────┬─────┬─────┐
  │  Top     │ ◄── Push / Pop             │  1  │  2  │  3  │  4  │ ──► Dequeue
  ├──────────┤                            └─────┴─────┴─────┴─────┘
  │  Bottom  │                               ▲
  └──────────┘                               └─ Enqueue

===================================================================================
```

---

## 🧠 Section 2: Asymptotic Topology & Benchmarking

### 1. Master Asymptotic Trade-off Matrix

| Data Structure | Search Cost | Insert Cost | Delete Cost | Architectural Engineering Note |
| :--- | :--- | :--- | :--- | :--- |
| **Static Array** | `O(1)` (Index) / `O(n)` | `O(n)` (Shifting) | `O(n)` (Shifting) | Exceptional cache locality and spatial contiguity. |
| **Singly Linked List** | `O(n)` (Linear sweep) | `O(1)` (Head insertion) | `O(1)` (With pointer) | Eliminates `realloc` overhead, but forfeits binary search capabilities. |
| **Hash Table (Chaining)** | `O(1)` (Avg) / `O(n)` | `O(1)` (Head insertion) | `O(1)` (Head insertion) | Runtime efficiency depends entirely on Hash Function distribution. |
| **Trie Tree** | `O(k)` (Word length) | `O(k)` (Word length) | `O(k)` (Word length) | Massive pointer memory overhead in exchange for ultra-fast, predictable latency. |
| **Balanced BST** | `O(log n)` | `O(log n)` | `O(log n)` | Guarantees stable logarithmic scaling but requires complex balancing algorithms. |

---

## 🧬 Section 3: Hash Functions & Collision Resolution

### 1. The Mathematical Inevitability of Collisions
Per the Pigeonhole Principle, if the universe of possible keys exceeds the table capacity `N`, hash collisions are mathematically inevitable.

### 2. Resolution Topologies
*   **Open Addressing (Linear Probing)**: Upon detecting a collision, the kernel searches for the next available sequential slot. This approach suffers from severe primary clustering.
*   **Chaining**: Every slot in the table acts as a pointer to the head of an independent linked list. This represents the premier architecture for systems with highly dynamic datasets.

---
**© Ayman Elmasry — AEL Digital Studio™ · All Rights Reserved.**
