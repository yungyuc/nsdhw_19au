import time
import random
import _matrix

if __name__ == "__main__":
    repeat = 5
    n = 1000
    A = _matrix.Matrix(n, n)
    B = _matrix.Matrix(n, n)
    for i in range(n):
        for j in range(n):
            A[i, j] = random.randint(0, 1000)
            B[i, j] = random.randint(0, 1000)
    naive_min = 99
    dgemm_min = 99
    for i in range(repeat):
        time_B = time.time()
        r1 = _matrix.multiply_naive(A, B)
        time_A = time.time()
        naive_min = min(naive_min, time_A-time_B);

        time_B = time.time()
        r2 = _matrix.multiply_mkl(A, B)
        time_A = time.time()
        dgemm_min = min(dgemm_min, time_A-time_B);
        if r1==r2:
            print('Answers from the two methods are the same! ')
    
    filename = "performance.txt"
    f = open(filename,"w+")
    f.write(
    """Start multiply_naive (repeat={}), take min = {} seconds
Start multiply_mkl (repeat={}), take min = {} seconds
MKL speed-up over naive: {} x
    """.format(repeat, naive_min,
               repeat, dgemm_min,
               naive_min/dgemm_min))
    f.close()

