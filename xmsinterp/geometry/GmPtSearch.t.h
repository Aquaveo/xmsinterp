#pragma once
//------------------------------------------------------------------------------
/// \file
/// \ingroup geometry
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
class PtSearchUnitTests : public CxxTest::TestSuite
{
public:
  void testCreateClass();
  void testTest2d();
  void testTest2dTutData();
  void testTest3d();
  void testTest3dOct();
  void testActivity2d();
  void testActivity3d();
  void testPtsWithinDist();
  void testVectorThatGrows();
};
//----- Function prototypes ----------------------------------------------------

//} // namespace xms
