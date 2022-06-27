import numpy as np
import cv2
import routing
import matcher
import math

#Globals
click_state = 1
center_x = 0
center_y = 0
radius = 0



def click_event(event, x, y, flags, params):


    global click_state, center_y, center_x, radius,  img
    if event == cv2.EVENT_LBUTTONDOWN:
        hcum = 0
        scum = 0
        vcum = 0

        hvarcum = 0
        svarcum = 0
        vvarcum = 0

        hmin = 255
        smin = 255
        vmin = 255

        hmax = 0
        smax = 0
        vmax = 0



        print(x, ' ', y)
        if (click_state == 1):
            center_x = x
            center_y = y
            click_state = 2
        elif (click_state == 2):
            radius = int(math.sqrt((x - center_x)**2 + (y - center_y)**2))
            click_state = 1

            #Find averages
            hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
            for yidx, row in enumerate(hsv):
                for xidx, pix in enumerate(row):
                    if ((xidx - center_x)**2 + (yidx - center_x)**2 < radius**2):
                        #Update selsction
                        h = pix[0]
                        s = pix[1]
                        v = pix[2]
                        hcum += h
                        scum += s
                        vcum += v

                        if (h < hmin):
                            hmin = h
                        if (s < smin):
                            smin = s
                        if (v < vmin):
                            vmin = v
                        if (h > hmax):
                            hmax = h
                        if (s > smax):
                            smax = s
                        if (v > vmax):
                            vmax = v
            havg = hcum / int(math.pi * (radius-1)**2)
            savg = scum / int(math.pi * (radius-1) ** 2)
            vavg = vcum / int(math.pi *  (radius-1) ** 2)

            for xidx, row in enumerate(hsv):
                for yidx, pix in enumerate(row):
                    if ((xidx - center_x)**2 + (yidx - center_x)**2 < (radius)**2):
                        #Update selsction
                        h = pix[0]
                        s = pix[1]
                        v = pix[2]
                        hvarcum += (h - havg)**2
                        svarcum += (s - savg)**2
                        vvarcum += (v - vavg)**2


            hvarcum = math.sqrt(hvarcum/ int(math.pi * radius**2))
            svarcum = math.sqrt(svarcum / int(math.pi * radius ** 2))
            vvarcum = math.sqrt(vvarcum / int(math.pi * radius ** 2))

            out = f"AVG: {havg:.2f}, {savg:.2f}, {vavg:.2f}  MIN: {hmin:.2f}, {smin:.2f}, {vmin:.2f}  MAX: {hmax:.2f}, {smax:.2f}, {vmax:.2f}"
            out2 = f"STDEV: {hvarcum}, {svarcum}, {vvarcum}"
            img = cv2.circle(img, (center_x, center_y), radius, (255, 0, 0), 2)
            cv2.imshow("Image", img)

            print(out)
            print(out2)







img = cv2.imread("arenapink.jpg", cv2.IMREAD_COLOR)
#img = cv2.imread("assets/yellow.jpg", cv2.IMREAD_COLOR)
img = cv2.resize(img, (640, 480))
print(img.shape)
x = 320
y = 240
r = 500
img = cv2.circle(img, (x, y), r, (255, 0, 0), 2)

cv2.imshow("Image", img)
cv2.setMouseCallback('Image', click_event)
cv2.waitKey(0)
cv2.destroyAllWindows()


# img = cv2.resize(img, (1,1))
# img[0][0] = np.array([90, 255, 128])
# img = cv2.resize(img, (1000,1000))
#