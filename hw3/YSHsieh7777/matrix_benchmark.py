#!/bin/bash

''':'
file_path="${BASH_SOURCE[0]}"

if [[ (-n "$PRELOAD_MKL") && ("Linux" == "$(uname)") ]] ; then
    # Workaround for cmake + MKL in conda.
    MKL_ROOT=$HOME/opt/conda
    MKL_LIB_DIR=$MKL_ROOT/lib
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

python3 $file_path;

exit 0
':'''

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
