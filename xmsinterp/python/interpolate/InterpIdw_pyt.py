"""Test InterpIdw_py.cpp."""
import unittest
import xmsinterp_py


class TestInterpIdw(unittest.TestCase):
    """Test IDW Interpolation Class."""

    def setUp(self):
        """Set up for each test case."""
        self.interp_idw_obj = xmsinterp_py.interpolate.InterpIdw()

    def test_id_string_prop(self):
        """Ensure id_string is editable."""
        self.assertEqual('', self.interp_idw_obj.id_string)
        test_name = 'test_name'
        self.interp_idw_obj.id_string = test_name
        self.assertEqual(test_name, self.interp_idw_obj.id_string)

    def test_to_string(self):
        """Ensure class can be printed as a string."""
        base_string = '1=2d 0=quadOctSearch 1=modifiedShepardWeights ' \
                      '16=nNearestPts 2=power 0=saveWeights \n=pts \n=tris ' \
                      '\n=scalarFrom \n=ptIdx \n=weights \n'
        self.assertEqual(base_string, self.interp_idw_obj.to_string())

    def test_set_pts_tris(self):
        """Set base points."""
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

    def test_tutorial(self):
        """Ensure the tutorial will work."""
        interp = self.interp_idw_obj
        pts = ((0, 0, 0), (10, 0, 1), (10, 10, 2), (0, 10, 3))
        tris = ()
        interp.set_pts_tris(pts, tris)
        val = interp.interp_to_pt((5, 5, 0))
        self.assertEqual(1.5, val)


if __name__ == '__main__':
    unittest.main()
