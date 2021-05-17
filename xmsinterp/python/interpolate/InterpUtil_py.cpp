//------------------------------------------------------------------------------
/// \file
/// \brief
/// \copyright (C) Copyright Aquaveo 2021. Distributed under FreeBSD License
/// (See accompanying file LICENSE or https://aqaveo.com/bsd/license.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <xmscore/misc/boost_defines.h> // BSHP
#include <xmscore/python/misc/PyUtils.h> // VecPt3dFrom*
#include <xmsinterp/interpolate/InterpUtil.h> // inGenerateRasterIdw

//----- Namespace declaration --------------------------------------------------
namespace py = pybind11;

//----- Python Interface -------------------------------------------------------
PYBIND11_DECLARE_HOLDER_TYPE(T, boost::shared_ptr<T>);

void initInterpUtil(py::module& m)
{
  py::module modInterpUtil = m.def_submodule("InterpUtil");

  // ---------------------------------------------------------------------------
  // function: size_function_from_depth
  // ---------------------------------------------------------------------------
  modInterpUtil.def(
    "generate_raster_idw",
    [](py::iterable pts, py::tuple min_pt, py::tuple size, int num_rows, int num_cols, float no_data_val, py::iterable activity) -> py::iterable {
      auto vec_pts = *xms::VecPt3dFromPyIter(pts);
      auto pt_min_pt = xms::Pt3dFromPyIter(min_pt);
      auto pt_size = xms::Pt3dFromPyIter(size);
      auto vec_activity = *xms::VecIntFromPyIter(activity);
      auto vals = xms::inGenerateRasterIdw(vec_pts, pt_min_pt, pt_size, num_rows, num_cols, no_data_val, vec_activity);
      return xms::PyIterFromVecFlt(vals);
    },
    py::arg("pts"), py::arg("min_pt"), py::arg("size"), py::arg("num_rows"), py::arg("num_cols"),
    py::arg("no_data_val"), py::arg("activity"));
}