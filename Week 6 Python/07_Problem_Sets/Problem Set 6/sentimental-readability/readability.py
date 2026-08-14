#!/usr/bin/env python3
# ==============================================================================
# Project   : AEL Sovereign — CS50x 2026-2027
# Module    : week6_sentimental_readability
# File      : readability.py
# Author    : Ayman Elmasry — AEL Digital Studio
# ------------------------------------------------------------------------------
# Description:
#   Computes the U.S. school grade level required to read a given text, using
#   the Coleman-Liau readability index — the same formula employed by the
#   CS50 "readability" problem. It prompts for a "Text: ", analyses its
#   letters, words and sentences, then reports the grade (with the check50
#   special cases "Before Grade 1" and "Grade 16+" handled explicitly).
#
# Algorithm (Coleman-Liau, 1975):
#     L = (letters / words) * 100          -> avg letters per 100 words
#     S = (sentences / words) * 100        -> avg sentences per 100 words
#     index = 0.0588 * L - 0.296 * S - 15.8
#   The index is rounded to the nearest integer to yield the grade.
#
# Counting rules:
#   - letter  : any alphabetic character (Unicode-aware via str.isalpha()).
#   - word    : any maximal run of non-whitespace separated by whitespace.
#   - sentence: any terminally punctuated clause ending in '.', '!' or '?'.
#
# Complexity:
#   Time : O(n) — a single linear scan of the text for each metric.
#   Space: O(n) — the input string plus its tokenised word list.
# ==============================================================================

import re


SENTENCE_TERMINATORS = (".", "!", "?")


def count_words(text: str) -> int:
    """
    Count the number of words in `text`.

    A word is defined as any contiguous run of non-whitespace characters,
    mirroring the CS50 specification. Splitting on whitespace (rather than
    manually tallying spaces) is more robust: it naturally ignores repeated
    and leading/trailing whitespace, so the token count is exact either way.
    """
    return len(re.split(r"\s+", text.strip())) if text.strip() else 0


def count_letters(text: str) -> int:
    """
    Count the number of alphabetic letters in `text`.

    `str.isalpha()` is preferred over manual ASCII range checks because it is
    Unicode-aware and therefore counts accented and non-Latin letters too.
    """
    return sum(1 for char in text if char.isalpha())


def count_sentences(text: str) -> int:
    """
    Count the number of sentences in `text`.

    A sentence is delimited by any of '.', '!' or '?'. Each such char
    contributes exactly one sentence boundary; the final clause need not be
    terminated because the text itself represents one complete thought.
    """
    return sum(1 for char in text if char in SENTENCE_TERMINATORS)


def coleman_liau_grade(letters: int, words: int, sentences: int) -> float:
    """
    Evaluate the raw Coleman-Liau index for the given counts.

    The formula yields a non-negative float; the caller is responsible for
    rounding and for applying the two grade-clamping bounds. Returning the
    unrounded float keeps this function pure and trivially unit-testable.
    """
    letters_per_100 = (letters / words) * 100.0
    sentences_per_100 = (sentences / words) * 100.0
    return round(0.0588 * letters_per_100 - 0.296 * sentences_per_100 - 15.8)


def main() -> None:
    """
    Drive the analysis: read a text, compute its grade, and print a verdict.

    The verdict format exactly matches check50:
        - "Before Grade 1" when index < 1
        - "Grade N"        when 1 <= index <= 16
        - "Grade 16+"      when index > 16
    """
    text = input("Text: ")
    words = count_words(text)
    if words == 0:
        return  # guard against division-by-zero on an empty text

    letters = count_letters(text)
    sentences = count_sentences(text)
    index = coleman_liau_grade(letters, words, sentences)

    if index < 1:
        print("Before Grade 1")
    elif index > 16:
        print("Grade 16+")
    else:
        print(f"Grade {index}")


if __name__ == "__main__":
    main()