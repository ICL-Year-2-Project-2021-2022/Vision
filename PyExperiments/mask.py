x_sum = 0
y_sum = 0
for x in range(0, 641):
    for y in range(0, 481):
       y_sum += y
       x_sum += x

print(x_sum)
print(y_sum)