
import unittest
from unittest import TestCase
import numpy as np
from q2 import rads


class TestRads(TestCase):

    def test_zero_vector(self):
        v1 = (0, 0)
        v2 = (1, 0)

        self.assertTrue(np.isnan(rads(v1, v2)))
        self.assertTrue(np.isnan(rads(v2, v1)))

    def test_same_vector(self):
        v = (1, 1)
        self.assertEqual(rads(v, v), 0)

    def test_rads(self):
        v1 = (1, 0)
        v2 = (-1, 0)

        self.assertAlmostEqual(rads(v1, v2), np.pi, places=4)

        v1 = (0, 1)
        v2 = (1, 0)

        self.assertAlmostEqual(rads(v1, v2), np.pi / 2, places=4)

        v1 = (1, 1)
        v2 = (0, -1)

        self.assertAlmostEqual(rads(v1, v2), 2.3561944, places=4)

        v1 = (3, 1)
        v2 = (-2, -0.5)

        self.assertAlmostEqual(rads(v1, v2), 3.0648207, places=4)

if __name__=='__main__':
    unittest.main()

