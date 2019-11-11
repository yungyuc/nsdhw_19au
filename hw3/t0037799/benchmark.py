import _matrix
import time

def make_matrices(size):

    mat1 = _matrix.Matrix(size,size)
    mat2 = _matrix.Matrix(size,size)

    for it in range(size):
        for jt in range(size):
            mat1[it, jt] = it * size + jt + 1
            mat2[it, jt] = it * size + jt + 1

    return mat1, mat2

def benchmark(fun, A, B):
    s = time.time()
    _ = fun(A, B)
    return time.time() - s

rep = 5
size = 1000
A, B = make_matrices(size)
min_mkl_sec = 100000000
min_naive_sec = 100000000
f_name = 'performance.txt'
print('matrix size ', A.nrow, 'x', A.ncol)
print('Start Benchmark')

print('doing naive mul (repeat', rep)
for i in range(rep):
    print(i+1, '...', end='\r')
    min_naive_sec = min(min_naive_sec, benchmark(_matrix.multiply_naive, A, B))

print('doing mkl mul (repeat', rep)
for i in range(rep):
    print(i+1, '...', end='\r')
    min_mkl_sec = min(min_mkl_sec, benchmark(_matrix.multiply_mkl, A, B))

print('Finish Benchmark')
print('Save to ', f_name)

with open(f_name, "w") as f:
    f.write(
            """
            Start multiply_naive (repeat={}), take min = {} seconds
            Start multiply_mkl (repeat={}), take min = {} seconds
            MKL speed-up over naive: {} x
            """.format(rep, min_naive_sec, rep, min_mkl_sec, min_naive_sec/min_mkl_sec)
            )
    f.writelines("\n")
