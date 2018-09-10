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
    iInterpAnisotropic.def(py::init(&xms::InterpAnisotropic::New))
    .def("set_points", [](xms::InterpAnisotropic &self, py::iterable a_centerlinePts, py::iterable a_interpolationPts, bool a_pickClosest) {
        BSHP<xms::VecPt3d> vecCenterlinePts = xms::VecPt3dFromPyIter(a_centerlinePts);
        BSHP<xms::VecPt3d> vecInterpolationPts = xms::VecPt3dFromPyIter(a_interpolationPts);
        self.SetPoints(*vecCenterlinePts, *vecInterpolationPts, a_pickClosest);
    },"Set the centerline to define the interpolation space and the points to interpolate from.", py::arg("centerline_pts"), py::arg("interpolation_pts"), py::arg("pick_closest"))
    .def("interp_to_pt", [](xms::InterpAnisotropic &self, py::iterable a_pt) -> float {
        xms::Pt3d pt = xms::Pt3dFromPyIter(a_pt);
        return self.InterpToPt(pt);
    },"Interpolate z (3rd coordinate) values to a point.", py::arg("pt"))
    .def("interp_to_pts", [](xms::InterpAnisotropic &self, py::iterable a_pts) -> py::iterable {
        BSHP<xms::VecPt3d> vecPts = xms::VecPt3dFromPyIter(a_pts);
        xms::VecFlt vecResults;
        self.InterpToPts(*vecPts, vecResults);
        return xms::PyIterFromVecFlt(vecResults, py::isinstance<py::array>(a_pts));
    },"Interpolate z values to many points.", py::arg("points"))
    .def("get_interpolation_pts", [](xms::InterpAnisotropic &self) -> py::iterable {
        xms::VecPt3d vecPts;
        self.GetInterpolationPts(vecPts);
        return xms::PyIterFromVecPt3d(vecPts);
    },"Get the interpolation points transformed into (s,n,z) space and scaled.")
    .def("get_transformed_pts", [](xms::InterpAnisotropic &self, py::iterable a_points, bool a_pickClosest) -> py::iterable {
        BSHP<xms::VecPt3d> vecPts = xms::VecPt3dFromPyIter(a_points);
        xms::VecPt3d transformed;
        self.GetTransformedPts(*vecPts, a_pickClosest, transformed);
        return xms::PyIterFromVecPt3d(transformed);
    },"Transform points into (s,n,z) space and then scale.", py::arg("points"), py::arg("pick_closest"))
    .def("set_power", &xms::InterpAnisotropic::SetPower,"Set the exponent to use on the inverse distance weighting (defaults to 2).",
        py::arg("power")
    )
    .def("set_x_scale", &xms::InterpAnisotropic::SetXScale,"Set the scale factor to apply to transformed x (s) coordinates.",
        py::arg("x_scale")
    );
}
