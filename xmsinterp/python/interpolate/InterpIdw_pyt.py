"""Test InterpIdw_py.cpp."""
import unittest
import xmsinterp
import xmscore

class MockObserver(xmscore.misc.Observer):
    """Mock Observer class for testing."""
    def __init__(self, obs_id="X"):
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

class TestInterpIdw(unittest.TestCase):
    """Test IDW Interpolation Class."""

    def setUp(self):
        """Set up for each test case."""
        pts = ((1, 2, 3), (1, 2, 3))
        self.interp_idw_obj = xmsinterp.interpolate.InterpIdw(pts)

    def test_to_string(self):
        """Ensure class can be printed as a string."""
        base_string = "1=2d 0=quadOctSearch 1=modifiedShepardWeights 16=nNearestPts 2=power " \
                      "0=saveWeights 1=multiThread \n1=2dSearch 0,1,2=min 2,3,4=max \n=activity \n" \
                      "1,2,3 1,2,3=bshpPt3d \n=ptSearch \n1,2,3 1,2,3=pts \n=tris \n3 3=scalarFrom \n" \
                      "=ptIdx \n=weights \n"
        self.assertEqual(base_string, self.interp_idw_obj.to_string())

    def test_set_pts_tris(self):
        """Set base points and tris"""
        interp = self.interp_idw_obj
        # Test that the a proper call does not throw
        base_before = "1=2d 0=quadOctSearch 1=modifiedShepardWeights 16=nNearestPts 2=power " \
                      "0=saveWeights 1=multiThread \n1=2dSearch 0,1,2=min 2,3,4=max \n" \
                      "=activity \n1,2,3 1,2,3=bshpPt3d \n=ptSearch \n1,2,3 1,2,3=pts \n" \
                      "=tris \n3 3=scalarFrom \n=ptIdx \n=weights \n"
        self.assertEqual(base_before, str(interp))

        interp.set_pts_tris(((1, 2, 3), (1, 2, 3)), (1, 2))
        base_after = "1=2d 0=quadOctSearch 1=modifiedShepardWeights 16=nNearestPts 2=power " \
                     "0=saveWeights 1=multiThread \n1=2dSearch 0,1,2=min 2,3,4=max \n=activity \n" \
                     "1,2,3 1,2,3=bshpPt3d \n=ptSearch \n1,2,3 1,2,3=pts \n1 2=tris \n" \
                     "3 3=scalarFrom \n=ptIdx \n=weights \n"
        self.assertEqual(base_after, str(interp))

    def test_set_pts(self):
        """Set base points"""
        interp = self.interp_idw_obj
        # Test that the a proper call does not throw
        interp.set_pts(((1, 2, 3), (1, 2, 3)), False)

        base = "0=2d 0=quadOctSearch 1=modifiedShepardWeights 16=nNearestPts 2=power 0=saveWeights " \
               "1=multiThread \n0=2dSearch 0,1,2=min 2,3,4=max \n=activity \n1,2,3 1,2,3=bshpPt3d \n" \
               "=ptSearch \n1,2,3 1,2,3=pts \n=tris \n3 3=scalarFrom \n=ptIdx \n=weights \n"
        self.assertEqual(base, str(interp))

    def test_set_pts_2d(self):
        """Set base points"""
        interp = self.interp_idw_obj
        # Test that the a proper call does not throw
        interp.set_pts(((1, 2, 3), (1, 2, 3)), True)

        base = '1=2d 0=quadOctSearch 1=modifiedShepardWeights ' \
               '16=nNearestPts 2=power 0=saveWeights 1=multiThread \n' \
               '1=2dSearch 0,1,2=min 2,3,4=max \n' \
               '=activity \n1,2,3 1,2,3=bshpPt3d \n=ptSearch \n' \
               '1,2,3 1,2,3=pts \n=tris ' \
               '\n3 3=scalarFrom \n=ptIdx \n=weights \n'
        self.assertEqual(base, str(interp))

    def test_interp_to_pt(self):
        """Interpolate to a specific point."""
        interp = self.interp_idw_obj
        val = interp.interp_to_pt((1, 2, 3))
        self.assertEqual(3.0, val)

    def test_interp_to_pts(self):
        """Interpolate to multiple points"""
        pts = ((0, 0, 0), (10, 0, 1), (10, 10, 2), (0, 10, 3))
        interp = xmsinterp.interpolate.InterpIdw(pts)
        ret = interp.interp_to_pts(((2, 1, 0), (5, 10, 2.5)))
        self.assertIsInstance(ret, tuple)
        self.assertEqual((0.02681550197303295, 2.5), ret)

    def test_interp_to_pts_numpy(self):
        """Interpolate to multiple points"""
        import numpy as np
        pts = np.array([(0, 0, 0), (10, 0, 1), (10, 10, 2), (0, 10, 3)])
        interp = xmsinterp.interpolate.InterpIdw(pts)
        ret = interp.interp_to_pts(np.array([(2, 1, 0), (5, 10, 2.5)]))
        self.assertIsInstance(ret, np.ndarray)
        np.testing.assert_array_equal(np.array([0.02681550197303295, 2.5]), ret)

    def test_set_pt_activity(self):
        """Setting point activity"""
        interp = self.interp_idw_obj

        pts = ((0, 0, 0), (10, 0, 1), (10, 10, 2), (0, 10, 3))
        tris = (0, 1, 3, 1, 2, 3)
        interp.set_pts_tris(pts, tris)
        act1 = (True, False, False, True)
        interp.set_pt_activity(act1)

        act2 = (0, 1, 0, 1)
        interp.set_pt_activity(act2)

    def test_set_tri_activity(self):
        """Setting tri activity"""
        interp = self.interp_idw_obj

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
        interp = self.interp_idw_obj

        pts = np.array([(0, 0, 0), (10, 0, 1), (10, 10, 2), (0, 10, 3)])
        tris = np.array([0, 1, 3, 1, 2, 3])
        interp.set_pts_tris(pts, tris)

        ret = interp.pts
        np.testing.assert_array_equal(pts, ret)

    def test_get_tris(self):
        """Getting interp object points"""
        import numpy as np
        interp = self.interp_idw_obj

        pts = np.array([(0, 0, 0), (10, 0, 1), (10, 10, 2), (0, 10, 3)])
        tris = np.array([0, 1, 3, 1, 2, 3])
        interp.set_pts_tris(pts, tris)

        ret = interp.tris
        np.testing.assert_array_equal(tris, ret)

    def test_set_trunc(self):
        """Test set_trunc"""
        import numpy as np
        t_min = 7.11
        t_max = 11.7

        pts = np.array([(0, 0, 0), (10, 0, 1), (10, 10, 2), (0, 10, 3)])
        tris = np.array([0, 1, 3, 1, 2, 3])
        interp = xmsinterp.interpolate.InterpIdw(pts, tris)

        interp.set_truncation_max_min(t_max, t_min)
        base = "1=2d 0=quadOctSearch 1=modifiedShepardWeights 16=nNearestPts " \
               "2=power 0=saveWeights 1=multiThread \n1=2dSearch -1,-1,-1=min 11,11,4=max \n" \
               "=activity \n0,0,0 10,0,1 10,10,2 0,10,3=bshpPt3d \n=ptSearch \n" \
               "0,0,0 10,0,1 10,10,2 0,10,3=pts \n0 1 3 1 2 3=tris \n" \
               "0 1 2 3=scalarFrom \n=ptIdx \n=weights \n"
        self.assertEqual(base, str(interp))

    def test_set_observer(self):
        """Test set_observer"""
        import numpy as np
        interp = self.interp_idw_obj
        observer1 = MockObserver("Obs1")
        observer2 = MockObserver("Obs2")

        pts = ((0, 0, 0), (10, 0, 1), (10, 10, 2), (0, 10, 3))
        tris = (0, 1, 3, 1, 2, 3)
        interp.set_pts_tris(pts, tris)

        # Non-number Type
        with self.assertRaises(TypeError) as context:
            interp.set_observer()
        err = context.exception
        self.assertIn("set_observer(): incompatible function arguments.", str(err))

        # Non-Observer Type
        with self.assertRaises(TypeError) as context:
            interp.set_observer("xyz")
        err = context.exception
        self.assertIn("set_observer(): incompatible function arguments.", str(err))

        interp.set_observer(observer1)
        interp.interp_to_pts(np.random.rand(100000, 3) + 5)
        self.assertGreater(observer1.status['percent_complete'], 0.0)
        self.assertGreater(observer1.status['elapsed_seconds'], 0.0)

        interp.set_observer(observer2)
        interp.interp_to_pts(np.random.rand(100000, 3) + 5)
        self.assertGreater(observer2.status['percent_complete'], 0.0)
        self.assertGreater(observer2.status['elapsed_seconds'], 0.0)

    def test_set_power(self):
        """Set power on InterpIdw objects"""
        interp = self.interp_idw_obj

        # No Arguments
        with self.assertRaises(TypeError) as context:
            interp.set_power()
        err = context.exception
        self.assertIn("set_power(): incompatible function arguments.", str(err))

        # None Type
        with self.assertRaises(TypeError) as context:
            interp.set_power(None)
        err = context.exception
        self.assertIn("set_power(): incompatible function arguments.", str(err))

        # Non-number Type
        with self.assertRaises(TypeError) as context:
            interp.set_power("xyz")
        err = context.exception
        self.assertIn("set_power(): incompatible function arguments.", str(err))

        # Good arguments
        power = 10
        interp.set_power(power)
        base = " 10=power "
        self.assertIn(base, str(interp))

    def test_set_search_opts(self):
        """Ensure the tutorial will work."""
        interp = self.interp_idw_obj
        typeerror = "set_search_opts(): incompatible function arguments."

        # No Arguments
        with self.assertRaises(TypeError) as context:
            interp.set_search_opts()
        err = context.exception
        self.assertIn(typeerror, str(err))

        # One Arguments
        with self.assertRaises(TypeError) as context:
            interp.set_search_opts(123)
        err = context.exception
        self.assertIn(typeerror, str(err))

        # None Arguments
        with self.assertRaises(TypeError) as context:
            interp.set_search_opts(None, None)
        err = context.exception
        self.assertIn(typeerror, str(err))

        # Bad Argument
        with self.assertRaises(TypeError) as context:
            interp.set_search_opts("123", False)
        err = context.exception
        self.assertIn(typeerror, str(err))

        # Good Arguments
        interp.set_search_opts(10, True)
        self.assertIn(" 1=quadOctSearch ", str(interp))
        self.assertIn(" 10=nNearestPts ", str(interp))

        interp.set_search_opts(15, False)
        self.assertIn(" 0=quadOctSearch ", str(interp))
        self.assertIn(" 15=nNearestPts ", str(interp))

    def test_set_weight_calc_method(self):
        """Setting weight calc method with enum"""
        interp = self.interp_idw_obj
        typeerror = "set_weight_calc_method(): incompatible function arguments."

        # No Argument
        with self.assertRaises(TypeError) as context:
            interp.set_weight_calc_method()
        err = context.exception
        self.assertIn(typeerror, str(err))

        # None Argument
        with self.assertRaises(TypeError) as context:
            interp.set_weight_calc_method(None)
        err = context.exception
        self.assertIn(typeerror, str(err))

        # Bad Arguments
        with self.assertRaises(ValueError) as context:
            interp.set_weight_calc_method("bad_string")
        err = context.exception
        errStr = "method string must be one of 'classic', 'modified' not bad_string"
        self.assertIn(errStr, str(err))

        with self.assertRaises(TypeError) as context:
            interp.set_weight_calc_method(123)
        err = context.exception
        self.assertIn(typeerror, str(err))

        # Valid Arguments

        interp.set_weight_calc_method("modified")
        self.assertIn(" 1=modifiedShepardWeights ", str(interp))

        interp.set_weight_calc_method("classic")
        self.assertIn(" 0=modifiedShepardWeights ", str(interp))

    def test_set_nodal_function(self):
        """Setting nodal function"""
        import numpy as np
        observer = MockObserver()
        typeerror = "set_nodal_function(): incompatible function arguments."

        pts = np.array([(0, 0, 0), (10, 0, 1), (10, 10, 2), (0, 10, 3)])
        tris = np.array([0, 1, 3, 1, 2, 3])
        interp = xmsinterp.interpolate.InterpIdw(pts, tris)

        # No Argument
        with self.assertRaises(TypeError) as context:
            interp.set_nodal_function()
        err = context.exception
        self.assertIn(typeerror, str(err))

        # None Args for each argument
        with self.assertRaises(TypeError) as context:
            interp.set_nodal_function(None, 1, True, observer)
        err = context.exception
        self.assertIn(typeerror, str(err))
        with self.assertRaises(TypeError) as context:
            interp.set_nodal_function("constant", None, True, observer)
        err = context.exception
        self.assertIn(typeerror, str(err))

        # Bad Args
        with self.assertRaises(TypeError) as context:
            interp.set_nodal_function(1, 1, True, observer)
        err = context.exception
        self.assertIn(typeerror, str(err))
        with self.assertRaises(ValueError) as context:
            interp.set_nodal_function("abc", 1, True, observer)
        err = context.exception
        errStr = "nodal_func_type string must be one of 'constant', " \
                 "'gradient_plane', 'quadratic' not abc"
        self.assertIn(errStr, str(err))
        with self.assertRaises(TypeError) as context:
            interp.set_nodal_function("constant", "1234", True, observer)
        err = context.exception
        self.assertIn(typeerror, str(err))
        with self.assertRaises(TypeError) as context:
            interp.set_nodal_function("constant", 1.4, True, observer)
        err = context.exception
        self.assertIn(typeerror, str(err))
        with self.assertRaises(TypeError) as context:
            interp.set_nodal_function("constant", 1, True, 1)
        err = context.exception
        self.assertIn(typeerror, str(err))
        with self.assertRaises(TypeError) as context:
            interp.set_nodal_function("constant", 1, True, "abcd")
        err = context.exception
        self.assertIn(typeerror, str(err))

        # Good Args
        base = str(interp)
        interp.set_nodal_function("constant", 7, True, observer)
        self.assertEqual(base, str(interp))

        interp.set_nodal_function("gradient_plane", 9, True, observer)
        base_grad_plane = "1=2d 0=quadOctSearch 1=modifiedShepardWeights 16=nNearestPts " \
                          "2=power 0=saveWeights 1=multiThread \n1=2dSearch -1,-1,-1=min 11,11,4=max " \
                          "\n=activity \n0,0,0 10,0,1 10,10,2 0,10,3=bshpPt3d \n=ptSearch " \
                          "\n0,0,0 10,0,1 10,10,2 0,10,3=pts \n0 1 3 1 2 3=tris " \
                          "\n0 1 2 3=scalarFrom \n1=2d 1=type 9=nNearest 1=quadOct 2=power " \
                          "1=modifiedShepardWeights 1=errorReport 0=debugTest " \
                          "\n0,0,0 10,0,1 10,10,2 0,10,3=pts \n0 1 2 3=scalar " \
                          "\n0,0,0 0,0,0 0,0,0 0,0,0=gradient \n=quadratic \n=nearestAll " \
                          "\n=nodalFunc \n=ptIdx \n=weights \n"
        self.assertEqual(base_grad_plane, str(interp))

        interp.set_nodal_function("quadratic", 11, True, observer)
        base_q = "1=2d 0=quadOctSearch 1=modifiedShepardWeights " \
                 "16=nNearestPts 2=power 0=saveWeights 1=multiThread \n" \
                 "1=2dSearch -1,-1,-1=min 11,11,4=max \n" \
                 "=activity \n" \
                 "0,0,0 10,0,1 10,10,2 0,10,3=bshpPt3d \n" \
                 "=ptSearch \n" \
                 "0,0,0 10,0,1 10,10,2 0,10,3=pts \n" \
                 "0 1 3 1 2 3=tris \n" \
                 "0 1 2 3=scalarFrom \n" \
                 "1=2d 2=type 11=nNearest 1=quadOct 2=power " \
                 "1=modifiedShepardWeights 1=errorReport 0=debugTest \n" \
                 "0,0,0 10,0,1 10,10,2 0,10,3=pts \n" \
                 "0 1 2 3=scalar \n" \
                 "=gradient \n" \
                 "0 0 0 0 0 0 0 0 0=quadratic \n" \
                 "0 0 0 0 0 0 0 0 0=quadratic \n" \
                 "0 0 0 0 0 0 0 0 0=quadratic \n" \
                 "0 0 0 0 0 0 0 0 0=quadratic \n" \
                 "=quadratic \n" \
                 "=nearestAll \n" \
                 "=nodalFunc \n" \
                 "=ptIdx \n" \
                 "=weights \n"
        self.maxDiff = None
        self.assertEqual(base_q, str(interp))

    def test_interp_weights(self):
        """Test interp_weights"""
        import numpy as np
        interp = self.interp_idw_obj

        pts = ((0, 0, 0), (10, 0, 1), (10, 10, 2), (0, 10, 3))
        tris = (0, 1, 3, 1, 2, 3)
        interp.set_pts_tris(pts, tris)

        idxs, wts = interp.interp_weights((5, 5, 2))
        np.testing.assert_array_equal(np.array([0, 1, 2, 3], np.int32), idxs)
        np.testing.assert_array_equal(np.array([0.25, 0.25, 0.25, 0.25]), wts)

        idxs, wts = interp.interp_weights((10, 10, 0))
        np.testing.assert_array_equal(np.array([2], np.int32), idxs)
        np.testing.assert_array_equal(np.array([1.]), wts)

        idxs, wts = interp.interp_weights((-10, -10, -10))
        np.testing.assert_array_equal(np.array([0, 1, 2, 3], np.int32), idxs)
        np.testing.assert_array_almost_equal(np.array([0.876919, 0.06154, 
                                             0.0, 0.06154]), wts, decimal=5)

    def test_set_multi_threading(self):
        """Setting multi threading"""
        interp = self.interp_idw_obj
        typeerror = "set_multi_threading(): incompatible function arguments."

        # No Argument
        with self.assertRaises(TypeError) as context:
            interp.set_multi_threading()
        err = context.exception
        self.assertIn(typeerror, str(err))

        interp.set_multi_threading(False)
        base_multi_off = "1=2d 0=quadOctSearch 1=modifiedShepardWeights " \
                         "16=nNearestPts 2=power 0=saveWeights 0=multiThread \n"\
                         "1=2dSearch 0,1,2=min 2,3,4=max \n=activity \n" \
                         "1,2,3 1,2,3=bshpPt3d \n=ptSearch \n1,2,3 1,2,3=pts \n" \
                         "=tris \n3 3=scalarFrom \n=ptIdx \n=weights \n"
        self.assertEqual(base_multi_off, str(interp))

        interp.set_multi_threading(True)
        base_multi_on = "1=2d 0=quadOctSearch 1=modifiedShepardWeights " \
                        "16=nNearestPts 2=power 0=saveWeights 1=multiThread \n" \
                        "1=2dSearch 0,1,2=min 2,3,4=max \n=activity \n" \
                        "1,2,3 1,2,3=bshpPt3d \n=ptSearch \n1,2,3 1,2,3=pts \n" \
                        "=tris \n3 3=scalarFrom \n=ptIdx \n=weights \n"
        self.assertEqual(base_multi_on, str(interp))

    def test_tutorial(self):
        """Ensure the tutorial will work."""
        interp = self.interp_idw_obj
        pts = ((0, 0, 0), (10, 0, 1), (10, 10, 2), (0, 10, 3))
        tris = ()
        tris2 = (0, 1, 3, 1, 2, 3)

        interp = xmsinterp.interpolate.InterpIdw(pts, tris)
        val = interp.interp_to_pt((5, 5, 0))
        self.assertEqual(1.5, val)

        interp = xmsinterp.interpolate.InterpIdw(pts, tris2)
        val2 = interp.interp_to_pt((5, 5, 0))
        self.assertEqual(1.5, val2)


if __name__ == '__main__':
    unittest.main()
