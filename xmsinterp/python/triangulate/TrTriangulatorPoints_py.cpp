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
#include <xmsinterp/triangulate/TrTriangulatorPoints.h>
#include <iostream>
#include <fstream>


//----- Namespace declaration --------------------------------------------------
namespace py = pybind11;

//----- Python Interface -------------------------------------------------------
PYBIND11_DECLARE_HOLDER_TYPE(T, boost::shared_ptr<T>);

void initTrTriangulatorPoints(py::module &m) {

      py::module modMeshUtils = m.def_submodule("TrTriangulatorPoints")
      .def("triangulate", [](py::iterable pts, py::iterable tris, py::iterable tris_adj) -> py::iterable {
          boost::shared_ptr<xms::VecPt3d> vec_pts = xms::VecPt3dFromPyIter(pts);
          boost::shared_ptr<xms::VecInt> vec_tris = xms::VecIntFromPyIter(tris);
          boost::shared_ptr<xms::VecInt2d> vec_tris_adj = xms::VecInt2dFromPyIter(tris_adj);
          xms::TrTriangulatorPoints triangulator(*vec_pts, *vec_tris, &(*vec_tris_adj));
          triangulator.Triangulate();

          // Return pts, tris, and tris_adj
          return py::make_tuple(xms::PyIterFromVecPt3d(*vec_pts),
                                xms::PyIterFromVecInt(*vec_tris), xms::PyIterFromVecInt2d(*vec_tris_adj));
      })
      ;
}