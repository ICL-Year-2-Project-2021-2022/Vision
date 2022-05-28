import cv2
import numpy as np

def passable(thresh, width, grid):
    #Threshold gating
    ret, grid = cv2.threshold(grid, thresh*255, 255, cv2.THRESH_BINARY)

    #Circular kernel and filter to avoid bumping
    kernel = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (2*width, 2*width))
    grid = cv2.filter2D(src=grid, ddepth=-1, kernel=kernel)

    #Discard everything that touched a forbidden area
    ret, passable = cv2.threshold(grid, 1, 1 , cv2.THRESH_BINARY)
    passable = passable.astype(int)
    return passable*255

def mod_bellman(src_x, src_y, dest_x, dest_y, passable):
    #Set all non 255 squares to 254
    initialize = np.vectorize(lambda y: 255 - int(y != 255))
    route = initialize(passable)

    if route[dest_y][dest_x] == 255 | route[src_y][src_x] == 255:
        print("Unreachable")
        return None
    #Count and destination initialization
    count = 0
    route[dest_y][dest_x] = 0
    maxnodes = route.shape[0]*route.shape[1]
    #Bellman ford
    while (route[src_y][src_x] == 254 and count < maxnodes):
        count += 1


        for y in range(route.shape[0]):
            for x in range(route.shape[1]):
                if route[y][x] != 255:
                    minimum = route[y][x]

                    #Find minimum neighbor with range guards
                    #East
                    j = x + 1
                    k = y
                    if (j < route.shape[1] ):
                        minimum = min(route[k][j]+1, minimum)
                    #North
                    j = x
                    k = y - 1
                    if (k >= 0):
                        minimum = min(route[k][j]+1, minimum)
                    #West
                    j = x - 1
                    k = y
                    if (j >= 0):
                        minimum = min(route[k][j]+1, minimum)
                    #South
                    j = x
                    k = y + 1
                    if (k < route.shape[0]):
                        minimum = min(route[k][j]+1, minimum)

                    route[y][x] = minimum
    return route

