//------------------------------------------------------------------------------
/// \file
/// \brief
/// \copyright (C) Copyright Aquaveo 2018. Distributed under FreeBSD License
/// (See accompanying file LICENSE or https://aqaveo.com/bsd/license.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <boost/shared_ptr.hpp>
#include <xmscore/python/misc/PyUtils.h>
#include <xmsinterp/triangulate/TrTin.h>
#include <iostream>
#include <fstream>


//----- Namespace declaration --------------------------------------------------
namespace py = pybind11;

//----- Python Interface -------------------------------------------------------
PYBIND11_DECLARE_HOLDER_TYPE(T, boost::shared_ptr<T>);

void initTrTin(py::module &m) {

      // Class
      py::class_<xms::TrTin, boost::shared_ptr<xms::TrTin>> iTrTin(m, "TrTin");
      iTrTin.def(py::init(&xms::TrTin::New))
      .def("set_points", [](xms::TrTin &self, py::iterable pts) {
          BSHP<xms::VecPt3d> vec_pts = xms::VecPt3dFromPyIter(pts);
          self.SetPoints(vec_pts);
      },"Sets the tin points.", py::arg("vec_pts")
      )
      .def("set_triangles", [](xms::TrTin &self, py::iterable tris) {
          BSHP<xms::VecInt> vec_tris = xms::VecIntFromPyIter(tris);
          self.SetTriangles(vec_tris);
      },"Sets the tin triangles.",py::arg("vec_tris_adj")
      )
      .def("set_triangles_adjacent_to_points", [](xms::TrTin &self, py::iterable tris_adj) {
          BSHP<xms::VecInt2d> vec_tris_adj = xms::VecInt2dFromPyIter(tris_adj);
          self.SetTrianglesAdjacentToPoints(vec_tris_adj);
      },"Sets the adjacency info of triangles adjacent to points.",py::arg("vec_tris_adj")
      )
      .def("set_geometry", [](xms::TrTin &self, py::iterable pts, py::iterable tris, py::iterable tris_adj) {
          boost::shared_ptr<xms::VecPt3d> vec_pts = xms::VecPt3dFromPyIter(pts);
          boost::shared_ptr<xms::VecInt> vec_tris = xms::VecIntFromPyIter(tris);
          boost::shared_ptr<xms::VecInt2d> vec_tris_adj = xms::VecInt2dFromPyIter(tris_adj);
          self.SetGeometry(vec_pts, vec_tris, vec_tris_adj);
      },"Set all the tin geometry at once (points, triangles, adjacency).",
        py::arg("vec_pts"), py::arg("vec_tris"), py::arg("vec_tris_adj")
      )
      .def_property_readonly("pts", [](xms::TrTin &self) -> py::iterable {
          xms::VecPt3d pts = self.Points();
          return xms::PyIterFromVecPt3d(pts);
      },"Return the tin points."
      )
      .def_property_readonly("tris", [](xms::TrTin &self) -> py::iterable {
          xms::VecInt tris = self.Triangles();
          return xms::PyIterFromVecInt(tris);
      },"Return 0-based indices of triangle points (grouped by 3s)."
      )
      .def_property_readonly("tris_adj", [](xms::TrTin &self) -> py::iterable {
          xms::VecInt2d tris_adj = self.TrisAdjToPts();
          return xms::PyIterFromVecInt2d(tris_adj);
      },"Returns triangles adjacent to points (0-based)."
      )
      .def_property_readonly("num_points", &xms::TrTin::NumPoints,"Return the number of points.")
      .def_property_readonly("num_triangles", &xms::TrTin::NumTriangles,"Return the number of triangles.")
      .def("triangle_from_edge", [](xms::TrTin &self, int pt1, int pt2) -> py::tuple {
          int tri, localPt1, localPt2;
          bool ret = self.TriangleFromEdge(pt1, pt2, tri, localPt1, localPt2);
          return py::make_tuple(ret, tri, localPt1, localPt2);
      },"Finds the triangle with the edge defined by a_pt1 and a_pt2 and the local index of those" 
        " points. Compare to trTinTriFromEdge.",
        py::arg("pt1"), py::arg("pt2")
      )
      .def("triangle_adjacent_to_edge", [](xms::TrTin &self, int pt1, int pt2) -> int {
          return self.TriangleAdjacentToEdge(pt1, pt2);
      },"Returns the triangle adjacent to the edge defined by a_pt1 and a_pt2. Compare to"
        " trTriangleAdjacentToEdge.", py::arg("pt1"), py::arg("pt2")
      )
      .def("local_index", &xms::TrTin::LocalIndex,"Returns index (0-2) of point within triangle given"
        " global index. Compare to trIndex.",py::arg("tri"), py::arg("pt"))
      .def("global_index", &xms::TrTin::GlobalIndex,"Returns index into m_pts of a_localPt which is 0-2.",
        py::arg("tri_idx"), py::arg("local_pt")
      )
      .def("vertices_are_adjacent", &xms::TrTin::VerticesAreAdjacent,"Return true if vertices form"
        " the edge of a triangle. Compare to vrVerticesAreAdjacent.", py::arg("pt1"),py::arg("pt2")    
      )
      .def("common_edge_index", &xms::TrTin::CommonEdgeIndex,"Return index of common edge between"
        " triangle and neighbor. Edge index is 0-2 based on a_tri. Compare to trCommonEdgeIndex.",
        py::arg("tri"), py::arg("adj_tri")
      )
      .def("adjacent_triangle", &xms::TrTin::AdjacentTriangle,"Returns the triangle adjacent" 
        "to a_triIdx across a_edgeIdx (0-2). Compare to trAdjacentTriangle.",
        py::arg("tri_idx"), py::arg("edge_idx")
      )
      .def("triangle_centroid", [](xms::TrTin &self, int tri) -> py::tuple {
          xms::Pt3d pt = self.TriangleCentroid(tri);
          return xms::PyIterFromPt3d(pt);
      },"Calculate and return the centroid of a triangle.",py::arg("tri")
      )
      .def("triangle_area", &xms::TrTin::TriangleArea,"Calculate and return the area of a triangle.",
        py::arg("tri")
      )
      .def("next_boundary_point", &xms::TrTin::NextBoundaryPoint,"Returns the next point CW from"
        " point on the boundary. CCW if in an inside hole. Compare to trNextBoundaryVertex (or"
        " trPreviousBoundaryVertex since order here is CW, not CCW).",
        py::arg("point")
      )
      .def("previous_boundary_point", &xms::TrTin::PreviousBoundaryPoint,"Returns the next point CCW"
        " from point on the boundary. CW if in an inside hole. Compare to trPreviousBoundaryVertex"
        " (or trNextBoundaryVertex since order here is CW, not CCW).",py::arg("point")  
      )
      .def("get_boundary_points", [](xms::TrTin &self) -> py::iterable {
          xms::VecInt bp;
          self.GetBoundaryPoints(bp);
          return xms::PyIterFromVecInt(bp);
      },"Gives the 0-based indices of all points on any boundary, in no particular order.")
      .def("get_boundary_polys", [](xms::TrTin &self) -> py::tuple {
          xms::VecInt2d bp;
          self.GetBoundaryPolys(bp);
          return xms::PyIterFromVecInt2d(bp);
      },"Gets exterior boundary and all interior voids as polygons of 0-based point indices."
        " First point is not repeated as the last point."
      )
      .def("get_extents", [](xms::TrTin &self) -> py::tuple {
          xms::Pt3d pt_min, pt_max;
          self.GetExtents(pt_min, pt_max);
          auto min_pt = xms::PyIterFromPt3d(pt_min);
          auto max_pt = xms::PyIterFromPt3d(pt_max);
          return py::make_tuple(min_pt, max_pt);
      },"Computes the extents (min, max) of the tin.")
      .def("export_tin_file", [](xms::TrTin &self, std::string fname) {
          // TODO: This needs some error checking
          std::filebuf fb;
          fb.open(fname, std::ios::out);
          std::ostream os(&fb);
          self.ExportTinFile(os);
          fb.close();
      },"Export in the .tin file format. Useful for debugging.",py::arg("fname")
      )
      .def("swap_edge", &xms::TrTin::SwapEdge,"Swap edges if triangles combine to form convex quad."
        " Compare to trSwapEdge.",py::arg("tri_A"), py::arg("tri_B"), py::arg("check_angle")
      )
      .def("delete_triangles", [](xms::TrTin &self, py::iterable tris) {
          xms::SetInt to_delete;
          for (auto item : tris) {
            to_delete.insert(item.cast<int>());
          }
          self.DeleteTriangles(to_delete);
      },"Deletes the triangles specified in \a a_trisToDelete and updates and renumbers triangle"
        " adjacency info.",py::arg("tris")
      )
      .def("delete_points", [](xms::TrTin &self, py::iterable pts) {
          xms::SetInt to_delete;
          for (auto item : pts) {
            to_delete.insert(item.cast<int>());
          }
          self.DeletePoints(to_delete);
      },"Deletes the points and any attached triangles, updates adjacency and renumbers things.",
        py::arg("pts")
      )
      .def("optimize_triangulation", &xms::TrTin::OptimizeTriangulation,
        "Swaps triangle edges until they are a Delauney triangulation."
      )
      .def("build_tris_adj_to_pts", &xms::TrTin::BuildTrisAdjToPts,
        "Build array of triangles adjacent to points."
      )
      .def("clear", &xms::TrTin::Clear,"Delete the memory.")
      .def("from_string", &xms::TrTin::FromString,"Use boost archive to turn the text into a TrTin.")
      .def("__str__", &xms::TrTin::ToString,"Use boost archive to get the TrTin as text.")
      ;
}