#!/bin/env python3.6
import unittest
import random

import numpy as np

from vec2 import vec2

"""
python implementation of vector computation
"""

import math

def dotproduct(v1, v2):
  return sum((a*b) for a, b in zip(v1, v2))

def length(v):
  return math.sqrt(dotproduct(v, v))

def angle(v1, v2):
  return math.acos(dotproduct(v1, v2) / (length(v1) * length(v2)))

class VecTC(unittest.TestCase):

    def test_construction(self):

        with self.assertRaisesRegex(
            RuntimeError,
            "cannot not calculate angle of zero length vec2",
        ):
            vec2(0, 0).angle(vec2(12, 3))
            vec2(12, 3).angle(vec2(0, 0))
            vec2(0, 0).angle(vec2(0, 0))

    def test_xcoord(self):
        pass
        #nx = (self.grid10.ncelm + self.grid10.BOUND_COUNT)*2 + 1
        #golden_x = np.arange(0.0, 10.1, 0.5)
        #golden_front = golden_x[0] - golden_x[self.grid10.BOUND_COUNT:0:-1]
        #golden_back = golden_x[-1] - golden_x[-2:-self.grid10.BOUND_COUNT-2:-1]
        #golden_back += golden_x[-1]
        #golden_x = np.hstack([golden_front, golden_x, golden_back])

        #self.assertEqual(nx, len(self.grid10.xcoord))
        #self.assertEqual(golden_x.tolist(), self.grid10.xcoord.tolist())
        #self.grid10.xcoord.fill(10)
        #self.assertEqual([10]*nx, self.grid10.xcoord.tolist())

    def test_number(self):

        """test constructor without any paramemter"""

        v = vec2()
        self.assertEqual(v.x(), 0)
        self.assertEqual(v.y(), 0)

        """test constructor with initial x and y value"""

        x, y = (random.random(), random.random())
        w = vec2(x, y)
        self.assertEqual(w.x(), x)
        self.assertEqual(w.y(), y)

        """test setter"""

        x, y = (random.random(), random.random())
        v.x(x), v.y(y)
        self.assertEqual(v.x(), x)
        self.assertEqual(v.y(), y)

        """test copy"""
        m = v.copy()
        self.assertEqual(m, v)
        self.assertNotEqual(repr(w), repr(x))

        """test dot"""
        self.assertEqual(w.dot(v), dotproduct([w.x(), w.y()], [v.x(), v.y()]))

        """test length"""
        self.assertEqual(w.length(), length([w.x(), w.y()]))

        """test angle"""
        self.assertEqual(
                angle([v.x(), v.y()], [w.x(), w.y()]),
                v.angle(w))


if __name__ == '__main__':
    unittest.main()
