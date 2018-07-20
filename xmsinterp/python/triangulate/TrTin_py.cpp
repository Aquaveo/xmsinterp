//------------------------------------------------------------------------------
/// \file
/// \brief
/// \copyright (C) Copyright Aquaveo 2018. Distributed under the xmsng
///  Software License, Version 1.0. (See accompanying file
///  LICENSE_1_0.txt or copy at http://www.aquaveo.com/xmsng/LICENSE_1_0.txt)
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
      })
      .def("set_triangles", [](xms::TrTin &self, py::iterable tris) {
          BSHP<xms::VecInt> vec_tris = xms::VecIntFromPyIter(tris);
          self.SetTriangles(vec_tris);
      })
      .def("set_triangles_adjacent_to_points", [](xms::TrTin &self, py::iterable tris_adj) {
          BSHP<xms::VecInt2d> vec_tris_adj = xms::VecInt2dFromPyIter(tris_adj);
          self.SetTrianglesAdjacentToPoints(vec_tris_adj);
      })
      .def("set_geometry", [](xms::TrTin &self, py::iterable pts, py::iterable tris, py::iterable tris_adj) {
          boost::shared_ptr<xms::VecPt3d> vec_pts = xms::VecPt3dFromPyIter(pts);
          boost::shared_ptr<xms::VecInt> vec_tris = xms::VecIntFromPyIter(tris);
          boost::shared_ptr<xms::VecInt2d> vec_tris_adj = xms::VecInt2dFromPyIter(tris_adj);
          self.SetGeometry(vec_pts, vec_tris, vec_tris_adj);
      })
      .def_property_readonly("pts", [](xms::TrTin &self) -> py::iterable {
          xms::VecPt3d pts = self.Points();
          return xms::PyIterFromVecPt3d(pts);
      })
      .def_property_readonly("tris", [](xms::TrTin &self) -> py::iterable {
          xms::VecInt tris = self.Triangles();
          return xms::PyIterFromVecInt(tris);
      })
      .def_property_readonly("tris_adj", [](xms::TrTin &self) -> py::iterable {
          xms::VecInt2d tris_adj = self.TrisAdjToPts();
          return xms::PyIterFromVecInt2d(tris_adj);
      })
      .def_property_readonly("num_points", &xms::TrTin::NumPoints)
      .def_property_readonly("num_triangles", &xms::TrTin::NumTriangles)
      .def("triangle_from_edge", [](xms::TrTin &self, int pt1, int pt2) -> py::tuple {
          int tri, localPt1, localPt2;
          bool ret = self.TriangleFromEdge(pt1, pt2, tri, localPt1, localPt2);
          return py::make_tuple(ret, tri, localPt1, localPt2);
      })
      .def("triangle_adjacent_to_edge", [](xms::TrTin &self, int pt1, int pt2) -> int {
          return self.TriangleAdjacentToEdge(pt1, pt2);
      })
      .def("local_index", &xms::TrTin::LocalIndex)
      .def("global_index", &xms::TrTin::GlobalIndex)
      .def("vertices_are_adjacent", &xms::TrTin::VerticesAreAdjacent)
      .def("common_edge_index", &xms::TrTin::CommonEdgeIndex)
      .def("adjacent_triangle", &xms::TrTin::AdjacentTriangle)
      .def("triangle_centroid", [](xms::TrTin &self, int tri) -> py::tuple {
          xms::Pt3d pt = self.TriangleCentroid(tri);
          return xms::PyIterFromPt3d(pt);
      })
      .def("triangle_area", &xms::TrTin::TriangleArea)
      .def("next_boundary_point", &xms::TrTin::NextBoundaryPoint)
      .def("previous_boundary_point", &xms::TrTin::PreviousBoundaryPoint)
      .def("get_boundary_points", [](xms::TrTin &self) -> py::iterable {
          xms::VecInt bp;
          self.GetBoundaryPoints(bp);
          return xms::PyIterFromVecInt(bp);
      })
      .def("get_boundary_polys", [](xms::TrTin &self) -> py::tuple {
          xms::VecInt2d bp;
          self.GetBoundaryPolys(bp);
          return xms::PyIterFromVecInt2d(bp);
      })
      .def("get_extents", [](xms::TrTin &self) -> py::tuple {
          xms::Pt3d pt_min, pt_max;
          self.GetExtents(pt_min, pt_max);
          auto min_pt = xms::PyIterFromPt3d(pt_min);
          auto max_pt = xms::PyIterFromPt3d(pt_max);
          return py::make_tuple(min_pt, max_pt);
      })
      .def("export_tin_file", [](xms::TrTin &self, std::string fname) {
          // TODO: This needs some error checking
          std::filebuf fb;
          fb.open(fname, std::ios::out);
          std::ostream os(&fb);
          self.ExportTinFile(os);
          fb.close();
      })
      .def("swap_edge", &xms::TrTin::SwapEdge)
      .def("delete_triangles", [](xms::TrTin &self, py::iterable tris) {
          xms::SetInt to_delete;
          for (auto item : tris) {
            to_delete.insert(item.cast<int>());
          }
          self.DeleteTriangles(to_delete);
      })
      .def("delete_points", [](xms::TrTin &self, py::iterable pts) {
          xms::SetInt to_delete;
          for (auto item : pts) {
            to_delete.insert(item.cast<int>());
          }
          self.DeletePoints(to_delete);
      })
      .def("optimize_triangulation", &xms::TrTin::OptimizeTriangulation)
      .def("build_tris_adj_to_pts", &xms::TrTin::BuildTrisAdjToPts)
      .def("clear", &xms::TrTin::Clear)
      .def("from_string", &xms::TrTin::FromString)
      .def("__str__", &xms::TrTin::ToString)
      ;
}