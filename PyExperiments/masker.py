import cv2
import numpy
import numpy as np

def hsv_mask(image, color):
    hsv = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)

    # Color mask type selection

    match color:
        case "yellow":
            lower = np.array([100, 60, 60])
            upper = np.array([110, 255, 255])
        case "green":
            lower = np.array([70, 80, 80])
            upper = np.array([90, 255, 255])
        case "red":
            lower = np.array([0, 80, 80])
            upper = np.array([10, 255, 255])
            lower1 = np.array([160, 80, 80])
            upper1 = np.array([179, 255, 255])
        case "black":
            lower = np.array([0, 0, 0])
            upper = np.array([179, 255, 75])
        case "white":
            lower = np.array([0, 0, 150])
            upper = np.array([179, 50, 255])


    # Mask gen
    mask = cv2.inRange(hsv, lower, upper)

    #Bodge for red wrapping around the coordinate system
    if color =="red":
        mask1 = cv2.inRange(hsv, lower1, upper1)
        mask = numpy.maximum(mask, mask1)

    cv2.imshow("Image", mask)
    cv2.waitKey(0)
    cv2.destroyAllWindows()

    # Averaging
    kernel1 = np.ones((5, 5), np.float32) / 25
    mask = cv2.filter2D(src=mask, ddepth=-1, kernel=kernel1)

    cv2.imshow("Image", mask)
    cv2.waitKey(0)
    cv2.destroyAllWindows()

    ret, mask = cv2.threshold(mask, 200, 255, cv2.THRESH_BINARY)

    cv2.imshow("Image", mask)
    cv2.waitKey(0)
    cv2.destroyAllWindows()

    return mask
