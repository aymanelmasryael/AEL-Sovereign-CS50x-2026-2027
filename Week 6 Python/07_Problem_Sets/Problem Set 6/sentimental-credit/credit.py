def main():
    while True:
        try:
            number = int(input("Number: "))
            if number > 0:
                break
        except ValueError:
            pass

    if is_valid(number):
        card_type(number)
    else:
        print("INVALID")


def is_valid(number):
    total = 0
    i = 0
    while number > 0:
        remainder = number % 10
        if i % 2 == 0:
            total += remainder
        else:
            n = remainder * 2
            total += n % 10 + n // 10
        number //= 10
        i += 1
    return total % 10 == 0


def card_type(number):
    number = str(number)
    first_digit = int(number[0])
    second_digit = int(number[1])
    if len(number) < 13 or 16 < len(number):
        print("INVALID")
    elif first_digit == 3 and second_digit == 4 or second_digit == 7:
        print("AMEX")
    elif first_digit == 5 and 1 <= second_digit <= 5:
        print("MASTERCARD")
    elif first_digit == 4:
        print("VISA")
    else:
        print("INVALID")


if __name__ == "__main__":
    main()