import unittest
import math
from question_2 import angle
import numpy as np

epsilon=1e-6

class TestAngle(unittest.TestCase):
    
    def test_zero_length(self):
        v1=(1,0)
        v2=(0,0)
        self.assertTrue(np.isnan(angle(v1, v2)))
    
    def test_zero_angle(self):
        v1=(1, 0)
        v2=(1, 0)
        self.assertTrue(angle(v1, v2)==0)

    def test_ninety_angle(self):
        v1=(1, 0)
        v2=(0, 1)
        self.assertTrue((angle(v1, v2)-np.pi/2)==0)

    def test_right_angle(self):
        v1=(1, 0)
        v2=(math.sqrt(3), 1)
        self.assertTrue((angle(v1, v2)-np.pi/6)<0)

if  __name__=='__main__':
    unittest.main()
