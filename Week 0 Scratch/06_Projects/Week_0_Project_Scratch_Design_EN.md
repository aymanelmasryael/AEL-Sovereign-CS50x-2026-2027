# Master Projects Architecture: Week 0 - Scratch (Oscartime & Ivy's Hardest Game)

---

## 🚀 Benchmark Capstone Projects in `src0` (Cartesian Physics & System Execution)

In the `src0` master asset directory, David Malan supplies the step-by-step architectural source code for two complete game systems. In the AEL engineering paradigm, we evaluate these projects as a masterclass in sensor mechanics, physics loops, and Cartesian coordinate orchestration.

```text
[ Canvas Plane ] ──> [ X: -240 to 240 / Y: -180 to 180 ] ──> [ Collision Engine (Sensing) ]
```

---

## 🎮 1. The Architecture of `Oscartime` (Freefall Physics & Mutation Variables)

The progression of `Oscartime` is structured across 6 successive benchmark builds in `src0`, culminating in the complete `Oscartime.sb3` bundle.

### 📂 Direct Source File Links in `src0`:
*   [Oscartime - 0 (Malan).sb3](file:///Volumes/Apple/انظمه%20تم%20تطويرها%20/نظام%20جديد%20/cs50x%202026/cs50x%202026-2027/src0/Oscartime%20-%200%20(Malan).sb3): Foundational sprite initialization and initial vertical drop vector.
*   [Oscartime - 3 (Malan).sb3](file:///Volumes/Apple/انظمه%20تم%20تطويرها%20/نظام%20جديد%20/cs50x%202026/cs50x%202026-2027/src0/Oscartime%20-%203%20(Malan).sb3): Implementation of the bounding box collision sensor (`touching trash can?`).
*   [Oscartime.sb3](file:///Volumes/Apple/انظمه%20تم%20تطويرها%20/نظام%20جديد%20/cs50x%202026/cs50x%202026-2027/src0/Oscartime.sb3): The final master game package incorporating dynamic scoring memory allocation (`Score Variable`).

### 🔬 Physics & Execution Deconstruction:
*   **Gravity Simulation:**
    Scratch possesses no native physics engine. Instead, we engineer an active simulation loop using `forever` wrapping the explicit command `change y by -5`. This directs the graphics pipeline to decrement the vertical Y-coordinate by 5 pixels every display frame update, rendering a visually pristine freefall effect.
*   **Cartesian Randomization:**
    To introduce game unpredictability, descending trash assets are reset to the vertical summit (`y: 180`) while acquiring an arbitrary horizontal displacement via the mathematical operator `pick random -240 to 240`.
*   **State Variable Mutation:**
    The `score` variable operates as an explicit memory allocation slot. When the collision event triggers (`if touching trash can?`), the processor executes a memory mutation instruction `change score by 1`, dispatched concurrently with an acoustic PCM audio side effect (`Play sound pop`).

---

## 🎮 2. The Architecture of `Ivy's Hardest Game` (Collision Engine & Bounding Walls)

`Ivy's Hardest Game` represents the absolute peak of architectural complexity in Week 0, mastering active keyboard input polling and rebound wall physics.

### 📂 Direct Source File Links in `src0`:
*   [Ivy's Hardest Game - Harvard 0.sb3](file:///Volumes/Apple/انظمه%20تم%20تطويرها%20/نظام%20جديد%20/cs50x%202026/cs50x%202026-2027/src0/Ivy's%20Hardest%20Game%20-%20Harvard%200.sb3): Preliminary player motion configuration via key binding blocks.
*   [Ivy's Hardest Game - Harvard 2.sb3](file:///Volumes/Apple/انظمه%20تم%20تطويرها%20/نظام%20جديد%20/cs50x%202026/cs50x%202026-2027/src0/Ivy's%20Hardest%20Game%20-%20Harvard%202.sb3): Construction of custom procedural sub-routines (`listen for keyboard` and `feel for walls`).
*   [Ivy's Hardest Game - Harvard.sb3](file:///Volumes/Apple/انظمه%20تم%20تطويرها%20/نظام%20جديد%20/cs50x%202026/cs50x%202026-2027/src0/Ivy's%20Hardest%20Game%20-%20Harvard.sb3): The massive master bundle featuring dynamic moving obstacles and failure triggers.

### 🔬 Deconstructing Modular Sub-Routines:
Rather than saturating the primary `forever` loop with 20 convoluted nested conditional statements, Malan introduces an exceptionally clean architectural model rooted in the **Separation of Concerns**.

```text
when [Green Flag] clicked
 ├── go to x: 0 y: 0 (Reset Vector)
 └── forever
      ├── listen for keyboard (Active Input Polling)
      └── feel for walls      (Bounding Box Collision Engine)
```

#### 1. `listen for keyboard` Routine:
Continuously polls the four physical arrow states (`up, down, left, right`) and increments/decrements Cartesian vectors (`change x by 1` / `change y by 1`).
#### 2. `feel for walls` Routine (Inverse Rebound Physics):
Upon colliding with the right boundary (`if touching right wall?`), the runtime immediately dispatches an inverted motion vector via `change x by -1`. This instantaneous vector cancellation prevents sprite clipping and enforces a hard physics bounding box (`Wall Rebound Engine`).

---
**© Ayman Elmasry — AEL Digital Studio™ · All Rights Reserved.**
