# Practical Reverse Engineering: Week 1 - C (Binary Forensics, Symbols & Syscalls)

---

## 🏛️ Binary Forensics & Architectural Inspection

In this dedicated Practical Reverse Engineering wing, we transcend the conventional developer workflow of merely authoring and compiling `hello.c`. Here, we adopt the exacting rigor of a **Binary Forensic Investigator**, dissecting the resultant binary artifacts (`hello.o` and `hello`) to analyze precisely how they are parsed and executed by the bare-metal CPU and operating system kernel.

```text
===================================================================================
                       THE REVERSE ENGINEERING TOOLBOX
===================================================================================

  [ Executable Binary: ./hello ]
            │
            ├─► $ strings hello    ──> Extracts ASCII Literals ("hello, world")
            ├─► $ nm hello         ──> Inspects Symbol Table (_main, _printf)
            ├─► $ objdump -d hello ──> Disassembles Machine Code to Assembly
            └─► $ strace / dtruss  ──> Traces Kernel System Calls (write)

===================================================================================
```

---

## 🔬 The Analytical Disassembly Suite

### 1. Extracting Static ASCII Assets (`The strings Command`)
When a C source file is transformed into machine code, where exactly does the literal string `"hello, world\n"` reside? Does it mutate into processor instructions?
*   **Internal Anatomy:** Static ASCII text strings (`String Literals`) are archived within a specialized section of the compiled binary image known as the **Read-Only Data Section (`.rodata - Read-Only Data`)**.
*   **Forensic Verification:** By deploying the `strings` utility, we can scan the raw binary file to extract all human-readable character sequences without executing the underlying code:
```bash
$ strings hello
...
hello, world
What's your name? 
hello, %s
...
```

### 2. Inspecting the Symbol Table (`The nm Command`)
An executable binary houses a comprehensive Symbol Table (`Symbol Table`), mapping function identifiers to exact physical memory offsets.
*   **Internal Anatomy:** The `nm` diagnostic tool identifies internal function declarations (e.g., `main`) alongside dynamically linked external dependencies (e.g., `printf` and `get_string`).
*   **Forensic Verification:**
```bash
$ nm hello
0000000100003f50 T _main
                 U _printf
                 U _get_string
```
*   **Deconstructing Symbols:** 
    *   The prefix `T` signifies that the function `_main` resides directly within the binary's text code section (`Text Section`).
    *   The prefix `U` denotes that `_printf` and `_get_string` are undefined within the local binary scope (`Undefined / External`), relying on dynamic runtime linkage to the C Standard Library (`libc`) and the CS50 dynamic library.

### 3. Disassembling Machine Code to Assembly (`The objdump Command`)
*   **Operational Objective:** Reverse-translates raw binary opcode sequences (`0s & 1s`) back into human-readable Assembly language mnemonics.
*   **Forensic Verification:**
```bash
$ objdump -d hello
hello:  file format Mach-O 64-bit x86-64

Disassembly of section __TEXT,__text:
_main:
100003f50: 55                    pushq %rbp
100003f51: 48 89 e5              movq  %rsp, %rbp
100003f54: 48 8d 3d 55 00 00 00  leaq  85(%rip), %rdi  # loading "hello, world" string
100003f5b: e8 12 00 00 00        callq _printf
100003f60: 31 c0                 xorl  %eax, %eax
100003f62: 5d                    popq  %rbp
100003f63: c3                    retq
```
*   **Reverse Engineering Breakdown:**
    *   `pushq %rbp` & `movq`: Standard function prologue establishing the Stack Frame (`Stack Frame`) for `main`.
    *   `leaq`: Loads the exact memory address of the `"hello, world\n"` string literal into processor register `%rdi` (the dedicated architectural register for passing the first argument to functions in x86_64 calling conventions).
    *   `callq _printf`: Executes the direct function invocation to the pre-linked `printf` symbol.
    *   `xorl %eax, %eax`: Zeroes out register `%eax`, representing the implicit return code `return 0;` signaling successful execution.

---

## ⚙️ Tracing Kernel System Calls (`Syscalls`)

In high-level systems architecture, `printf` is merely an elegant user-space wrapper. The physical CPU architecture possesses no native instruction for printing characters to a physical monitor. This is where **Kernel System Calls (`Syscalls`)** bridge the gap.

```text
===================================================================================
                       KERNEL SYSTEM CALL PATHWAY
===================================================================================

  [ Application: printf("hello, world\n") ]
            │
            ▼
  [ C Standard Library: libc (Buffers output) ]
            │
            ▼  (User Mode -> Kernel Mode Switch)
  ┌────────────────────────────────────────────────────────┐
  │ [ OS Kernel Call: write(1, "hello, world\n", 13) ]     │ ──> Writes to File Descriptor 1 (stdout)
  └────────────────────────────────────────────────────────┘
            │
            ▼
  [ Hardware Device / Terminal Display ]

===================================================================================
```

### Deconstructing the Kernel Output Pathway:
When `printf("hello, world\n")` is executed, the C Standard Library issues a formal request to the Operating System Kernel via an explicit system call named `write`.
*   **On Linux Environments (`strace`):** Executing `strace ./hello` intercepts the kernel trap, logging the explicit operational arguments:
    `write(1, "hello, world\n", 13) = 13`
*   **On macOS Environments (`dtruss`):** Utilizing `sudo dtruss ./hello` demonstrates that the application requests the OS kernel to push exactly `13` bytes (the length of the string plus the `\n` character) into File Descriptor `1` (`File Descriptor 1`), representing the standard output stream `stdout`.

---

## 🧱 Investigating Intermediate Object vs. Executable Binary

| Architectural Metric | Intermediate Object (`hello.o`) | Final Standalone Binary (`hello`) |
| :--- | :--- | :--- |
| **Execution Autonomy** | Inert & Non-executable | Standalone Executable |
| **Symbol Resolution** | Contains unresolved external symbol stubs | External symbols fully resolved to static memory addresses |
| **Data Segment (`.rodata`)** | Contains local source strings only | Contains local strings alongside pre-linked shared library metadata |
| **Executable Headers** | Lacks executable metadata headers | Features formal execution headers (`ELF` or `Mach-O`) |

---
**© Ayman Elmasry — AEL Digital Studio™ · All Rights Reserved.**
