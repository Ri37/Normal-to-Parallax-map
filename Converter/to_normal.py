import numpy as np

def convert_height_to_normal(heightmap_image):
    # Convert image to an array and scale it
    height_array = np.array(heightmap_image, dtype=np.float32) / 10

    dx = np.zeros_like(height_array)
    dy = np.zeros_like(height_array)

    # Approximate derivatives
    dx[:, 1:-1] = (height_array[:, 2:] - height_array[:, :-2]) / 2
    dy[1:-1, :] = (height_array[2:, :] - height_array[:-2, :]) / 2

    # Create vectors for the cross product
    v1 = np.stack((np.ones_like(dx), np.zeros_like(dx), dx), axis=-1)
    v2 = np.stack((np.zeros_like(dy), np.ones_like(dy), -dy), axis=-1)

    # Kereszt szorzat normalok kiszámításához
    normals = np.cross(v1, v2)

    # Normalizálás
    norms = np.linalg.norm(normals, axis=-1)
    normals /= norms[..., np.newaxis]

    # Vissza váltas RGB-be
    normal_map = np.zeros((*height_array.shape, 3), dtype=np.uint8)
    normal_map[:, :, 0] = (normals[:, :, 0] + 1) / 2 * 255
    normal_map[:, :, 1] = (normals[:, :, 1] + 1) / 2 * 255
    normal_map[:, :, 2] = (normals[:, :, 2] + 1) / 2 * 255

    return normal_map.astype(np.uint8)