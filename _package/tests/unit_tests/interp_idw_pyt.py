"""Test InterpIdw_py.cpp."""
import unittest

import numpy as np

from xms.core.misc import Observer

from xms.interp.interpolate import InterpIdw


class MockObserver(Observer):
    """Mock Observer class for testing."""
    def __init__(self, obs_id="X"):
        """Constructor.

        Args:
            obs_id (str): Id of the observer
        """
        self.status = {
            'operation': None,
            'operation_end': False,
            'operation_begin': False,
            'percent_complete': None,
            'message': '',
            'remaining_seconds': None,
            'elapsed_seconds': None,
            'obs_id': obs_id
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


class TestInterpIdw(unittest.TestCase):
    """Test IDW Interpolation Class."""

    def setUp(self):
        """Set up for each test case."""
        pts = ((1, 2, 3), (1, 2, 3), (1, 2, 3))
        self.interp_idw_obj = InterpIdw(pts)

    def test_set_pts(self):
        """Set base points."""
        interp = self.interp_idw_obj
        # Test that the a proper call does not throw
        interp.set_points(((1, 2, 3), (1, 2, 3), (3, 3, 3)), False)

    def test_set_pts_2d(self):
        """Set base points."""
        interp = self.interp_idw_obj
        # Test that the a proper call does not throw
        interp.set_points(((1, 2, 3), (1, 2, 3), (3, 3, 3)), True)

    def test_interp_to_pt(self):
        """Interpolate to a specific point."""
        interp = self.interp_idw_obj
        val = interp.interpolate_to_point((1, 2, 3))
        self.assertEqual(3.0, val)

    def test_interp_to_pts(self):
        """Interpolate to multiple points."""
        pts = ((0, 0, 0), (10, 0, 1), (10, 10, 2), (0, 10, 3))
        interp = InterpIdw(pts)
        ret = interp.interpolate_to_points(np.array([(2.0, 1.0, 0.0), (5.0, 10.0, 2.5)]))
        np.testing.assert_array_almost_equal((0.02681550197303295, 2.5), ret)

    def test_interp_to_pts_numpy(self):
        """Interpolate to multiple points."""
        pts = np.array([(0, 0, 0), (10, 0, 1), (10, 10, 2), (0, 10, 3)])
        interp = InterpIdw(pts)
        ret = interp.interpolate_to_points(np.array(((2.0, 1.0, 0.0), (5.0, 10.0, 2.5))))
        self.assertIsInstance(ret, np.ndarray)
        np.testing.assert_array_equal(np.array([0.02681550197303295, 2.5]), ret)

    def test_set_pt_activity(self):
        """Setting point activity."""
        pts = ((0, 0, 0), (10, 0, 1), (10, 10, 2), (0, 10, 3))
        tris = (0, 1, 3, 1, 2, 3)
        interp = InterpIdw(pts, tris)
        act1 = (True, False, False, True)
        interp.point_activity = act1

        act2 = (0, 1, 0, 1)
        interp.point_activity = act2

    # IDW doesn't use triangles
    # def test_triangle_activity(self):
    #     """Setting tri activity."""
    #     pts = ((0, 0, 0), (10, 0, 1), (10, 10, 2), (0, 10, 3))
    #     tris = (0, 1, 3, 1, 2, 3)
    #     interp = InterpIdw(pts, tris)
    #     act1 = (True, False)
    #     interp.triangle_activity = act1
    #
    #     act2 = (1, 1)
    #     interp.triangle_activity = act2

    def test_get_pts(self):
        """Getting interp object points."""
        pts = np.array([(0, 0, 0), (10, 0, 1), (10, 10, 2), (0, 10, 3)])
        tris = np.array([0, 1, 3, 1, 2, 3])
        interp = InterpIdw(pts, tris)

        ret = interp.points
        np.testing.assert_array_equal(pts, ret)

    # IDW doesn't use triangles
    # def test_get_tris(self):
    #     """Getting interp object points."""
    #     pts = np.array([(0, 0, 0), (10, 0, 1), (10, 10, 2), (0, 10, 3)])
    #     tris = np.array([0, 1, 3, 1, 2, 3])
    #     interp = InterpIdw(pts, tris)
    #
    #     ret = interp.triangles
    #     np.testing.assert_array_equal(tris, ret)

    def test_set_trunc(self):
        """Test set_trunc."""
        t_min = 7.11
        t_max = 11.7

        pts = np.array([(0, 0, 0), (10, 0, 1), (10, 10, 2), (0, 10, 3)])
        tris = np.array([0, 1, 3, 1, 2, 3])
        interp = InterpIdw(pts, tris)

        interp.set_truncation(t_max, t_min)
        self.assertEquals(t_min, interp.truncate_min)
        self.assertEquals(t_max, interp.truncate_max)

    def test_set_observer(self):
        """Test set_observer."""
        observer1 = MockObserver("Obs1")
        observer2 = MockObserver("Obs2")

        pts = ((0, 0, 0), (10, 0, 1), (10, 10, 2), (0, 10, 3))
        tris = (0, 1, 3, 1, 2, 3)
        interp = InterpIdw(pts, tris)

        # Non-Observer Type
        with self.assertRaises(TypeError) as context:
            interp.set_observer("xyz")
        err = context.exception
        self.assertIn("SetObserver(): incompatible function arguments.", str(err))

        interp.set_observer(observer1)
        interp.interpolate_to_points(np.random.rand(100000, 3) + 5)
        self.assertGreater(observer1.status['percent_complete'], 0.0)
        self.assertGreater(observer1.status['elapsed_seconds'], 0.0)

        interp.set_observer(observer2)
        interp.interpolate_to_points(np.random.rand(100000, 3) + 5)
        self.assertGreater(observer2.status['percent_complete'], 0.0)
        self.assertGreater(observer2.status['elapsed_seconds'], 0.0)

    def test_set_power(self):
        """Set power on InterpIdw objects."""
        interp = self.interp_idw_obj

        # None Type
        with self.assertRaises(TypeError) as context:
            interp.power = None
        err = context.exception
        self.assertIn("SetPower(): incompatible function arguments.", str(err))

        # Non-number Type
        with self.assertRaises(TypeError) as context:
            interp.power = "xyz"
        err = context.exception
        self.assertIn("SetPower(): incompatible function arguments.", str(err))

        # Good arguments
        power = 10
        interp.power = power
        self.assertEquals(power, interp.power)

    def test_set_search_opts(self):
        """Ensure the tutorial will work."""
        interp = self.interp_idw_obj
        typeerror = "SetSearchOpts(): incompatible function arguments."

        # No Arguments
        with self.assertRaises(TypeError) as context:
            interp.set_search_options()
        err = context.exception

        # One Arguments
        with self.assertRaises(TypeError) as context:
            interp.set_search_options(123)
        err = context.exception

        # None Arguments
        with self.assertRaises(TypeError) as context:
            interp.set_search_options(None, None)
        err = context.exception
        self.assertIn(typeerror, str(err))

        # Bad Argument
        with self.assertRaises(TypeError) as context:
            interp.set_search_options("123", False)
        err = context.exception
        self.assertIn(typeerror, str(err))

        # Good Arguments
        interp.set_search_options(10, True)

        interp.set_search_options(15, False)

    def test_set_weight_calc_method(self):
        """Setting weight calc method with enum."""
        interp = self.interp_idw_obj

        # None Argument
        with self.assertRaises(ValueError) as context:
            interp.weight_calculation_method = None
        err = context.exception
        none_value_error = '"weights" must be one of {}, not None'.format(', '.join(InterpIdw.weights))
        self.assertIn(none_value_error, str(err))

        # Bad Arguments
        with self.assertRaises(ValueError) as context:
            interp.weight_calculation_method = "bad_string"
        err = context.exception
        err_str = '"weights" must be one of {}, not bad_string'.format(', '.join(InterpIdw.weights))
        self.assertIn(err_str, str(err))

        with self.assertRaises(ValueError) as context:
            interp.weight_calculation_method = 123
        err = context.exception
        err_str = '"weights" must be one of {}, not 123'.format(', '.join(InterpIdw.weights))
        self.assertIn(err_str, str(err))

        # Valid Arguments

        interp.weight_calculation_method = "modified"

        interp.weight_calculation_method = "classic"

    def test_set_nodal_function(self):
        """Setting nodal function."""
        observer = MockObserver()
        typeerror = "SetNodalFunction(): incompatible function arguments."

        pts = np.array([(0, 0, 0), (10, 0, 1), (10, 10, 2), (0, 10, 3)])
        tris = np.array([0, 1, 3, 1, 2, 3])
        interp = InterpIdw(pts, tris)

        # No Argument
        interp.set_nodal_function()

        # None Args for each argument
        with self.assertRaises(ValueError) as context:
            interp.set_nodal_function(None, 1, True, observer)
        err = context.exception
        func_type_error = '"nodal_function_type" must be one of {}, not None'.format(
            ", ".join(InterpIdw.nodal_function_types))
        self.assertIn(func_type_error, str(err))
        with self.assertRaises(TypeError) as context:
            interp.set_nodal_function("constant", None, True, observer)
        err = context.exception
        self.assertIn(typeerror, str(err))

        # Bad Args
        with self.assertRaises(ValueError) as context:
            interp.set_nodal_function(1, 1, True, observer)
        err = context.exception
        func_type_error = '"nodal_function_type" must be one of {}, not 1'.format(
            ", ".join(InterpIdw.nodal_function_types))
        self.assertIn(func_type_error, str(err))
        with self.assertRaises(ValueError) as context:
            interp.set_nodal_function("abc", 1, True, observer)
        err = context.exception
        err_str = '"nodal_function_type" must be one of {}, not abc'.format(
            ", ".join(InterpIdw.nodal_function_types))
        self.assertIn(err_str, str(err))
        with self.assertRaises(TypeError) as context:
            interp.set_nodal_function("constant", "1234", True, observer)
        err = context.exception
        self.assertIn(typeerror, str(err))
        with self.assertRaises(TypeError) as context:
            interp.set_nodal_function("constant", 1.4, True, observer)
        err = context.exception
        self.assertIn(typeerror, str(err))
        with self.assertRaises(ValueError) as context:
            interp.set_nodal_function("constant", 1, True, 1)
        err_str = "observer must be of type xmscore.misc.Observer"
        err = context.exception
        self.assertIn(err_str, str(err))
        with self.assertRaises(ValueError) as context:
            interp.set_nodal_function("constant", 1, True, "abcd")
        err = context.exception
        self.assertIn(err_str, str(err))

        # Good Args
        base = str(interp)
        interp.set_nodal_function("constant", 7, True, observer)
        self.assertEqual(base, str(interp))

        interp.set_nodal_function("gradient_plane", 9, True, observer)
        # TODO: Removed to_string checks. Need another way to test this

        interp.set_nodal_function("quadratic", 11, True, observer)
        # TODO: Removed to_string checks. Need another way to test this

    def test_interpolate_weights(self):
        """Test interpolate_weights."""
        pts = ((0, 0, 0), (10, 0, 1), (10, 10, 2), (0, 10, 3))
        tris = (0, 1, 3, 1, 2, 3)
        interp = InterpIdw(pts, tris)

        idxs, wts = interp.interpolate_weights((5, 5, 2))
        np.testing.assert_array_equal(np.array([0, 1, 2, 3], np.int32), idxs)
        np.testing.assert_array_equal(np.array([0.25, 0.25, 0.25, 0.25]), wts)

        idxs, wts = interp.interpolate_weights((10, 10, 0))
        np.testing.assert_array_equal(np.array([2], np.int32), idxs)
        np.testing.assert_array_equal(np.array([1.]), wts)

        idxs, wts = interp.interpolate_weights((-10, -10, -10))
        np.testing.assert_array_equal(np.array([0, 1, 2, 3], np.int32), idxs)
        np.testing.assert_array_almost_equal(np.array([0.876919, 0.06154,
                                             0.0, 0.06154]), wts, decimal=5)

    def test_set_multi_threading(self):
        """Setting multi threading."""
        interp = self.interp_idw_obj
        interp.set_multithreading(False)
        interp.set_multithreading(True)

    def test_tutorial(self):
        """Ensure the tutorial will work."""
        pts = ((0, 0, 0), (10, 0, 1), (10, 10, 2), (0, 10, 3))
        tris = ()
        tris2 = (0, 1, 3, 1, 2, 3)

        interp = InterpIdw(pts, tris)
        val = interp.interpolate_to_point((5, 5, 0))
        self.assertEqual(1.5, val)

        interp = InterpIdw(pts, tris2)
        val2 = interp.interpolate_to_point((5, 5, 0))
        self.assertEqual(1.5, val2)


if __name__ == '__main__':
    unittest.main()
