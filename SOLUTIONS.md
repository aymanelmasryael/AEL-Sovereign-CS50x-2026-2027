# ⚙️ AEL Sovereign — CS50x Master Solutions Index

> **Production-grade, expert-level problem sets & labs** for every CS50x 2026-2027 week, engineered by **Ayman Elmasry — AEL Digital Studio**.
> Each week carries its own `Problem Sets/` wing where every official problem set and lab has been **completely rewritten** to an architecture-first standard — above the reference implementations — with rigorous algorithm analysis, defensive engineering, and full check50-compatible behavior.
>
> **Every solution includes:** Doxygen-style design headers, algorithmic complexity analysis, modular decomposition, robust validation, and expert commentary in English.

---

## 🗂️ Solution Index (Week → Wing)

| Week | Wing Folder | Official Problem Sets | Labs |
|------|-------------|-----------------------|------|
| [Week 1 — C](./Week%201%20C/Problem Sets/) | `Week 1 C/Problem Sets` | Hello, Mario (More), Credit | Population |
| [Week 2 — Arrays](./Week%202%20Arrays/Problem Sets/) | `Week 2 Arrays/Problem Sets` | Readability, Wordle | Scrabble |
| [Week 3 — Algorithms](./Week%203%20Algorithms/Problem Sets/) | `Week 3 Algorithms/Problem Sets` | Plurality, Tideman | Sort |
| [Week 4 — Memory](./Week%204%20Memory/Problem Sets/) | `Week 4 Memory/Problem Sets` | Filter (More), Reverse | Smiley, Volume |
| [Week 5 — Data Structures](./Week%205%20Data%20Structures/Problem Sets/) | `Week 5 Data Structures/Problem Sets` | Speller | Inheritance |
| [Week 6 — Python](./Week%206%20Python/Problem Sets/) | `Week 6 Python/Problem Sets` | DNA, Sentimental Hello/Mario/Credit/Readability | World Cup |
| [Week 7 — SQL & AI](./Week%207%20SQL%20Artificial%20Intelligence/Problem Sets/) | `Week 7 SQL Artificial Intelligence/Problem Sets` | Movies, Fiftyville | Songs |
| [Week 8 — HTML, CSS, JS](./Week%208%20HTML%2C%20CSS%2C%20JavaScript/Problem Sets/) | `Week 8 HTML, CSS, JavaScript/Problem Sets` | Homepage | Trivia |
| [Week 9 — Flask](./Week%209%20Flask/Problem Sets/) | `Week 9 Flask/Problem Sets` | Finance | Birthdays |
| [Week 10 — Final Project](./Week%2010%20The%20End/Problem Sets/) | `Week 10 The End/Problem Sets` | Python Travel (Flask) | — |

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

Each solution is self-contained, **fully check50-compatible**, and compiles cleanly with `-Wall -Werror` against the standard CS50 toolchain:

```bash
# C solutions (weeks 1–5) — verified: zero warnings under -Wall -Werror
cc -Wall -Werror -o solution solution.c -lcs50

# Python solutions (weeks 6, 9, 10) — verified: python3 -m py_compile
python3 solution.py

# SQL solutions (week 7) — verified against the bundled databases
sqlite3 songs.db < 8.sql
sqlite3 fiftyville.db < log.sql
# movies.db (≈129 MB) is downloaded from the CS50 distribution (see Week 7 README)

# JavaScript (week 8) — verified: node --check script.js
```

> **Quality bar:** All C sources pass `gcc -Wall -Werror`; all Python sources pass `py_compile` and flake8; all Jinja templates and HTML divs are balanced; check50 I/O contracts (prompts, output formats, function signatures, table schemas) are preserved exactly.

> **Note:** Compiled binaries, build artifacts, and runtime session files were intentionally excluded from this wing to keep the repository clean and portable.

---

**© Ayman Elmasry — AEL Digital Studio™ · All Rights Reserved.**
