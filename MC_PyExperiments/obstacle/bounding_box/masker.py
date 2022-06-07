import cv2
import numpy as np

def hsv_mask(image, color):
   
    hsv = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)
    # Color mask type selection
    ## Gets upper and lower boudn of a color HSV
    match color:
        case "yellow":
            lower = np.array([20, 90, 60])
            upper = np.array([40, 255, 255])
        case "green":
            lower = np.array([50, 80, 80])
            upper = np.array([70, 255, 255])
        ## I am assuming here that red is a special case that wraps around the color coordinates
        case "red":
            lower = np.array([0, 80, 80])
            upper = np.array([10, 255, 255])
            lower1 = np.array([170, 80, 80])
            upper1 = np.array([179, 255, 255])
        case "white":
            lower = np.array([0, 0, 170])
            upper = np.array([180, 80, 255])
        case "pink":
            lower = np.array([155, 70, 80])
            upper = np.array([165, 255, 255])

    
    # Mask gen
    ## return array of 255(white) if within bounds. 
    mask = cv2.inRange(hsv, lower, upper)

    
    #Bodge for red wrapping around the coordinate system
    if color =="red":
        mask1 = cv2.inRange(hsv, lower1, upper1)
        mask = np.maximum(mask, mask1)

    # cv2.imshow("Image", mask)
    # cv2.waitKey(0)
    # cv2.destroyAllWindows()

    # Averaging
    ## This create a kernal of 5x5 for blurring image, this removes noise
    kernel1 = np.ones((5, 5), np.float32) / 25
    ##Convolution with the image
    cv2.imshow('Before Filter',mask)
    mask = cv2.filter2D(src=mask, ddepth=-1, kernel=kernel1)
    cv2.imshow('after filter',mask)
    # cv2.imshow("Image", mask)
    # cv2.waitKey(0)
    # cv2.destroyAllWindows()

    ##difference fo threshold and inrange
    ret, mask = cv2.threshold(mask, 200, 255, cv2.THRESH_BINARY)
    cv2.imshow('after threshold',mask)

    # cv2.imshow(color, mask)
    # cv2.waitKey(0)
    # cv2.destroyAllWindows()

    return mask
