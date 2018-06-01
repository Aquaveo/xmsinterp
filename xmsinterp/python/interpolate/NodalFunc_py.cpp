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
#include <xmsinterp/interpolate/detail/NodalFunc.h>
#include <xmsinterp/python/interpolate/interpolate_py.h>
#include <xmsinterp/geometry/GmTriSearch.h>
#include <xmscore/stl/vector.h>

//----- Namespace declaration --------------------------------------------------
namespace py = pybind11;

//----- Python Interface -------------------------------------------------------
PYBIND11_DECLARE_HOLDER_TYPE(T, boost::shared_ptr<T>);

void initNodalFunc(py::module &m) {
    py::class_<xms::NodalFunc>(m, "NodalFunc")
    .def(py::init([](int type, bool a2d, py::tuple pts,
           py::tuple scaler, int nnearest, bool quad_oct, double power,
           xms::Observer &p, xms::InterpNatNeigh &nat_neigh) {
        BSHP<xms::VecPt3d> vec_pts(new xms::VecPt3d());
        BSHP<xms::GmTriSearch> ts = xms::GmTriSearch::New();
        std::vector<float> vec_scalar;
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
        for (auto item : scalar)   {
          vec_scalar.push_back(item.cast<float>());
        }
        ts->TrisToSearch(vec_pts, vec_tris);
        return xms::NodalFunc::New(type, a2d, ts, vec_pts, vec_scalar,
                                    nnearest, quad_oct, power, p, *nat_neigh)
    }))
    ;
}  // initInterpNatNeigh(py::module &m)