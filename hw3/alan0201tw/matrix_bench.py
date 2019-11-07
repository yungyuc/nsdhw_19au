#!/bin/bash

''':'
path="${BASH_SOURCE[0]}"

if [[ (-n "$PRELOAD_MKL") && ("Linux" == "$(uname)") ]] ; then
    # Workaround for cmake + MKL in conda.
    #MKL_ROOT=$HOME/opt/conda
    MKL_ROOT=/opt/intel/mkl
    MKL_LIB_DIR=$MKL_ROOT/lib/intel64_lin
    MKL_LIBS=$MKL_LIB_DIR/libmkl_def.so
    MKL_LIBS=$MKL_LIBS:$MKL_LIB_DIR/libmkl_avx2.so
    MKL_LIBS=$MKL_LIBS:$MKL_LIB_DIR/libmkl_core.so
    MKL_LIBS=$MKL_LIBS:$MKL_LIB_DIR/libmkl_intel_lp64.so
    MKL_LIBS=$MKL_LIBS:$MKL_LIB_DIR/libmkl_sequential.so
    export LD_PRELOAD=$MKL_LIBS
    echo "set LD_PRELOAD=$LD_PRELOAD for MKL"
else
    echo "set PRELOAD_MKL if you see (Linux) MKL linking error"
fi

python3 $path
exit 0
':'''


# The python module that wraps the matrix code.
import _matrix

import time
import random

class Test:
    def main(self):
        # Reference : https://www.geeksforgeeks.org/generating-random-number-list-in-python/
        naive_time_min = 1e9
        naive_time_max = 0
        naive_time_sum = 0

        for i in range(5):
            matrix0 = _matrix.Matrix(2048, 1024, [random.randrange(1, 50, 1) for i in range(2048 * 1024)])
            matrix1 = _matrix.Matrix(1024, 2048, [random.randrange(1, 50, 1) for i in range(1024 * 2048)])

            naive_start = time.time()
            result_matrix_naive = _matrix.multiply_naive(matrix0, matrix1)
            naive_end = time.time()
            naive_time = naive_end - naive_start

            naive_time_min = min(naive_time_min, naive_time)
            naive_time_max = max(naive_time_max, naive_time)
            naive_time_sum += naive_time

        mkl_time_min = 1e9
        mkl_time_max = 0
        mkl_time_sum = 0

        for i in range(5):
            matrix0 = _matrix.Matrix(2048, 1024, [random.randrange(1, 50, 1) for i in range(2048 * 1024)])
            matrix1 = _matrix.Matrix(1024, 2048, [random.randrange(1, 50, 1) for i in range(1024 * 2048)])

            mkl_start = time.time()
            result_matrix_mkl = _matrix.multiply_mkl(matrix0, matrix1)
            mkl_end = time.time()
            mkl_time = mkl_end - mkl_start

            mkl_time_min = min(mkl_time_min, mkl_time)
            mkl_time_max = max(mkl_time_max, mkl_time)
            mkl_time_sum += mkl_time


        f = open("performance.txt", "w+")
        f.write("Start multiply_naive (repeat=5), take min = {0} seconds \n".format(naive_time_min))
        f.write("Start multiply_mkl   (repeat=5), take min = {0} seconds \n".format(  mkl_time_min))
        f.write("Start multiply_naive (repeat=5), take max = {0} seconds \n".format(naive_time_max))
        f.write("Start multiply_mkl   (repeat=5), take max = {0} seconds \n".format(  mkl_time_max))

        f.write("Start multiply_naive (repeat=5), take average = {0} seconds \n".format(naive_time_sum / 5.0))
        f.write("Start multiply_mkl   (repeat=5), take average = {0} seconds \n".format(  mkl_time_sum / 5.0))
        f.write("MKL speed-up over naive: {0} x \n".format(naive_time_sum / mkl_time_sum))
        # f.write("Start multiply_naive (repeat=5), take min = 1.2351381540065631 seconds"
        #         "Start multiply_mkl (repeat=5), take min = 0.056176671001594514 seconds"
        #         "MKL speed-up over naive: 21.9867 x")
        f.close()

if __name__ == "__main__":
    T = Test()
    T.main()
