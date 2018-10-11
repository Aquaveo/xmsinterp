#pragma once
#ifdef CXX_TEST
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
class TriSearchUnitTests : public CxxTest::TestSuite
{
public:
  void testCreateClass();
  void testInterpWeights();
  void testPtActivity();
  void testTriActivity();
  void testSmsCase1();
  void testTouch();
  void testTriEnvelopesOverlap();
};
//----- Function prototypes ----------------------------------------------------

//} // namespace xms
#endif
