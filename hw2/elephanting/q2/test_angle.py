import pytest
import math
from Vector import Vector, angle

def test_two_zero_vec():
    v1 = Vector(0, 0)
    v2 = Vector(0, 0)
    assert v1.len() != 0 and v2.len() != 0, 'invalid input.'

def test_zero_angle():
    v1 = Vector(1, 0)
    v2 = v1
    assert angle(v1, v2) == 0

def test_90_degree():
    v1 = Vector(1, 0)
    v2 = Vector(0, 1)
    assert str(angle(v1, v2))[:8] == str(math.pi / 2)[:8]

def test_other_angle():
    v1 = Vector(5, 12)
    v2 = Vector(3, 4)
    assert str(angle(v1, v2))[:8] == str(math.acos(63 / (13 * 5)))[:8]

v3 = Vector(3, 5)
print(v3.len())