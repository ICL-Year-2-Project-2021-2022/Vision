import cv2
import numpy as np
import masker
import locator
import matcher

img = cv2.imread("assets/clean.jpg", cv2.IMREAD_COLOR)
#img = cv2.resize(img, (320,240))
img = cv2.resize(img, (0,0), fx=0.2, fy=0.2)



# cv2.imshow("Image", img)
# cv2.waitKey(0)
# cv2.moveWindow("Image", 40,30)  # Move it to (40,30)
# cv2.destroyAllWindows()
#
# imgsmol = cv2.resize(img, (30,40))
# print(imgsmol[22][18])
# imgsmol = cv2.cvtColor(imgsmol, cv2.COLOR_BGR2GRAY)
# cv2.imshow("Image", imgsmol)
# cv2.waitKey(0)
# cv2.destroyAllWindows()


images = []
positions = []
images.append(img)
objects = []
colors = ["red", "green", "yellow", "black", "white"]


#Masking
for img in images:
    imgobjects = []
    for color in colors:
        mask = masker.hsv_mask(img, color)
        com = locator.find_com(mask)
        print(color, com[2], 0.01*img.shape[0]*img.shape[1])
        if com[2] > 0.01*img.shape[0]*img.shape[1]:
            img = cv2.circle(img, (com[0], com[1]), 10, (255, 0, 0), 2)
            font = cv2.FONT_HERSHEY_SIMPLEX
            img = cv2.putText(img,color,(com[0],com[1]), font, 1,(255,255,255),1,cv2.LINE_AA)
            imgobjects.append([com[0], com[1], com[2], color])
    objects.append(imgobjects)

rays = matcher.generate_rays(objects,positions, img)
#aliens = matcher.match_rays(rays)

occupancy_grid = []

#occupancy_grid = update_occupancy(occupancy_grid)


cv2.imshow("Image", img)
cv2.waitKey(0)
cv2.destroyAllWindows()


#
#cv2.imwrite("img.jpg", img)
