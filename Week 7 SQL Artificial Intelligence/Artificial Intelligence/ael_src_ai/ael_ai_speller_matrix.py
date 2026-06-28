#!/usr/bin/env python3
# ==============================================================================
# AEL CS Encyclopedia™ - © Ayman Elmasry
# Week 7 AI Subsystem: ael_ai_speller_matrix.py (Hybrid Speller & Fuzzy Matcher)
# ==============================================================================

import sys

def compute_levenshtein_distance(token_a: str, token_b: str) -> int:
    """
    Calculates the minimum edit distance (insertions, deletions, substitutions)
    between two string tokens.
    """
    m, n = len(token_a), len(token_b)
    dp = [[0] * (n + 1) for _ in range(m + 1)]

    for i in range(m + 1):
        dp[i][0] = i
    for j in range(n + 1):
        dp[0][j] = j

    for i in range(1, m + 1):
        for j in range(1, n + 1):
            if token_a[i - 1] == token_b[j - 1]:
                dp[i][j] = dp[i - 1][j - 1]
            else:
                dp[i][j] = min(dp[i - 1][j] + 1,      # Deletion
                               dp[i][j - 1] + 1,      # Insertion
                               dp[i - 1][j - 1] + 1)  # Substitution
    return dp[m][n]

class SovereignHybridSpeller:
    def __init__(self):
        # Deterministic dictionary hash set
        self.dictionary_vault = {
            "algorithm", "artificial", "intelligence", "neural", "network",
            "relational", "matrix", "sovereign", "encrypt", "structure"
        }

    def check_spelling(self, word: str):
        normalized = word.lower()
        print(f"\n[*] Evaluating Candidate Word: '{word}'")
        
        # Step 1: Classical Deterministic O(1) Check
        if normalized in self.dictionary_vault:
            print("  ├── [HASH MATCH]: EXACT MATCH (O(1) Memory Verification)")
            return True, normalized

        print("  ├── [HASH MATCH]: MISS (Word not found in core memory)")
        print("  ├── [AI FALLBACK]: Initiating Levenshtein Fuzzy Matrix Inspection...")
        
        # Step 2: AI Fuzzy Correction Fallback
        best_candidate = None
        min_edit = float('inf')

        for valid_word in self.dictionary_vault:
            dist = compute_levenshtein_distance(normalized, valid_word)
            if dist < min_edit:
                min_edit = dist
                best_candidate = valid_word

        if min_edit <= 3:
            print(f"  └── [FUZZY CORRECTION IDENTIFIED]: '{best_candidate}' (Edit Distance: {min_edit})")
            return False, best_candidate
        else:
            print(f"  └── [OUT OF SCOPE]: No close candidates matching structural integrity.")
            return False, None

def main():
    print("================================================================")
    print("        AEL SOVEREIGN HYBRID SPELLER & FUZZY MATRIX ENGINE")
    print("================================================================\n")

    speller = SovereignHybridSpeller()

    # Case 1: Exact spelling
    speller.check_spelling("Intelligence")

    # Case 2: Minor misspelling (e.g., 'artifishal' -> artificial)
    speller.check_spelling("artifishal")

    # Case 3: Minor misspelling (e.g., 'sovereignn' -> sovereign)
    speller.check_spelling("sovereignn")

    # Case 4: Complete gibberish
    speller.check_spelling("xyzqwerty")

    print("\nHybrid speller evaluation executed successfully.")

if __name__ == "__main__":
    main()
