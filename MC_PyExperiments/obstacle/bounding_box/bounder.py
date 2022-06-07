import numpy as np
def getbbox(mask):
    mask = mask/255
    shape = mask.shape
    max_x, max_y= 0,0
    min_x= int(mask.shape[1])
    min_y = int(mask.shape[0])
    max_stripe_w = tmp_stripe_w = 0
    last_white = False
    for y in range(int(mask.shape[0])):
        for x in range(int(mask.shape[1])):
            pixel= mask.item(y,x)
            #find bounding box
            if (pixel != 0):
                if x<min_x:
                    min_x=x
                if x>max_x:
                    max_x=x
                if y<min_y:
                    min_y=y
                if y>max_y:
                    max_y=y
      
            
    
    print('bbox: ',min_x, max_x, min_y, max_y)

    return min_x, max_x, min_y, max_y
