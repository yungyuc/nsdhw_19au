#! /usr/bin/env python3
import time
import random
import _matrix


def time_delta(func):
    def time_wrap(*args, **kwargs):
        start = time.time()
        result = func(*args, **kwargs)
        delta = time.time() - start
        return result, delta

    return time_wrap


@time_delta
def MKL_MM(A, B):
    return _matrix.multiply_mkl(A, B)


@time_delta
def naive_MM(A, B):
    return _matrix.multiply_naive(A, B)

def copy_to_mat(mat, row, col, li):
  for i in range(row):
    for j in range(col):
      mat[i,j] = li[ i*mat.ncol+ j]
  

if __name__ == "__main__":
    test_num = 10
    A_nrow, A_ncol = 1000, 1000
    B_nrow, B_ncol = 1000, 1000
    total_size = A_nrow * B_ncol

    population_size = 10
    random_population = [int(random.randrange(100, 1000))
                         for _ in range(int(population_size))]

    naive_list = [0 for _ in range(test_num)]
    MKL_list = [0 for _ in range(test_num)]

    print("Begin benchmark...")
    for i in range(test_num):
        A = _matrix.Matrix(A_nrow, A_ncol)
        A_content = random.choices(random_population, k=A_nrow * A_ncol)
        copy_to_mat(A,A_nrow, A_ncol, A_content )

        B = _matrix.Matrix(B_nrow, B_ncol)
        B_content = random.choices(random_population, k=B_nrow * B_ncol)
        copy_to_mat(B,B_nrow, B_ncol, B_content )

        naive_result, naive_time = naive_MM(A, B)
        MKL_result, MKL_time = MKL_MM(A, B)
        # assert naive_result == MKL_result

        naive_list[i] = naive_time
        MKL_list[i] = MKL_time

    naive_avg = sum(naive_list) / len(naive_list)
    MKL_avg = sum(MKL_list) / len(MKL_list)
    diff = naive_avg / MKL_avg
    report = [
        f"Naive avg time: {naive_avg} sec\n",
        f"MKL avg time: {MKL_avg} sec\n",
        f"MKL speed-up over {diff} x\n",
    ]

    with open("performance.txt", "w") as f:
        f.writelines(report)

    print("Finish benchmark...")
