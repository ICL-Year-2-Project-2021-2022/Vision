import cv2
import numpy as np
import masker
import locator
import matcher
import routing

images = []
positions = []

images.append(cv2.imread("assets/0.jpg", cv2.IMREAD_COLOR))
positions.append([0,0,0])

images.append(cv2.imread("assets/125.jpg", cv2.IMREAD_COLOR))
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
colors = ["red", "green", "yellow", "pink"]  #, "black", "white"


#Masking
for img in images:
    imgobjects = []
    for color in colors:
        mask = masker.hsv_mask(img, color)

        com = locator.find_com(mask)
        print(color, com[2], 0.001*img.shape[0]*img.shape[1])
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

    objects.append(imgobjects)

rays = matcher.generate_rays(objects,positions, img)

side = 40
scale = 40
occupancy_grid = np.zeros((side, side))
probability_grid = np.zeros((side, side))

probability_grid, occupancy_grid = matcher.match_rays(rays, occupancy_grid, probability_grid, scale)

passable = routing.passable(1/255,4,probability_grid)
path = routing.mod_bellman(0,35, 5,6, passable)

print("Done")


# cv2.imshow("Image", path)
# cv2.waitKey(0)
# cv2.destroyAllWindows()


#
#cv2.imwrite("img.jpg", img)
