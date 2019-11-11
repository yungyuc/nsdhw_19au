import unittest
import random
import _matrix
class Test(unittest.TestCase):

    def make_matrix(self, size):
        mat1 = _matrix.Matrix(size,size)
        for it in range(size):
            for jt in range(size):
                mat1[it, jt] = it * size + jt + 1
        return mat1

    def test_create(self):
        for i in range(20):
            m = self.make_matrix(i)
            m2 = self.make_matrix(i)
            self.assertEqual(m.nrow, i)
            self.assertEqual(m.ncol, i)
            self.assertEqual(m, m2)

    def test_mul(self):
        m1 = self.make_matrix(3)
        m2 = self.make_matrix(3)
        r1 = _matrix.multiply_naive(m1, m2)
        r2 = _matrix.multiply_mkl(m1, m2)
        self.assertEqual(r1, r2)
if __name__ == '__main__':
    unittest.main()
