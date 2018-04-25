//------------------------------------------------------------------------------
/// \file
/// \brief root module for xmsinterp Python bindings.
/// \copyright (C) Copyright Aquaveo 2018. Distributed under the xmsng
///  Software License, Version 1.0. (See accompanying file
///  LICENSE_1_0.txt or copy at http://www.aquaveo.com/xmsng/LICENSE_1_0.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------
#include <pybind11/pybind11.h>
#include <xmsinterp/python/interpolate/interpolate_py.h>

//----- Namespace declaration --------------------------------------------------
namespace py = pybind11;

//----- Python Interface -------------------------------------------------------
//PYBIND11_DECLARE_HOLDER_TYPE(T, boost::shared_ptr<T>);

std::string version() {
    return "1.0.0";
}

//------ Submodules Initializers -----------------------------------------------
void initInterpLinear(py::module &);

//------ Primary Module --------------------------------------------------------
PYBIND11_MODULE(xmsinterp_py, m) {
    m.doc() = "Python bindings for xmsinterp"; // optional module docstring
    m.def("version", &version,
          "Get current version of xmsinterp Python bindings.");

    // Interpolate module
    py::module modInterpolate = m.def_submodule("interpolate");
    initInterpLinear(modInterpolate);
}