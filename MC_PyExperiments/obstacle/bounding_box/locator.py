import numpy as np
def find_com(mask):
    mask = mask/255
    shape = mask.shape
    sum_x = np.sum(mask, axis=0)
    mass = np.sum(sum_x)
    if mass == 0:
        return -1,-1,0

    x_weights = np.arange(start=1, stop=shape[1]+1, step=1)
    com_x =int(np.sum(np.multiply(x_weights, sum_x))/mass)


    sum_y = np.sum(mask, axis=1)
    y_weights = np.arange(start=1, stop=shape[0]+1, step=1)
    com_y =int(np.sum(np.multiply(y_weights, sum_y))/mass)
    print(com_x,com_y)
    return com_x, com_y, mass
