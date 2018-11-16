#pragma once
#ifdef CXX_TEST
//------------------------------------------------------------------------------
/// \file
/// \ingroup triangulate
/// \copyright (C) Copyright Aquaveo 2018. Distributed under FreeBSD License
/// (See accompanying file LICENSE or https://aqaveo.com/bsd/license.txt)
//------------------------------------------------------------------------------

// 3. Standard Library Headers

// 4. External Library Headers
#include <boost/shared_ptr.hpp>

// 5. Shared Headers
#include <cxxtest/TestSuite.h>

// 6. Non-shared Headers

//----- Namespace declaration --------------------------------------------------

// namespace xms {

//----- Forward declarations ---------------------------------------------------
namespace xms
{
class TrTin;
}
//----- Classes ----------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
class TrTinUnitTests : public CxxTest::TestSuite
{
public:
  void test1();
  void testOptimizeTriangulation();
  void testSwap();
  // void testFirstBoundaryPoint();
  void testBoundaries();
  void testDeleteTriangles();
  void testDeletePoints();
  void testRemoveLongThinTrianglesOnPerimeter();
};

//----- Global functions -------------------------------------------------------

boost::shared_ptr<xms::TrTin> trBuildTestTin();
boost::shared_ptr<xms::TrTin> trBuildTestTin2();
boost::shared_ptr<xms::TrTin> trBuildTestTin6();
boost::shared_ptr<xms::TrTin> trBuildTestTin7();
boost::shared_ptr<xms::TrTin> trBuildTestTin8();
boost::shared_ptr<xms::TrTin> trBuildTestTin9();

//} // namespace xms
#endif
