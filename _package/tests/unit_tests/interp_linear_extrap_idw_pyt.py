"""Test InterpLinear_py.cpp."""
import unittest

from xms.interp.interpolate import InterpLinearExtrapIdw


class TestInterpLinearExtrapIdw(unittest.TestCase):
    """Test Linear Interpolation Class."""

    def setUp(self):
        """Set up for each test case."""
        pts = ((0, 0, 0), (1, 1, 0), (1, 0, 0))
        self.interp_linear_extrap_idw_obj = InterpLinearExtrapIdw(pts)

    def test_set_pts_tris(self):
        """Set base points."""
        # Test that the a proper call does not throw
        pts = ((0, 0, 0), (1, 1, 0), (1, 0, 0))
        tris = (0, 2, 1)
        interper = InterpLinearExtrapIdw(pts, tris)
        base_string = '<InterpLinearExtrapIdw - Point Count: 3, Triangle Count: 1>'
        rstr = repr(interper)
        self.assertEqual(base_string, rstr)

    def test_interpolate_weights(self):
        """Test interpolate_weights."""
        pts = ((0, 0, 0), (10, 0, 1), (10, 10, 2), (0, 10, 3))
        tris = (0, 1, 3, 1, 2, 3)
        interp = InterpLinearExtrapIdw(pts, tris)
        self.assertAlmostEqual(2.70349288, interp.interpolate_to_point((0, 25, 0)))
        self.assertAlmostEqual(2.0, interp.interpolate_to_point((5, 5, 0)))
        self.assertAlmostEqual(2.0, interp.interpolate_to_point((10, 10, 0)))
