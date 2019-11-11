from _matrix import *
import math
import time

def bench(iterations):
    ret_naive = []
    ret_mkl = []
    print("start testing speed up...")
    for i in range(iterations):
        A = Matrix(1000, 1200)
        B = Matrix(1200, 1500)
        C = Matrix(1000, 1500)
        tic = time.time()
        C = A * B
        toc = time.time()
        ret_naive.append(toc - tic)
        print("multiply_naive iteration {} complete!".format(i + 1))
    for i in range(iterations):
        A = Matrix(1000, 1200)
        B = Matrix(1200, 1500)
        D = Matrix(1000, 1500)
        tic = time.time()
        D = multiply_mkl(A, B)
        toc = time.time()
        ret_mkl.append(toc - tic)
        print("multiply_mkl iteration {} complete!".format(i + 1))
    with open("performace.txt", "w") as f:
        naive_avg = sum(ret_naive) / len(ret_naive)
        mkl_avg = sum(ret_mkl) / len(ret_mkl)
        ret = "Start multiply_naive (repeat={iter}),\ntake min = {} seconds\nStart multiply_mkl (repeat={iter}),\ntake min = {} seconds\
            \nMKL speed-up over naive: {} x".format(min(ret_naive), min(ret_mkl), naive_avg / mkl_avg, iter=iterations)
        f.write(ret)
        print("MKL speed-up over naive: {} x".format(naive_avg / mkl_avg))

if __name__ == '__main__':
    bench(5)