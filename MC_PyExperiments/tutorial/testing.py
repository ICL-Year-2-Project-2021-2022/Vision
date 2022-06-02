import cv2

img = cv2.imread('assets/125.jpg',1)

#dimensions, width, length
#img = cv2.resize(img, (700,400))

#fx,fy scaling
img = cv2.resize(img, (0,0), fx=1, fy=1)
img = cv2.rotate(img, cv2.cv2.ROTATE_90_COUNTERCLOCKWISE)

# -1, cv2.IMREAD_COLOUR : Load a color image. Any Transparency of image would be ignored
#  0, cv2.IMREAD_GRAYSCALE : Load image in grayscale mode
#  1, cv2.IMREAD_UNCHANGED : Loads image as such including alpha channel

#write out img into a new file called new_img.jpg
cv2.imwrite('new_img.jpg',img)
cv2.imshow('Image',img)

#wait an infinite amount of time
cv2.waitKey(0)
#once pressed a key, it will destroy all windows. 
cv2.destroyAllWindows()

