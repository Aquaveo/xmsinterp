"""Test InterpLinear_py.cpp."""
import math
import unittest

from xms import interp
from xms.core.misc import Observer

from xms.interp.interpolate import InterpLinear


class MockObserver(Observer):
    """Mock Observer class for testing."""

    def __init__(self):
        """Constructor."""
        self.status = {
            'operation': None,
            'operation_end': False,
            'operation_begin': False,
            'percent_complete': None,
            'message': '',
            'remaining_seconds': None,
            'elapsed_seconds': None
        }
        super(MockObserver, self).__init__()

    def __str__(self):
        """Returns a string representation of the mock observer."""
        return str(self.status)

    def on_progress_status(self, percent_complete):
        """Update progress status.

        Args:
            percent_complete (float): The percent of the operation completed
        """
        self.status['percent_complete'] = percent_complete

    def on_begin_operation_string(self, operation):
        """Called when an operation begins.

        Args:
            operation (str): Name of the operation
        """
        self.status['operation_begin'] = True
        self.status['operation'] = operation

    def on_end_operation(self):
        """Called when an operation ends."""
        self.status['operation_end'] = True

    def on_update_message(self, message):
        """Update the progress message.

        Args:
            message (str): The new progress message
        """
        self.status['message'] = message

    def time_remaining_in_seconds(self, remaining_seconds):
        """Update the remaining time in seconds.

        Args:
            remaining_seconds (float): The remaining time in seconds
        """
        self.status['remaining_seconds'] = remaining_seconds

    def time_elapsed_in_seconds(self, elapsed_seconds):
        """Update the elapsed time in seconds.

        Args:
            elapsed_seconds (float): The elapsed time in seconds
        """
        self.status['elapsed_seconds'] = elapsed_seconds


class TestInterpLinear(unittest.TestCase):
    """Test Linear Interpolation Class."""

    def setUp(self):
        """Set up for each test case."""
        pts = ((0, 0, 0), (1, 1, 0), (1, 0, 0))
        self.interp_linear_obj = interp.interpolate.InterpLinear(pts)

    def test_set_pts_tris(self):
        """Set base points."""
        # Test that the a proper call does not throw
        pts = ((0, 0, 0), (1, 1, 0), (1, 0, 0))
        tris = (0, 2, 1)
        interper = InterpLinear(pts, tris)
        base_string = '<InterpLinear - Point Count: 3, Triangle Count: 1>'
        rstr = repr(interper)
        self.assertEqual(base_string, rstr)

    def test_set_pts_tris_numpy(self):
        """Set base points."""
        import numpy as np

        # Test that the a proper call does not throw
        pts = np.array([(0, 0, 0), (1, 1, 0), (1, 0, 0)])
        tris = np.array([0, 2, 1])
        base_string = '<InterpLinear - Point Count: 3, Triangle Count: 1>'
        interper = InterpLinear(pts, tris)
        rstr = repr(interper)
        self.assertEqual(base_string, rstr)

    def test_interpolate_to_point(self):
        """Interpolate to a specific point."""
        interp = self.interp_linear_obj
        val = interp.interpolate_to_point((1, 2, 3))
        math.isnan(val)

    def test_intperp_to_pts(self):
        """Interpolate to multiple points."""
        pts = ((0, 0, 0), (10, 0, 1), (10, 10, 2), (0, 10, 3))
        tris = (0, 1, 3, 1, 2, 3)
        interp_ = InterpLinear(pts, tris)
        ret = interp_.interpolate_to_points(((2, 1, 0), (5, 10, 2.5)))
        self.assertIsInstance(ret, tuple)
        self.assertEqual((0.5, 2.5), ret)

    def test_intperp_to_pts_numpy(self):
        """Interpolate to multiple points."""
        import numpy as np
        pts = np.array([(0, 0, 0), (10, 0, 1), (10, 10, 2), (0, 10, 3)])
        tris = np.array([0, 1, 3, 1, 2, 3])
        interp_ = InterpLinear(pts, tris)
        ret = interp_.interpolate_to_points(np.array([(2, 1, 0), (5, 10, 2.5)]))
        self.assertIsInstance(ret, np.ndarray)
        np.testing.assert_array_equal(np.array([0.5, 2.5]), ret)

    def test_set_pt_activity(self):
        """Setting point activity."""
        pts = ((0, 0, 0), (10, 0, 1), (10, 10, 2), (0, 10, 3))
        tris = (0, 1, 3, 1, 2, 3)
        interper = InterpLinear(pts, tris)
        act1 = (True, False, False, True)
        interper.point_activity = act1

        act2 = (0, 1, 0, 1)
        interper.point_activity = act2

    def test_set_triangle_activity(self):
        """Setting tri activity."""
        pts = ((0, 0, 0), (10, 0, 1), (10, 10, 2), (0, 10, 3))
        tris = (0, 1, 3, 1, 2, 3)
        interper = InterpLinear(pts, tris)
        act1 = (True, False)
        interper.triangle_activity = act1

        act2 = (1, 1)
        interper.triangle_activity = act2

    def test_get_pts(self):
        """Getting interp object points."""
        import numpy as np
        pts = np.array([(0, 0, 0), (10, 0, 1), (10, 10, 2), (0, 10, 3)])
        tris = np.array([0, 1, 3, 1, 2, 3])
        interper = InterpLinear(pts, tris)

        ret = interper.points
        np.testing.assert_array_equal(pts, ret)

    def test_get_tris(self):
        """Getting interp object points."""
        import numpy as np
        pts = np.array([(0, 0, 0), (10, 0, 1), (10, 10, 2), (0, 10, 3)])
        tris = np.array([0, 1, 3, 1, 2, 3])
        interper = InterpLinear(pts, tris)

        ret = interper.triangles
        np.testing.assert_array_equal(tris, ret)

    def test_tri_containing_pt(self):
        """Getting tri containing point."""
        pts = ((0, 0, 0), (10, 0, 1), (10, 10, 2), (0, 10, 3))
        tris = (0, 1, 3, 1, 2, 3)
        interper = InterpLinear(pts, tris)
        ret = interper.triangle_containing_point((5, 5, 2))
        self.assertEqual(0, ret)

    def test_tri_envelops_containing_pt(self):
        """Getting tri containing point."""
        import numpy as np
        pts = ((0, 0, 0), (10, 0, 1), (10, 10, 2), (0, 10, 3))
        tris = (0, 1, 3, 1, 2, 3)

        interper = InterpLinear(pts, tris)
        ret = interper.triangle_envelopes_containing_point((5, 5, 2))
        np.testing.assert_array_equal(np.array([0, 3], np.int32), ret)

    def test_interp_weights(self):
        """Test interp_weights."""
        import numpy as np
        pts = ((0, 0, 0), (10, 0, 1), (10, 10, 2), (0, 10, 3))
        tris = (0, 1, 3, 1, 2, 3)
        interper = InterpLinear(pts, tris)

        pt_inside, idxs, wts = interper.interpolate_weights((5, 5, 2))
        self.assertTrue(pt_inside)
        np.testing.assert_array_equal(np.array([0, 1, 3], np.int32), idxs)
        np.testing.assert_array_equal(np.array([0, 0.5, 0.5]), wts)

        pt_inside, idxs, wts = interper.interpolate_weights((-10, -10, -10))
        self.assertFalse(pt_inside)
        np.testing.assert_array_equal(np.array([], np.int32), idxs)
        np.testing.assert_array_equal(np.array([]), wts)

    def test_set_extrap_val(self):
        """Test set_extrap_val."""
        interper = self.interp_linear_obj
        extrap_val = 7.11
        interper.extrapolation_value = extrap_val
        base_string = '<InterpLinear - Point Count: 3, Triangle Count: 1>'
        rstr = repr(self.interp_linear_obj)
        self.assertEqual(base_string, rstr)

    def test_set_trunc(self):
        """Test set_trunc."""
        interper = self.interp_linear_obj
        t_min = 7.11
        t_max = 11.7
        interper.set_truncation(t_max, t_min)
        base_string = '<InterpLinear - Point Count: 3, Triangle Count: 1>'
        rstr = repr(self.interp_linear_obj)
        self.assertEqual(base_string, rstr)

    def test_set_use_clough_tocher(self):
        """Test set_use_clough_tocher."""
        interper = self.interp_linear_obj
        observer = MockObserver()
        pts = ((0, 0, 0), (10, 0, 1), (10, 10, 2), (0, 10, 3))
        tris = (0, 1, 3, 1, 2, 3)
        interper = InterpLinear(pts, tris)
        interper.set_use_clough_tocher(True, observer)
        val = interper.interpolate_to_point((2, 1, 0))
        self.assertAlmostEqual(0.2574999928474426, val, places=9)

    def test_set_use_nat_neigh(self):
        """Test set_use_nat_neigh."""
        observer = MockObserver()
        pts = ((0, 0, 0), (10, 0, 1), (10, 10, 2), (0, 10, 3))
        tris = (0, 1, 3, 1, 2, 3)
        interper = InterpLinear(pts, tris)

        nodal_func = "constant"
        nodal_func_opts = "nearest_points"
        n_nearest = 12
        blend_weights = False
        interper.set_use_natural_neighbor(True, nodal_func, nodal_func_opts, n_nearest, blend_weights, observer)

        val = interper.interpolate_to_point((2, 1, 0))
        self.assertAlmostEqual(0.4600000, val, places=7)

    def test_tutorial(self):
        """Ensure the tutorial will work."""
        pts = ((0, 0, 0), (10, 0, 1), (10, 10, 2), (0, 10, 3))
        tris = (0, 1, 3, 1, 2, 3)
        interper = InterpLinear(pts, tris)
        val = interper.interpolate_to_point((2, 1, 0))
        self.assertEqual(0.5, val)


if __name__ == '__main__':
    unittest.main()
