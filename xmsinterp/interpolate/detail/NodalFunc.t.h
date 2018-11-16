#pragma once
#ifdef CXX_TEST
//------------------------------------------------------------------------------
/// \file
/// \ingroup interpolate_detail
/// \copyright (C) Copyright Aquaveo 2018. Distributed under FreeBSD License
/// (See accompanying file LICENSE or https://aqaveo.com/bsd/license.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------
#include <cxxtest/TestSuite.h>

//----- Forward declarations ---------------------------------------------------

//----- Namespace declaration --------------------------------------------------
// namespace xms {

//----- Constants / Enumerations -----------------------------------------------

//----- Structs / Classes ------------------------------------------------------
class NodalFuncUnitTests : public CxxTest::TestSuite
{
public:
  void testCreateClass();
  void testComputeGradientForPoint();
};

//----- Function prototypes ----------------------------------------------------

//} // namespace xms
#endif // CXX_TEST
