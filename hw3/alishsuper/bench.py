import _matrix
from random import seed
from random import random
import time

if __name__ == '__main__':
    seed(1000)

    A = _matrix.Matrix(1000, 1000)
    B = _matrix.Matrix(1000, 1000)
    for i in range(1000):
        for j in range(1000):
            A[i, j] = random()
            B[i, j] = random()

    start = time.time()
    _matrix.multiply_naive(A, B)
    end = time.time()
    naive_time = end - start

    start = time.time()
    _matrix.multiply_mkl(A, B)
    end = time.time()
    mkl_time = end-start

    speed_up = naive_time / mkl_time

    print('Start multiply_naive take min = ' + str(naive_time) + ' seconds')
    print('Start multiply_mkl, take min = ' + str(mkl_time) + ' seconds')
    print('MKL speed-up over naive: ' + str(speed_up) + ' x')
