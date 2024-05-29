import numpy as np
from scipy.sparse import lil_matrix
from scipy.sparse.linalg import cg

def get_A(n):
    A = lil_matrix((n*n,n*n))

    for i in range(n):
        for j in range(n):
            k = i * n + j

            # b[i][j] = x[i-1][j] + x[i+1][j] + x[i][j-1] + x[i][j+1] -4x[i][j]
            A[k, k] = -4
            temp_i = i-1
            if temp_i < 0:
                temp_i = n-1
            A[k, temp_i * n + j] = 1
            temp_i = i+1
            if temp_i >= n:
                temp_i = 0
            A[k, temp_i * n + j] = 1

            temp_j = j-1
            if temp_j < 0:
                temp_j = n-1
            A[k, i * n + temp_j] = 1
            temp_j = j+1
            if temp_j >= n:
                temp_j = 0
            A[k, i * n + temp_j] = 1

    return A

def get_b(normal):
    n, _, _ = normal.shape
    X =  normal[:,:,0] / normal[:,:,2]
    Y = -normal[:,:,1] / normal[:,:,2]

    b = np.zeros((n, n))

    # b[i][j] = Nx[i+1][j] - Nx[i-1][j] + Ny[i][j+1] - Ny[i][j-1]
    for i in range(n):
        for j in range(n):
            x_right = X[i, (j+1) % n]
            x_left = X[i, (j-1) % n]
            y_up = Y[(i+1) % n, j]
            y_down = Y[(i-1) % n, j]

            b[i, j] = x_right - x_left + y_up - y_down

    b_flat = b.flatten()
    b_flat_transposed = np.transpose(b_flat)

    return b_flat_transposed

def convert_normal_to_height_advanced(normal_map):
    normal = normal_map / 255.0
    normal = normal * 2 - 1

    n = normal.shape[0]

    A = get_A(n)
    A_transpose = A.transpose()
    A_new = A_transpose.dot(A)

    b = get_b(normal)
    b_new = A_transpose.dot(b)

    x, exit_code = cg(A_new, b_new)

    height_map = -x.reshape(n, n)
    min_height = np.min(height_map)
    max_height = np.max(height_map)
    height_map = (height_map - min_height) / (max_height - min_height) * 255
    return height_map.astype(np.uint8)