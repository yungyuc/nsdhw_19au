from _matrix import *
import math
import pytest
import time

def test_equal():
    A = Matrix(1000, 1200)
    B = Matrix(1200, 1500)
    C = Matrix(1000, 1500)
    D = Matrix(1000, 1500)

    print("testing two kinds of implementation of matrix multiplication...")
    C = A * B
    mkl_MM(A, B, D)
    ret = ifequal(C, D)
    print("testing complete!")

    assert ret == True, "result matrics of two implementations are not match."
    print("Success! result matrics are match")

if __name__ == "__main__":
    test_equal()