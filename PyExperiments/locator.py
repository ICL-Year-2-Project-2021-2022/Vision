import numpy as np


def find_com(mask):

    ## from matrix of black and white to 0 and 1
    mask = mask/255

    ## Shape here contains 3 values. ( height, width, channels =1)
    shape = mask.shape
    ## sum the columns [x1 , x2 ,x3]
    sum_x = np.sum(mask, axis=0)
    ## sum everything
    mass = np.sum(sum_x) 
    if mass == 0:
        return -1,-1,0

    ## start =1? 
    ## To get coordinates of x, we first do an arrange which represent x axis
    x_weights = np.arange(start=1, stop=shape[1]+1, step=1)
    com_x =int(np.sum(np.multiply(x_weights, sum_x))/mass)

    sum_y = np.sum(mask, axis=1)
    y_weights = np.arange(start=1, stop=shape[0]+1, step=1)
    com_y =int(np.sum(np.multiply(y_weights, sum_y))/mass)

    return com_x, com_y, mass
