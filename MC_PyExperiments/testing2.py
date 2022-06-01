from contextlib import redirect_stderr
import cv2
import random

img = cv2.imread('assets/125.jpg',-1)
#get pixels colour in row 125 and column 45 - 400
#print(img[125][45:400])

# for i in range(100):
#     for j in range(img.shape[1]): #loop through width of image
#         #(rows, columns, channels)
#         img[i][j]=[random.randint(0,255), random.randint(0,255), random.randint(0,255)]
# cv2.imwrite('new_img.jpg',img)
# cv2.imshow('Image',img)

tag = img[50:100, 200:300] # copy column 500-699 and 600-899
img[150:200, 250:350] = tag
cv2.imshow('Image', img)
#wait an infinite amount of time
cv2.waitKey(0)
#once pressed a key, it will destroy all windows. 
cv2.destroyAllWindows()
print(img[257][400])
#480 rows, 640 columns, 3 channels. Channel= how many value vectors are representing eaach pixel
#height, width, channels
print(img.shape)

# [
# [[0,0,0],[255,255,255],[129,129,129]]
# [[0,0,0],[255,255,255],[129,129,129]]
# [[0,0,0],[255,255,255],[129,129,129]]
# ]
# #3x3 pixels

# blue green red
# [128,  0,   0]
# 0-255

