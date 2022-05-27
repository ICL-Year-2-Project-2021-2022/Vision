import numpy as np
import math

def generate_rays(objects,positions, image):
    CAMERA_HEIGHT = 110  # mm

    rays = np.array([[]])
    for i,position in enumerate(positions):
        for object in objects[i]:
            #Makes distance guess based on depression
            horizontal_angle, vertical_angle, est_distance = map_pixel_to_angle()

            #To cartesian coordinates
            x_dir = math.cos(np.pi/2-horizontal_angle + position[2])*math.sin(np.pi/2-vertical_angle)
            y_dir = math.cos(np.pi / 2 - horizontal_angle + position[2]) * math.sin(np.pi / 2 - vertical_angle)
            z_dir = math.cos(np.pi/2-vertical_angle)
            dir_vector = np.array([x_dir, y_dir, z_dir])
            point = np.array([position[0], position[1], 110 ])
            rays[i].append([dir_vector, point, object[3]])
    return rays

def map_pixel_to_angle(object, image):
    position = (object[0], object[1])
    HORIZONTAL_FOV = 70/180*np.pi #Degrees
    VERTICAL_FOV = image.shape[0]/image.shape[1] * HORIZONTAL_FOV
    STAND_HEIGHT = 30 #mm
    CAMERA_HEIGHT = 110 #mm
    BALL_RADIUS = 20 #mm


    horizontal_angle = (position[0]/image.shape[1]-0.5)*HORIZONTAL_FOV

    #size estimation
    vertical_angle = (position[0]/image.shape[0]-0.5)*VERTICAL_FOV
    est_distance_angle = np.tan(np.pi/2-vertical_angle) * (CAMERA_HEIGHT-STAND_HEIGHT)

    est_pixel_diameter = np.sqrt(4 * object[2]/ np.pi)
    est_occupied_angle = est_pixel_diameter/image.shape[1]*HORIZONTAL_FOV
    est_distance_size = BALL_RADIUS/(np.tan(est_distance_angle/2))

    est_distance = (est_distance_size + est_distance_angle)/2
    return horizontal_angle, vertical_angle, est_distance

def match_rays(rays, ):
    np.zeroes(())
    found_colors = []

    #Sort by color, assume only one ball of color per frame assumed
    for ray_set in rays:
        for ray in ray_set:
            if ray[2] not in found_colors:
                found_colors[ray[2]] = [ray]
            else:
                found_colors[ray[2]].append(ray)

    for color in found_colors:
        for idxa, ballA in enumerate(color):
            for idxb, ballB in enumerate(color):
                if idxb != idxa:
                    find_mid(ballA, ballB)






def find_mid(ray1, ray2):
    matrix = np.array([[np.dot(ray2[0],ray1[0]),np.negative(np.dot(ray1[0], ray1[0]))],[np.dot(ray2[0], ray2[0]), np.negative(np.dot(ray1[0], ray2[0]))]])
    point_diff = np.minus(ray1[1], ray2[1])
    vector = np.array([np.dot(point_diff, ray1[0]), np.dot(point_diff, ray2[0])])
    solution = np.linalg.solve(matrix, vector)
    s = solution[0]
    r = solution[1]

    #Find closest points
    point1 = np.add(ray1[1], r*ray1[0])
    point2 = np.add(ray2[1], r * ray2[0])

    #Average them to estimate the position
    midpoint = np.add(point1, point2)/2

    #How far the two points are
    error = np.linalg.norm(np.subtract(point1, point2))

    return midpoint, error, ray1[2]