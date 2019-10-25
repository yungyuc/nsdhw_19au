import pybind11_angle as pa
import cmath
import unittest

class AngleTestCase(unittest.TestCase):
    def test_invalid_input(self):
        result = pa.angle((0,0),(0,0))
        self.assertEqual(result,500)

    def test_zero_angle_int(self):
        result = pa.angle((1,1),(1,1))
        #print('A test for zero angle\n')
        self.assertEqual(result,0.0)

    def test_90_angle_int(self):
        result = pa.angle((1,0),(0,1))
        #print('A test for 90 degree\n')
        self.assertEqual(result,90.0)

    def test_45_angle_int(self):
        result = pa.angle((1,0),(1,1))
        #print('A test for 45 degree\n')
        self.assertEqual(result,45.0)

if __name__ == '__main__':
    print("This is a program to test C++ module")

    unittest.main()
