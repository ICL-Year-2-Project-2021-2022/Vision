import numpy as np
import math

def generate_rays(objects,positions, image):
    CAMERA_HEIGHT = 110  # mm

    rays = []
    for i,position in enumerate(positions):
        rays.append([])
        for object in objects[i]:
            #Makes distance guess based on depression
            horizontal_angle, vertical_angle, est_distance = map_pixel_to_angle(object, image)

            #To cartesian coordinates
            x_dir = math.sin(np.pi / 2 - horizontal_angle - position[2]) * math.sin(np.pi / 2 - vertical_angle)
            y_dir = -math.cos(np.pi/2-horizontal_angle - position[2])*math.sin(np.pi/2-vertical_angle)
            z_dir = math.cos(np.pi/2-vertical_angle)
            dir_vector = [x_dir, y_dir, z_dir]
            point = [position[0], position[1], 110 ]
            rays[i].append([dir_vector, point, object[3]])
    return rays

def map_pixel_to_angle(object, image):
    position = (object[0], object[1])
    HORIZONTAL_FOV = 65/180*np.pi #Rad
    VERTICAL_FOV = image.shape[0]/image.shape[1] * HORIZONTAL_FOV
    STAND_HEIGHT = 30 #mm
    CAMERA_HEIGHT = 110 #mm
    BALL_RADIUS = 20 #mm


    horizontal_angle = (position[0]/image.shape[1]-0.5)*HORIZONTAL_FOV

    #size estimation
    vertical_angle = -(position[1]/image.shape[0]-0.5)*VERTICAL_FOV

    #Only works for points below horizon
    est_distance_angle = np.tan(np.pi/2 + vertical_angle) * (CAMERA_HEIGHT-STAND_HEIGHT)

    #Estimates distance based on mask size
    est_pixel_diameter = np.sqrt(4 * object[2]/ np.pi)
    est_occupied_angle = est_pixel_diameter/image.shape[1]*HORIZONTAL_FOV
    est_distance_size = BALL_RADIUS/(np.tan(est_occupied_angle/2))

    #Average heuristics
    est_distance = (est_distance_size + est_distance_angle)/2
    return horizontal_angle, vertical_angle, est_distance

def match_rays(rays, occupancy_grid, probability_grid, scale):
    found_colors = {}
    offset_y = occupancy_grid.shape[0]*scale/2
    offset_x = occupancy_grid.shape[1]*scale/2


    #Sort by color, assume only one ball of color per frame assumed
    for ray_set in rays:
        for ray in ray_set:
            if ray[2] not in found_colors:
                found_colors[ray[2]] = [ray]
            else:
                found_colors[ray[2]].append(ray)

    for colidx, color in enumerate(found_colors):
        for idxa, ballA in enumerate(found_colors[color]):
            for idxb, ballB in enumerate(found_colors[color]):
                if idxb != idxa:
                    mid, error = find_mid(ballA, ballB)
                    if error<50:
                        probability_grid[int((offset_y+mid[1])/scale)][int((offset_x+mid[0])/scale)] += 1
                        occupancy_grid[int((offset_y + mid[1]) / scale)][int((offset_x + mid[0]) / scale)] = colidx + 10

    return probability_grid, occupancy_grid






def find_mid(ray1, ray2):
    matrix = np.array([[np.dot(ray1[0], ray1[0]),np.negative(np.dot(ray2[0],ray1[0]))],[np.dot(ray2[0], ray1[0]), np.negative(np.dot(ray2[0], ray2[0]))]])
    point_diff = np.subtract(ray1[1], ray2[1])
    vector = np.array([np.negative(np.dot(point_diff, ray1[0])), np.negative( np.dot(point_diff, ray2[0]))])
    solution = np.linalg.solve(matrix, vector)

    r = solution[0]
    s = solution[1]

    #Find closest points
    point1 = np.add(ray1[1],np.multiply( r,  ray1[0]))
    point2 = np.add(ray2[1], np.multiply( s,  ray2[0]))

    #Average them to estimate the position
    midpoint = np.add(point1, point2)/2

    #How far the two points are
    error = np.linalg.norm(np.subtract(point1, point2))
    return midpoint, error