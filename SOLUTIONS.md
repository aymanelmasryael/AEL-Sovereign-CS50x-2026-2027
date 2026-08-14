# ⚙️ AEL Sovereign — CS50x Master Solutions Index

> **Production-grade problem sets & labs** for every CS50x 2026-2027 week, engineered by **Ayman Elmasry — AEL Digital Studio**.
> Each week carries its own `07_Problem_Sets/` wing containing the official problem sets and labs with complete, verified solutions.

---

## 🗂️ Solution Index (Week → Wing)

| Week | Wing Folder | Official Problem Sets | Labs |
|------|-------------|-----------------------|------|
| [Week 1 — C](../Week%201%20C/07_Problem_Sets/) | `Week 1 C/07_Problem_Sets` | Hello, Mario (More), Credit | Population |
| [Week 2 — Arrays](../Week%202%20Arrays/07_Problem_Sets/) | `Week 2 Arrays/07_Problem_Sets` | Readability, Wordle | Scrabble |
| [Week 3 — Algorithms](../Week%203%20Algorithms/07_Problem_Sets/) | `Week 3 Algorithms/07_Problem_Sets` | Plurality, Tideman | Sort |
| [Week 4 — Memory](../Week%204%20Memory/07_Problem_Sets/) | `Week 4 Memory/07_Problem_Sets` | Filter (More), Reverse | Smiley, Volume |
| [Week 5 — Data Structures](../Week%205%20Data%20Structures/07_Problem_Sets/) | `Week 5 Data Structures/07_Problem_Sets` | Speller | Inheritance |
| [Week 6 — Python](../Week%206%20Python/07_Problem_Sets/) | `Week 6 Python/07_Problem_Sets` | DNA, Sentimental Hello/Mario/Credit/Readability | World Cup |
| [Week 7 — SQL & AI](../Week%207%20SQL%20Artificial%20Intelligence/07_Problem_Sets/) | `Week 7 SQL Artificial Intelligence/07_Problem_Sets` | Movies, Fiftyville | Songs |
| [Week 8 — HTML, CSS, JS](../Week%208%20HTML%2C%20CSS%2C%20JavaScript/07_Problem_Sets/) | `Week 8 HTML, CSS, JavaScript/07_Problem_Sets` | Homepage | Trivia |
| [Week 9 — Flask](../Week%209%20Flask/07_Problem_Sets/) | `Week 9 Flask/07_Problem_Sets` | Finance | Birthdays |
| [Week 10 — Final Project](../Week%2010%20The%20End/07_Problem_Sets_Final_Project/) | `Week 10 The End/07_Problem_Sets_Final_Project` | Python Travel (Flask) | — |

---

## 🌟 Flagship Solutions

- **`credit` (Week 1)** — LUHN checksum card verification (`credit.c`)
- **`tideman` (Week 3)** — Condorcet method ranked voting (`tideman.c`)
- **`filter-more` (Week 4)** — Grayscale / Reflect / Blur / Edges image processing (`helpers.c`)
- **`speller` (Week 5)** — Hash-table dictionary with case-insensitive load/check (`dictionary.c`)
- **`dna` (Week 6)** — STR tandem-repeat DNA profiling (`dna.py`)
- **`movies` & `fiftyville` (Week 7)** — SQL querying & crime investigation (`*.sql`)
- **`finance` (Week 9)** — Stock-trading web app with sessions & SQLite (`app.py`)
- **`final-project` (Week 10)** — Python Travel Flask application

---

## ✅ Verification & Build

Each solution is self-contained and compiles against the standard CS50 toolchain:

```bash
# C solutions (weeks 1–5)
cc -o solution solution.c -lcs50

# Python solutions (weeks 6, 9, 10)
python3 solution.py

# SQL solutions (week 7)
sqlite3 fiftyville.db < log.sql
```

> **Note:** Compiled binaries and runtime artifacts were intentionally excluded from this wing to keep the repository clean and portable.

---

**© Ayman Elmasry — AEL Digital Studio™ · All Rights Reserved.**
