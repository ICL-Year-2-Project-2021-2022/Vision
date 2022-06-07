import cv2
import numpy as np
import os
import sys
from matplotlib import pyplot as plt


img = cv2.imread("../assets/white_obstacle0.jpeg",cv2.IMREAD_GRAYSCALE)

f = np.fft.fft2(img)
#shift the zero-frequency component to the center of the spectrum
fshift = np.fft.fftshift(f)
magnitude = 20*np.log(np.abs(f))
magnitude = np.asarray(magnitude,dtype = np.uint8)

cv2.imshow("magnitude spectrum", magnitude)

cv2.imshow("Image",img)
cv2.waitKey(0)
cv2.destroyAllWindows()