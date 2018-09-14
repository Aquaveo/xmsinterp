import os
import unittest
import numpy as np
import xmsinterp_py.triangulate as xt


class TestTrTin(unittest.TestCase):
    """Test Observer Class."""

    def setUp(self):
        """Set up for each test case."""
        self.trTin = xt.TrTin()
        self.pts = ((0, 0, 0), (10, 0, 0), (20, 0, 0), (5, 10, 0), (15, 10, 0))
        self.pts_np = np.array([(0, 0, 0), (10, 0, 0), (20, 0, 0), (5, 10, 0), (15, 10, 0)])
        self.tris = (3, 0, 1, 1, 2, 4, 1, 4, 3)
        self.tris_np = np.array([3, 0, 1, 1, 2, 4, 1, 4, 3])
        self.tris_adj = ((0,), (0, 1, 2), (1,), (0, 2), (1, 2))
        self.tris_adj_np = np.array([[0], [0, 1, 2], [1], [0, 2], [1, 2]])

    def test_to_string(self):
        base = "22 serialization::archive 16 1 0\n0 0 0 0 0 0 0 0 0 0 0 0 0\n"
        self.assertEqual(base, str(self.trTin))

    def test_set_pts(self):
        trtin = xt.TrTin()
        pts = self.pts
        trtin.set_points(pts)

        base = "22 serialization::archive 16 1 0\n0 0 0 0 0 5 0 0 0 0.00000000000000000e+00 " \
               "0.00000000000000000e+00 0.00000000000000000e+00 1.00000000000000000e+01 " \
               "0.00000000000000000e+00 0.00000000000000000e+00 2.00000000000000000e+01 " \
               "0.00000000000000000e+00 0.00000000000000000e+00 5.00000000000000000e+00 " \
               "1.00000000000000000e+01 0.00000000000000000e+00 1.50000000000000000e+01 " \
               "1.00000000000000000e+01 0.00000000000000000e+00 0 0 0 0 0 0\n"
        self.assertEqual(base, str(trtin))

    def test_set_pts_numpy(self):
        trtin = xt.TrTin()
        pts = self.pts_np
        trtin.set_points(pts)

        base = "22 serialization::archive 16 1 0\n0 0 0 0 0 5 0 0 0 0.00000000000000000e+00 " \
               "0.00000000000000000e+00 0.00000000000000000e+00 1.00000000000000000e+01 " \
               "0.00000000000000000e+00 0.00000000000000000e+00 2.00000000000000000e+01 " \
               "0.00000000000000000e+00 0.00000000000000000e+00 5.00000000000000000e+00 " \
               "1.00000000000000000e+01 0.00000000000000000e+00 1.50000000000000000e+01 " \
               "1.00000000000000000e+01 0.00000000000000000e+00 0 0 0 0 0 0\n"
        self.assertEqual(base, str(trtin))

    def test_set_tris(self):
        trtin = xt.TrTin()
        tris = self.tris
        trtin.set_triangles(tris)

        base = "22 serialization::archive 16 1 0\n0 0 0 0 0 0 0 9 0 3 0 1 1 2 4 1 4 3 0 0 0 0\n"
        self.assertEqual(base, str(trtin))

    def test_set_tris_numpy(self):
        trtin = xt.TrTin()
        tris = self.tris_np
        trtin.set_triangles(tris)

        base = "22 serialization::archive 16 1 0\n0 0 0 0 0 0 0 9 0 3 0 1 1 2 4 1 4 3 0 0 0 0\n"
        self.assertEqual(base, str(trtin))

    def test_set_tris_adj(self):
        trtin = xt.TrTin()
        tris_adj = self.tris_adj
        trtin.set_triangles_adjacent_to_points(tris_adj)

        base = "22 serialization::archive 16 1 0\n0 0 0 0 0 0 0 0 0 0 0 5 0 1 0 0 3 0 0 1 " \
               "2 1 0 1 2 0 0 2 2 0 1 2\n"
        self.assertEqual(base, str(trtin))

    def test_set_tris_adj_numpy(self):
        trtin = xt.TrTin()
        tris_adj = self.tris_adj_np
        trtin.set_triangles_adjacent_to_points(tris_adj)

        base = "22 serialization::archive 16 1 0\n0 0 0 0 0 0 0 0 0 0 0 5 0 1 0 0 3 0 0 1 " \
               "2 1 0 1 2 0 0 2 2 0 1 2\n"
        self.assertEqual(base, str(trtin))

    def test_set_geometry(self):
        trtin = xt.TrTin()
        pts = self.pts
        tris = self.tris
        tris_adj = self.tris_adj
        trtin.set_geometry(pts, tris, tris_adj)

        base = "22 serialization::archive 16 1 0\n0 0 0 0 0 5 0 0 0 0.00000000000000000e+00 " \
               "0.00000000000000000e+00 0.00000000000000000e+00 1.00000000000000000e+01 " \
               "0.00000000000000000e+00 0.00000000000000000e+00 2.00000000000000000e+01 " \
               "0.00000000000000000e+00 0.00000000000000000e+00 5.00000000000000000e+00 " \
               "1.00000000000000000e+01 0.00000000000000000e+00 1.50000000000000000e+01 " \
               "1.00000000000000000e+01 0.00000000000000000e+00 9 0 3 0 1 1 2 4 1 4 3 0 0 " \
               "5 0 1 0 0 3 0 0 1 2 1 0 1 2 0 0 2 2 0 1 2\n"
        self.assertEqual(base, str(trtin))

    def test_set_geometry_numpy(self):
        trtin = xt.TrTin()
        pts = self.pts_np
        tris = self.tris_np
        tris_adj = self.tris_adj_np
        trtin.set_geometry(pts, tris, tris_adj)

        base = "22 serialization::archive 16 1 0\n0 0 0 0 0 5 0 0 0 0.00000000000000000e+00 " \
               "0.00000000000000000e+00 0.00000000000000000e+00 1.00000000000000000e+01 " \
               "0.00000000000000000e+00 0.00000000000000000e+00 2.00000000000000000e+01 " \
               "0.00000000000000000e+00 0.00000000000000000e+00 5.00000000000000000e+00 " \
               "1.00000000000000000e+01 0.00000000000000000e+00 1.50000000000000000e+01 " \
               "1.00000000000000000e+01 0.00000000000000000e+00 9 0 3 0 1 1 2 4 1 4 3 0 0 " \
               "5 0 1 0 0 3 0 0 1 2 1 0 1 2 0 0 2 2 0 1 2\n"
        self.assertEqual(base, str(trtin))

    def test_get_pts(self):
        trtin = xt.TrTin()
        pts = self.pts
        trtin.set_points(pts)
        np.testing.assert_array_equal(pts, trtin.pts)

    def test_get_tris(self):
        trtin = xt.TrTin()
        tris = self.tris
        trtin.set_triangles(tris)
        np.testing.assert_array_equal(tris, trtin.tris)

    def test_get_tris_adj(self):
        trtin = xt.TrTin()
        tris_adj = self.tris_adj
        trtin.set_triangles_adjacent_to_points(tris_adj)
        self.assertEqual(tris_adj, trtin.tris_adj)

    def test_num_points(self):
        trtin = xt.TrTin()
        pts = self.pts_np
        trtin.set_points(pts)
        self.assertEqual(5, trtin.num_points)

    def test_num_triangles(self):
        trtin = xt.TrTin()
        tris = self.tris_np
        trtin.set_triangles(tris)
        self.assertEqual(3, trtin.num_triangles)

    def test_triangle_from_edge(self):
        trtin = xt.TrTin()
        trtin.set_geometry(self.pts, self.tris, self.tris_adj)
        rv, tri, idx1, idx2 = trtin.triangle_from_edge(1, 3)
        self.assertEqual(rv, True)
        self.assertEqual(tri, 0)
        self.assertEqual(idx1, 2)
        self.assertEqual(idx2, 0)
        rv, tri, idx1, idx2 = trtin.triangle_from_edge(3, 1)
        self.assertEqual(rv, True)
        self.assertEqual(tri, 2)
        self.assertEqual(idx1, 2)
        self.assertEqual(idx2, 0)
        rv, tri, idx1, idx2 = trtin.triangle_from_edge(1, 0)
        self.assertEqual(rv, False)

    def test_triangle_adjacent_to_edge(self):
        trtin = xt.TrTin()
        trtin.set_geometry(self.pts, self.tris, self.tris_adj)
        tri = trtin.triangle_adjacent_to_edge(1, 3)
        self.assertEqual(2, tri)
        tri = trtin.triangle_adjacent_to_edge(3, 1)
        self.assertEqual(0, tri)
        tri = trtin.triangle_adjacent_to_edge(1, 0)
        self.assertEqual(0, tri)

    def test_local_index(self):
        trtin = xt.TrTin()
        trtin.set_geometry(self.pts, self.tris, self.tris_adj)
        self.assertEqual(1, trtin.local_index(0, 0))
        self.assertEqual(2, trtin.local_index(0, 1))
        self.assertEqual(0, trtin.local_index(0, 3))
        self.assertEqual(-1, trtin.local_index(0, 4))

    def test_global_index(self):
        trtin = xt.TrTin()
        trtin.set_geometry(self.pts, self.tris, self.tris_adj)
        self.assertEqual(3, trtin.global_index(0, 0))
        self.assertEqual(0, trtin.global_index(0, 1))
        self.assertEqual(1, trtin.global_index(0, 2))
        self.assertEqual(-1, trtin.global_index(3, 2))

    def test_common_edge_index(self):
        trtin = xt.TrTin()
        trtin.set_geometry(self.pts, self.tris, self.tris_adj)
        self.assertEqual(2, trtin.common_edge_index(0, 2))
        self.assertEqual(2, trtin.common_edge_index(2, 0))
        self.assertEqual(2, trtin.common_edge_index(1, 2))
        self.assertEqual(0, trtin.common_edge_index(2, 1))
        self.assertEqual(-1, trtin.common_edge_index(0, 1))

    def test_adjacent_triangle(self):
        trtin = xt.TrTin()
        trtin.set_geometry(self.pts, self.tris, self.tris_adj)
        self.assertEqual(-1, trtin.adjacent_triangle(0, 0))
        self.assertEqual(-1, trtin.adjacent_triangle(0, 1))
        self.assertEqual(2, trtin.adjacent_triangle(0, 2))
        self.assertEqual(-1, trtin.adjacent_triangle(1, 0))
        self.assertEqual(-1, trtin.adjacent_triangle(1, 1))
        self.assertEqual(2, trtin.adjacent_triangle(1, 2))
        self.assertEqual(1, trtin.adjacent_triangle(2, 0))
        self.assertEqual(-1, trtin.adjacent_triangle(2, 1))
        self.assertEqual(0, trtin.adjacent_triangle(2, 2))

    def test_triangle_centroid(self):
        trtin = xt.TrTin()
        trtin.set_geometry(self.pts, self.tris, self.tris_adj)
        x0, y0, z0 = trtin.triangle_centroid(0)
        self.assertEqual(5, x0)
        self.assertAlmostEqual(3.333333333, y0, places=8)
        self.assertEqual(0, z0)
        x1, y1, z1 = trtin.triangle_centroid(1)
        self.assertEqual(15, x1)
        self.assertAlmostEqual(3.333333333, y1, places=8)
        self.assertEqual(0, z1)
        x2, y2, z2 = trtin.triangle_centroid(2)
        self.assertEqual(10, x2)
        self.assertAlmostEqual(6.666666666, y2, places=8)
        self.assertEqual(0, z2)

    def test_triangle_area(self):
        trtin = xt.TrTin()
        trtin.set_geometry(self.pts, self.tris, self.tris_adj)
        self.assertAlmostEqual(50.00000, trtin.triangle_area(0), places=4)
        self.assertAlmostEqual(50.00000, trtin.triangle_area(1), places=4)
        self.assertAlmostEqual(50.00000, trtin.triangle_area(2), places=4)

    def test_next_boundry_point(self):
        trtin = xt.TrTin()
        trtin.set_geometry(self.pts, self.tris, self.tris_adj)
        self.assertEqual(3, trtin.next_boundary_point(0))
        self.assertEqual(4, trtin.next_boundary_point(3))
        self.assertEqual(2, trtin.next_boundary_point(4))
        self.assertEqual(1, trtin.next_boundary_point(2))
        self.assertEqual(0, trtin.next_boundary_point(1))

    def test_previous_boundary_point(self):
        trtin = xt.TrTin()
        trtin.set_geometry(self.pts, self.tris, self.tris_adj)
        self.assertEqual(1, trtin.previous_boundary_point(0))
        self.assertEqual(2, trtin.previous_boundary_point(1))
        self.assertEqual(4, trtin.previous_boundary_point(2))
        self.assertEqual(3, trtin.previous_boundary_point(4))
        self.assertEqual(0, trtin.previous_boundary_point(3))

    def test_get_boundary_points(self):
        trtin = xt.TrTin()
        pts = ((0, 0, 0), (10, 0, 0), (20, 0, 0), (0, 10, 0), (10, 10, 0),
               (20, 10, 0), (0, 20, 0), (10, 20, 0), (20, 20, 0),)
        tris = (0, 1, 3, 1, 6, 3, 1, 4, 6, 4, 7, 6,
                1, 2, 4, 2, 7, 4, 2, 5, 7, 5, 8, 7)
        tris_adj = ((0,), (0, 1, 2, 4), (4, 5, 6), (0, 1), (2, 4, 5, 3),
                    (6, 7), (1, 2, 3), (3, 5, 6, 7), (7,))
        trtin.set_geometry(pts, tris, tris_adj)
        np.testing.assert_array_equal(
            np.array([0, 1, 2, 3, 5, 6, 7, 8]),
            trtin.get_boundary_points()
        )

    def test_get_boundary_polys(self):
        trtin = xt.TrTin()
        trtin.set_geometry(self.pts, self.tris, self.tris_adj)
        self.assertEqual(((0, 3, 4, 2, 1, 0),), trtin.get_boundary_polys())

    def test_get_extents(self):
        trtin = xt.TrTin()
        trtin.set_geometry(self.pts, self.tris, self.tris_adj)
        mn, mx = trtin.get_extents()
        self.assertEqual((0, 0, 0), mn)
        self.assertEqual((20, 10, 0), mx)

    def test_export_tin_file(self):
        trtin = xt.TrTin()
        trtin.set_geometry(self.pts, self.tris, self.tris_adj)
        trtin.export_tin_file("testfile.txt")
        self.assertTrue(os.path.isfile("testfile.txt"))

    def test_swap_edge(self):
        trtin = xt.TrTin()
        trtin.set_geometry(self.pts, self.tris, self.tris_adj)
        rv = trtin.swap_edge(0, 2, True)
        self.assertEqual(True, rv)
        self.assertEqual(trtin.tris[0], 3)
        self.assertEqual(trtin.tris[1], 0)
        self.assertEqual(trtin.tris[2], 4)
        self.assertEqual(trtin.tris[3], 1)
        self.assertEqual(trtin.tris[4], 2)
        self.assertEqual(trtin.tris[5], 4)
        self.assertEqual(trtin.tris[6], 1)
        self.assertEqual(trtin.tris[7], 4)
        self.assertEqual(trtin.tris[8], 0)

        tris_adj = trtin.tris_adj
        self.assertEqual((0, 2), tris_adj[0])
        self.assertEqual((1, 2), tris_adj[1])
        self.assertEqual((1,), tris_adj[2])
        self.assertEqual((0,), tris_adj[3])
        self.assertEqual((1, 2, 0), tris_adj[4])

    def test_delete_triangles(self):
        # TODO: I don't think this is right.
        trtin = xt.TrTin()
        trtin.set_geometry(self.pts, self.tris, self.tris_adj)
        to_delete = (1, 0)
        trtin.delete_triangles(to_delete)
        self.assertEqual(5, trtin.num_points)
        self.assertEqual(1, trtin.num_triangles)

    def test_delete_points(self):
        trtin = xt.TrTin()
        trtin.set_geometry(self.pts, self.tris, self.tris_adj)
        to_delete = (2, 4)
        trtin.delete_points(to_delete)
        self.assertEqual(3, trtin.num_points)
        self.assertEqual(1, trtin.num_triangles)

    def test_optimize_triangulation(self):
        trtin = xt.TrTin()
        pts = ((0, 0, 0), (10, 0, 0), (20, 0, 0), (0, 10, 0), (10, 10, 0),
               (20, 10, 0), (0, 20, 0), (10, 20, 0), (20, 20, 0),)
        tris = (0, 1, 3, 1, 6, 3, 1, 4, 6, 4, 7, 6,
                1, 2, 4, 2, 7, 4, 2, 5, 7, 5, 8, 7)
        tris_adj = ((0,), (0, 1, 2, 4), (4, 5, 6), (0, 1), (2, 4, 5, 3),
                    (6, 7), (1, 2, 3), (3, 5, 6, 7), (7,))
        trtin.set_geometry(pts, tris, tris_adj)
        trtin.optimize_triangulation()
        np.testing.assert_array_equal(np.array(tris), trtin.tris)

    def test_build_tris_adj_to_pts(self):
        trtin = xt.TrTin()
        trtin.set_geometry(self.pts, self.tris, self.tris_adj)
        trtin.build_tris_adj_to_pts()
        self.assertEqual(((0,), (0, 1, 2), (1,), (0, 2), (1, 2)), trtin.tris_adj)

    def test_clear(self):
        trtin = xt.TrTin()
        trtin.set_geometry(self.pts, self.tris, self.tris_adj)
        np.testing.assert_array_equal(self.pts_np, trtin.pts)
        np.testing.assert_array_equal(self.tris_np, trtin.tris)
        self.assertEqual(self.tris_adj, trtin.tris_adj)
        trtin.clear()
        self.assertEqual(0, len(trtin.pts))
        self.assertEqual(0, len(trtin.tris))
        self.assertEqual((), trtin.tris_adj)

    def test_from_string(self):
        trtin = xt.TrTin()

        inpt = "22 serialization::archive 16 1 0\n0 0 0 0 0 5 0 0 0 0.00000000000000000e+00 " \
               "0.00000000000000000e+00 0.00000000000000000e+00 1.00000000000000000e+01 " \
               "0.00000000000000000e+00 0.00000000000000000e+00 2.00000000000000000e+01 " \
               "0.00000000000000000e+00 0.00000000000000000e+00 5.00000000000000000e+00 " \
               "1.00000000000000000e+01 0.00000000000000000e+00 1.50000000000000000e+01 " \
               "1.00000000000000000e+01 0.00000000000000000e+00 9 0 3 0 1 1 2 4 1 4 3 0 0 " \
               "5 0 1 0 0 3 0 0 1 2 1 0 1 2 0 0 2 2 0 1 2\n"

        trtin.from_string(inpt)

        np.testing.assert_array_equal(self.pts_np, trtin.pts)
        np.testing.assert_array_equal(self.tris_np, trtin.tris)
        self.assertEqual(self.tris_adj, trtin.tris_adj)
        self.assertEqual(inpt, str(trtin))
