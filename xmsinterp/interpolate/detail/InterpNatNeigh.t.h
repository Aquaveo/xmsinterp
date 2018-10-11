#pragma once
#ifdef CXX_TEST
//------------------------------------------------------------------------------
/// \file
/// \ingroup interpolate_detail
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
class InterpNatNeighUnitTests : public CxxTest::TestSuite
{
public:
  void testCreateClass();

  void testGetTris();
  void testGetNeighbors();
  void testHaleNnSortOuterEdges();
  void testHaleNnInterp();
};
//----- Function prototypes ----------------------------------------------------

//} // namespace xms
#endif // CXX_TEST
