import serial
import csv
import datetime
import time
import numpy as np
import cv2
import routing
import matcher

# import matplotlib.pyplot as plt
# import pandas as pd

portPath = "COM9"  # Must match value shown on Arduino IDE
baud = 9600  # Must match Arduino baud rate
timeout = 5  # Seconds
filename = "../assets/serial_read_" + str(datetime.datetime.now().strftime("%y_%m_%d_%H_%M_%S_%p")) + ".txt"
max_num_readings = 32000


def create_serial_obj(portPath, baud_rate, tout):
    """
    Given the port path, baud rate, and timeout value, creates
    and returns a pyserial object.
    """
    return serial.Serial(portPath, baud_rate, timeout=tout)


def read_serial_data_and_clean(serial):
    """
    Given a pyserial object (serial). Outputs a list of lines read in
    from the serial port
    """
    serial.reset_input_buffer()

    clean_data = []
    readings_left = True
    timeout_reached = False

    image_counter = 0

    while readings_left and not timeout_reached:
        serial_line = serial.readline()

        """
        serial_line_utf8 = serial_line[0:12].decode("utf-8")
        print(serial_line_utf8)
        if "accumulated:" in serial_line_utf8 and ":end" in serial_line_utf8:
            ascii_line = serial_line.decode("ascii")
            img_data = ascii_line[12:-4]
            print(len(ascii_line))
            img_str = ascii_line.strip()
            lines = ascii_line.split(",")
            str = ''.join(lines)

            blank_image = np.zeros((60, 80, 3), np.uint8)

            for idx, c in enumerate(str):
                print(ord(c[0]))
                if ((idx % 2) == 0):
                    current_char = 16 * (ord(c[0]) - 97)
                else:
                    current_char = current_char + (ord(c[0]) - 97)
                    if (current_char != 0):
                        print("got here")

                    red = (current_char & 0xc0) >> 6
                    green = (current_char & 0x38) >> 3
                    blue = (current_char & 0x07)
                    y = (idx // 2) // 80
                    x = (idx // 2) % 80
                    blank_image[y][x][0] = blue * 32
                    blank_image[y][x][1] = green * 32
                    blank_image[y][x][2] = red * 64

            cv2.imshow("Image", blank_image)

            cv2.waitKey(0)
            cv2.destroyAllWindows()

            cv2.imwrite("img-" + str(image_counter) +".jpg", blank_image)
"""
        print(serial_line)
        """
        if serial_line == '':
            timeout_reached = True
        else:
            line_data = serial_line.decode(encoding='UTF-8').splitlines()[0].split(",")
            line_data = [time.time()] + [str(element) for element in line_data]
            line_data[1] = int(line_data[1], 2)
            print(line_data[1])
            if len(line_data) >= 2:
                clean_data.append(line_data)

            with open(filename, 'a', newline='') as csvfile:
                csvwrite = csv.writer(csvfile)
                csvwrite.writerow(line_data)

            if len(clean_data) == max_num_readings:
                readings_left = False
        """

    return clean_data


def is_number(string):
    """
    Given a string returns True if the string represents a number.
    Returns False otherwise.
    """
    try:
        float(string)
        return True
    except ValueError:
        return False


def map_value(x, in_min, in_max, out_min, out_max):
    return (((x - in_min) * (out_max - out_min)) / (in_max - in_min)) + out_min


'''    
def simple_plot(csv_file, columns, headers):
    plt.clf()
    plt.close()
    plt.plotfile(csv_file, columns, names=headers, newfig=True)
    plt.show()
def plot_csv(csv_file, cols):
    # Create Pandas DataFrame from csv data
    data_frame = pd.read_csv(csv_file)
    # Set the names of the columns
    data_frame.columns = cols
    # Set the first column (Time) as the index 
    data_frame = data_frame.set_index(cols[0])
    # Map the voltage values from 0-1023 to 0-5
    data_frame = data_frame.apply(lambda x: map_value(x,0.,1023,0,5))
    # Bring back the Time column
    data_frame = data_frame.reset_index()
    plt.clf()
    plt.close()
    # Plot the data
    data_frame.plot(x=cols[0],y=cols[1:])
    plt.show()
'''

print("Creating serial object...")
serial_obj = create_serial_obj(portPath, baud, timeout)

print("Reading serial data...")
clean_data = read_serial_data_and_clean(serial_obj)
print(len(clean_data))
