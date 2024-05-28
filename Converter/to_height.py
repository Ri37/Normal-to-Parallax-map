import numpy as np

def normal_to_height_map(normal_map, direction):
    height, width = normal_map.shape[:2]
    height_map = np.zeros((height, width))

    if direction == 0:   # top to bottom
        i_range = range(1, height)
        j_range = range(0, width)
        delta_H_func = lambda i, j: normal_map[i, j, 1] / normal_map[i, j, 2]
        update_func = lambda i, j, delta_H: height_map[i-1, j] + delta_H
    elif direction == 1: # bottom to top
        i_range = range(height-2, -1, -1)
        j_range = range(0, width)
        delta_H_func = lambda i, j: normal_map[i, j, 1] / normal_map[i, j, 2]
        update_func = lambda i, j, delta_H: height_map[i+1, j] - delta_H
    elif direction == 2: # left to right
        i_range = range(0, height)
        j_range = range(1, width)
        delta_H_func = lambda i, j: normal_map[i, j, 0] / normal_map[i, j, 2]
        update_func = lambda i, j, delta_H: height_map[i, j-1] - delta_H
    elif direction == 3: # right to left
        i_range = range(0, height)
        j_range = range(width-2, -1, -1)
        delta_H_func = lambda i, j: normal_map[i, j, 0] / normal_map[i, j, 2]
        update_func = lambda i, j, delta_H: height_map[i, j+1] + delta_H

    for i in i_range:
        for j in j_range:
            delta_H = delta_H_func(i, j)
            height_map[i, j] = update_func(i, j, delta_H)

    min_height = np.min(height_map)
    max_height = np.max(height_map)
    height_map = (height_map - min_height) / (max_height - min_height)

    return height_map

def convert_normal_to_height_with_individual_maps(normal_map_image):
    normal_map = np.array(normal_map_image) / 255.0
    normal_map = normal_map * 2 - 1
    height_map = np.zeros(normal_map.shape[:2])
    height_maps = []
    
    for i in range(4):
        hm = normal_to_height_map(normal_map, i)
        height_maps.append(hm)
        height_map += hm
    
    height_map /= 4
    
    min_height = np.min(height_map)
    max_height = np.max(height_map)
    height_map = (height_map - min_height) / (max_height - min_height) * 255
    
    height_maps = [(hm - np.min(hm)) / (np.max(hm) - np.min(hm)) * 255 for hm in height_maps]
    height_maps = [hm.astype(np.uint8) for hm in height_maps]
    final_height_map = height_map.astype(np.uint8)
    
    return height_maps, final_height_map