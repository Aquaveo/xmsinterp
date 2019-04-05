import unittest
import xmsinterp
import xmscore

class TestTriSearch(unittest.TestCase):

    def setUp(self):
        self.pts = ((0, 0, 0), (1, 1, 1), (1, 0, 2), (0, 1, 2), (0.5, 1.5, 1))
        self.tris = (0, 2, 1, 0, 1, 3, 3, 1, 4)

    def test_create_class(self):
        """Test creating class"""
        tri_search = xmsinterp.geometry.TriSearch(self.pts, self.tris)
        self.assertIsInstance(tri_search,
                              xmsinterp.geometry.TriSearch)

    def test_interp_weights(self):
        """Test interp_weights"""
        tri_search = xmsinterp.geometry.TriSearch(self.pts, self.tris)
        pt = (0.5, 0.2, 0)

        result, idx, wts = tri_search.interp_weights(pt)
        self.assertTrue(result)

        base_idx = (0, 2, 1)
        base_wts = (0.5, 0.3, 0.2)

        self.assertEqual(base_idx, idx)
        self.assertEqual(base_wts, wts)

    def test_interp_weights_triangle_idx(self):
        """Test interp_weights_triangle_idx"""
        tri_search = xmsinterp.geometry.TriSearch(self.pts, self.tris)
        pt = (0.25, 0.75, 0)

        result, tri_idx, idx, wts = \
            tri_search.interp_weights_triangle_idx(pt)
        self.assertTrue(result)

        base_tri_idx = 3
        self.assertEqual(base_tri_idx, tri_idx)

        base_idx = (0, 1, 3)
        self.assertEqual(base_idx, idx)

        base_wts = (.25, .25, .5)
        self.assertEqual(base_wts, wts)

    def test_interp_weights_triangle_idx_outside(self):
        """Test interp_weights_triangle_idx outside"""
        tri_search = xmsinterp.geometry.TriSearch(self.pts, self.tris)
        pt = (0, 1.25, 0)

        result, tri_idx, idx, wts = \
            tri_search.interp_weights_triangle_idx(pt)
        self.assertFalse(result)

        base_tri_idx = -1
        self.assertEqual(base_tri_idx, tri_idx)

        base_idx = ()
        self.assertEqual(base_idx, idx)

        base_wts = ()
        self.assertEqual(base_wts, wts)

    def test_pt_activity(self):
        """Test pt_activity"""
        tri_search = xmsinterp.geometry.TriSearch(self.pts, self.tris)
        wrong_size = [True for i in range(0, 6)]
        tri_search.set_pt_activity(wrong_size)
        pt = (0.5, 0.2, 0)
        self.assertEqual(0, tri_search.tri_containing_pt(pt))

        actual = [True for i in range(0, 5)]
        actual[1] = False
        tri_search.set_pt_activity(actual)

        self.assertEqual(-1, tri_search.tri_containing_pt(pt))

    def test_tri_activity(self):
        """Test tri_activity"""
        tri_search = xmsinterp.geometry.TriSearch(self.pts, self.tris)
        pt1 = (0.5, 0.2, 0)
        pt2 = (0.5, 0.5, 0)  # Right on the border of the first two triangles

        wrong_size = [True for i in range(0, 4)]
        tri_search.set_tri_activity(wrong_size)
        self.assertEqual(0, tri_search.tri_containing_pt(pt1))
        self.assertEqual(0, tri_search.tri_containing_pt(pt2))

        actual = [True for i in range(0, 3)]
        actual[0] = False
        tri_search.set_tri_activity(actual)

        self.assertEqual(-1, tri_search.tri_containing_pt(pt1))
        self.assertEqual(3, tri_search.tri_containing_pt(pt2))

    def test_sms_case_1(self):
        """Test test case from sms"""
        pt = (-31.459823375717541, 29.927133417260336, 0)

        pts =  ((-20.150000000000002, 46.579999999999998, 7),
        (-41.100000000000001, 30.370000000000001, 8),
        (-19.550000000000001, 29.379999999999999, 9))
        tris = (2, 0, 1)

        tri_search = xmsinterp.geometry.TriSearch(pts, tris)

        self.assertEqual(0, tri_search.tri_containing_pt(pt))

    def test_touch(self):
        """Test test a point that touches a triangle"""
        pt = (.5, .5, 0)
        pts = ((0, 0, 7), (1, 0, 8), (1, 1, 9))
        tris = (2, 0 , 1)

        tri_search = xmsinterp.geometry.TriSearch(pts, tris)

        self.assertEqual(0, tri_search.tri_containing_pt(pt))

if __name__ == '__main__':
    unittest.main()
