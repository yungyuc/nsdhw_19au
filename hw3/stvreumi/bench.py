import _matrix

# borrow from validate.py
def make_matrices(self, size):

    mat1 = _matrix.Matrix(size,size)
    mat2 = _matrix.Matrix(size,size)

    for it in range(size):
        for jt in range(size):
            mat1[it, jt] = it * size + jt + 1
            mat2[it, jt] = it * size + jt + 1

    return mat1, mat2

def run_native(test_mat1, test_mat2):
    print("run native...")
    _matrix.multiply_naive(test_mat1, test_mat2)

def run_mkl(test_mat1, test_mat2):
    print("run mkl...")
    _matrix.multiply_mkl(test_mat1, test_mat2)


if __name__=="__main__":
    import timeit
    size = 1000
    repeat = 5
    native_time = \
        timeit.repeat("run_native(test_mat1, test_mat2)",
            setup="from __main__ import run_native, make_matrices;\
                test_mat1, test_mat2 = make_matrices({s}, {s})".format(s=size),
            number=1,
            repeat=repeat
        )
    mkl_time = \
        timeit.repeat("run_mkl(test_mat1, test_mat2)", 
            setup="from __main__ import run_mkl, make_matrices;\
                test_mat1, test_mat2 = make_matrices({s}, {s})".format(s=size),
            number=1,
            repeat=repeat
        )
    with open("performance.txt", "w") as f:
        print("Start multiply_naive (repeat={}), take min = {} seconds".format(repeat, min(native_time)), file=f)
        print("Start multiply_mkl (repeat={}), take min = {} seconds".format(repeat, min(mkl_time)), file=f)
        print("MKL speed-up over naive: {} x".format(min(native_time) / min(mkl_time)), file=f)
