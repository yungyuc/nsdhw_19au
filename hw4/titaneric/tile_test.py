import timeit

import numpy as np
import _matrix

def different_tile_size():
    matrix_size = 512
    A_content = np.eye(matrix_size)
    A = _matrix.Matrix(A_content)
    # print(A)

    B_content = np.arange(1, matrix_size ** 2 + 1).reshape(matrix_size, matrix_size)
    B = _matrix.Matrix(B_content)

    # print(B)

    MKL_MM = _matrix.multiply_mkl(A, B)

    for p in range(1, 9):
        tiled_MM = _matrix.multiply_tile(A, B, 2 ** p)
        assert tiled_MM == MKL_MM
    
    for p in [14, 22, 56]:
        tiled_MM = _matrix.multiply_tile(A, B, p)
        assert tiled_MM == MKL_MM

def performance():
    matrix_size = 512
    A_content = np.eye(matrix_size)
    A = _matrix.Matrix(A_content)
    # print(A)

    B_content = np.arange(1, matrix_size ** 2 + 1).reshape(matrix_size, matrix_size)
    B = _matrix.Matrix(B_content)


    for p in range(4, 8):
        tsize = 2 ** p
        ns = dict(_matrix=_matrix, mat1=A, mat2=B, tsize=tsize)
        t_tile = timeit.Timer('_matrix.multiply_tile(mat1, mat2, tsize)', globals=ns)
        t_naive = timeit.Timer('_matrix.multiply_naive(mat1, mat2)', globals=ns)

        time_tile = min(t_tile.repeat(10, 1))
        time_naive = min(t_naive.repeat(10, 1))
        ratio = time_tile/time_naive
        print(tsize, ratio)

def odd_tile_size():
    matrix_size = 3
    A_content = np.eye(matrix_size)
    A = _matrix.Matrix(A_content)
    # print(A)

    B_content = np.arange(1, matrix_size ** 2 + 1).reshape(matrix_size, matrix_size)
    B = _matrix.Matrix(B_content)

    print(_matrix.multiply_tile(A, B, 2))

if __name__ == "__main__":
    performance()
