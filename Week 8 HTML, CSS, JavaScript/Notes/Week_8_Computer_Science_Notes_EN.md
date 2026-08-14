# AEL CS Encyclopedia™ – Week 8: Computer Science Notes (EN)

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
  "domain": "Week 8 (HTML, CSS, JavaScript): DOM Architecture, Event Loops & UI Glassmorphism",
  "document_type": "02_Computer_Science_Notes (EN)",
  "methodology": "8-Stage Sub-Silicon Execution Paradigm",
  "execution_state": "Active Master Verification"
}
```

---

## 🧠 Computer Science Notes: Tree Graphs & The Asynchronous Event Loop

### 1. Document Object Model (`DOM`) Tree Hierarchy
*   **Tree Data Structure**: The browser engine parses raw `HTML` tags and constructs a structural tree graph of `Nodes`. The root object is `document`, branching down to `html`, `head`, and `body`.
*   **Memory Footprint**: Every single node within the DOM tree is instantiated in memory as a live, dynamic object containing mutable properties and callable methods accessible directly via runtime scripting.

```text
===================================================================================
                   DOCUMENT OBJECT MODEL (DOM) TREE HIERARCHY
===================================================================================

                        [ document ]
                             │
                         [ html ]
                        /        \
                    [ head ]    [ body ]
                                 /    \
                           [ div ]    [ section ]
                            /   \          │
                       [ h1 ]  [ p ]    [ button ]

===================================================================================
```

### 2. The Asynchronous Event Loop
JavaScript operates natively as a single-threaded runtime environment but achieves non-blocking, highly concurrent behavior through a dedicated orchestration system:
*   **Call Stack**: Operates on a Last-In, First-Out (`LIFO`) basis, executing active synchronous function frames.
*   **Web APIs**: Browser-managed background threads handling timers (`setTimeout`), network sockets (`fetch`), and event listeners.
*   **Task Queue**: Operates on a First-In, First-Out (`FIFO`) basis, queuing completed callback functions ready for execution.
*   **Event Loop**: The perpetual sentinel that monitors the Call Stack; the moment the stack clears, it instantly pushes the next callback from the Task Queue onto the stack.

---
**© Ayman Elmasry — AEL Digital Studio™ · All Rights Reserved.**
