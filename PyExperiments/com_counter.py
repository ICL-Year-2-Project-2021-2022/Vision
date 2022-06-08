from PIL import Image

im = Image.open('assets/masked-image.jpg')
pix_val = list(im.getdata())

mass = 0
x_sum = 0
y_sum = 0

expected_x = 0
expected_y = 0
expected_mass = 0

for y in range(220, 261):
    for x in range(320, 361):
        expected_x += x
        expected_y += y
        expected_mass += 1

print("Expected X: " + str(expected_x))
print("Expected Y: " + str(expected_y))
print("Expected mass: " + str(expected_mass))

for y in range(0, 1080):
    for x in range(0, 1920):
        pixel = pix_val[y * 1920 + x]
        if pixel[0] > 10 and pixel[1] > 10 and pixel[2] > 10:
            mass += 1
            x_sum += x
            y_sum += y

print("Mass: " + str(mass))
print("X sum: " + str(x_sum))
print("Y sum: " + str(y_sum))
