import numpy as np
import cv2
from matplotlib import pyplot as plt

def fourier_trans(img):
    #invert black and white
    img= cv2.bitwise_not(img)
    y = int(img.shape[0])//2
    x = int(img.shape[1])
    img = img[y:, :x]
    cv2.imshow('cropped out', img)
    sum_x = np.sum(img/255, axis=0)
    f = np.fft.fft(sum_x)
    #shift the zero-frequency component to the center of the spectrum
    fshift = np.fft.fftshift(f)
    magnitude = 20*np.log(np.abs(f))
    fun_f = fun_freq(f)
    print('fundamental frequency',fun_f)
    plt.plot(magnitude)
    plt.title('Magnitude Spectrum')
    plt.xticks([]), plt.yticks([])
    plt.show()
    return fun_f


def fun_freq(fft_data):
    filter = [-1,1] #identify slope and find local maximum
    con_data = np.convolve(fft_data, filter)

    pre_pos = False
    for i in range(len(con_data)):
        if i==0:
            pre_pos=True
        elif con_data[i]>0:
            pre_pos=True
        elif pre_pos and con_data[i]<0:
            y_points = [abs(fft_data[i-2]),abs(fft_data[i-1]),abs(fft_data[i])]
            x_points = [i-2,i-1,i]
            p1=np.polyfit(x_points, y_points,2) #coefficient of quadratic fit
            max_f1 = -p1[1]/2/p1[0]
            return max_f1