import cv2
import numpy as np

img = cv2.resize(cv2.imread("../assets/obstacle.jpeg",1),(0,0),fx=1,fy=1)

hsv_img = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
lower_gray = np.array([0,5,50],np.uint8)
upper_gray = np.array([230,60,255], np.uint8)
mask_gray = cv2.inRange(hsv_img, lower_gray, upper_gray)
img_res=cv2.bitwise_and(img,img,mask=mask_gray)
cv2.imshow('img_res',img_res)
img_res = cv2.cvtColor(img_res,cv2.COLOR_BGR2GRAY)
thresh_img = cv2.threshold(img_res, 200, 255, cv2.THRESH_BINARY + cv2.THRESH_OTSU)[1]

cnts = cv2.findContours(thresh_img, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
cnts = cnts[0] if len(cnts) == 2 else cnts[1]
for cnt in cnts:
    approx = cv2.contourArea(cnt)
    print(approx)

cv2.imshow('image', img)
cv2.imshow('Binary',thresh_img)
cv2.waitKey()