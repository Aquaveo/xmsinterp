//------------------------------------------------------------------------------
/// \file
/// \brief root module for xmsinterp Python bindings.
/// \copyright (C) Copyright Aquaveo 2018. Distributed under FreeBSD License
/// (See accompanying file LICENSE or https://aqaveo.com/bsd/license.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------
#include <pybind11/pybind11.h>
#include <xmsinterp/python/interpolate/interpolate_py.h>

//----- Namespace declaration --------------------------------------------------
namespace py = pybind11;

//----- Python Interface -------------------------------------------------------
//PYBIND11_DECLARE_HOLDER_TYPE(T, boost::shared_ptr<T>);

#ifndef XMS_VERSION
  #define XMS_VERSION "99.99.99";
#endif


//------ Primary Module --------------------------------------------------------
PYBIND11_MODULE(_xmsinterp, m) {
    m.doc() = "Python bindings for xmsinterp"; // optional module docstring
    m.attr("__version__") = XMS_VERSION;

    // Interpolate module
    const char* interpolate_doc = R"pydoc(
        The interpolate module of the xmsinterp python library contains classes
        and functions for performing the following types of interpolation given
        a set of points: IDW, linear, natural neighbor, and anisotropic.
    )pydoc";
    py::module modInterpolate = m.def_submodule("interpolate",interpolate_doc);
    initInterpolate(modInterpolate);
}