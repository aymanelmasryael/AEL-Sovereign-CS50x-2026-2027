# AEL CS Encyclopedia™ – Week 2 Arrays: Practical Reverse Engineering (EN)

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
  "domain": "Week 2: Scrabble, Readability & Ciphers",
  "document_type": "05_Practical_Reverse_Eng (EN)",
  "methodology": "8-Stage Sub-Silicon Execution Paradigm",
  "execution_state": "Active Master Verification"
}
```

---

## 🔬 Section 1: Scrabble Engine Deconstruction

### 1. Architectural Objective
The execution binary prompts two interactive users (`Player 1` and `Player 2`) for distinct vocabulary string inputs. It dynamically calculates the absolute cumulative score of each word utilizing an immutable point matrix, subsequently printing the definitive winner to standard output.

```text
===================================================================================
             SCRABBLE REVERSE ENGINEERING PIPELINE
===================================================================================

  [ Input String (Word) ] ──> [ Loop Character by Character (strlen) ]
                                      │
                                      ▼
             [ isalpha() ? Convert to Uppercase (toupper) ]
                                      │
                                      ▼
             [ Index Calculation: char - 'A' (0 to 25) ]
                                      │
                                      ▼
             [ Lookup in POINTS[] Matrix ] ──> [ Accumulate Total Score ]

===================================================================================
```

### 2. The POINTS Lookup Matrix
Character weightings are statically allocated within a 26-element integer matrix:
```c
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};
```
*   **Forensic Indexing Mechanism**: To extract the integer weight of character `C` (ASCII decimal 67), the CPU executes the baseline subtraction `67 - 65 ('A') = 2`. Evaluating `POINTS[2]` yields the correct point value `3`.

---

## 🧠 Section 2: Readability Engine Deconstruction

### 1. Integrated Text Scanner Architecture
Rather than instantiating three isolated loops to evaluate letter, word, and sentence totals, we construct an optimized, unified single-pass scanner that inspects the character stream in real-time:

```text
===================================================================================
             READABILITY PARSER LOGIC TREE
===================================================================================

                [ Character Scanner (s[i]) ]
                 ├── isalpha(s[i]) ──> letters++
                 ├── isspace(s[i]) ──> words++ (Initialize words at 1)
                 └── s[i] == '.' || s[i] == '!' || s[i] == '?' ──> sentences++

===================================================================================
```

### 2. Strict Coleman-Liau Mathematical Application
```c
float L = ((float) letters / words) * 100;
float S = ((float) sentences / words) * 100;
int index = round(0.0588 * L - 0.296 * S - 15.8);
```
*   **Definitive Output Verification**:
    *   If `index < 1`, print `Before Grade 1`.
    *   If `index >= 16`, print `Grade 16+`.
    *   Otherwise, print `Grade X` (where X represents the rounded integer).

---

## 🧬 Section 3: Cryptographic Cipher Engines (`Caesar & Substitution`)

### 1. Caesar Cipher Deconstruction
*   **CLI Argument Guardrails**: The main entry point must enforce `argc == 2`. It must iterate across `argv[1]` to verify every discrete byte satisfies `isdigit()`.
*   **Integer Conversion**: Transform the ASCII digit string `argv[1]` into a pure integer utilizing `atoi()`.
*   **Case Preservation Protocol**: During mathematical rotation, uppercase characters (`isupper`) and lowercase characters (`islower`) maintain their exact casing, while non-alphabetical punctuation and whitespace bypass transformation entirely.

### 2. Substitution Cipher Deconstruction
*   **Forensic Key Verification**: The program verifies `argv[1]` contains exactly 26 bytes (`strlen == 26`), confirming all characters are alphabetical and devoid of duplicates (enforced via an internal 26-element character frequency array).
*   **Substitution Mapping**: Replaces every plaintext character with the corresponding index in the substitution key string derived via alphabetical offset `char - 'A'`.

---
**© Ayman Elmasry — AEL Digital Studio™ · All Rights Reserved.**
