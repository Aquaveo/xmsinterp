#pragma once
#ifdef CXX_TEST
//------------------------------------------------------------------------------
/// \file
/// \ingroup interpolate
/// \copyright (C) Copyright Aquaveo 2018. Distributed under FreeBSD License
/// (See accompanying file LICENSE or https://aqaveo.com/bsd/license.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------
// 3. Standard Library Headers

// 4. External Library Headers
#include <cxxtest/TestSuite.h>

// 5. Shared Headers

// 6. Non-shared Headers

//----- Forward declarations ---------------------------------------------------

//----- Namespace declaration --------------------------------------------------
// namespace xms {

//----- Constants / Enumerations -----------------------------------------------

//----- Structs / Classes ------------------------------------------------------
class InterpIdwUnitTests : public CxxTest::TestSuite
{
public:
  void testCreateClass();

  void testInterpToPts();
  void testInterp2d_a();
  void testInterp2d_b();
  void testInterp2d_c();
  void testInterp2d_d();
  void testInterp2d_e();
  void testInterp2d_f();
  void testInterp3d();

  void testErrors();
  void testErrors2();
};

//----- Function prototypes ----------------------------------------------------

//} // namespace xms
#endif // CXX_TEST
