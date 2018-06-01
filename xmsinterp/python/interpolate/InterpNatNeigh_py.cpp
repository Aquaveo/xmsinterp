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
#include <xmsinterp/interpolate/detail/InterpNatNeigh.h>
#include <xmsinterp/python/interpolate/interpolate_py.h>
#include <xmsinterp/geometry/GmTriSearch.h>

//----- Namespace declaration --------------------------------------------------
namespace py = pybind11;

//----- Python Interface -------------------------------------------------------
PYBIND11_DECLARE_HOLDER_TYPE(T, boost::shared_ptr<T>);

void initInterpNatNeigh(py::module &m) {
    py::class_<xms::InterpNatNeigh, boost::shared_ptr<xms::InterpNatNeigh>>(m, "InterpNatNeigh")
    .def(py::init([](py::tuple pts, py::tuple tris, py::tuple scalar) {
        BSHP<xms::VecPt3d> vec_pts(new xms::VecPt3d());
        BSHP<xms::VecInt> vec_tris(new xms::VecInt());
        std::vector<float> vec_scalar;
        BSHP<xms::GmTriSearch> ts = xms::GmTriSearch::New();
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
        for (auto item : scalar)   {
          vec_scalar.push_back(item.cast<float>());
        }
        ts->TrisToSearch(vec_pts, vec_tris);
        return xms::InterpNatNeigh::New(*vec_pts, *vec_tris, vec_scalar, ts.get());
    }))
    .def("interp_to_pt",[](xms::InterpNatNeigh &self, py::tuple pt) -> float {
        if(py::len(pt) != 3) {
          throw py::type_error("Input point should be a 3-tuple");
        } else {
          xms::Pt3d point(pt[0].cast<double>(), pt[1].cast<double>(), pt[2].cast<double>());
          return self.InterpToPt(point);
        }
    })
    .def("get_neighbors",[](xms::InterpNatNeigh &self, int pt_idx, py::tuple &neigh) {
        if(py::len(neigh) != 0) {
          throw py::type_error("Input tuple should be empty");
        }
        std::vector<int> tIdxs;
        self.GetNeighbors(pt_idx, tIdxs);
        neigh = py::make_tuple(tIdxs);
    })
    .def("recalc_nodal_func", &xms::InterpNatNeigh::RecalcNodalFunc)
    .def("set_blend_weights", &xms::InterpNatNeigh::SetBlendWeights)
    .def("__str__", &xms::InterpNatNeigh::ToString)
    .def("to_string", &xms::InterpNatNeigh::ToString)
    ;
}  // initInterpNatNeigh(py::module &m)