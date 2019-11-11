from _matrix import *
import time

def benchmark(multiply, A, B):
    start = time.time()
    multiply(A, B)
    return time.time() - start

A = B = Matrix(1000, 1000)
for i in range(1000):
    for j in range(1000):
        A[i, j] = i + j
        B[i, j] = i * j + i + j

rep = 5
naive_time = min([ benchmark(multiply_naive, A, B) for _ in range(rep)])
mkl_time = min([ benchmark(multiply_mkl, A, B) for _ in range(rep)])

with open("performace.txt", "w") as f:
    f.write("""Start multiply_naive (repeat={}), take min = {} seconds
Start multiply_mkl (repeat={}), take min = {} seconds
MKL speed-up over naive: {} x\n
""".format(rep, naive_time, rep, mkl_time, naive_time/mkl_time)
    )
