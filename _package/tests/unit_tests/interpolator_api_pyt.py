"""Test InterpLinear_py.cpp."""
import unittest
import numpy as np

from xms import interp


class TestInterpLinear(unittest.TestCase):
    """Test Interpolator API."""
    def setUp(self):
        self.interp_pts = ((2, 1, 0), (5, 10, 2.5), (5, 5), (7, 3), (2, 7), (7, 7))

    def test_interpolate_to_points_linear(self):
        """Test linear method on api interpolate_to_points"""
        pts = ((0, 0, 0), (10, 0, 1), (10, 10, 2), (0, 10, 3))
        tris = (0, 1, 3, 1, 2, 3)
        ret = interp.interpolate_to_points(method='linear', points=self.interp_pts,
                                           interpolation_points=pts)
        np.testing.assert_array_almost_equal(np.array([0.5, 2.5, 2.0, 1.6, 2.3, 2.0]), ret, decimal=4)

    def test_interpolate_to_points_idw(self):
        """Test idw method on api interpolate_to_points"""
        pts = ((0, 0, 0), (10, 0, 1), (10, 10, 2), (0, 10, 3))
        tris = (0, 1, 3, 1, 2, 3)
        ret = interp.interpolate_to_points(method='idw', points=self.interp_pts,
                                           interpolation_points=pts)
        np.testing.assert_array_almost_equal(np.array([0.0268, 2.5, 1.5, 1.0, 2.8292, 2.0]), ret, decimal=4)

    def test_interpolate_to_points_natural_neighbor(self):
        """Test natural_neighbor method on api interpolate_to_points"""
        pts = ((0, 0, 0), (10, 0, 1), (10, 10, 2), (0, 10, 3))
        tris = (0, 1, 3, 1, 2, 3)
        ret = interp.interpolate_to_points(method='natural_neighbor', points=self.interp_pts,
                                           interpolation_points=pts)
        np.testing.assert_array_almost_equal(np.array([0.5, 2.5, 2.0, 1.6, 2.3, 2.0]), ret, decimal=4)