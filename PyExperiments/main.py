import cv2
import numpy as np


#img = cv2.imread("assets/img1.jpg", cv2.IMREAD_COLOR)
img = cv2.imread("assets/img1.jpg", cv2.IMREAD_COLOR)
img = cv2.resize(img, (640,480))

# #Yellow to hsv
# y_bgr = np.array([[[255, 0, 0]]], dtype=bytes)
# print(y_bgr.shape)
# y_hsv = cv2.cvtColor(y_bgr, cv2.COLOR_BGR2HSV)


hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
print(hsv)

lower_yellow  = np.array([25, 60, 60])
upper_yellow  = np.array([35, 255, 255])

hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
mask = cv2.inRange(hsv, lower_yellow, upper_yellow)

cv2.imshow("Image", mask)
cv2.waitKey(0)
cv2.destroyAllWindows()

#Averaging
kernel1 = np.ones((5,5),np.float32)/25
mask = cv2.filter2D(src=mask,ddepth=-1,kernel=kernel1)

cv2.imshow("Image", mask)
cv2.waitKey(0)
cv2.destroyAllWindows()

ret, mask = cv2.threshold(mask, 200, 255, cv2.THRESH_BINARY)

cv2.imshow("Image", mask)
cv2.waitKey(0)
cv2.destroyAllWindows()


result = cv2.bitwise_and(img, img, mask=mask)
cv2.imshow("Image", result)
cv2.waitKey(0)
cv2.destroyAllWindows()


#img = cv2.resize(img, (0,0), fx=0, fy=0.5)
#cv2.imwrite("img.jpg", img)
