from _matrix import *
import pytest

def test_constructor():
    A = Matrix(1000, 2500)
    assert A.nrow == 1000
    assert A.ncol == 2500

def test_equal():
    A = Matrix(100, 100)
    B = Matrix(100, 100)
    C = Matrix(100, 100)
    for i in range(100):
        for j in range(100):
            A[i,j] = 1
            B[i,j] = 1
            C[i,j] = 2
    assert isequal(A,B) == True
    assert isequal(A,C) == False

def test_multiply():
    A = Matrix(100, 100)
    C = Matrix(100, 100)
    for i in range(100):
        for j in range(100):
            A[i,j] = 1
            C[i,j] = 100
    B = multiply_naive(A,A)
    assert isequal(B, C) == True
    B = multiply_mkl(A,A)
    assert isequal(B, C) == True

