import cv2
import numpy as np
import masker
import locator

img = cv2.imread("assets/yellow_ball.jpg", cv2.IMREAD_COLOR)
img = cv2.resize(img, (320,240))
img = cv2.resize(img, (32,24))

cv2.imshow("Image", img)
cv2.waitKey(0)
cv2.destroyAllWindows()

#print(img[121][160])
print(img[12][16])

# imgsmol = cv2.resize(img, (30,40))
# cv2.imshow("Image", imgsmol)
# cv2.waitKey(0)
# cv2.destroyAllWindows()


#Masking
colors = ["red", "green", "yellow", "black"]
for color in colors:
    mask = masker.hsv_mask(img, color)
    com = locator.find_com(mask)
    print(color, com[3])
    img = cv2.circle(img, (com[0], com[1]), 10, (255, 0, 0), 2)




# result = cv2.bitwise_and(img, img, mask=mask)
# cv2.imshow("Image", result)

cv2.imshow("Image", img)
cv2.waitKey(0)
cv2.destroyAllWindows()


#img = cv2.resize(img, (0,0), fx=0, fy=0.5)
#cv2.imwrite("img.jpg", img)
