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
    py::class_<xms::InterpAnisotropic, 
        boost::shared_ptr<xms::InterpAnisotropic>> iInterpAnisotropic(m, 
        "InterpAnisotropic");
    iInterpAnisotropic.def(py::init(&xms::InterpAnisotropic::New));
  // ---------------------------------------------------------------------------
  // function: set_points
  // ---------------------------------------------------------------------------
    const char* set_points_doc = R"pydoc(
        Set the centerline to define the interpolation space and the points to
        interpolate from.

        Args:
            centerline_pts (iterable):  The centerline
            interpolation_pts (iterable): The points to interpolate from
                (typically cross sections of the centerline).
            pick_closest (bool): if true, only keep the transformed point
                for each interpolation point closest to the centerline.
    )pydoc";
    iInterpAnisotropic.def("set_points", [](xms::InterpAnisotropic &self, 
        py::iterable a_centerlinePts, py::iterable a_interpolationPts, 
        bool a_pickClosest) {
        BSHP<xms::VecPt3d> vecCenterlinePts = 
            xms::VecPt3dFromPyIter(a_centerlinePts);
        BSHP<xms::VecPt3d> vecInterpolationPts = 
            xms::VecPt3dFromPyIter(a_interpolationPts);
        self.SetPoints(*vecCenterlinePts, *vecInterpolationPts, a_pickClosest);
    },set_points_doc, py::arg("centerline_pts"), py::arg("interpolation_pts"), 
        py::arg("pick_closest"));
  // ---------------------------------------------------------------------------
  // function: interp_to_pt
  // ---------------------------------------------------------------------------
    const char* interp_to_pt_doc = R"pydoc(
        Interpolate z (3rd coordinate) values to a point.

        Args:
            pt (iterable):  The point to interpolate to.

        Returns:
            float: The scaled inverse distance weighted interpolated value.
    )pydoc";
    iInterpAnisotropic.def("interp_to_pt", [](xms::InterpAnisotropic &self, 
        py::iterable a_pt) -> float {
        xms::Pt3d pt = xms::Pt3dFromPyIter(a_pt);
        return self.InterpToPt(pt);
    },interp_to_pt_doc, py::arg("pt"));
  // ---------------------------------------------------------------------------
  // function: interp_to_pts
  // ---------------------------------------------------------------------------
    const char* interp_to_pts_doc = R"pydoc(
        Interpolate z values to many points.

        Args:
            points (iterable):  The points to interpolate to.

        Returns:
            iterable: The interpolated values (one for each of a_pts)
    )pydoc";
    iInterpAnisotropic.def("interp_to_pts", [](xms::InterpAnisotropic &self, 
        py::iterable a_pts) -> py::iterable {
        BSHP<xms::VecPt3d> vecPts = xms::VecPt3dFromPyIter(a_pts);
        xms::VecFlt vecResults;
        self.InterpToPts(*vecPts, vecResults);
        return xms::PyIterFromVecFlt(vecResults, 
            py::isinstance<py::array>(a_pts));
    },interp_to_pts_doc, py::arg("points"));
  // ---------------------------------------------------------------------------
  // function: get_interpolation_pts
  // ---------------------------------------------------------------------------
    const char* get_interpolation_pts_doc = R"pydoc(
        Get the interpolation points transformed into (s,n,z) space and scaled.

        Returns:
            iterable: The transformed and scaled interpolation points 
                (passed to SetPoints
    )pydoc";
    iInterpAnisotropic.def("get_interpolation_pts", []
        (xms::InterpAnisotropic &self) -> py::iterable {
        xms::VecPt3d vecPts;
        self.GetInterpolationPts(vecPts);
        return xms::PyIterFromVecPt3d(vecPts);
    },get_interpolation_pts_doc);
  // ---------------------------------------------------------------------------
  // function: get_transformed_pts
  // ---------------------------------------------------------------------------
    const char* get_transformed_pts_doc = R"pydoc(
        Transform points into (s,n,z) space and then scale.

        Args:
            points (iterable): The points to transform into (s,n,z) space.
            pick_closest (bool): Pick only the transform point closest to 
                the centerline.

        Returns:
            iterable: The points transformed into (s,n,z) space.
    )pydoc";
    iInterpAnisotropic.def("get_transformed_pts", []
        (xms::InterpAnisotropic &self, py::iterable a_points, 
        bool a_pickClosest) -> py::iterable {
        BSHP<xms::VecPt3d> vecPts = xms::VecPt3dFromPyIter(a_points);
        xms::VecPt3d transformed;
        self.GetTransformedPts(*vecPts, a_pickClosest, transformed);
        return xms::PyIterFromVecPt3d(transformed);
    },get_transformed_pts_doc, py::arg("points"), py::arg("pick_closest"));
  // ---------------------------------------------------------------------------
  // function: set_power
  // ---------------------------------------------------------------------------
    const char* set_power_doc = R"pydoc(
        Set the exponent to use on the inverse distance weighting 
        (defaults to 2).

        Args:
            power (iterable): The exponent to use.
    )pydoc";
    iInterpAnisotropic.def("set_power", &xms::InterpAnisotropic::SetPower,
    set_power_doc, py::arg("power"));
  // ---------------------------------------------------------------------------
  // function: set_x_scale
  // ---------------------------------------------------------------------------
    const char* set_x_scale_doc = R"pydoc(
        Set the scale factor to apply to transformed x (s) coordinates. 

        Args:
            power (iterable): Set the scale factor to apply to transformed x (s)
                coordinates. before computing the inverse distance weighting.
    )pydoc";
    iInterpAnisotropic.def("set_x_scale", &xms::InterpAnisotropic::SetXScale,
    set_x_scale_doc, py::arg("x_scale")
    );
}
