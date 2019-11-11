import unittest


class MatrixTest(unittest.TestCase):
    def setUp(self):
        import _matrix
        self.matrix = _matrix.Matrix
        self.multiply = {
            'naive': _matrix.multiply_naive,
            'mkl': _matrix.multiply_mkl
        }

    def test_basic(self):
        shape = [(1, 1), (2, 3), (30, 20), (1000, 1000)]
        for row, col in shape:
            m = self.matrix(row, col)
            self.assertEqual(m.nrow, row)
            self.assertEqual(m.ncol, col)

    def test_equal(self):
        shape = [(1, 1), (2, 3), (30, 20), (1000, 1000)]
        for row, col in shape:
            m1 = self.matrix(row, col)
            m2 = self.matrix(row, col)
            self.assertEqual(m1, m2)

    def identity_test(self, lmul, rmul):
        shape = [1, 2, 30, 100]
        for s in shape:
            # idm: identity matrix
            idm = self.matrix(s, s)
            for i in range(s):
                for j in range(s):
                    idm[i, j] = 0 if i != j else 1
            # m: arbitrary matrix
            m = self.matrix(s, s)
            for i in range(s):
                for j in range(s):
                    idm[i, j] = i + j
            self.assertEqual(lmul(idm, m), rmul(idm, m))
            self.assertEqual(lmul(m, idm), rmul(m, idm))
            self.assertEqual(lmul(idm, m), lmul(m, idm))
            self.assertEqual(rmul(idm, m), rmul(m, idm))

    def small_test(self, lmul, rmul):
        # ma: arbitrary matrix
        ma = self.matrix(3, 2)
        ma[0, 0] = 0
        ma[0, 1] = -1
        ma[1, 0] = 1
        ma[1, 1] = 0
        ma[2, 0] = 1
        ma[2, 1] = 1
        # mb: arbitrary matrix
        mb = self.matrix(2, 2)
        mb[0, 0] = 0
        mb[0, 1] = 1
        mb[1, 0] = -1
        mb[1, 1] = 0
        # mc == ma * mb
        mc = self.matrix(3, 2)
        mc[0, 0] = 1
        mc[0, 1] = 0
        mc[1, 0] = 0
        mc[1, 1] = 1
        mc[2, 0] = -1
        mc[2, 1] = 1
        # ma * mb == idm
        self.assertEqual(lmul(ma, mb), mc)
        self.assertEqual(rmul(ma, mb), mc)
        self.assertEqual(lmul(ma, mb), rmul(ma, mb))

    def test_multiply_naive(self):
        muls = [self.multiply[x] for x in ('naive', ) * 2]
        self.identity_test(*muls)
        self.small_test(*muls)

    def test_multiply_mkl(self):
        muls = [self.multiply[x] for x in ('mkl', ) * 2]
        self.identity_test(*muls)
        self.small_test(*muls)

    def test_multiply_match(self):
        muls = [self.multiply[x] for x in ('naive', 'mkl')]
        self.identity_test(*muls)
        self.small_test(*muls)


if __name__ == '__main__':
    unittest.main()
