#pragma once
//------------------------------------------------------------------------------
/// \file
/// \brief initializer functions for members of triangulate python module.
/// \copyright (C) Copyright Aquaveo 2018. Distributed under the xmsng
///  Software License, Version 1.0. (See accompanying file
///  LICENSE_1_0.txt or copy at http://www.aquaveo.com/xmsng/LICENSE_1_0.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------
#include <pybind11/pybind11.h>

//----- Namespace declaration --------------------------------------------------
namespace py = pybind11;

//----- Function declarations --------------------------------------------------
void initTriangulate(py::module &);

void initTrTin(py::module &);
void initTrTriangulatorPoints(py::module &);
