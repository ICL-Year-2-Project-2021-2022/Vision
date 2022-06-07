import numpy as np
import fourier

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

def find_distance(bbox, img):
    #assume that the width of stripe is 70 pixels at 40 cm and pixel no. prop with angle. 
    FOV_angle = 65
    img_width= int(img.shape[1])
    max_stripe_w = find_stripe_width(bbox)
    ref_angle = 70/img_width * FOV_angle
    curr_angle = max_stripe_w/img_width * FOV_angle
    distance1  = 40* ref_angle/curr_angle
    print('distance1', distance1)


    return distance1

def abs_freq(bbox, img):
    img_width = int(img.shape[1])
    bbox_width = int(bbox.shape[1])
    f_bb = fourier.fourier_trans(img) # frequency of bounding box
    f_abs = f_bb * img_width/ bbox_width
    return f_abs




    


