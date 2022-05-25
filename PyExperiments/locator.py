import numpy
def find_com(mask):
    mask = mask/255
    shape = mask.shape
    sum_x = numpy.sum(mask, axis=0)
    mass = numpy.sum(sum_x)
    if mass == 0:
        return -1,-1,0

    x_weights = numpy.arange(start=1, stop=shape[1]+1, step=1)
    com_x =int(numpy.sum(numpy.multiply(x_weights, sum_x))/mass)

    sum_y = numpy.sum(mask, axis=1)
    y_weights = numpy.arange(start=1, stop=shape[0]+1, step=1)
    com_y =int(numpy.sum(numpy.multiply(y_weights, sum_y))/mass)

    return com_x, com_y, mass
