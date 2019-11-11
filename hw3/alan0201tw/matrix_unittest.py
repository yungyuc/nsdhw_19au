#!/usr/bin/env python3

import unittest

# The python module that wraps the matrix code.
import _matrix

import random

class Test(unittest.TestCase):

    def test_constructor(self):
        matrix0 = _matrix.Matrix(2, 2, [1, 2, 3, 4])
        self.assertEqual(matrix0.nrow, 2)
        self.assertEqual(matrix0.ncol, 2)
        self.assertEqual(matrix0[0, 0], 1)
        self.assertEqual(matrix0[0, 1], 2)
        self.assertEqual(matrix0[1, 0], 3)
        self.assertEqual(matrix0[1, 1], 4)

    def test_item_assignment(self):
        matrix0 = _matrix.Matrix(2, 2, [1, 2, 3, 4])
        self.assertEqual(matrix0.nrow, 2)
        self.assertEqual(matrix0.ncol, 2)
        self.assertEqual(matrix0[0, 0], 1)
        self.assertEqual(matrix0[0, 1], 2)
        self.assertEqual(matrix0[1, 0], 3)
        self.assertEqual(matrix0[1, 1], 4)

        # start explicit item assignment
        matrix0[0, 0] = 123
        self.assertEqual(matrix0[0, 0], 123)
        matrix0[0, 1] = 456
        self.assertEqual(matrix0[0, 1], 456)
        matrix0[1, 0] = 789
        self.assertEqual(matrix0[1, 0], 789)
        matrix0[1, 1] = 0
        self.assertEqual(matrix0[1, 1], 0)
        

    def test_index_error(self):
        matrix0 = _matrix.Matrix(2, 2, [1, 2, 3, 4])

        # Expect index error when accessing matrix with false indices
        with self.assertRaises(IndexError):
            self.assertEqual(matrix0[1, 2], 4)
        with self.assertRaises(IndexError):
            self.assertEqual(matrix0[2, 0], 4)

        # Expect index error when assigning item with false indices
        with self.assertRaises(IndexError):
            matrix0[0, 2] = 0
        with self.assertRaises(IndexError):
            matrix0[2, 0] = 1

    def test_out_of_range(self):
        matrix0 = _matrix.Matrix(2, 2, [1, 2, 3, 4])
        matrix1 = _matrix.Matrix(3, 1, [1, 2, 3])

        # according to https://stackoverflow.com/questions/37619848/python-loop-list-index-out-of-range
        # out of range exception is mapped to index error in python
        with self.assertRaises(IndexError):
            _matrix.multiply_naive(matrix0, matrix1)
        with self.assertRaises(IndexError):
            _matrix.multiply_mkl(matrix0, matrix1)

    def test_naive_validity(self):
        matrix0 = _matrix.Matrix(2, 1, [1, 2])
        matrix1 = _matrix.Matrix(1, 3, [1, 2, 3])

        result_matrix = _matrix.multiply_naive(matrix0, matrix1)
        self.assertEqual(result_matrix, _matrix.Matrix(2, 3, [1, 2, 3, 2, 4, 6]))

    def test_mkl_validity(self):
        matrix0 = _matrix.Matrix(2, 1, [1, 2])
        matrix1 = _matrix.Matrix(1, 3, [1, 2, 3])

        result_matrix = _matrix.multiply_mkl(matrix0, matrix1)
        self.assertEqual(result_matrix, _matrix.Matrix(2, 3, [1, 2, 3, 2, 4, 6]))

    def test_naive_mkl_identification(self):
        matrix0 = _matrix.Matrix(2, 1, [1, 2])
        matrix1 = _matrix.Matrix(1, 3, [1, 2, 3])

        result_matrix = _matrix.multiply_mkl(matrix0, matrix1)
        self.assertEqual(result_matrix, _matrix.Matrix(2, 3, [1, 2, 3, 2, 4, 6]))

    # This pressure test might take a while to run
    def test_naive_mkl_identification_pressure(self):
        # Reference : https://www.geeksforgeeks.org/generating-random-number-list-in-python/
        matrix0 = _matrix.Matrix(1024, 2048, [random.randrange(1, 50, 1) for i in range(1024 * 2048)])
        matrix1 = _matrix.Matrix(2048, 512, [random.randrange(1, 50, 1) for i in range(2048 * 512)])

        result_matrix_naive = _matrix.multiply_naive(matrix0, matrix1)
        result_matrix_mkl = _matrix.multiply_mkl(matrix0, matrix1)
        self.assertEqual(result_matrix_naive, result_matrix_mkl)
