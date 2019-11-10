import unittest
import random

# The python module that wraps the matrix code.
import _matrix


class MatrixTest(unittest.TestCase):

    def test_matrix_init_100_100(self):
        mat = _matrix.Matrix(100, 100)

        for it in range(100):
            for jt in range(100):
                mat[it, jt] = it + jt

        self.assertEqual(100, mat.nrow)
        self.assertEqual(100, mat.ncol)
        self.assertEqual(0, mat[0, 0])
        self.assertEqual(1, mat[0, 1])
        self.assertEqual(1, mat[1, 0])
        self.assertEqual(100, mat[50, 50])
        self.assertEqual(198, mat[99, 99])

    def test_matrix_init_200_100(self):
        mat = _matrix.Matrix(200, 100)

        for it in range(200):
            for jt in range(100):
                mat[it, jt] = it + jt

        self.assertEqual(200, mat.nrow)
        self.assertEqual(100, mat.ncol)
        self.assertEqual(0, mat[0, 0])
        self.assertEqual(1, mat[0, 1])
        self.assertEqual(1, mat[1, 0])
        self.assertEqual(100, mat[50, 50])
        self.assertEqual(198, mat[99, 99])
        self.assertEqual(298, mat[199, 99])
         
    def test_matrix_init_100_200(self):
        mat = _matrix.Matrix(100, 200)

        for it in range(100):
            for jt in range(200):
                mat[it, jt] = it + jt

        self.assertEqual(100, mat.nrow)
        self.assertEqual(200, mat.ncol)
        self.assertEqual(0, mat[0, 0])
        self.assertEqual(1, mat[0, 1])
        self.assertEqual(1, mat[1, 0])
        self.assertEqual(100, mat[50, 50])
        self.assertEqual(198, mat[99, 99])
        self.assertEqual(298, mat[99, 199])

    def test_matrix_naive_multiply_100_100_100(self):
        mat1 = _matrix.Matrix(100, 100)
        mat2 = _matrix.Matrix(100, 100)

        for it in range(100):
            for jt in range(100):
                mat1[it, jt] = 1
                mat2[it, jt] = 2

        ret_mat = _matrix.multiply_naive(mat1, mat2)

        self.assertEqual(100, ret_mat.nrow)
        self.assertEqual(100, ret_mat.ncol)
        self.assertEqual(200, ret_mat[0, 0])
        self.assertEqual(200, ret_mat[0, 1])
        self.assertEqual(200, ret_mat[1, 0])
        self.assertEqual(200, ret_mat[50, 50])
        self.assertEqual(200, ret_mat[99, 99])

    def test_matrix_naive_multiply_100_200_100(self):
        mat1 = _matrix.Matrix(100, 200)
        mat2 = _matrix.Matrix(200, 100)

        for it in range(100):
            for jt in range(200):
                mat1[it, jt] = 1
        for it in range(200):
            for jt in range(100):
                mat2[it, jt] = 2

        ret_mat = _matrix.multiply_naive(mat1, mat2)

        self.assertEqual(100, ret_mat.nrow)
        self.assertEqual(100, ret_mat.ncol)
        self.assertEqual(400, ret_mat[0, 0])
        self.assertEqual(400, ret_mat[0, 1])
        self.assertEqual(400, ret_mat[1, 0])
        self.assertEqual(400, ret_mat[50, 50])
        self.assertEqual(400, ret_mat[99, 99])

    def test_matrix_mkl_multiply_100_100_100(self):
        mat1 = _matrix.Matrix(100, 100)
        mat2 = _matrix.Matrix(100, 100)

        for it in range(100):
            for jt in range(100):
                mat1[it, jt] = 1
                mat2[it, jt] = 2

        ret_mat = _matrix.multiply_mkl(mat1, mat2)

        self.assertEqual(100, ret_mat.nrow)
        self.assertEqual(100, ret_mat.ncol)
        self.assertEqual(200, ret_mat[0, 0])
        self.assertEqual(200, ret_mat[0, 1])
        self.assertEqual(200, ret_mat[1, 0])
        self.assertEqual(200, ret_mat[50, 50])
        self.assertEqual(200, ret_mat[99, 99])

    def test_matrix_mkl_multiply_100_200_100(self):
        mat1 = _matrix.Matrix(100, 200)
        mat2 = _matrix.Matrix(200, 100)

        for it in range(100):
            for jt in range(200):
                mat1[it, jt] = 1
        for it in range(200):
            for jt in range(100):
                mat2[it, jt] = 2

        ret_mat = _matrix.multiply_mkl(mat1, mat2)

        self.assertEqual(100, ret_mat.nrow)
        self.assertEqual(100, ret_mat.ncol)
        self.assertEqual(400, ret_mat[0, 0])
        self.assertEqual(400, ret_mat[0, 1])
        self.assertEqual(400, ret_mat[1, 0])
        self.assertEqual(400, ret_mat[50, 50])
        self.assertEqual(400, ret_mat[99, 99])

    def test_matrix_naive_mkl_result(self):
        mat1 = _matrix.Matrix(100, 100)
        mat2 = _matrix.Matrix(100, 100)

        for it in range(100):
            for jt in range(100):
                mat1[it, jt] = random.uniform(-100,100)
                mat2[it, jt] = random.uniform(-100,100)

        naive_mat = _matrix.multiply_naive(mat1, mat2)
        mkl_mat = _matrix.multiply_mkl(mat1, mat2)

        self.assertEqual(naive_mat, mkl_mat)

if __name__ == '__main__':
    unittest.main(verbosity=2)
