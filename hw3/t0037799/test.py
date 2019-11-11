import unittest
import _matrix
import random

class MatrixTest(unittest.TestCase):
    def setup(self):
        self.eps = 1e-6
    def rand_matrix(self, m, n):
        o = _matrix.Matrix(m, n)
        for i in range(m):
            for j in range(n):
                o[i,j] = random.randint(-100,100)
        return o
    def test_dimension_match(self):
        m = _matrix.Matrix(3,4)
        n = _matrix.Matrix(4,4)
        o = _matrix.multiply_mkl(m,n)
        self.assertEqual(o.nrow, 3)
        self.assertEqual(o.ncol, 4)
        o = _matrix.multiply_naive(m,n)
        self.assertEqual(o.nrow, 3)
        self.assertEqual(o.ncol, 4)

    def test_identity(self):
        d = 10
        I = _matrix.Matrix(d, d)
        A = self.rand_matrix(d, d)
        for i in range(d):
            I[i,i] = 1
        self.assertEqual(A, _matrix.multiply_mkl(A,I))
        self.assertEqual(A, _matrix.multiply_naive(A,I))
        self.assertEqual(A, _matrix.multiply_mkl(I,A))
        self.assertEqual(A, _matrix.multiply_naive(I,A))
        
    def test_setter(self):
        t = 100
        A = _matrix.Matrix(1,1)
        for i in range(t):
            n = random.randint(-100000,100000)
            A[0,0] = n
            self.assertEqual(n, A[0,0])

    def test_rand_mul(self):
        t = 10
        for i in range(t):
            n = random.randint(10,100)
            m = random.randint(10,100)
            r = random.randint(10,100)
            A = self.rand_matrix(n, m)
            B = self.rand_matrix(m, r)
            self.assertEqual(_matrix.multiply_mkl(A,B), _matrix.multiply_naive(A,B))

if __name__ == '__main__':
    unittest.main()
