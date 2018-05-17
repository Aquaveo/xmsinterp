//------------------------------------------------------------------------------
/// \file
/// \brief
/// \copyright (C) Copyright Aquaveo 2018. Distributed under the xmsng
///  Software License, Version 1.0. (See accompanying file
///  LICENSE_1_0.txt or copy at http://www.aquaveo.com/xmsng/LICENSE_1_0.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------
#include <pybind11/pybind11.h>
#include <boost/shared_ptr.hpp>
#include <xmsinterp/interpolate/InterpIdw.h>
#include <xmsinterp/python/interpolate/interpolate_py.h>

//----- Namespace declaration --------------------------------------------------
namespace py = pybind11;

//----- Python Interface -------------------------------------------------------
PYBIND11_DECLARE_HOLDER_TYPE(T, boost::shared_ptr<T>);

void initInterpIdw(py::module &m) {
      py::class_<xms::InterpIdw, boost::shared_ptr<xms::InterpIdw>>(m, "InterpIdw")
          .def(py::init(&xms::InterpIdw::New))
          .def("__str__", &xms::InterpIdw::ToString)
          .def("to_string", &xms::InterpIdw::ToString)
          .def_property("id_string", &xms::InterpIdw::GetIdString, &xms::InterpIdw::SetIdString)
          .def("set_pts_tris", [](xms::InterpIdw &self, py::tuple pts, py::tuple tris) {
              BSHP<xms::VecPt3d> vec_pts(new xms::VecPt3d());
              BSHP<xms::VecInt> vec_tris(new xms::VecInt());
              for (auto item : pts) {
                if(!py::isinstance<py::tuple>(item)) {
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
              for (auto item : tris) {
                vec_tris->push_back(item.cast<int>());
              }
              self.SetPtsTris(vec_pts, vec_tris);
          })
          .def("interp_to_pt",[](xms::InterpIdw &self, py::tuple pt) -> float {
              if(py::len(pt) != 3) {
                throw py::type_error("Input point should be a 3-tuple");
              } else {
                xms::Pt3d point(pt[0].cast<double>(), pt[1].cast<double>(), pt[2].cast<double>());
                return self.InterpToPt(point);
              }
          });
}
