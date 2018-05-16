"""Test InterpLinear_py.cpp."""
import unittest
import xmsinterp_py


class TestInterpLinear(unittest.TestCase):
    """Test Linear Interpolation Class."""

    def setUp(self):
        """Set up for each test case."""
        self.interp_linear_obj = xmsinterp_py.interpolate.InterpLinear()

    def test_id_string_prop(self):
        """Ensure id_string is editable."""
        self.assertEqual('', self.interp_linear_obj.id_string)
        test_name = 'test_name'
        self.interp_linear_obj.id_string = test_name
        self.assertEqual(test_name, self.interp_linear_obj.id_string)

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
