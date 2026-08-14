"""
AEL Sovereign — CS50x 2026-2027
Lab 6: World Cup
Author: Ayman Elmasry — AEL Digital Studio

Simulates the FIFA World Cup using Monte Carlo methods.  A bracket of
teams and their FIFA ratings is read from a CSV file, thousands of
tournaments are simulated, and the probability of winning is reported for
each team that wins at least one tournament.

Usage:
    python3 tournament.py <file.csv>
"""

import csv
import random
import sys

# Number of tournaments to simulate.
N = 1000


def simulate_game(team1, team2):
    """
    Simulate a single game between two teams and return True if team1
    wins and False otherwise.  The probability that team1 defeats team2 is
    derived from the FIFA rating model:
    p = 1 / (1 + 10 ** ((team2_rating - team1_rating) / 600))
    """
    rating1 = team1["rating"]
    rating2 = team2["rating"]
    probability = 1 / (1 + 10 ** ((rating2 - rating1) / 600))
    return random.random() < probability


def simulate_round(bracket):
    """
    Simulate one round of the tournament.  The bracket lists teams in
    seeded order, so the first team plays the second, the third plays the
    fourth, and so on.  Return the list of winners in match order.
    """
    winners = []

    for index in range(0, len(bracket), 2):
        team1 = bracket[index]
        team2 = bracket[index + 1]
        if simulate_game(team1, team2):
            winners.append(team1)
        else:
            winners.append(team2)

    return winners


def simulate_tournament(bracket):
    """
    Simulate a full knockout tournament and return the name of the team
    that wins the championship.  Rounds repeat until one team remains.
    """
    teams = list(bracket)

    while len(teams) > 1:
        teams = simulate_round(teams)

    return teams[0]["team"]


def load_teams(path):
    """Read a CSV file of teams and return a list of team dictionaries."""
    teams = []

    with open(path) as file:
        reader = csv.DictReader(file)
        for row in reader:
            teams.append({"team": row["team"], "rating": int(row["rating"])})

    return teams


def main():
    """Load the bracket, simulate N tournaments, and report probabilities."""
    if len(sys.argv) != 2:
        sys.exit("Usage: python3 tournament.py <file.csv>")

    teams = load_teams(sys.argv[1])

    # Tally how many tournaments each team wins.
    counts = {}
    for _ in range(N):
        winner = simulate_tournament(teams)
        counts[winner] = counts.get(winner, 0) + 1

    # Report each team's win percentage, best teams first.
    for team in sorted(counts, key=lambda name: (-counts[name], name)):
        print(f"{team}: {counts[team] / N * 100:.1f}% chance of winning")


if __name__ == "__main__":
    main()
