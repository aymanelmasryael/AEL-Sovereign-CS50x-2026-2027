"""
AEL Sovereign — CS50x 2026-2027
Problem Set 6: Sentimental Readability
Author: Ayman Elmasry — AEL Digital Studio

Estimates the U.S. school grade level required to understand a passage of
text, using the Coleman-Liau index.  The user is prompted for a sentence
or paragraph and the computed grade level is printed.
"""


def count_letters(text):
    """Return the number of alphabetic characters in the text."""
    return sum(1 for character in text if character.isalpha())


def count_words(text):
    """Return the number of words in the text, split on whitespace."""
    return len(text.split())


def count_sentences(text):
    """Return the number of sentences, based on '.', '!', and '?'."""
    return sum(1 for character in text if character in ".!?")


def grade_level(letters, words, sentences):
    """
    Compute the Coleman-Liau reading grade.  L is the average number of
    letters per 100 words and S is the average number of sentences per
    100 words; the index is 0.0588 * L - 0.296 * S - 15.8.
    """
    per_100 = 100 / words
    letters_per = letters * per_100
    sentences_per = sentences * per_100
    return 0.0588 * letters_per - 0.296 * sentences_per - 15.8


def report(index):
    """
    Render the reading level as a string.  Indexes below 1 are reported
    as "Before Grade 1", indexes of 16 or higher as "Grade 16+", and any
    other value is rounded to the nearest whole grade.
    """
    if index < 1:
        return "Before Grade 1"
    if index >= 16:
        return "Grade 16+"
    return f"Grade {round(index)}"


def main():
    """Prompt for text and print the estimated reading grade level."""
    text = input("Text: ")
    letters = count_letters(text)
    words = count_words(text)
    sentences = count_sentences(text)
    index = grade_level(letters, words, sentences)
    print(report(index))


if __name__ == "__main__":
    main()
