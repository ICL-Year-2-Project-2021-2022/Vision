import numpy
import cv2

img = cv2.imread("assets/yellow_ball.jpg", cv2.IMREAD_COLOR)
img = cv2.resize(img, (1,1))
img[0][0] = numpy.array([90, 255, 128])
img = cv2.resize(img, (1000,1000))

cv2.imshow("Image", img)
cv2.waitKey(0)
cv2.destroyAllWindows()