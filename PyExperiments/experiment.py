import numpy as np
import cv2
import routing
import matcher

# img = cv2.imread("assets/yellow_ball.jpg", cv2.IMREAD_COLOR)
# img = cv2.resize(img, (1,1))
# img[0][0] = np.array([90, 255, 128])
# img = cv2.resize(img, (1000,1000))
#
# cv2.imshow("Image", img)
# cv2.waitKey(0)
# cv2.destroyAllWindows()

# z = np.zeros((30, 20))
# z[0][0] = 200
# z[10][10] = 100
# z[19][0] = 200
#
# passable = routing.passable(0.2,3,z)
# path = routing.mod_bellman(0,29, 5,6, passable)
# print(passable)
ray1 = [[1,1,2], [0,2,-1],  "red"]
ray2 = [[1,1,3], [1,0,-1],  "red"]
matcher.find_mid(ray1, ray2)
