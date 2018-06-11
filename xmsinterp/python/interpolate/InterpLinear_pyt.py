"""Test InterpLinear_py.cpp."""
import unittest
import xmsinterp_py
import xmscore_py


class MockObserver(xmscore_py.misc.Observer):
    """Mock Observer class for testing."""

    def __init__(self):
        self.status = {
            'operation': None,
            'operation_end': False,
            'operation_begin': False,
            'percent_complete': None,
            'message': '',
            'remaining_seconds': None,
            'elapsed_seconds': None
        }
        super().__init__()

    def __str__(self):
        return str(self.status)

    def on_progress_status(self, percent_complete):
        self.status['percent_complete'] = percent_complete

    def on_begin_operation_string(self, operation):
        self.status['operation_begin'] = True
        self.status['operation'] = operation

    def on_end_operation(self):
        self.status['operation_end'] = True

    def on_update_message(self, message):
        self.status['message'] = message

    def time_remaining_in_seconds(self, remaining_seconds):
        self.status['remaining_seconds'] = remaining_seconds

    def time_elapsed_in_seconds(self, elapsed_seconds):
        self.status['elapsed_seconds'] = elapsed_seconds


class TestInterpLinear(unittest.TestCase):
    """Test Linear Interpolation Class."""

    def setUp(self):
        """Set up for each test case."""
        self.interp_linear_obj = xmsinterp_py.interpolate.InterpLinear()

    def test_to_string(self):
        """Ensure class can be printed as a string."""
        base_string = '0=cloughTocher 0=natNeigh 0=trunc 0=truncMax ' \
                      '0=truncMin -1e+07=extrap \n' \
                      '3.40282e+38,3.40282e+38,3.40282e+38=min-3.40282e+38,' \
                      '-3.40282e+38,-3.40282e+38=max\n=pts \n=tris \n' \
                      '=triActivity \n=triSearch \n=pts \n=tris \n=scalar \n\n'
        self.assertEqual(base_string, self.interp_linear_obj.to_string())

    def test_set_pts_tris(self):
        """Set base points."""
        interp = self.interp_linear_obj
        # Test rejection of ints as args
        with self.assertRaises(TypeError):
            interp.set_pts_tris((1), (1))

        # Test rejections of 2-tuples as args
        with self.assertRaises(TypeError) as context:
            interp.set_pts_tris((1, 2), (1, 2))
        err = context.exception
        self.assertEqual("First arg must be a n-tuple of 3-tuples", str(err))

        # Test rejection of 2-tuples as sub-tuples of arg1
        with self.assertRaises(TypeError) as context:
            interp.set_pts_tris(((1, 2), (1, 2)), (1, 2))
        err = context.exception
        self.assertEqual("Input points must be 3-tuples", str(err))

        # Test that the a proper call does not throw
        interp.set_pts_tris(((1, 2, 3), (1, 2, 3)), (1, 2))
        # TODO: Find some way to verify the internal state

    def test_set_pts_tris_numpy(self):
        """Set base points."""
        import numpy as np

        interp = self.interp_linear_obj
        # Test rejection of ints as args
        with self.assertRaises(TypeError):
            interp.set_pts_tris(np.array([1]), np.array([1]))

        # Test rejections of 2-tuples as args
        with self.assertRaises(TypeError) as context:
            interp.set_pts_tris(np.array([1, 2]), np.array([1, 2]))
        err = context.exception
        self.assertEqual("First arg must be a n-tuple of 3-tuples", str(err))

        # Test rejection of 2-tuples as sub-tuples of arg1
        with self.assertRaises(TypeError) as context:
            interp.set_pts_tris(np.array([(1, 2), (1, 2)]), np.array([1, 2]))
        err = context.exception
        self.assertEqual("Input points must be 3-tuples", str(err))

        # Test that the a proper call does not throw
        interp.set_pts_tris(np.array([(1, 2, 3), (1, 2, 3)]), np.array([1, 2]))
        # TODO: Find some way to verify the internal state

    def test_interp_to_pt(self):
        """Interpolate to a specific point."""
        interp = self.interp_linear_obj

        with self.assertRaises(TypeError):
            interp.interp_to_pt()

        with self.assertRaises(TypeError) as context:
            interp.interp_to_pt((1, 2))
        err = context.exception

        self.assertEqual("Input point should be a 3-tuple", str(err))
        val = interp.interp_to_pt((1, 2, 3))
        self.assertEqual(-9999999.0, val)

    def test_intperp_to_pts(self):
        """Interpolate to multiple points"""
        interp = self.interp_linear_obj

        with self.assertRaises(TypeError):
            interp.interp_to_pts()

        with self.assertRaises(TypeError) as context:
            interp.interp_to_pts((1,2))
        err = context.exception

        pts = ((0, 0, 0), (10, 0, 1), (10, 10, 2), (0, 10, 3))
        tris = (0, 1, 3, 1, 2, 3)
        interp.set_pts_tris(pts, tris)

        self.assertEqual("First arg must be a n-tuple of 3-tuples", str(err))
        ret = interp.interp_to_pts(((2, 1, 0), (5, 10, 2.5)))
        self.assertIsInstance(ret, tuple)
        self.assertEqual((0.5, 2.5), ret)

    def test_intperp_to_pts_numpy(self):
        """Interpolate to multiple points"""
        import numpy as np
        interp = self.interp_linear_obj

        with self.assertRaises(TypeError):
            interp.interp_to_pts()

        with self.assertRaises(TypeError) as context:
            interp.interp_to_pts(np.array([1,2]))
        err = context.exception

        pts = np.array([(0, 0, 0), (10, 0, 1), (10, 10, 2), (0, 10, 3)])
        tris = np.array([0, 1, 3, 1, 2, 3])
        interp.set_pts_tris(pts, tris)

        self.assertEqual("First arg must be a n-tuple of 3-tuples", str(err))
        ret = interp.interp_to_pts(np.array([(2, 1, 0), (5, 10, 2.5)]))
        self.assertIsInstance(ret, np.ndarray)
        np.testing.assert_array_equal(np.array([0.5, 2.5]), ret)

    def test_set_pt_activity(self):
        """Setting point activity"""
        interp = self.interp_linear_obj

        pts = ((0, 0, 0), (10, 0, 1), (10, 10, 2), (0, 10, 3))
        tris = (0, 1, 3, 1, 2, 3)
        interp.set_pts_tris(pts, tris)
        act1 = (True, False, False, True)
        interp.set_pt_activity(act1)

        act2 = (0, 1, 0, 1)
        interp.set_pt_activity(act2)

    def test_set_tri_activity(self):
        """Setting tri activity"""
        interp = self.interp_linear_obj

        pts = ((0, 0, 0), (10, 0, 1), (10, 10, 2), (0, 10, 3))
        tris = (0, 1, 3, 1, 2, 3)
        interp.set_pts_tris(pts, tris)
        act1 = (True, False)
        interp.set_tri_activity(act1)

        act2 = (1, 1)
        interp.set_tri_activity(act2)

    def test_get_pts(self):
        """Getting interp object points"""
        import numpy as np
        interp = self.interp_linear_obj

        pts = np.array([(0, 0, 0), (10, 0, 1), (10, 10, 2), (0, 10, 3)])
        tris = np.array([0, 1, 3, 1, 2, 3])
        interp.set_pts_tris(pts, tris)

        ret = interp.pts
        np.testing.assert_array_equal(pts, ret)

    def test_get_tris(self):
        """Getting interp object points"""
        import numpy as np
        interp = self.interp_linear_obj

        pts = np.array([(0, 0, 0), (10, 0, 1), (10, 10, 2), (0, 10, 3)])
        tris = np.array([0, 1, 3, 1, 2, 3])
        interp.set_pts_tris(pts, tris)

        ret = interp.tris
        np.testing.assert_array_equal(tris, ret)

    def test_tri_containing_pt(self):
        """Getting tri containing point"""
        interp = self.interp_linear_obj

        pts = ((0, 0, 0), (10, 0, 1), (10, 10, 2), (0, 10, 3))
        tris = (0, 1, 3, 1, 2, 3)
        interp.set_pts_tris(pts, tris)
        ret = interp.tri_containing_pt((5, 5, 2))
        self.assertEqual(0, ret)

    def test_tri_envelops_containing_pt(self):
        """Getting tri containing point"""
        import numpy as np
        interp = self.interp_linear_obj

        pts = ((0, 0, 0), (10, 0, 1), (10, 10, 2), (0, 10, 3))
        tris = (0, 1, 3, 1, 2, 3)
        interp.set_pts_tris(pts, tris)
        ret = interp.tri_envelops_containing_pt((5, 5, 2))
        np.testing.assert_array_equal(np.array([0, 3], np.int32), ret)

    def test_interp_weights(self):
        """Test interp_weights"""
        import numpy as np
        interp = self.interp_linear_obj

        pts = ((0, 0, 0), (10, 0, 1), (10, 10, 2), (0, 10, 3))
        tris = (0, 1, 3, 1, 2, 3)
        interp.set_pts_tris(pts, tris)

        pt_inside, idxs, wts = interp.interp_weights((5, 5, 2))
        self.assertTrue(pt_inside)
        np.testing.assert_array_equal(np.array([0, 1, 3], np.int32), idxs)
        np.testing.assert_array_equal(np.array([0, 0.5, 0.5]), wts)

        pt_inside, idxs, wts = interp.interp_weights((-10, -10, -10))
        self.assertFalse(pt_inside)
        np.testing.assert_array_equal(np.array([], np.int32), idxs)
        np.testing.assert_array_equal(np.array([]), wts)

    def test_set_extrap_val(self):
        """Test set_extrap_val"""
        interp = self.interp_linear_obj
        extrap_val = 7.11
        interp.set_extrap_val(extrap_val)
        base_string = '0=cloughTocher 0=natNeigh 0=trunc 0=truncMax ' \
                      '0=truncMin {}=extrap \n' \
                      '3.40282e+38,3.40282e+38,3.40282e+38=min-3.40282e+38,' \
                      '-3.40282e+38,-3.40282e+38=max\n=pts \n=tris \n' \
                      '=triActivity \n=triSearch \n=pts \n=tris \n=scalar \n\n' \
                      .format(extrap_val)
        self.assertEqual(base_string, self.interp_linear_obj.to_string())

    def test_set_trunc(self):
        """Test set_trunc"""
        interp = self.interp_linear_obj
        t_min = 7.11
        t_max = 11.7
        interp.set_trunc(t_max, t_min)
        base_string = '0=cloughTocher 0=natNeigh 1=trunc {}=truncMax ' \
                      '{}=truncMin -1e+07=extrap \n' \
                      '3.40282e+38,3.40282e+38,3.40282e+38=min-3.40282e+38,' \
                      '-3.40282e+38,-3.40282e+38=max\n=pts \n=tris \n' \
                      '=triActivity \n=triSearch \n=pts \n=tris \n=scalar \n\n' \
                      .format(t_max, t_min)
        self.assertEqual(base_string, self.interp_linear_obj.to_string())

    def test_set_use_clough_tocher(self):
        """Test set_use_clough_tocher"""
        interp = self.interp_linear_obj
        observer = MockObserver()
        pts = ((0, 0, 0), (10, 0, 1), (10, 10, 2), (0, 10, 3))
        tris = (0, 1, 3, 1, 2, 3)
        interp.set_pts_tris(pts, tris)
        interp.set_use_clough_tocher(True, observer)
        val = interp.interp_to_pt((2, 1, 0))
        self.assertIn('1=cloughTocher', self.interp_linear_obj.to_string())
        self.assertAlmostEqual(0.2574999928474426, val, places=9)

    def test_set_use_nat_neigh(self):
        """Test set_use_nat_neigh"""
        interp = self.interp_linear_obj
        observer = MockObserver()
        pts = ((0, 0, 0), (10, 0, 1), (10, 10, 2), (0, 10, 3))
        tris = (0, 1, 3, 1, 2, 3)
        interp.set_pts_tris(pts, tris)

        nodal_func = 0
        nodal_func_opts = 1
        n_nearest = 12
        blend_weights = False
        interp.set_use_nat_neigh(True, nodal_func, nodal_func_opts, n_nearest, blend_weights, observer)

        val = interp.interp_to_pt((2, 1, 0))
        self.assertIn('1=natNeigh', self.interp_linear_obj.to_string())
        self.assertAlmostEqual(0.4600000, val, places=7)

    def test_tutorial(self):
        """Ensure the tutorial will work."""
        interp = self.interp_linear_obj
        pts = ((0, 0, 0), (10, 0, 1), (10, 10, 2), (0, 10, 3))
        tris = (0, 1, 3, 1, 2, 3)
        interp.set_pts_tris(pts, tris)
        val = interp.interp_to_pt((2, 1, 0))
        self.assertEqual(0.5, val)


if __name__ == '__main__':
    unittest.main()
