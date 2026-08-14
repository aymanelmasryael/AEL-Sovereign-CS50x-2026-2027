#!/usr/bin/env python3
# ==============================================================================
# AEL CS Encyclopedia™ - © Ayman Elmasry
# Week 7 AI Subsystem: ael_decision_tree_engine.py (Minimax Alpha-Beta Engine)
# ==============================================================================

import math
import sys

class GameTreeNode:
    def __init__(self, name, children=None, value=None):
        self.name = name
        self.children = children if children is not None else []
        self.value = value

def minimax_alpha_beta(node: GameTreeNode, depth: int, alpha: float, beta: float, is_maximizing: bool, indent=""):
    """
    Evaluates the game tree using the Minimax algorithm enhanced with Alpha-Beta pruning.
    """
    if node.value is not None or depth == 0:
        print(f"{indent}├── [Terminal Node Evaluated]: {node.name} -> Utility Value: {node.value}")
        return node.value

    if is_maximizing:
        max_eval = -math.inf
        print(f"{indent}┌── [MAX TURN]: {node.name} (seeking highest utility)")
        for child in node.children:
            eval_child = minimax_alpha_beta(child, depth - 1, alpha, beta, False, indent + "│   ")
            max_eval = max(max_eval, eval_child)
            alpha = max(alpha, eval_child)
            if beta <= alpha:
                print(f"{indent}│   └── [ALPHA-BETA PRUNING TRIGGERED]: β ({beta}) <= α ({alpha}). Truncating remaining branches.")
                break
        print(f"{indent}└── [MAX DECISION]: {node.name} optimal payoff -> {max_eval}")
        return max_eval

    else:
        min_eval = math.inf
        print(f"{indent}┌── [MIN TURN]: {node.name} (adversary seeking lowest utility)")
        for child in node.children:
            eval_child = minimax_alpha_beta(child, depth - 1, alpha, beta, True, indent + "│   ")
            min_eval = min(min_eval, eval_child)
            beta = min(beta, eval_child)
            if beta <= alpha:
                print(f"{indent}│   └── [ALPHA-BETA PRUNING TRIGGERED]: β ({beta}) <= α ({alpha}). Truncating remaining branches.")
                break
        print(f"{indent}└── [MIN DECISION]: {node.name} minimal payoff -> {min_eval}")
        return min_eval

def main():
    print("================================================================")
    print("        AEL SOVEREIGN MINIMAX DECISION TREE ENGINE (α-β)")
    print("================================================================\n")

    # Constructing a sample game tree
    # Level 2 (Leaf nodes with utility values)
    leaf1 = GameTreeNode("Leaf 1", value=3)
    leaf2 = GameTreeNode("Leaf 2", value=5)
    leaf3 = GameTreeNode("Leaf 3", value=6)
    leaf4 = GameTreeNode("Leaf 4", value=9)
    leaf5 = GameTreeNode("Leaf 5", value=1)
    leaf6 = GameTreeNode("Leaf 6", value=2)
    leaf7 = GameTreeNode("Leaf 7", value=0)
    leaf8 = GameTreeNode("Leaf 8", value=-1)

    # Level 1 (Minimizing adversary nodes)
    min1 = GameTreeNode("Branch Min A", [leaf1, leaf2])
    min2 = GameTreeNode("Branch Min B", [leaf3, leaf4])
    min3 = GameTreeNode("Branch Min C", [leaf5, leaf6])
    min4 = GameTreeNode("Branch Min D", [leaf7, leaf8])

    # Level 0 (Maximizing root node)
    root = GameTreeNode("Root Node", [min1, min2, min3, min4])

    print("[INITIALIZING AUTONOMOUS α-β TREE EXPLORATION]")
    print(f"[*] Initial Alpha: -inf | Initial Beta: +inf | Search Depth: 2\n")

    optimal_value = minimax_alpha_beta(root, depth=2, alpha=-math.inf, beta=math.inf, is_maximizing=True)

    print(f"\n================================================================")
    print(f"  └── [AI OPTIMAL MINIMAX POLICY SCORE]: {optimal_value}")
    print("================================================================\n")
    print("Minimax decision matrix successfully solved.")

if __name__ == "__main__":
    main()
