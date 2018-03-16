#pragma once
//------------------------------------------------------------------------------
/// \file
/// \ingroup geometry
/// \copyright (C) Copyright Aquaveo 2018. Distributed under the xmsng
///  Software License, Version 1.0. (See accompanying file
///  LICENSE_1_0.txt or copy at http://www.aquaveo.com/xmsng/LICENSE_1_0.txt)
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
