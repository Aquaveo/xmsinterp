//------------------------------------------------------------------------------
/// \file
/// \ingroup tutorial
/// \copyright (C) Copyright Aquaveo 2018. Distributed under the xmsng
///  Software License, Version 1. (See accompanying file
///  LICENSE_1_0.txt or copy at http://www.aquaveo.com/xmsng/LICENSE_1_0.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------

// 1. Precompiled header

// 2. My own header

// 3. Standard library headers

// 4. External library headers

// 5. Shared code headers

// 6. Non-shared code headers

//----- Forward declarations ---------------------------------------------------

//----- External globals -------------------------------------------------------

//----- Namespace declaration --------------------------------------------------

namespace xms
{
//----- Constants / Enumerations -----------------------------------------------

//----- Classes / Structs ------------------------------------------------------

//----- Internal functions -----------------------------------------------------

//----- Class / Function definitions -------------------------------------------

} // namespace xms

#if CXX_TEST
////////////////////////////////////////////////////////////////////////////////
// UNIT TESTS
////////////////////////////////////////////////////////////////////////////////
#include <xmsinterp/tutorial/TutInterpolation.t.h>

#include <xmscore/testing/TestTools.h>

#include <xmsinterp/interpolate/InterpIdw.h>
#include <xmsinterp/interpolate/InterpLinear.h>

////////////////////////////////////////////////////////////////////////////////
/// \class TutInterpolationIntermediateTests
/// \brief Tests for interpolation tutorial.
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/// \brief    Defines the test group.
/// \return CxxTest::TestGroup reference.
//------------------------------------------------------------------------------
#ifndef CXXTEST4
//------------------------------------------------------------------------------
/// \brief
//------------------------------------------------------------------------------
const CxxTest::TestGroup& TutInterpolationIntermediateTests::group()
{
  // return CxxTest::TestSuite::group();
  return *CxxTest::TestGroup::GetGroup(CxxTest::TG_INTERMEDIATE);
} // TutInterpolationIntermediateTests::group
#endif
//------------------------------------------------------------------------------
/// \brief Example for IDW Interpolation
//------------------------------------------------------------------------------
//! [snip_test_Example_Idw]
void TutInterpolationIntermediateTests::test_Example_IDW()
{
  BSHP<xms::VecInt> tris(new xms::VecInt()); // not used by idw

  // Create the points used to interpolate
  BSHP<xms::VecPt3d> pts(new xms::VecPt3d());
  *pts = {{0, 0, 0}, {10, 0, 1}, {10, 10, 2}, {0, 10, 3}};

  // create the interpolator class
  BSHP<xms::InterpIdw> interp = xms::InterpIdw::New();
  // Set the points and triangles used by the interpolator. The triangles are
  // empty and are not used by idw.
  interp->SetPtsTris(pts, tris);

  // interpolate to a location
  xms::Pt3d loc(5, 5, 0);
  float val = interp->InterpToPt(loc);

  // verify the value
  float base(1.5);
  TS_ASSERT_EQUALS(base, val);

} // TutInterpolationIntermediateTests::test_Example_IDW
//! [snip_test_Example_Idw]
//------------------------------------------------------------------------------
/// \brief Example for Linear Interpolation
//------------------------------------------------------------------------------
//! [snip_test_Example_Linear]
void TutInterpolationIntermediateTests::test_Example_Linear()
{
  // Create the points used to interpolate
  BSHP<xms::VecPt3d> pts(new xms::VecPt3d());
  *pts = {{0, 0, 0}, {10, 0, 1}, {10, 10, 2}, {0, 10, 3}};

  // Create the triangles used to interpolate. The triangles are specified as
  // 3 indexes that refer to the points vector. The indexes should be ordered
  // counter clockwise.
  BSHP<xms::VecInt> tris(new xms::VecInt());
  *tris = {0, 1, 3, 1, 2, 3};

  // create the interpolator class
  BSHP<xms::InterpLinear> interp = xms::InterpLinear::New();
  // Set the points and triangles used by the interpolator.
  interp->SetPtsTris(pts, tris);

  // interpolate to a location
  xms::Pt3d loc(2, 1, 0);
  float val = interp->InterpToPt(loc);

  // verify the value
  float base(0.5);
  TS_ASSERT_EQUALS(base, val);
} // TutInterpolationIntermediateTests::test_Example_Linear
//! [snip_test_Example_Linear]
//------------------------------------------------------------------------------
/// \brief Example for Natural Neighbor
//------------------------------------------------------------------------------
//! [snip_test_Example_NatNeigh]
void TutInterpolationIntermediateTests::test_Example_NaturalNeighbor()
{
  // Create the points used to interpolate
  BSHP<xms::VecPt3d> pts(new xms::VecPt3d());
  *pts = {{0, 0, 0}, {10, 0, 1}, {10, 10, 2}, {0, 10, 3}};

  // Create the triangles used to interpolate. The triangles are specified as
  // 3 indexes that refer to the points vector. The indexes should be ordered
  // counter clockwise.
  BSHP<xms::VecInt> tris(new xms::VecInt());
  *tris = {0, 1, 3, 1, 2, 3};

  // create the interpolator class
  BSHP<xms::InterpLinear> interp = xms::InterpLinear::New();
  // Set the points and triangles used by the interpolator.
  interp->SetPtsTris(pts, tris);

  int nodalFunc = 0;            // 0 constant, 1 gradient plane, 3 quadratic
  int nodalFuncOpt = 1;         // 0 natural neighbors, 1 nearest n points
  int nNearest = 12;            // 12 nearest points used for nodal function
  bool blendWeights = false;    // flag to add weight blending
  BSHP<xms::Observer> progress; // observer to get feedback
  // set to use natural neighbor interpolation
  interp->SetUseNatNeigh(true, nodalFunc, nodalFuncOpt, nNearest, blendWeights, progress);

  // interpolate to a location
  xms::Pt3d loc(2, 1, 0);
  float val = interp->InterpToPt(loc);

  // verify the value
  float base(0.46f);
  TS_ASSERT_EQUALS(base, val);
} // TutInterpolationIntermediateTests::test_Example_NaturalNeighbor
//! [snip_test_Example_NatNeigh]
#endif
