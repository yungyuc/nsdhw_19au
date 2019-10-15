import numpy as np
from vector import *

# import nose for unit-test
from nose import *

c_PI = np.pi
epsilon = np.finfo(np.float64).eps

################################################
##
##  Setting up
##
################################################

def setup_module(module):
    print ("") # this is to get a newline after the dots
    print ("========================================================================")
    print ("NOSE_SETUP : Setting up nosetest environment...")
    print ("NOSE_SETUP : using constant PI from numpy, PI = ", c_PI)
    print ("NOSE_SETUP : using constant epsilon for type 'double' from numpy, epsilon = ", epsilon)
    print ("========================================================================")

################################################
##
##  Helper functions
##
################################################

def expect_difference_within_epsolon(a, b):
    return ( np.abs(a - b) ) <= epsilon

################################################
##
##  Unit tests
##
################################################

def test_zero_to_zero():
    assert np.isnan(compute_angle((0, 0), (0, 0)))

def test_unitX_to_unitNegX():
    assert expect_difference_within_epsolon(
        compute_angle((1, 0), (-1, 0)),
        c_PI)

def test_unitX_to_unitX():
    assert expect_difference_within_epsolon(
        compute_angle((1, 0), (1, 0)),
        0)

def test_unitX_to_unitY():
    assert expect_difference_within_epsolon(
        compute_angle((1, 0), (0, 1)),
        c_PI / 2)

def test_unitX_to_unitXY():
    assert expect_difference_within_epsolon(
        compute_angle((1, 0), (1, 1)),
        c_PI / 4)

def test_unitXY_to_unitNegXY():
    assert expect_difference_within_epsolon(
        compute_angle((1, 1), (-1, -1)),
        c_PI)

def test_identical_vector():
    assert expect_difference_within_epsolon(
        compute_angle((0.78221, 1.12313231), (0.78221, 1.12313231)),
        0)

def test_identical_vector_to_neg():
    assert expect_difference_within_epsolon(
        compute_angle((0.78221, 1.12313231), (-0.78221, -1.12313231)),
        c_PI)

def test_random_test_case():
    assert expect_difference_within_epsolon(
        compute_angle((3, 4), (-4, 3)),
        c_PI / 2)