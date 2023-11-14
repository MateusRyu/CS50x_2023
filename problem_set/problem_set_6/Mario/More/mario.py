from cs50 import get_int

while True:
    height = get_int("Height: ")
    if height > 0 and height <= 8:
        break

for i in range(height):
    space = " " * (height - i - 1)
    blocks = "#" * (i + 1)
    print(space + blocks + "  " + blocks)
