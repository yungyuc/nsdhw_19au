import _matrix

A = _matrix.Matrix(2, 2, [1, 0, 0, 1])

B = _matrix.Matrix(2, 2, [1, 2, 3, 4])

print(A)
print(B)

print(_matrix.multiply_naive(A, B))
print(_matrix.multiply_mkl(A, B))

