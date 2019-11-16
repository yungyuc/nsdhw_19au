import numpy as np
import _matrix

def different_tile_size():
    matrix_size = 1024
    A_content = np.eye(matrix_size)
    A = _matrix.Matrix(A_content)
    # print(A)

    B_content = np.arange(1, matrix_size ** 2 + 1).reshape(matrix_size, matrix_size)
    B = _matrix.Matrix(B_content)

    # print(B)

    MKL_MM = _matrix.multiply_mkl(A, B)

    for p in range(10):
        tiled_MM = _matrix.multiply_tile(A, B, 2 ** p)
        assert tiled_MM == MKL_MM

def odd_tile_size():
    matrix_size = 3
    A_content = np.eye(matrix_size)
    A = _matrix.Matrix(A_content)
    # print(A)

    B_content = np.arange(1, matrix_size ** 2 + 1).reshape(matrix_size, matrix_size)
    B = _matrix.Matrix(B_content)

    print(_matrix.multiply_tile(A, B, 2))

if __name__ == "__main__":
    odd_tile_size()
