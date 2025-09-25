#pragma once
//------------------------------------------------------------------------------
/// \file
/// \brief initializer functions for members of interpolate python module.
/// \copyright (C) Copyright Aquaveo 2018. Distributed under FreeBSD License
/// (See accompanying file LICENSE or https://aqaveo.com/bsd/license.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------
#include <pybind11/pybind11.h>

//----- Namespace declaration --------------------------------------------------
namespace py = pybind11;
namespace xms { class InterpBase; }

//----- Function declarations --------------------------------------------------
void initInterpolate(py::module &);

void initInterpAnisotropic(py::module &);
void initInterpLinear(py::module &);
void initInterpLinearExtrapIdw(py::module &);
void initInterpIdw(py::module &);
void initInterpBase(py::module&);
void initInterpUtil(py::module&);
std::string PyReprStringFromInterpBase(const xms::InterpBase& a_interpBase);
