def main():
    text = input("Text: ")
    letter_count = 0
    word_count = 1
    sentence_count = 0
    length = len(text)
    for i in range(length):
        c = text[i]
        if c.isalpha():
            letter_count += 1
        elif c.isspace():
            word_count += 1
        elif c == "." or c == "!" or c == "?":
            sentence_count += 1
    calculate_grade(letter_count, word_count, sentence_count)


def calculate_grade(letter_count, word_count, sentence_count):
    L = letter_count / word_count * 100
    S = sentence_count / word_count * 100
    index = round(0.0588 * L - 0.296 * S - 15.8)
    if index < 1:
        print("Before Grade 1")
    elif index > 16:
        print("Grade 16+")
    else:
        print(f"Grade {index}")


if __name__ == "__main__":
    main()