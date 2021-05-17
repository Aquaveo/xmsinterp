"""Test InterpIdw_py.cpp."""
import unittest

import numpy as np

from xms.interp.interpolate import interp_util


class TestInterpUtil(unittest.TestCase):
    """Test InterpUtil functions."""

    def test_generate_raster_idw(self):
        """Tests generating a raster using IDW interpolation from mesh XYZD information."""
        pts = ((0.0, 0.0, 1.0), (0.0, 1.0, 2.0), (1.0, 0.0, 3.0))
        vals = interp_util.generate_raster_idw(pts, (-1.0, -1.0, 0.0), (1.0, 1.0, 0.0), 4, 4, -999.0, [])
        np.testing.assert_array_almost_equal((1.00, 2.00, 2.94, 2.94, 1.50, 2.00, 3.00, 3.00, 1.97, 2.00, 2.50, 2.87,
                                              1.97, 2.00, 2.13, 2.50), vals, 2)
        vals = interp_util.generate_raster_idw(pts, (-1.0, -1.0, 0.0), (1.0, 1.0, 0.0), 4, 4, -999.0, [0, 0, 0, 0, 1, 1,
                                                                                                       1, 1, 1, 1, 1, 1,
                                                                                                       0, 0, 0, 0])
        np.testing.assert_array_almost_equal((-999.0, -999.0, -999.0, -999.0, 1.50, 2.00, 3.00, 3.00, 1.97, 2.00, 2.50,
                                              2.87, -999.0, -999.0, -999.0, -999.0), vals, 2)


if __name__ == '__main__':
    unittest.main()
