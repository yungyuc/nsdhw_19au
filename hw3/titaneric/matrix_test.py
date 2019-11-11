import unittest

import _matrix

def len_to_2d_index(length, ncol):
    irow, icol = 0, 0
    for _ in range(length):
        yield (irow, icol)
        icol += 1
        if icol == ncol:
            icol = 0
            irow += 1

class TestMatrix(unittest.TestCase):

    def test_square_matrix_attribute(self):
        nrow, ncol = 2, 2
        A = _matrix.Matrix(nrow, ncol)
        self.assertEqual(A.ncol, ncol)
        self.assertEqual(A.nrow, nrow)
    
    def test_nonsquare_matrix_attribute(self):
        nrow, ncol = 2, 4
        A = _matrix.Matrix(nrow, ncol)
        self.assertEqual(A.ncol, ncol)
        self.assertEqual(A.nrow, nrow)
    
    def test_square_matrix_content(self):
        nrow, ncol = 2, 2
        content = [1, 0, 0, 1]
        A = _matrix.Matrix(nrow, ncol, content)
        for (row, col), i in zip(len_to_2d_index(len(content), ncol), range(len(content))):
            self.assertEqual(A[row, col], content[i])

    def test_nonsquare_matrix_content(self):
        nrow, ncol = 2, 3
        content = [1, 0, 0, 1, 6, 7]
        A = _matrix.Matrix(nrow, ncol, content)
        for (row, col), i in zip(len_to_2d_index(len(content), ncol), range(len(content))):
            self.assertEqual(A[row, col], content[i])

    def test_naive_square_MM(self):
        A_nrow, A_ncol = 2, 2
        A_content = [1, 0, 0, 1]
        A = _matrix.Matrix(A_nrow, A_ncol, A_content)

        B_nrow, B_ncol = 2, 2
        B_content = [1, 2, 3, 4]
        B = _matrix.Matrix(B_nrow, B_ncol, B_content)

        C = _matrix.multiply_naive(A, B)
        expected_result = [1, 2, 3, 4]
        self.assertEqual(C.nrow, A_nrow)
        self.assertEqual(C.ncol, B_ncol)
        for (row, col), i in zip(len_to_2d_index(len(expected_result), B_ncol), range(len(expected_result))):
            self.assertEqual(C[row, col], expected_result[i])

    def test_naive_nonsquare_MM(self):
        A_nrow, A_ncol = 2, 2
        A_content = [1, 2, 3, 4]
        A = _matrix.Matrix(A_nrow, A_ncol, A_content)

        B_nrow, B_ncol = 2, 3
        B_content = [1, 2, 3, 4, 5, 6]
        B = _matrix.Matrix(B_nrow, B_ncol, B_content)

        C = _matrix.multiply_naive(A, B)
        expected_result = [9, 12, 15, 19, 26, 33]
        self.assertEqual(C.nrow, A_nrow)
        self.assertEqual(C.ncol, B_ncol)
        for (row, col), i in zip(len_to_2d_index(len(expected_result), B_ncol), range(len(expected_result))):
            self.assertEqual(C[row, col], expected_result[i])

    def test_MKL_square_MM(self):
        A_nrow, A_ncol = 2, 2
        A_content = [1, 0, 0, 1]
        A = _matrix.Matrix(A_nrow, A_ncol, A_content)

        B_nrow, B_ncol = 2, 2
        B_content = [1, 2, 3, 4]
        B = _matrix.Matrix(B_nrow, B_ncol, B_content)

        C = _matrix.multiply_mkl(A, B)
        expected_result = [1, 2, 3, 4]
        self.assertEqual(C.nrow, A_nrow)
        self.assertEqual(C.ncol, B_ncol)
        for (row, col), i in zip(len_to_2d_index(len(expected_result), B_ncol), range(len(expected_result))):
            self.assertEqual(C[row, col], expected_result[i])

    def test_MKL_nonsquare_MM(self):
        A_nrow, A_ncol = 2, 2
        A_content = [1, 2, 3, 4]
        A = _matrix.Matrix(A_nrow, A_ncol, A_content)

        B_nrow, B_ncol = 2, 3
        B_content = [1, 2, 3, 4, 5, 6]
        B = _matrix.Matrix(B_nrow, B_ncol, B_content)

        C = _matrix.multiply_mkl(A, B)
        expected_result = [9, 12, 15, 19, 26, 33]
        self.assertEqual(C.nrow, A_nrow)
        self.assertEqual(C.ncol, B_ncol)
        for (row, col), i in zip(len_to_2d_index(len(expected_result), B_ncol), range(len(expected_result))):
            self.assertEqual(C[row, col], expected_result[i])

    def test_MM_equal_attributes(self):
        A_nrow, A_ncol = 2, 2
        A_content = [1, 0, 0, 1]
        A = _matrix.Matrix(A_nrow, A_ncol, A_content)

        B_nrow, B_ncol = 2, 2
        B_content = [1, 2, 3, 4]
        B = _matrix.Matrix(B_nrow, B_ncol, B_content)

        MKL_C = _matrix.multiply_mkl(A, B)
        naive_C = _matrix.multiply_naive(A, B)

        self.assertEqual(MKL_C.nrow, naive_C.nrow)
        self.assertEqual(MKL_C.ncol, naive_C.ncol)

    def test_MM_equal_content(self):
        A_nrow, A_ncol = 2, 2
        A_content = [1, 0, 0, 1]
        A = _matrix.Matrix(A_nrow, A_ncol, A_content)

        B_nrow, B_ncol = 2, 2
        B_content = [1, 2, 3, 4]
        B = _matrix.Matrix(B_nrow, B_ncol, B_content)

        expected_result = [1, 2, 3, 4]
        MKL_C = _matrix.multiply_mkl(A, B)
        naive_C = _matrix.multiply_naive(A, B)

        for (row, col) in len_to_2d_index(len(expected_result), A_ncol):
            self.assertEqual(MKL_C[row, col], naive_C[row, col])
        



if __name__ == "__main__":
    unittest.main()