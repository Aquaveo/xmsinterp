#pragma once
#ifdef CXX_TEST
//------------------------------------------------------------------------------
/// \file
/// \ingroup tutorial
/// \copyright (C) Copyright Aquaveo 2018. Distributed under FreeBSD License
/// (See accompanying file LICENSE or https://aqaveo.com/bsd/license.txt)
//------------------------------------------------------------------------------

// 3. Standard Library Headers

// 4. External Library Headers
#include <cxxtest/TestSuite.h>

// 5. Shared Headers

// 6. Non-shared Headers

//----- Namespace declaration --------------------------------------------------

class TutInterpolationIntermediateTests : public CxxTest::TestSuite
{
public:
#ifndef CXXTEST4
  virtual const CxxTest::TestGroup& group();
#endif
  void test_Example_IDW();
  void test_Example_Linear();
  void test_Example_NaturalNeighbor();
  void test_Example_Anisotropic();
};

#endif
