import numpy as np

def find_stripe_width(bbox):
    #find max stripe width pixel
    y = int(bbox.shape[0])//2
    max_stripe_w=tmp_stripe_w=0
    for x in range(int(bbox.shape[1])):
        pixel = bbox.item(y, x)
        if pixel==0:
            tmp_stripe_w +=1
            if (tmp_stripe_w>max_stripe_w):
                max_stripe_w = tmp_stripe_w
        else:
            tmp_stripe_w=0  
    return max_stripe_w

def find_distance(max_stripe_w, img):
    #assume that the width of stripe is 70 pixels at 20 cm
    FOV_angle = 65
    img_width= int(img.shape[1])

    ref_angle = 70/img_width * FOV_angle 
    curr_angle = max_stripe_w/img_width * FOV_angle
    distance = 20* ref_angle/curr_angle

    return distance


    


