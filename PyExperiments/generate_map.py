import random

matrix = []
for x in range(0, 30):
    for y in range(0, 30):
        if x == 15 and y == 15:
            matrix.append("2")
        else:
            num = random.randint(1, 10)
            if num > 7:
                matrix.append("1")
            else:
                matrix.append("0")

print(', '.join(matrix))
