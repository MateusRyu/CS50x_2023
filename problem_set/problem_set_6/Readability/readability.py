from cs50 import get_string
import re


def main():
    text = get_string("Text: ")
    letters = count_letters(text)
    words = count_words(text)
    sentences = count_sentences(text)

    # L is the average number of letters per 100 words in the text
    L = letters / words * 100

    # S is the average number of sentences per 100 words in the text.
    S = sentences / words * 100

    # Coleman-Liau index
    grade = round(0.0588 * L - 0.296 * S - 15.8)

    if grade < 1:
        print("Before Grade 1")
    elif grade >= 16:
        print("Grade 16+")
    else:
        print(f"Grade {grade}")


def count_letters(text):
    n = 0
    for char in text:
        match = re.match("^[a-zA-Z]$", char)
        if match:
            n += 1
    return n


def count_words(text):
    n = 0
    is_word = False

    for char in text:
        if char == " ":
            is_word = False
        elif is_word == False:
            n += 1
            is_word = True
    return n


def count_sentences(text):
    n = 0
    # Don't consider abbreviations, like "Mr. Robot?"
    for char in text:
        if char in [".", "!", "?"]:
            n += 1
    return n


main()
