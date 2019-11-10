import time
import random

# The python module that wraps the matrix code.
import _matrix

def benchmark_gen():
    mat1 = _matrix.Matrix(1000, 1000)
    mat2 = _matrix.Matrix(1000, 1000)

    for it in range(1000):
        for jt in range(1000):
            mat1[it, jt] = random.uniform(-100,100)
            mat2[it, jt] = random.uniform(-100,100)
    
    start_time = time.time()
    for i in range(5):
        naive_mat = _matrix.multiply_naive(mat1, mat2)
    naive_time = time.time() - start_time

    start_time = time.time()
    for i in range(5):
        mkl_mat = _matrix.multiply_mkl(mat1, mat2)
    mkl_time = time.time() - start_time

    with open("performance.txt", "w") as out:
        out.write("Start multiply_naive (repeat=5), take min = {} seconds\n".format(naive_time))
        out.write("Start multiply_mkl (repeat=5), take min = {} seconds\n".format(mkl_time))
        out.write("MKL speed-up over naive: {} x\n".format(naive_time/mkl_time))

if __name__ == '__main__':
    benchmark_gen()
