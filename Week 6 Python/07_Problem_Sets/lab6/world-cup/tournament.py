#!/usr/bin/env python3
# ==============================================================================
# Project   : AEL Sovereign — CS50x 2026-2027
# Module    : week6_worldcup_lab
# File      : tournament.py
# Author    : Ayman Elmasry — AEL Digital Studio
# ------------------------------------------------------------------------------
# Description:
#   Monte-Carlo simulates a FIFA-style single-elimination World Cup tournament
#   and estimates each team's probability of winning. Wins are decided by a
#   probabilistic model derived from each team's Elo-style "rating" from the
#   CSV input ([19]18m.csv / 2019w.csv), and the whole tournament is replayed
#   N times so that empirical win shares converge toward true likelihoods.
#
# Game model:
#   For two teams with ratings r1 and r2, the probability that team1 beats
#   team2 is modelled by the logistic curve
#       P(team1 wins) = 1 / (1 + 10^((r2 - r1) / 600)).
#   A random draw uniform in [0, 1) below this probability declares team1 the
#   winner; otherwise team2 wins. This is the same rating-difference model
#   used across the CS50 World Cup lab.
#
# Output:
#   For every team, the line "  <name>: <pct>.1% chance of winning", sorted by
#   descending probability — exactly the format check50 expects.
#
# Complexity:
#   Time : O(N * T) — N tournament replays, each with T-1 games, where T is a
#         power-of-two team count.
#   Space: O(T) — the roster and the running counts dict.
# ==============================================================================

import csv
import random
import sys

# Number of tournament simulations to run. Increasing N reduces sampling error
# (standard error ~ 1/sqrt(N)) at the cost of linear runtime; 1000 yields a
# stable two-decimal estimate while staying fast.
N = 1000


def load_teams(filename: str) -> list:
    """
    Read the tournament roster from CSV (columns: team, rating).

    Ratings arrive as text in the file and are coerced to int once at load
    time, so simulation functions can arithmetic directly on them without
    repeated conversions. A defensive copy of the list is returned so that
    mutation during simulation never corrupts later replay rounds.
    """
    teams = []
    with open(filename, newline="") as file:
        reader = csv.DictReader(file)
        for entry in reader:
            entry["rating"] = int(entry["rating"])
            teams.append(entry)
    return list(teams)


def simulate_game(team1: dict, team2: dict) -> bool:
    """
    Simulate a single match. Return True if team1 wins, False if team2 wins.

    The logistic difference model converts the rating gap into an upset
    probability; a uniform random draw then resolves the match. Because
    probabilities are symmetric (P12 + P21 = 1), exactly one team always wins.
    """
    rating1 = team1["rating"]
    rating2 = team2["rating"]

    # 10^(delta/600); smaller rating gaps produce probabilities near 0.5,
    # large gaps saturate toward 1.0 for the stronger side.
    probability = 1 / (1 + 10 ** ((rating2 - rating1) / 600))

    return random.random() < probability


def simulate_round(teams: list) -> list:
    """
    Simulate one knockout round: pair up consecutive teams and keep winners.

    Input is assumed to hold an even number of teams (power-of-two field).
    Adjacent pairs (indices 0-1, 2-3, ...) compete; the victor of each pair
    advances to build the list returned for the next round.
    """
    winners = []
    for i in range(0, len(teams), 2):
        team1, team2 = teams[i], teams[i + 1]
        winners.append(team1 if simulate_game(team1, team2) else team2)
    return winners


def simulate_tournament(teams: list) -> str:
    """
    Simulate a full knockout tournament and return the champion's name.

    Rounds are played until one team remains. The `remaining` list is
    re-bound to each round's winner set, so the caller's roster is never
    mutated and can be reused across all N simulations.
    """
    remaining = list(teams)
    while len(remaining) > 1:
        remaining = simulate_round(remaining)
    return remaining[0]["team"]


def main() -> None:
    """
    Driver: load the roster, run N tournaments, print per-team win shares.

    Win counts are tallied across simulations; the printed chance is the
    empirical frequency `count / N` percent, formatted to one decimal place
    and ordered from most to least likely champion.
    """
    if len(sys.argv) != 2:
        sys.exit("Usage: python tournament.py FILENAME")

    teams = load_teams(sys.argv[1])

    # Tally how many of the N tournaments each team won.
    counts = {}
    for _ in range(N):
        winner = simulate_tournament(teams)
        counts[winner] = counts.get(winner, 0) + 1

    # Emit the probability table, most likely winners first.
    for team in sorted(counts, key=counts.get, reverse=True):
        print(f"{team}: {counts[team] * 100 / N:.1f}% chance of winning")


if __name__ == "__main__":
    main()