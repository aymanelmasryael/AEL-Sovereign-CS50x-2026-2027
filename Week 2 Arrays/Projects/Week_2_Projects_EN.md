# AEL CS Encyclopedia™ – Week 2 Arrays: Projects & Codebases (EN)

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
  "domain": "Week 2: Official Problem Sets & Architecture",
  "document_type": "06_Projects (EN)",
  "methodology": "8-Stage Sub-Silicon Execution Paradigm",
  "execution_state": "Active Master Verification"
}
```

---

## 🔬 Section 1: Scrabble Project Specifications

### 1. Strict Functional Requirements
Develop an interactive terminal application that prompts two distinct players for vocabulary strings, computes their cumulative point totals, and displays the definitive victor or a tie state (`Tie!`).

```text
===================================================================================
             SCRABBLE EXECUTION FLOWCHART
===================================================================================

  [ Input P1 ] ──> compute_score() ──┐
                                     ├──> Comparison ──> [ Print: Player X wins! / Tie! ]
  [ Input P2 ] ──> compute_score() ──┘

===================================================================================
```

### 2. Architecture of `compute_score`
The subroutine must accept a character string parameter (`string word`) and return a pure integer (`int`) score, strictly ignoring all numerical digits, punctuation, and whitespace symbols.

---

## 🧠 Section 2: Readability Project Specifications

### 1. Academic Readability Index
The application prompts the user for an expansive text string (`Text`) and applies the strict Coleman-Liau formulation:
$$\text{Index} = 0.0588 \times L - 0.296 \times S - 15.8$$

### 2. Forensic Edge Cases & Guardrails
*   **Mathematical Rounding**: Strict utilization of `round()` from `<math.h>` to evaluate a rounded integer index.
*   **Integer Division Hazard**: When calculating averages, developers must explicitly cast integer operands to floating-point values (e.g., `(float) letters / words`) to prevent runtime truncation of decimal precision.

---

## 🧬 Section 3: Cryptographic Project Specifications (`Caesar & Substitution`)

### 1. Caesar Execution Path Specifications
*   **CLI Key Verification**: The runtime must verify exactly one parameter is passed (`argc == 2`), ensuring it represents a valid numeric key.
*   **Standard Error Notification**: If invalid parameters are detected, the program must print `Usage: ./caesar key` and return exit code `1` to signal abnormal termination.

### 2. Substitution Execution Path Specifications
*   **Key Engineering**: The runtime must verify the key (`argv[1]`) contains exactly 26 unique alphabetical characters (completely devoid of case-insensitive duplicates).
*   **Structural Preservation**: Complete retention of original cleartext casing (`Case Sensitivity`), punctuation, and whitespace structures.

---
**© Ayman Elmasry — AEL Digital Studio™ · All Rights Reserved.**
