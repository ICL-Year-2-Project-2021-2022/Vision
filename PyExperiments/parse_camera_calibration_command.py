import csv

header = ['Object code', 'COM pixel X coordinate', 'COM pixel Y coordinate', 'COM mass',
          'Bounding box (BB) left X coordinate', 'BB top  Y coordinate', 'BB bottom Y coordinate', 'White tape measurement', 'Yellow tape measurement']
data = [header]

input_header = True

with open('assets\camera-calibrationLime.csv', newline='') as csvfile:
    spamreader = csv.reader(csvfile, delimiter=',', quotechar='|')
    for row in spamreader:
        if not input_header:
            data_row = []
            split_command = row[0].split(' ')
            data_row.append(split_command[0][1:])
            data_row.extend(list(map(lambda hex_num: int(hex_num, 16), split_command[1:-1])))
            data_row.append(row[2])
            data_row.append(row[3])
            data.append(data_row)
        else:
            input_header = False

with open("assets/camera_convertedLime.csv", "w+") as result_file:
    for row in data:
        result_file.write(','.join(list(map(lambda num: str(num), row))) + "\n")
    result_file.close()
