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
#include <xmscore/misc/DynBitset.h>
#include <xmscore/python/misc/PyUtils.h>
#include <xmscore/python/misc/PublicObserver.h>
#include <xmsinterp/interpolate/InterpAnisotropic.h>
#include <xmsinterp/python/interpolate/interpolate_py.h>

//----- Namespace declaration --------------------------------------------------
namespace py = pybind11;

//----- Python Interface -------------------------------------------------------
PYBIND11_DECLARE_HOLDER_TYPE(T, boost::shared_ptr<T>);

void initInterpAnisotropic(py::module &m) {

    py::class_<xms::InterpAnisotropic, boost::shared_ptr<xms::InterpAnisotropic>> iInterpAnisotropic(m, "InterpAnisotropic");

    iInterpAnisotropic.def(py::init(&xms::InterpAnisotropic::New));
  // ---------------------------------------------------------------------------
  // function: SetPoints
  // ---------------------------------------------------------------------------
  iInterpAnisotropic.def("SetPoints", [](xms::InterpAnisotropic &self, py::iterable centerline_pts, 
    py::iterable interpolation_pts, bool pick_closest) {
      BSHP<xms::VecPt3d> vecCenterlinePts = xms::VecPt3dFromPyIter(centerline_pts);
      BSHP<xms::VecPt3d> vecInterpolationPts = xms::VecPt3dFromPyIter(interpolation_pts);
      self.SetPoints(*vecCenterlinePts, *vecInterpolationPts, pick_closest);
  }, py::arg("centerline_pts"), py::arg("interpolation_pts"), py::arg("pick_closest"));
  // ---------------------------------------------------------------------------
  // function: InterpToPt
  // ---------------------------------------------------------------------------
  iInterpAnisotropic.def("InterpToPt", [](xms::InterpAnisotropic &self, py::iterable pt) -> float {
      xms::Pt3d pt3d = xms::Pt3dFromPyIter(pt);
      return self.InterpToPt(pt3d);
  }, py::arg("pt"));
  // ---------------------------------------------------------------------------
  // function: InterpToPts
  // ---------------------------------------------------------------------------
  iInterpAnisotropic.def("InterpToPts", [](xms::InterpAnisotropic &self, py::iterable points) -> py::iterable {
      BSHP<xms::VecPt3d> vecPts = xms::VecPt3dFromPyIter(points);
      xms::VecFlt vecResults;
      self.InterpToPts(*vecPts, vecResults);
      return xms::PyIterFromVecFlt(vecResults, py::isinstance<py::array>(points));
  }, py::arg("points"));
  // ---------------------------------------------------------------------------
  // function: GetInterpolationPts
  // ---------------------------------------------------------------------------
  iInterpAnisotropic.def("GetInterpolationPts", [] (xms::InterpAnisotropic &self) -> py::iterable {
    xms::VecPt3d vecPts;
    self.GetInterpolationPts(vecPts);
    return xms::PyIterFromVecPt3d(vecPts);
  });
  // ---------------------------------------------------------------------------
  // function: GetTransformedPts
  // ---------------------------------------------------------------------------
  iInterpAnisotropic.def("GetTransformedPts", [](xms::InterpAnisotropic &self, py::iterable points, bool pick_closest) -> py::iterable {
    BSHP<xms::VecPt3d> vecPts = xms::VecPt3dFromPyIter(points);
    xms::VecPt3d transformed;
    self.GetTransformedPts(*vecPts, pick_closest, transformed);
    return xms::PyIterFromVecPt3d(transformed);
  }, py::arg("points"), py::arg("pick_closest"));
  // ---------------------------------------------------------------------------
  // function: SetPower
  // ---------------------------------------------------------------------------
    iInterpAnisotropic.def("SetPower", &xms::InterpAnisotropic::SetPower, py::arg("power"));
  // ---------------------------------------------------------------------------
  // function: SetXScale
  // ---------------------------------------------------------------------------
    iInterpAnisotropic.def("SetXScale", &xms::InterpAnisotropic::SetXScale, py::arg("x_scale"));
}
