from cs50 import get_int
import re


def main():
    number = get_card()
    checksum = check_sum(number)
    report(checksum, number)


def get_card():
    while True:
        number = get_int("Number: ")
        if number > 0:
            return str(number)


def check_sum(card_number):
    sum = 0

    # Credit card = cc
    cc = card_number[:-1]
    while len(cc) > 0:
        digit = int(cc[-1]) * 2
        if digit < 10:
            sum += digit
        else:
            digit = str(digit)
            sum += int(digit[0]) + int(digit[1])

        if len(cc) == 1:
            cc = ""
        else:
            cc = cc[:-2]

    cc = card_number
    while len(cc) > 0:
        sum += int(cc[-1])
        if len(cc) == 1:
            cc = ""
        else:
            cc = cc[:-2]

    return sum


def report(checksum, number):
    length = len(number)
    if (checksum % 10) != 0:
        print("INVALID")
        return False
    elif length == 15:
        # Credit card = cc
        cc = "AMEX"
        pattern = "^(34|37)"
    elif number[0] == "4":
        cc = "VISA"
        pattern = "^[0-9]{13}|[0-9]{16}$"
    elif length == 16:
        cc = "MASTERCARD"
        pattern = "^5[1-5]"

    match = re.match(pattern, number)
    if match:
        print(cc)
    else:
        print("INVALID")


main()
