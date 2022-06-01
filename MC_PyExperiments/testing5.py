import numpy as np
import cv2

cap = cv2.VideoCapture(0)

while True:
    ret, frame = cap.read()
    #default get is floating point. 3 is just width, 4 is height. 17 different properties for capture.
    width = int (cap.get(3))
    height  = int(cap.get(4))

    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    lower_blue= np.array([70,50,50])
    upper_blue = np.array([150,150,150])
    #mask is a portion of an image. return a new mask of an image that only the blue pixel exists
    mask = cv2.inRange(hsv,lower_blue,upper_blue)

    #blend two image together and mask through it. Bascially keep colour same as mask
    result = cv2.bitwise_and(frame, frame, mask=mask)


    #RGB: Red Green Blue, BGR: Blue Green Red, HSV: Hue Saturation and Lightness/Brightness
    cv2.imshow('frame',result)
    cv2.imshow('mask',mask)

    if cv2.waitKey(1) == ord('q'):
        break


cap.release()
cv2.destroyAllWindows()
