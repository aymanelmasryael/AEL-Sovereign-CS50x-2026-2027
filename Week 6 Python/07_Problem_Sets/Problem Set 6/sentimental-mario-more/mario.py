def main():
    # take height in int format from the user
    while True:
        try:
            height = int(input("Height: "))
            if 9 > height > 0:
                break
        except ValueError:
            pass

    # print
    for i in range(height):
        print(" " * (height - 1 - i), end="")
        print("#" * (i + 1), end="")
        print("  ", end="")
        print("#" * (i + 1))


if __name__ == "__main__":
    main()