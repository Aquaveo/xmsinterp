"""Test InterpIdw_py.cpp."""
import unittest
import xmsinterp_py


class MockObserver(xmsinterp_py.misc.Observer):
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


class TestIdwEnums(unittest.TestCase):
    """Test IDW Enums"""

    def test_idw_weight_enum(self):
        """weight_enum test"""
        from xmsinterp_py.interpolate import InterpIdw as iIdw
        self.assertEqual("weight_enum.CLASSIC", str(iIdw.weight_enum.CLASSIC))
        self.assertEqual("weight_enum.MODIFIED", str(iIdw.weight_enum.MODIFIED))
        self.assertEqual(2, len(iIdw.weight_enum.__members__))
        self.assertDictEqual(
            iIdw.weight_enum.__members__,
            {
                "CLASSIC": iIdw.weight_enum.CLASSIC,
                "MODIFIED": iIdw.weight_enum.MODIFIED
            }
        )

    def test_idw_nodal_func_enum(self):
        """weight_enum test"""
        from xmsinterp_py.interpolate import InterpIdw as iIdw
        self.assertEqual("nodal_func_enum.CONSTANT", str(iIdw.nodal_func_enum.CONSTANT))
        self.assertEqual("nodal_func_enum.GRAD_PLANE", str(iIdw.nodal_func_enum.GRAD_PLANE))
        self.assertEqual("nodal_func_enum.QUADRATIC", str(iIdw.nodal_func_enum.QUADRATIC))
        self.assertEqual(3, len(iIdw.nodal_func_enum.__members__))
        self.assertDictEqual(
            iIdw.nodal_func_enum.__members__,
            {
                "CONSTANT": iIdw.nodal_func_enum.CONSTANT,
                "GRAD_PLANE": iIdw.nodal_func_enum.GRAD_PLANE,
                "QUADRATIC": iIdw.nodal_func_enum.QUADRATIC
            }
        )


class TestInterpIdw(unittest.TestCase):
    """Test IDW Interpolation Class."""

    def setUp(self):
        """Set up for each test case."""
        self.interp_idw_obj = xmsinterp_py.interpolate.InterpIdw()

    def test_to_string(self):
        """Ensure class can be printed as a string."""
        base_string = '1=2d 0=quadOctSearch 1=modifiedShepardWeights ' \
                      '16=nNearestPts 2=power 0=saveWeights \n=pts \n=tris ' \
                      '\n=scalarFrom \n=ptIdx \n=weights \n'
        self.assertEqual(base_string, self.interp_idw_obj.to_string())

    def test_set_pts_tris(self):
        """Set base points and tris"""
        interp = self.interp_idw_obj
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

    def test_set_pts(self):
        """Set base points"""
        interp = self.interp_idw_obj
        # Test rejection of ints as args
        with self.assertRaises(TypeError):
            interp.set_pts((1), False)

        # Test rejections of 2-tuples as args
        with self.assertRaises(TypeError) as context:
            interp.set_pts((1, 2), False)
        err = context.exception
        self.assertEqual("First arg must be a n-tuple of 3-tuples", str(err))

        # Test rejection of 2-tuples as sub-tuples of arg1
        with self.assertRaises(TypeError) as context:
            interp.set_pts(((1, 2), (1, 2)), False)
        err = context.exception
        self.assertEqual("Input points must be 3-tuples", str(err))

        # Test that the a proper call does not throw
        interp.set_pts(((1, 2, 3), (1, 2, 3)), False)

        base = '0=2d 0=quadOctSearch 1=modifiedShepardWeights ' \
               '16=nNearestPts 2=power 0=saveWeights \n' \
               '0=2dSearch 0,1,2=min 2,3,4=max \n' \
               '=activity \n1,2,3 1,2,3=bshpPt3d \n=ptSearch \n' \
               '1,2,3 1,2,3=pts \n=tris ' \
               '\n=scalarFrom \n=ptIdx \n=weights \n'
        self.assertEqual(base, str(interp))

    def test_set_pts_2d(self):
        """Set base points"""
        interp = self.interp_idw_obj
        # Test rejection of ints as args
        with self.assertRaises(TypeError):
            interp.set_pts((1), True)

        # Test rejections of 2-tuples as args
        with self.assertRaises(TypeError) as context:
            interp.set_pts((1, 2), True)
        err = context.exception
        self.assertEqual("First arg must be a n-tuple of 3-tuples", str(err))

        # Test rejection of 2-tuples as sub-tuples of arg1
        with self.assertRaises(TypeError) as context:
            interp.set_pts(((1, 2), (1, 2)), True)
        err = context.exception
        self.assertEqual("Input points must be 3-tuples", str(err))

        # Test that the a proper call does not throw
        interp.set_pts(((1, 2, 3), (1, 2, 3)), True)

        base = '1=2d 0=quadOctSearch 1=modifiedShepardWeights ' \
               '16=nNearestPts 2=power 0=saveWeights \n' \
               '1=2dSearch 0,1,2=min 2,3,4=max \n' \
               '=activity \n1,2,3 1,2,3=bshpPt3d \n=ptSearch \n' \
               '1,2,3 1,2,3=pts \n=tris ' \
               '\n3 3=scalarFrom \n=ptIdx \n=weights \n'
        self.assertEqual(base, str(interp))

    def test_interp_to_pt(self):
        """Interpolate to a specific point."""
        interp = self.interp_idw_obj

        with self.assertRaises(TypeError):
            interp.interp_to_pt()

        with self.assertRaises(TypeError) as context:
            interp.interp_to_pt((1, 2))
        err = context.exception

        self.assertEqual("Input point should be a 3-tuple", str(err))
        val = interp.interp_to_pt((1, 2, 3))
        self.assertEqual(0.0, val)

    def test_intperp_to_pts(self):
        """Interpolate to multiple points"""
        interp = self.interp_idw_obj

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
        self.assertEqual((0.02681550197303295, 2.5), ret)

    def test_intperp_to_pts_numpy(self):
        """Interpolate to multiple points"""
        import numpy as np
        interp = self.interp_idw_obj

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
        interp = self.interp_idw_obj
        t_min = 7.11
        t_max = 11.7
        interp.set_trunc(t_max, t_min)
        # TODO: Check state somehow...

    def test_set_observer(self):
        """Test set_observer"""
        interp = self.interp_idw_obj
        observer = MockObserver()
        interp.set_observer(observer)
        # TODO: Check state somehow...

    def test_tutorial(self):
        """Ensure the tutorial will work."""
        interp = self.interp_idw_obj
        pts = ((0, 0, 0), (10, 0, 1), (10, 10, 2), (0, 10, 3))
        tris = ()
        tris2 = (0, 1, 3, 1, 2, 3)

        interp.set_pts_tris(pts, tris)
        val = interp.interp_to_pt((5, 5, 0))
        self.assertEqual(1.5, val)

        interp.set_pts_tris(pts, tris2)
        val2 = interp.interp_to_pt((5, 5, 0))
        self.assertEqual(1.5, val2)


if __name__ == '__main__':
    unittest.main()
