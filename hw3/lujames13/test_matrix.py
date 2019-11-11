import unittest
import _matrix

class TestMatrix(unittest.TestCase):

    def make_matrices(self, size_n, size_m, size_k):

        mat1 = _matrix.Matrix(size_n,size_k)
        mat2 = _matrix.Matrix(size_k,size_m)
        mat3 = _matrix.Matrix(size_n,size_m)

        for it in range(size_n):
            for jt in range(size_k):
                mat1[it, jt] = it * size_n + jt + 1

        for it in range(size_k):
            for jt in range(size_m):
                mat2[it, jt] = it * size_k + jt + 1

        for it in range(size_n):
            for jt in range(size_m):
                mat3[it, jt] = 0

        return mat1, mat2, mat3

    def test_basic(self):

        size = 100
        mat1, mat2, mat3, *_ = self.make_matrices(size, size, size)

        self.assertEqual(size, mat1.nrow)
        self.assertEqual(size, mat1.ncol)
        self.assertEqual(size, mat2.nrow)
        self.assertEqual(size, mat2.ncol)
        self.assertEqual(size, mat3.nrow)
        self.assertEqual(size, mat3.ncol)

        self.assertEqual(2, mat1[0,1])
        self.assertEqual(size+2, mat1[1,1])
        self.assertEqual(size*2, mat1[1,size-1])
        self.assertEqual(size*size, mat1[size-1,size-1])

        for i in range(mat1.nrow):
            for j in range(mat1.ncol):
                self.assertNotEqual(0, mat1[i,j])
                self.assertEqual(mat1[i,j], mat2[i,j])
                self.assertEqual(0, mat3[i,j])

        self.assertEqual(mat1, mat2)
        self.assertTrue(mat1 is not mat2)

    def test_basic_100_200_100(self):

        size_n = 100
        size_m = 100
        size_k = 100
        mat1, mat2, mat3, *_ = self.make_matrices(size_n, size_m, size_k)

        self.assertEqual(size_n, mat1.nrow)
        self.assertEqual(size_k, mat1.ncol)
        self.assertEqual(size_k, mat2.nrow)
        self.assertEqual(size_m, mat2.ncol)
        self.assertEqual(size_n, mat3.nrow)
        self.assertEqual(size_m, mat3.ncol)

        self.assertEqual(2, mat1[0,1])
        self.assertEqual(size_k+2, mat1[1,1])
        self.assertEqual(size_k*2, mat1[1,size_k-1])
        self.assertEqual(size_n*size_k, mat1[size_n-1,size_k-1])

        for i in range(mat1.nrow):
            for j in range(mat1.ncol):
                self.assertNotEqual(0, mat1[i,j])
        for i in range(mat3.nrow):
            for j in range(mat3.ncol):
               self.assertEqual(0, mat3[i,j])

    def test_match(self):

        size = 100
        mat1, mat2, *_ = self.make_matrices(size, size, size)

        ret_naive = _matrix.multiply_naive(mat1, mat2)
        ret_mkl = _matrix.multiply_mkl(mat1, mat2)

        self.assertEqual(size, ret_naive.nrow)
        self.assertEqual(size, ret_naive.ncol)
        self.assertEqual(size, ret_mkl.nrow)
        self.assertEqual(size, ret_mkl.ncol)

        for i in range(ret_naive.nrow):
            for j in range(ret_naive.ncol):
                self.assertNotEqual(mat1[i,j], ret_mkl[i,j])
                self.assertEqual(ret_naive[i,j], ret_mkl[i,j])

    def test_match_100_200_100(self):

        size_n = 100
        size_m = 200
        size_k = 100
        mat1, mat2, *_ = self.make_matrices(size_n, size_m, size_k)

        ret_naive = _matrix.multiply_naive(mat1, mat2)
        ret_mkl = _matrix.multiply_mkl(mat1, mat2)

        self.assertEqual(size_n, ret_naive.nrow)
        self.assertEqual(size_m, ret_naive.ncol)
        self.assertEqual(size_n, ret_mkl.nrow)
        self.assertEqual(size_m, ret_mkl.ncol)

        for i in range(ret_naive.nrow):
            for j in range(ret_naive.ncol):
                self.assertNotEqual(mat1[i,j], ret_mkl[i,j])
                self.assertEqual(ret_naive[i,j], ret_mkl[i,j])

    def test_zero(self):

        size = 100
        mat1, mat2, mat3, *_ = self.make_matrices(size, size, size)

        ret_naive = _matrix.multiply_naive(mat1, mat3)
        ret_mkl = _matrix.multiply_mkl(mat1, mat3)

        self.assertEqual(size, ret_naive.nrow)
        self.assertEqual(size, ret_naive.ncol)
        self.assertEqual(size, ret_mkl.nrow)
        self.assertEqual(size, ret_mkl.ncol)

        for i in range(ret_naive.nrow):
            for j in range(ret_naive.ncol):
                self.assertEqual(0, ret_naive[i,j])
                self.assertEqual(0, ret_mkl[i,j])
