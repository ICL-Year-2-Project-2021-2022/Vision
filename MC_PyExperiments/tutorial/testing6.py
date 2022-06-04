#shi-tomasi corner detector
import numpy as np
import cv2

img = cv2.imread('assets/black.jpg')
img = cv2.resize(img, (0,0), fx=0.1,fy=0.1)
gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

#(source image, number of best corners to return, quality of corner 0-1, minimum euclidean(absolute) distanc between 2 corners)
corners = cv2.goodFeaturesToTrack(gray,100, 0.03,10)
#take numpy array and turn it into integers
corners = np.int0(corners)

for corner in corners:
    x,y = corner.ravel() #[[[0,1,2]]] ravel just removes interior array [[1,2],[3,4]] = [1,2,3,4]
    cv2.circle(img, (x,y),5,(255,0,0),-1)

for i in range(len(corners)):
    for j in range(i+1, len(corners)):
        #tuple convert list into a single variable
        corner1 = tuple(corners[i][0])
        corner2 = tuple(corners[j][0])
        #lambda is for a one line function. SO this converts all of the values into an int
        #map applies all the value into a function
        colour = tuple(map(lambda x: int(x), np.random.randint(0,255,size = 3)))
        cv2.line(img, corner1, corner2, (),1)
cv2.imshow('Frame',img)
cv2.waitKey(0)
cv2.destroyAllWindows()

