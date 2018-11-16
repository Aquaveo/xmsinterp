//------------------------------------------------------------------------------
/// \file
/// \brief root module for xmsinterp Python bindings.
/// \copyright (C) Copyright Aquaveo 2018. Distributed under FreeBSD License
/// (See accompanying file LICENSE or https://aqaveo.com/bsd/license.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------
#include <pybind11/pybind11.h>
#include <xmsinterp/python/geometry/geometry_py.h>
#include <xmsinterp/python/interpolate/interpolate_py.h>
#include <xmsinterp/python/triangulate/triangulate_py.h>

//----- Namespace declaration --------------------------------------------------
namespace py = pybind11;

//----- Python Interface -------------------------------------------------------
//PYBIND11_DECLARE_HOLDER_TYPE(T, boost::shared_ptr<T>);

std::string version() {
    return "1.0.0";
}


//------ Primary Module --------------------------------------------------------
PYBIND11_MODULE(xmsinterp_py, m) {
    m.doc() = "Python bindings for xmsinterp"; // optional module docstring
    m.def("version", &version,
          "Get current version of xmsinterp Python bindings.");

    // Interpolate module
    const char* interpolate_doc = R"pydoc(
        The interpolate module of the xmsinterp python library contains classes
        and functions for performing the following types of interpolation given
        a set of points: IDW, linear, natural neighbor, and anisotropic.
    )pydoc";
    py::module modInterpolate = m.def_submodule("interpolate",interpolate_doc);
    initInterpolate(modInterpolate);

    const char* triangulate_doc = R"pydoc(
        The triangulate module of the xmsinterp python library contains classes 
        and functions for creating and triangulating a TIN from a set of points.
    )pydoc";
    py::module modTriangulate = m.def_submodule("triangulate",triangulate_doc);
    initTriangulate(modTriangulate);

    const char* geometry_doc = R"pydoc(
        The geometry module of the xmsinterp python library contains classes 
        and functions for finding triangles.
    )pydoc";
    py::module modGeometry = m.def_submodule("geometry",geometry_doc);
    initGeometry(modGeometry);
}