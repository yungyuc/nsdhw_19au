from _matrix import *
import pytest

def test_equal():
    A = Matrix(100, 120)
    B = Matrix(120, 150)
    C = Matrix(100, 150)
    D = Matrix(100, 150)

    # print("testing two kinds of implementation of matrix multiplication...")
    C = A * B
    D = multiply_mkl(A, B)
    ret = isequal(C, D)
    # print("testing complete!")

    assert ret == True, "result matrics of two implementations are not match."
    # print("Success! result matrics are match")


def test_index():
    A = Matrix(100, 120)
    row1 = -1
    row2 = 0
    col1 = 120
    col2 = 0
    with pytest.raises(Exception) as error:
        A[row1, col2]
        A[row2, col1]

def test_zero():
    A = Matrix(100, 120)
    B = Matrix(120, 150)
    C = Matrix(100, 150)
    D = Matrix(100, 150)
    for i in range(C.nrow):
        for j in range(C.ncol):
            C[i, j] = 0
    D = D * 0
    assert D == C, "broadcasting fail"
    A = 0 * A
    tmp = A * B
    assert tmp == C, "mutiply zero matrix fail"