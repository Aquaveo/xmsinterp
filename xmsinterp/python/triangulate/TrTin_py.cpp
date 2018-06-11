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
          BSHP<xms::VecPt3d> vec_pts(new xms::VecPt3d());
          for (auto item : pts) {
            if(!py::isinstance<py::iterable>(item)) {
              throw py::type_error("First arg must be a n-tuple of 3-tuples");
            }

            py::tuple tuple = item.cast<py::tuple>();
            if (py::len(tuple) != 3) {
              throw py::type_error("Input points must be 3-tuples");
            } else {
              xms::Pt3d point(tuple[0].cast<double>(), tuple[1].cast<double>(), tuple[2].cast<double>());
              vec_pts->push_back(point);
            }
          }
          self.SetPoints(vec_pts);
      })
      .def("set_triangles", [](xms::TrTin &self, py::iterable tris) {
          BSHP<xms::VecInt> vec_tris(new xms::VecInt());
          for (auto item : tris) {
            vec_tris->push_back(item.cast<int>());
          }
          self.SetTriangles(vec_tris);
      })
      .def("set_triangles_adjacent_to_points", [](xms::TrTin &self, py::iterable tris_adj) {
          BSHP<xms::VecInt2d> vec_tris_adj(new xms::VecInt2d(py::len(tris_adj)));
          auto sizes = py::tuple(py::len(tris_adj));
          int j = 0;
          for (auto item : tris_adj) {
            if(!py::isinstance<py::iterable>(item)) {
              throw py::type_error("First arg must be a n-tuple of n-tuples");
            }

            py::tuple tuple = item.cast<py::iterable>();
            xms::VecInt inner_vec(py::len(tuple));
            for (int i = 0; i < py::len(tuple); i++) {
              double point(tuple[i].cast<double>());
              inner_vec[i] = point;
            }
            sizes[j] = inner_vec.size();
            vec_tris_adj->at(j) = inner_vec;
            j++;
          }
          self.SetTrianglesAdjacentToPoints(vec_tris_adj);
      })
      .def("set_geometry", [](xms::TrTin &self, py::iterable pts, py::iterable tris, py::iterable tris_adj) {
          BSHP<xms::VecPt3d> vec_pts(new xms::VecPt3d());
          for (auto item : pts) {
            if(!py::isinstance<py::iterable>(item)) {
              throw py::type_error("First arg must be a n-tuple of 3-tuples");
            }

            py::tuple tuple = item.cast<py::iterable>();
            if (py::len(tuple) != 3) {
              throw py::type_error("Input points must be 3-tuples");
            } else {
              xms::Pt3d point(tuple[0].cast<double>(), tuple[1].cast<double>(), tuple[2].cast<double>());
              vec_pts->push_back(point);
            }
          }
          BSHP<xms::VecInt> vec_tris(new xms::VecInt());
          for (auto item : tris) {
            vec_tris->push_back(item.cast<int>());
          }
          BSHP<xms::VecInt2d> vec_tris_adj(new xms::VecInt2d(py::len(tris_adj)));
          auto sizes = py::tuple(py::len(tris_adj));
          int j = 0;
          for (auto item : tris_adj) {
            if(!py::isinstance<py::iterable>(item)) {
              throw py::type_error("First arg must be a n-tuple of n-tuples");
            }

            py::tuple tuple = item.cast<py::iterable>();
            xms::VecInt inner_vec(py::len(tuple));
            for (int i = 0; i < py::len(tuple); i++) {
              double point(tuple[i].cast<double>());
              inner_vec[i] = point;
            }
            sizes[j] = inner_vec.size();
            vec_tris_adj->at(j) = inner_vec;
            j++;
          }
          self.SetGeometry(vec_pts, vec_tris, vec_tris_adj);
      })
      .def_property_readonly("pts", [](xms::TrTin &self) -> py::iterable {
          xms::VecPt3d pts = self.Points();
          py::array_t<double, py::array::c_style> a({(int)pts.size(), 3});
          auto r = a.mutable_unchecked<2>();
          int i = 0;
          for (ssize_t i = 0; i < r.shape(0); i++) {
           r(i, 0) = pts[i].x;
           r(i, 1) = pts[i].y;
           r(i, 2) = pts[i].z;
          }
          return a;
      })
      .def_property_readonly("tris", [](xms::TrTin &self) -> py::iterable {
          xms::VecInt tris = self.Triangles();
          return py::array(tris.size(), tris.data());
      })
      .def_property_readonly("tris_adj", [](xms::TrTin &self) -> py::iterable {
          xms::VecInt2d tris_adj = self.TrisAdjToPts();
          std::vector<py::iterable> inners(tris_adj.size());

          for (int i = 0; i < tris_adj.size(); i++) {
              auto inner = py::tuple(tris_adj.at(i).size());
              for (int j = 0; j < tris_adj.at(i).size(); j++) {
                inner[j] = tris_adj.at(i).at(j);
              }
              inners.at(i) = inner;
          }

          auto tuple = py::tuple(inners.size());
          for (int i = 0; i < inners.size(); i++) {
              tuple[i] = inners.at(i);
          }
          return tuple;

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
          return py::make_tuple(pt.x, pt.y, pt.z);
      })
      .def("triangle_area", &xms::TrTin::TriangleArea)
      .def("next_boundary_point", &xms::TrTin::NextBoundaryPoint)
      .def("previous_boundary_point", &xms::TrTin::PreviousBoundaryPoint)
      .def("get_boundary_points", [](xms::TrTin &self) -> py::iterable {
          xms::VecInt bp;
          self.GetBoundaryPoints(bp);
          return py::array(bp.size(), bp.data());
      })
      .def("get_boundary_polys", [](xms::TrTin &self) -> py::tuple {
          xms::VecInt2d bp;
          self.GetBoundaryPolys(bp);
          std::vector<py::iterable> inners(bp.size());

          for (int i = 0; i < bp.size(); i++) {
              auto inner = py::tuple(bp.at(i).size());
              for (int j = 0; j < bp.at(i).size(); j++) {
                inner[j] = bp.at(i).at(j);
              }
              inners.at(i) = inner;
          }

          auto tuple = py::tuple(inners.size());
          for (int i = 0; i < inners.size(); i++) {
              tuple[i] = inners.at(i);
          }
          return tuple;
      })
      .def("get_extents", [](xms::TrTin &self) -> py::tuple {
          xms::Pt3d pt_min, pt_max;
          self.GetExtents(pt_min, pt_max);
          auto min_pt = py::make_tuple(pt_min.x, pt_min.y, pt_min.z);
          auto max_pt = py::make_tuple(pt_max.x, pt_max.y, pt_max.z);
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