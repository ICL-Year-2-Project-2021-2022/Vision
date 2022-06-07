import cv2
import numpy as np
import masker
import locator
import bounder
import distance
import fourier


images = []
positions = []

##Appended 2 images here, why 2 images?
images.append(cv2.resize(cv2.imread("../../assets/white_obstacle3.jpeg", cv2.IMREAD_COLOR),(0,0),fx=1,fy=1))

positions.append([0,0,0])

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
        bmin_x, bmax_x, bmin_y, bmax_y = bounder.getbbox(mask)
        #obs_distance = distance.triangle_distance(bmax_y-bmin_y,bmax_x-bmin_x)
        com = locator.find_com(mask)
        bounded_mask = mask[bmin_y:bmax_y,bmin_x:bmax_x]
        max_stripe_width =distance.find_stripe_width(bounded_mask) 
        print('stripe width',distance.find_stripe_width(bounded_mask))
        print('distance to obstacle: ', distance.find_distance(bounded_mask,img))
        cv2.imshow('bounded', bounded_mask)

        fourier.fourier_trans(bounded_mask)


        ##com[2] is total mass mass
        print(color, com[2], 0.001*img.shape[0]*img.shape[1])
        ## I guess here is accuracy check??? if passes accuracy, then put a circle and text
        if com[2] > 0.001*img.shape[0]*img.shape[1]:
            img = cv2.circle(img, (com[0], com[1]), 10, (255, 0, 0), 2)
            font = cv2.FONT_HERSHEY_SIMPLEX 
            img = cv2.putText(img,color,(com[0],com[1]), font, 1,(255,255,255),1,cv2.LINE_AA)
            img = cv2.rectangle(img, (bmin_x,bmin_y),(bmax_x,bmax_y),(0,0,0),2)
            imgobjects.append([com[0], com[1], com[2], color])

    #Display color masks
    cv2.imshow("Final", img)
    cv2.moveWindow("Final", 40, 30)  # Move it to (40,30)
    cv2.waitKey(0)
    cv2.destroyAllWindows()

    ##img objects has center of mass for different balls
    objects.append(imgobjects)
