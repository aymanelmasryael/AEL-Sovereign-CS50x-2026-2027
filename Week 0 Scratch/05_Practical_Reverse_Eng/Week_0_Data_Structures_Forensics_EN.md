# Practical Reverse Engineering: Week 0 - Scratch (AST Forensics & .sb3 Deconstruction)

---

## 🔬 Reverse Engineering `src0/*.sb3` Files (Bare-Metal AST Forensics)

In deep systems engineering, we do not view Scratch project files (`.sb3`) located in the `src0` directory as opaque graphical black boxes. We inspect them as standardized, containerized archives.

```text
[ Sample.sb3 ] ──(Unzip Archive)──> [ project.json (Abstract Syntax Tree) ] + [ 83a9b2...svg (Vector Assets) ] + [ 0d3b...wav (PCM Audio) ]
```

### 💡 The Structural Truth of `.sb3` Files:
A `.sb3` file is physically a compressed ZIP archive. If you rename any `.sb3` asset to `.zip` and extract its contents, you uncover a foundational master configuration file named `project.json`. This file contains the complete Abstract Syntax Tree (`AST`), structuring every visual block and sprite as an explicit JSON node parsed by the underlying JavaScript virtual machine.

---

## 📦 1. Deconstructing the `hello` Progression (Static Literals to Dynamic Arguments)

The `src0` directory hosts the progressive architectural evolution of the `hello` series across three benchmark files:

### 📂 Direct Source File Links in `src0`:
*   [hello0.sb3](file:///Volumes/Apple/انظمه%20تم%20تطويرها%20/نظام%20جديد%20/cs50x%202026/cs50x%202026-2027/src0/hello0.sb3): Static foundational block (`say [hello, world]`).
*   [hello1.sb3](file:///Volumes/Apple/انظمه%20تم%20تطويرها%20/نظام%20جديد%20/cs50x%202026/cs50x%202026-2027/src0/hello1.sb3): Interactive sensing prompt (`ask [What's your name?] and wait`).
*   [hello2.sb3](file:///Volumes/Apple/انظمه%20تم%20تطويرها%20/نظام%20جديد%20/cs50x%202026/cs50x%202026-2027/src0/hello2.sb3): Variable concatenation via the `join` operator (`say (join (hello, ) (answer))`).

#### 🔬 AST Mechanics in `hello2.sb3`:
Evaluating the `project.json` of `hello2.sb3` reveals that the `say` opcode ceases to accept a raw string literal. Instead, it receives an embedded sub-opcode named `operator_join`. This node accepts two explicit parameters: the static string `"hello, "` and the dynamic sensor variable `sensing_answer`.

---

## 📦 2. Deconstructing the `meow` Progression (The Abstraction Ladder AST)

Files `meow0` through `meow4` map the four rungs of the Abstraction Ladder directly onto the underlying AST payload.

### 📂 Direct Source File Links in `src0`:
*   [meow0.sb3](file:///Volumes/Apple/انظمه%20تم%20تطويرها%20/نظام%20جديد%20/cs50x%202026/cs50x%202026-2027/src0/meow0.sb3): Repetitive boilerplate execution (3 consecutive sound blocks).
*   [meow1.sb3](file:///Volumes/Apple/انظمه%20تم%20تطويرها%20/نظام%20جديد%20/cs50x%202026/cs50x%202026-2027/src0/meow1.sb3): Control flow looping structure `repeat (3)`.
*   [meow2.sb3](file:///Volumes/Apple/انظمه%20تم%20تطويرها%20/نظام%20جديد%20/cs50x%202026/cs50x%202026-2027/src0/meow2.sb3) & [meow3.sb3](file:///Volumes/Apple/انظمه%20تم%20تطويرها%20/نظام%20جديد%20/cs50x%202026/cs50x%202026-2027/src0/meow3.sb3): Procedural custom block definition (`define meow`).
*   [meow4.sb3](file:///Volumes/Apple/انظمه%20تم%20تطويرها%20/نظام%20جديد%20/cs50x%202026/cs50x%202026-2027/src0/meow4.sb3): The architectural summit; parameterized procedures (`define meow (n)`).

#### 🔬 AST Mechanics of `define meow (n)`:
In `meow4.sb3`, the Scratch runtime constructs a distinct parent node in `project.json` called `procedures_definition`. This node encapsulates a local symbol table (`Mutation Block`) allocating the parameter `n`. When invoked in the primary block via `procedures_call`, the runtime executes a call stack push, passing the dynamic argument integer seamlessly.

---

## 📦 3. Deconstructing the `pet` Progression (Active Polling vs Hardware Interrupts)

The most glaring architectural contrast in processor efficiency manifests when deconstructing `pet0` and `pet1`.

### 📂 Direct Source File Links in `src0`:
*   [pet0.sb3](file:///Volumes/Apple/انظمه%20تم%20تطويرها%20/نظام%20جديد%20/cs50x%202026/cs50x%202026-2027/src0/pet0.sb3): Active busy polling loop (`forever -> if touching mouse-pointer?`).
*   [pet1.sb3](file:///Volumes/Apple/انظمه%20تم%20تطويرها%20/نظام%20جديد%20/cs50x%202026/cs50x%202026-2027/src0/pet1.sb3): Intelligent hardware interrupt hook (`when video motion > 10`).

#### 🔬 AST Comparative Critique:
*   In `pet0.sb3`, the `control_forever` node traps the processor in continuous, synchronous execution cycles (`CPU Polling Loop`).
*   In `pet1.sb3`, the `control_forever` loop is completely eradicated! A new top-level node `videoSensing_whenMotionGreaterThan` emerges, operating as an asynchronous event hook that remains completely dormant until an explicit hardware interrupt arrives from the webcam sensor.

---
**© Ayman Elmasry — AEL Digital Studio™ · All Rights Reserved.**
