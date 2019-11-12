import unittest
import random
import _matrix

class Test(unittest.TestCase):
    def setup(self, m, n):
        A = _matrix.Matrix(m, n)
        for i in range(m):
            for j in range(n):
                A[i, j] = random.randint(0, 1000)
        return A

    def test_matrix_size(self):
        for i in range(10, 200, 10):
            m = self.setup(i, i)
            self.assertEqual(m.nrow, i)
            self.assertEqual(m.ncol, i)

    def test_naive_multiplication(self):
        m1 = self.setup(8, 10)
        m2 = self.setup(10, 12)
        c1 = _matrix.multiply_naive(m1, m2)
        for i in range(8):
            for j in range(12):
                v = 0.0
                for k in range(10):
                    v += m1[i,k] * m2[k,j]
                self.assertEqual(v, c1[i,j])

    def test_mkl_multiplication(self):
        m1 = self.setup(8, 10)
        m2 = self.setup(10, 12)
        c2 = _matrix.multiply_mkl(m1, m2)
        for i in range(8):
            for j in range(12):
                v = 0.0
                for k in range(10):
                    v += m1[i,k] * m2[k,j]
                self.assertEqual(v, c2[i,j])

    def test_multiply_equal(self):
        m1 = self.setup(8, 10)
        m2 = self.setup(10, 12)
        c1 = _matrix.multiply_naive(m1, m2)
        c2 = _matrix.multiply_mkl(m1, m2)
        self.assertEqual(c1, c2)
