import cv2
import numpy as np
import masker
import locator


images = []
positions = []

##Appended 2 images here, why 2 images?
images.append(cv2.imread("../assets/white_obstacle0.jpeg", cv2.IMREAD_COLOR))
images.append(cv2.resize(cv2.imread("../assets/img1.jpg", cv2.IMREAD_COLOR),(0,0),fx=0.3,fy=0.3))
positions.append([0,0,0])

##What is positions and why -125?
images.append(cv2.imread("../assets/white_obstacle2.jpeg", cv2.IMREAD_COLOR))
positions.append([0,-125,0])

# images.append(cv2.imread("assets/235.jpg", cv2.IMREAD_COLOR))
# positions.append([0,-235,0])

#img = cv2.resize(img, (320,240))
# img = cv2.resize(img, (0,0), fx=0.2, fy=0.2)
#



#
# imgsmol = cv2.resize(img, (30,40))
# print(imgsmol[22][18])
# imgsmol = cv2.cvtColor(imgsmol, cv2.COLOR_BGR2GRAY)
# cv2.imshow("Image", imgsmol)
# cv2.waitKey(0)
# cv2.destroyAllWindows()

objects = []
colors = ["white"]  #, "black", "white"


#Masking
for img in images:
    imgobjects = []
    for color in colors:

        ## Get the mask for the specific color. Mask is a binary image of Zero and non-zero.
        ## We are trying to set zero for parts of image that aren't the same color.
        mask = masker.hsv_mask(img, color)


        com = locator.find_com(mask,color)

        cv2.imshow()
        cv2.waitkey(0)
        ##com[2] is total mass mass
        print(color, com[2], 0.001*img.shape[0]*img.shape[1])
        ## I guess here is accuracy check??? if passes accuracy, then put a circle and text
        if com[2] > 0.001*img.shape[0]*img.shape[1]:
            img = cv2.circle(img, (com[0], com[1]), 10, (255, 0, 0), 2)
            font = cv2.FONT_HERSHEY_SIMPLEX
            img = cv2.putText(img,color,(com[0],com[1]), font, 1,(255,255,255),1,cv2.LINE_AA)
            imgobjects.append([com[0], com[1], com[2], color])

    #Display color masks
    cv2.imshow("Final", img)
    cv2.moveWindow("Final", 40, 30)  # Move it to (40,30)
    cv2.waitKey(0)
    cv2.destroyAllWindows()

    ##img objects has center of mass for different balls
    objects.append(imgobjects)
