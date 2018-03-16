//------------------------------------------------------------------------------
/// \file
/// \ingroup geometry
/// \copyright (C) Copyright Aquaveo 2018. Distributed under the xmsng
///  Software License, Version 1.0. (See accompanying file
///  LICENSE_1_0.txt or copy at http://www.aquaveo.com/xmsng/LICENSE_1_0.txt)
//------------------------------------------------------------------------------
//----- Included files ---------------------------------------------------------

// 1. Precompiled header

// 2. My own header
#include <xmsinterp/geometry/GmPolygon.h>

// 3. Standard library headers

// 4. External library headers
#pragma warning(push)
#pragma warning(disable : 4512) // boost code: no assignment operator
#pragma warning(disable : 4244) // boost code: possible loss of data
#pragma warning(disable : 4127) // boost code: conditional expression is constant
#include <boost/geometry/geometry.hpp>
#pragma warning(pop)

// 5. Shared code headers
#include <xmscore/misc/XmError.h>
#include <xmsinterp/geometry/GmBoostTypes.h> // GmBstPoly3d, GmBstRing3d
#include <xmscore/misc/boost_defines.h>

// 6. Non-shared code headers

//----- Forward declarations ---------------------------------------------------

//----- External globals -------------------------------------------------------

//----- Namespace declaration --------------------------------------------------

namespace xms
{
//----- Constants / Enumerations -----------------------------------------------

//----- Classes / Structs ------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
class GmPolygonImpl : public GmPolygon
{
public:
  GmPolygonImpl();
  ~GmPolygonImpl() {}

  virtual void Setup(const VecPt3d& a_poly, const VecPt3d2d& a_inPolys) override;
  virtual void GetPoints(VecPt3d& a_poly, VecPt3d2d& a_inPolys) const override;

  virtual bool CoveredBy(Pt3d a_point) const override;
  virtual bool Within(Pt3d a_point) const override;
  virtual double MinDistanceToBoundary(Pt3d a_pt) const override;
  virtual void Intersection(const GmPolygon& a_,
                            std::vector<BSHP<GmPolygon>>& a_out) const override;
  virtual void Union(const GmPolygon& a_, std::vector<BSHP<GmPolygon>>& a_out) const override;

private:
  GmBstPoly3d m_poly; ///< The boost geom polygon
};                    // GmPolygonImpl

//----- Internal functions -----------------------------------------------------

static double iDistanceToRing(const GmBstRing3d& a_ring, const Pt3d& a_pt);
//----- Class / Function definitions -------------------------------------------

//------------------------------------------------------------------------------
/// \brief Return the minimum distance from the point to a line on the ring.
/// \param[in] a_ring: The ring.
/// \param[in] a_pt:   A point.
/// \return The distance.
//------------------------------------------------------------------------------
static double iDistanceToRing(const GmBstRing3d& a_ring, const Pt3d& a_pt)
{
  GmBstLine3d line;
  // make each ring a linestring and get minimum distance
  for (size_t i = 0; i < a_ring.size(); ++i)
  {
    line.push_back(a_ring[i]);
  }
  double d = boost::geometry::distance(line, a_pt);
  return d;
} // iDistanceToRing

////////////////////////////////////////////////////////////////////////////////
/// \class GmPolygonImpl
/// \brief Wraps a boost polygon for point in poly, min distance from point
///        to poly etc.
///
/// gmPointInPolygon2D is faster, but if you need a boost polygon, consider
/// using this class. This class is best if being used repeatedly
/// for the same polygon as it takes time for setup due to it being
/// allocated on the heap and creating a boost polygon under the hood,
/// but the methods can then be called repeatedly at speed.
/// \see EFastPointInPoly
/// \see gmPointInPolygon2D
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/// \brief Constructor.
//------------------------------------------------------------------------------
GmPolygonImpl::GmPolygonImpl()
: m_poly()
{
} // GmPolygonImpl::GmPolygonImpl
//------------------------------------------------------------------------------
/// \brief Create a boost geometry polygon to use in point in polygon.
///
/// The first point should be repeated as the last point but if not, it will
/// get fixed automatically. Also, if the orientation of the polygons is
/// not correct, it is automatically corrected.
/// \param a_outPoly: Outer polygon, clockwise.
/// \param a_inPolys: Inner polygons (if any), counter clockwise.
//------------------------------------------------------------------------------
void GmPolygonImpl::Setup(const VecPt3d& a_outPoly, const VecPt3d2d& a_inPolys)
{
  namespace bg = boost::geometry;

  m_poly.clear();

  // See http://www.boost.org/doc/libs/1_55_0/libs/geometry/doc/src/examples/core/rings.cpp
  // Outer poly
  for (size_t i = 0; i < a_outPoly.size(); ++i)
  {
    bg::exterior_ring(m_poly).push_back(a_outPoly[i]);
  }

  // Inner polys
  for (size_t i = 0; i < a_inPolys.size(); ++i)
  {
    GmBstRing3d inner;
    for (size_t j = 0; j < a_inPolys[i].size(); ++j)
    {
      inner.push_back(a_inPolys[i][j]);
    }
    bg::interior_rings(m_poly).push_back(inner);
  }

  // Fix the orientation and closure of the polygon.
  bg::correct(m_poly);

} // GmPolygonImpl::Setup
//------------------------------------------------------------------------------
/// \brief Get the points from a boost polygon as vectors of Pt3d.
/// \param a_outPoly: Outer polygon, clockwise.
/// \param a_inPolys: Inner polygons (if any), counter clockwise.
//------------------------------------------------------------------------------
void GmPolygonImpl::GetPoints(VecPt3d& a_outPoly, VecPt3d2d& a_inPolys) const
{
  a_outPoly.clear();
  a_inPolys.clear();
  a_outPoly = m_poly.outer();
  for (auto& p : m_poly.inners())
  {
    a_inPolys.push_back(p);
  }
} // GmPolygonImpl::GetPoints
//------------------------------------------------------------------------------
/// \brief Tests if a_point is inside or on the polygon and not in a hole.
/// \param a_point The location to test.
/// \return true if inside or on.
///
/// gmPointInPolygon2D is faster.
/// \see EFastPointInPoly
/// \see gmPointInPolygon2D
//------------------------------------------------------------------------------
bool GmPolygonImpl::CoveredBy(Pt3d a_point) const
{
  namespace bg = boost::geometry;

  if (bg::covered_by(a_point, m_poly))
  {
    return true;
  }
  return false;
} // GmPolygonImpl::CoveredBy
//------------------------------------------------------------------------------
/// \brief Tests if a_point is inside the polygon and not in a hole.
/// \param a_point The location to test.
/// \return true if inside.
///
/// gmPointInPolygon2D is faster.
/// \see EFastPointInPoly
/// \see gmPointInPolygon2D
//------------------------------------------------------------------------------
bool GmPolygonImpl::Within(Pt3d a_point) const
{
  namespace bg = boost::geometry;

  if (bg::within(a_point, m_poly))
  {
    return true;
  }
  return false;
} // GmPolygonImpl::Within
//------------------------------------------------------------------------------
/// \brief Computes the min distance from a point to a polygon.
/// \param a_pt The location from which the minimum distance is calculated.
/// \return the distance
//------------------------------------------------------------------------------
double GmPolygonImpl::MinDistanceToBoundary(Pt3d a_pt) const
{
  namespace bg = boost::geometry;
  double d = iDistanceToRing(m_poly.outer(), a_pt);
  for (size_t i = 0; i < m_poly.inners().size(); ++i)
  {
    double d1 = iDistanceToRing(m_poly.inners()[i], a_pt);
    if (d1 < d)
      d = d1;
  }
  return d;
} // GmPolygonImpl::Within
//------------------------------------------------------------------------------
/// \brief Performs a polygon intersection
/// \param a_ Another polygon to do the intersection operation with
/// \param a_out Vector of polygons created from the intersection operation
//------------------------------------------------------------------------------
void GmPolygonImpl::Intersection(const GmPolygon& a_, std::vector<BSHP<GmPolygon>>& a_out) const
{
  a_out.clear();
  namespace bg = boost::geometry;
  const GmPolygonImpl* p = dynamic_cast<const GmPolygonImpl*>(&a_);
  XM_ENSURE_TRUE(p);
  std::vector<GmBstPoly3d> output;
  bg::intersection(m_poly, p->m_poly, output);

  for (auto& o : output)
  {
    VecPt3d outPoly = o.outer();
    VecPt3d2d inPoly;
    for (auto& i : o.inners())
    {
      inPoly.push_back(i);
    }
    BSHP<GmPolygon> gp = GmPolygon::New();
    gp->Setup(outPoly, inPoly);
    a_out.push_back(gp);
  }
} // GmPolygonImpl::Intersection
//------------------------------------------------------------------------------
/// \brief Performs a polygon union
/// \param a_ Another polygon to do the union operation with
/// \param a_out Vector of polygons created from the union operation
//------------------------------------------------------------------------------
void GmPolygonImpl::Union(const GmPolygon& a_, std::vector<BSHP<GmPolygon>>& a_out) const
{
  a_out.clear();
  namespace bg = boost::geometry;
  const GmPolygonImpl* p = dynamic_cast<const GmPolygonImpl*>(&a_);
  XM_ENSURE_TRUE(p);
  std::vector<GmBstPoly3d> output;
  bg::union_(m_poly, p->m_poly, output);

  for (auto& o : output)
  {
    VecPt3d outPoly = o.outer();
    VecPt3d2d inPoly;
    for (auto& i : o.inners())
    {
      inPoly.push_back(i);
    }
    BSHP<GmPolygon> gp = GmPolygon::New();
    gp->Setup(outPoly, inPoly);
    a_out.push_back(gp);
  }
} // GmPolygonImpl::Union

////////////////////////////////////////////////////////////////////////////////
/// \class GmPolygon
/// \see GmPolygonImpl
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/// \brief Constructor
//------------------------------------------------------------------------------
GmPolygon::GmPolygon()
{
} // GmPolygon::GmPolygon
//------------------------------------------------------------------------------
/// \brief Destructor
//------------------------------------------------------------------------------
GmPolygon::~GmPolygon()
{
} // GmPolygon::~GmPolygon
//------------------------------------------------------------------------------
/// \brief Creates a class
/// \return GmPolygon.
//------------------------------------------------------------------------------
BSHP<GmPolygon> GmPolygon::New()
{
  BSHP<GmPolygon> ret(new GmPolygonImpl);
  return ret;
} // GmPolygon::New

} // namespace xms

#if CXX_TEST
////////////////////////////////////////////////////////////////////////////////
// UNIT TESTS
////////////////////////////////////////////////////////////////////////////////

#include <xmsinterp/geometry/GmPolygon.t.h>

#include <boost/assign.hpp>
#include <boost/timer/timer.hpp>

#include <xmscore/testing/TestTools.h>
#include <xmsinterp/geometry/geoms.h>
#include <xmsinterp/geometry/geoms.t.h>
#include <xmscore/misc/carray.h>

//----- Namespace declaration --------------------------------------------------

// namespace xms {

////////////////////////////////////////////////////////////////////////////////
/// \class GmPointInPolyTester_GmPolygon
/// \brief Helper class for testing GmPolygon.
class GmPointInPolyTester_GmPolygon : public GmPointInPolyUnitTests
{
public:
  //----------------------------------------------------------------------------
  /// \brief Constructor.
  //----------------------------------------------------------------------------
  GmPointInPolyTester_GmPolygon()
  : GmPointInPolyUnitTests()
  , m_poly()
  {
  }

private:
  //----------------------------------------------------------------------------
  /// \brief Sets things up.
  //----------------------------------------------------------------------------
  virtual void Setup() override
  {
    GmPointInPolyUnitTests::Setup(); // Call the base class

    m_poly = xms::GmPolygon::New();
    xms::VecPt3d2d inPolys(1, m_inPoly);
    m_poly->Setup(m_outPoly, inPolys);
  }
  //----------------------------------------------------------------------------
  /// \brief Checks if point is covered by the poly and updates status.
  /// \param[in] a_pt: The point.
  //----------------------------------------------------------------------------
  virtual void CheckPoint(const xms::Pt3d& a_pt) override
  {
    // We'll just do covered by. If we want to see if it's on an edge we'd
    // have to do Within.
    m_status = m_poly->CoveredBy(a_pt);
  } // CheckPoint
  //----------------------------------------------------------------------------
  /// \brief Return the max time this test should ever take.
  /// \return The max time.
  //----------------------------------------------------------------------------
  virtual double MaxTime() override
  {
    // It takes .51 - .65 seconds on my machine in release.
    return 1.2;
    // Hopefully big enough for the tests to pass on all machines but give us
    // an error if it starts going a lot slower for some reason.
  } // MaxTime

private:
  BSHP<xms::GmPolygon> m_poly; ///< Polygon to test

  XM_DISALLOW_COPY_AND_ASSIGN(
    GmPointInPolyTester_GmPolygon); ///< Hide compiler generated copy and assign
};                                  // GmPointInPolyTester_GmPolygon

//} // namespace xms

////////////////////////////////////////////////////////////////////////////////
/// \class GmPolygonUnitTests
/// \brief Tester for the GmPolygon class.
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/// \brief Test a simple polygon with some points outside, some inside,
///        some on, and some coincident with polygon points.
/// \verbatim
// 15 -    0   1   2   3   4
//    |
// 10 -    5   6---7---8   9
//    |        |       |
//  5 -   10  11  12  13  14
//    |        |       |
//  0 -   15  16--17--18  19
//    |
// -5 -   20  21  22  23  24
//
//         |---|---|---|---|
//        -5   0   5   10  15
/// \endverbatim
//------------------------------------------------------------------------------
void GmPolygonUnitTests::testNoHoles()
{
  using xms::Pt3d;

  // Counter clockwise outer poly (wrong, but will be fixed automatically).
  // First point not repeated (wrong, but will be fixed automatically).
  xms::VecPt3d poly{{0, 0, 0}, {5, 0, 0}, {10, 0, 0}, {10, 5, 0}, {10, 10, 0}, {0, 10, 0}};
  BSHP<xms::GmPolygon> m = xms::GmPolygon::New();
  m->Setup(poly, xms::VecPt3d2d());
  TS_ASSERT_EQUALS(m->CoveredBy(Pt3d(-5, 15, 0)), false); // 0
  TS_ASSERT_EQUALS(m->CoveredBy(Pt3d(0, 15, 0)), false);  // 1
  TS_ASSERT_EQUALS(m->CoveredBy(Pt3d(5, 15, 0)), false);  // 2
  TS_ASSERT_EQUALS(m->CoveredBy(Pt3d(10, 15, 0)), false); // 3
  TS_ASSERT_EQUALS(m->CoveredBy(Pt3d(15, 15, 0)), false); // 4
  TS_ASSERT_EQUALS(m->CoveredBy(Pt3d(-5, 10, 0)), false); // 5
  TS_ASSERT_EQUALS(m->CoveredBy(Pt3d(0, 10, 0)), true);   // 6
  TS_ASSERT_EQUALS(m->CoveredBy(Pt3d(5, 10, 0)), true);   // 7
  TS_ASSERT_EQUALS(m->CoveredBy(Pt3d(10, 10, 0)), true);  // 8
  TS_ASSERT_EQUALS(m->CoveredBy(Pt3d(15, 10, 0)), false); // 9
  TS_ASSERT_EQUALS(m->CoveredBy(Pt3d(-5, 5, 0)), false);  // 10
  TS_ASSERT_EQUALS(m->CoveredBy(Pt3d(0, 5, 0)), true);    // 11
  TS_ASSERT_EQUALS(m->CoveredBy(Pt3d(5, 5, 0)), true);    // 12
  TS_ASSERT_EQUALS(m->CoveredBy(Pt3d(10, 5, 0)), true);   // 13
  TS_ASSERT_EQUALS(m->CoveredBy(Pt3d(15, 5, 0)), false);  // 14
  TS_ASSERT_EQUALS(m->CoveredBy(Pt3d(-5, 0, 0)), false);  // 15
  TS_ASSERT_EQUALS(m->CoveredBy(Pt3d(0, 0, 0)), true);    // 16
  TS_ASSERT_EQUALS(m->CoveredBy(Pt3d(5, 0, 0)), true);    // 17
  TS_ASSERT_EQUALS(m->CoveredBy(Pt3d(10, 0, 0)), true);   // 18
  TS_ASSERT_EQUALS(m->CoveredBy(Pt3d(15, 0, 0)), false);  // 19
  TS_ASSERT_EQUALS(m->CoveredBy(Pt3d(-5, -5, 0)), false); // 20
  TS_ASSERT_EQUALS(m->CoveredBy(Pt3d(0, -5, 0)), false);  // 21
  TS_ASSERT_EQUALS(m->CoveredBy(Pt3d(5, -5, 0)), false);  // 22
  TS_ASSERT_EQUALS(m->CoveredBy(Pt3d(10, -5, 0)), false); // 23
  TS_ASSERT_EQUALS(m->CoveredBy(Pt3d(15, -5, 0)), false); // 24

  TS_ASSERT_EQUALS(m->Within(Pt3d(-5, 15, 0)), false); // 0
  TS_ASSERT_EQUALS(m->Within(Pt3d(0, 15, 0)), false);  // 1
  TS_ASSERT_EQUALS(m->Within(Pt3d(5, 15, 0)), false);  // 2
  TS_ASSERT_EQUALS(m->Within(Pt3d(10, 15, 0)), false); // 3
  TS_ASSERT_EQUALS(m->Within(Pt3d(15, 15, 0)), false); // 4
  TS_ASSERT_EQUALS(m->Within(Pt3d(-5, 10, 0)), false); // 5
  TS_ASSERT_EQUALS(m->Within(Pt3d(0, 10, 0)), false);  // 6
  TS_ASSERT_EQUALS(m->Within(Pt3d(5, 10, 0)), false);  // 7
  TS_ASSERT_EQUALS(m->Within(Pt3d(10, 10, 0)), false); // 8
  TS_ASSERT_EQUALS(m->Within(Pt3d(15, 10, 0)), false); // 9
  TS_ASSERT_EQUALS(m->Within(Pt3d(-5, 5, 0)), false);  // 10
  TS_ASSERT_EQUALS(m->Within(Pt3d(0, 5, 0)), false);   // 11
  TS_ASSERT_EQUALS(m->Within(Pt3d(5, 5, 0)), true);    // 12
  TS_ASSERT_EQUALS(m->Within(Pt3d(10, 5, 0)), false);  // 13
  TS_ASSERT_EQUALS(m->Within(Pt3d(15, 5, 0)), false);  // 14
  TS_ASSERT_EQUALS(m->Within(Pt3d(-5, 0, 0)), false);  // 15
  TS_ASSERT_EQUALS(m->Within(Pt3d(0, 0, 0)), false);   // 16
  TS_ASSERT_EQUALS(m->Within(Pt3d(5, 0, 0)), false);   // 17
  TS_ASSERT_EQUALS(m->Within(Pt3d(10, 0, 0)), false);  // 18
  TS_ASSERT_EQUALS(m->Within(Pt3d(15, 0, 0)), false);  // 19
  TS_ASSERT_EQUALS(m->Within(Pt3d(-5, -5, 0)), false); // 20
  TS_ASSERT_EQUALS(m->Within(Pt3d(0, -5, 0)), false);  // 21
  TS_ASSERT_EQUALS(m->Within(Pt3d(5, -5, 0)), false);  // 22
  TS_ASSERT_EQUALS(m->Within(Pt3d(10, -5, 0)), false); // 23
  TS_ASSERT_EQUALS(m->Within(Pt3d(15, -5, 0)), false); // 24

} // GmPolygonUnitTests::testNoHoles
//------------------------------------------------------------------------------
/// \brief Test a polygon with holes.
/// \see StripPointTests::testWithHoles
/// \verbatim
// 35 -    *   *   *   *   *   *   *   *   *   *   *   *   *
//    |
// 30 -    *   5---*---*---*---*---*---*---*---*---*---4   *
//    |        |                                       |
// 25 -    *   *   *   *   *   *   *   *   *   *   *   *   *
//    |        |                                       |
// 20 -    *   *   *   1---*---2   *   1---*---2   *   *   *
//    |        |       |       |       |       |       |
// 15 -    *   *   *   *   *   3   *   *   *   3   *   3   *
//    |        |       |       |       |       |       |
// 10 -    *   *   *   0---5---4   *   0---5---4   *   *   *
//    |        |                                       |
//  5 -    *   *   *   *   *   *   *   *   *   *   *   *   *
//    |        |                                       |
//  0 -    *   0---*---*---*---*---1---*---*---*---*---2   *
//    |
// -5 -    *   *   *   *   *   *   *   *   *   *   *   *   *
//
//         |---|---|---|---|---|---|---|---|---|---|---|---|
//        -5   0   5  10  15  20  25  30  35  40  45  50  55
/// \endverbatim
//------------------------------------------------------------------------------
void GmPolygonUnitTests::testWithHoles()
{
  using xms::Pt3d;

  xms::VecPt3d outPoly;
  xms::VecPt3d2d inPolys;
  xms::VecPt3d pts;
  SetUpPolyWithHoles1(outPoly, inPolys, pts);

  // Don't repeat the first point on the inner poly to test we can handle that
  inPolys[1].erase(inPolys[1].begin() + inPolys[1].size() - 1);
  // Make an inner poly CW to test that we can handle that
  std::reverse(inPolys[1].begin(), inPolys[1].end());

  // Set up GmPolygon class
  BSHP<xms::GmPolygon> m = xms::GmPolygon::New();
  m->Setup(outPoly, inPolys);

  // CoveredBy
  {
    std::vector<int> actual(pts.size(), 0);
    for (size_t i = 0; i < pts.size(); ++i)
    {
      actual[i] = (m->CoveredBy(pts[i]) != 0);
    }

    std::vector<int> expected{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 12
                              0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,  // 25
                              0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,  // 38
                              0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,  // 51
                              0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0,  // 64
                              0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,  // 77
                              0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,  // 90
                              0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,  // 103
                              0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // 116
    TS_ASSERT_EQUALS_VEC(expected, actual);
  }

  // Within
  {
    std::vector<int> actual(pts.size(), 0);
    for (size_t i = 0; i < pts.size(); ++i)
    {
      actual[i] = (m->Within(pts[i]) != 0);
    }

    std::vector<int> expected{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 12
                              0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 25
                              0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,  // 38
                              0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0,  // 51
                              0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0,  // 64
                              0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0,  // 77
                              0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,  // 90
                              0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 103
                              0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // 116
    TS_ASSERT_EQUALS_VEC(expected, actual);
  }

} // GmPolygonUnitTests::testWithHoles
//------------------------------------------------------------------------------
/// \brief Used in tests to create a polygon with two holes.
/// \param a_outPoly The outside polygon.
/// \param a_inPolys The inside polygons.
/// \param a_ptsToTest Locations to test if they are inside the polygon.
/// The first point is not repeated. Outer poly is CCW, inners are CW.
/// \verbatim
// 30 -        1---------------------------------------2
//    |        |                                       |
// 25 -        |                                       |
//    |        |                                       |
// 20 -        |       5-------4       5-------4       |
//    |        |       |       |       |       |       |
// 15 -        |       |       3       |       3       3
//    |        |       |       |       |       |       |
// 10 -        |       0---1---2       0---1---2       |
//    |        |                                       |
//  5 -        |                                       |
//    |        |                                       |
//  0 -        0-------------------5-------------------4
//
//             |---|---|---|---|---|---|---|---|---|---|
//             0   5  10  15  20  25  30  35  40  45  50  55
/// \endverbatim
//------------------------------------------------------------------------------
void GmPolygonUnitTests::SetUpPolyWithHoles1(xms::VecPt3d& a_outPoly,
                                             xms::VecPt3d2d& a_inPolys,
                                             xms::VecPt3d& a_ptsToTest)
{
  using xms::Pt3d;

  a_outPoly.clear();
  a_inPolys.clear();

  // Set up inner and outer polygons
  // Clockwise outer poly. First point repeated.
  a_outPoly = {{0, 0, 0}, {0, 30, 0}, {50, 30, 0}, {50, 15, 0}, {50, 0, 0}, {25, 0, 0}, {0, 0, 0}};

  // Counter clockwise inner polys. First point repeated.
  xms::VecPt3d inPoly1{{10, 10, 0}, {15, 10, 0}, {20, 10, 0}, {20, 15, 0},
                       {20, 20, 0}, {10, 20, 0}, {10, 10, 0}};
  xms::VecPt3d inPoly2{{30, 10, 0}, {35, 10, 0}, {40, 10, 0}, {40, 15, 0},
                       {40, 20, 0}, {30, 20, 0}, {30, 10, 0}};
  a_inPolys.push_back(inPoly1);
  a_inPolys.push_back(inPoly2);

  // Set up points to test
  a_ptsToTest.clear();
  for (int y = 35; y >= -5; y -= 5)
  {
    for (int x = -5; x <= 55; x += 5)
    {
      a_ptsToTest.push_back(Pt3d(x, y, 0));
    }
  }

} // GmPolygonUnitTests::SetUpPolyWithHoles1
//------------------------------------------------------------------------------
/// \brief Used in tests to create a polygon with lots of segments and 2 holes.
/// \param a_outPoly The outside polygon.
/// \param a_inPoly The inside polygons.
/// The first point is not repeated. Outer poly is CCW, inners are CW.
/// \verbatim
// 30 -        8---9  12--13  16--17  20--21  24------25
//    |        |   |   |   |   |   |   |   |   |       |
// 25 -        7  10--11  14--15  18--19  22--23  27--26
//    |        |                                   |
// 20 -        6---5   2---3---4---5---6---7---8  28--29
//    |            |   |                       |       |
// 15 -        3---4   1  18---17 14---13 10---9  31--30
//    |        |       |   |   |   |   |   |       |
// 10 -        2       0---19 16---15 12---11     32--33
//    |        |                                       |
//  5 -        1   51-50   47-46   43-42   39-38   35-34
//    |        |   |   |   |   |   |   |   |   |   |
//  0 -        0--52   49-48  45--44   41-40   37-36
//
//             |---|---|---|---|---|---|---|---|---|---|
//             0   5  10  15  20  25  30  35  40  45  50  55
/// \endverbatim
//------------------------------------------------------------------------------
void GmPolygonUnitTests::SetUpPolyWithHole(xms::VecPt3d& a_outPoly, xms::VecPt3d& a_inPoly)
{
  using xms::Pt3d;

  a_outPoly.clear();
  a_inPoly.clear();

  // Set up inner and outer polygons
  // Clockwise outer poly. First point repeated.
  double outPoly[] = {
    0,  0,  0,  5,  0,  10, 0,  15, 5,  15, 5,  20, 0,  20, 0,  25, 0,  30, 5,  30, // 9
    5,  25, 10, 25, 10, 30, 15, 30, 15, 25, 20, 25, 20, 30, 25, 30, 25, 25, 30, 25, // 19
    30, 30, 35, 30, 35, 25, 40, 25, 40, 30, 50, 30, 50, 25, 45, 25, 45, 20, 50, 20, // 29
    50, 15, 45, 15, 45, 10, 50, 10, 50, 5,  45, 5,  45, 0,  40, 0,  40, 5,  35, 5,  // 39
    35, 0,  30, 0,  30, 5,  25, 5,  25, 0,  20, 0,  20, 5,  15, 5,  15, 0,  10, 0,  // 49
    10, 5,  5,  5,  5,  0,  0,  0};                                                 // 52
  a_outPoly = xms::gmArrayToVecPt3d(outPoly, XM_COUNTOF(outPoly));

  // double outPolyBA[] =
  //{ 0,0,   5,0,   5,5,  10,5,  10,0,  15,0,  15,5,  20,5,  20,0,  25,0,  // 9
  // 25,5,  30,5,  30,0,  35,0,  35,5,  40,5,  40,0,  45,0,  45,5,  50,5,  // 19
  // 50,10, 45,10, 45,15, 50,15, 50,20, 45,20, 45,25, 50,25, 50,30, 40,30, // 29
  // 40,25, 35,25, 35,30, 30,30, 30,25, 25,25, 25,30, 20,30, 20,25, 15,25, // 39
  // 15,30, 10,30, 10,25,  5,25,  5,30,  0,30,  0,25,  0,20,  5,20,  5,15, // 49
  //  0,15,  0,10,  0,5, 0,0}; // 52
  // xms::VecPt3d outPolyB = xms::gmArrayToVecPt3d(outPolyBA, XM_COUNTOF(outPolyBA));
  // std::reverse(outPolyB.begin(), outPolyB.end());
  // TS_ASSERT_EQUALS(outPolyB, a_outPoly);

  // Counter clockwise inner poly. First point repeated.
  double inPoly1[] = {
    10, 10, 15, 10, 15, 15, 20, 15, 20, 10, 25, 10, 25, 15, 30, 15, 30, 10, 35, 10, // 9
    35, 15, 40, 15, 40, 20, 35, 20, 30, 20, 25, 20, 20, 20, 15, 20, 10, 20, 10, 15, // 19
    10, 10};
  a_inPoly = xms::gmArrayToVecPt3d(inPoly1, XM_COUNTOF(inPoly1));

  // double inPoly1BA[] =
  //{10,10, 10,15, 10,20, 15,20, 20,20, 25,20, 30,20, 35,20, 40,20, 40,15,  // 9
  // 35,15, 35,10, 30,10, 30,15, 25,15, 25,10, 20,10, 20,15, 15,15, 15,10}; // 19
  // xms::VecPt3d inPoly1B = xms::gmArrayToVecPt3d(inPoly1BA, XM_COUNTOF(inPoly1BA));
  // std::reverse(inPoly1B.begin(), inPoly1B.end());
  // TS_ASSERT_EQUALS(inPoly1B, a_inPoly);

} // GmPolygonUnitTests::SetUpPolyWithHole
//------------------------------------------------------------------------------
/// \brief tests calculating the minimum distance to the boundary of a polygon.
//------------------------------------------------------------------------------
void GmPolygonUnitTests::testMinDistanceToBoundary()
{
  using namespace xms;
  std::vector<Pt3d> pts{{0, 0, 0}, {0, 10, 0}, {10, 10, 0}, {10, 0, 0}};
  std::vector<std::vector<Pt3d>> v2d;
  GmPolygonImpl i;
  i.Setup(pts, v2d);
  Pt3d pt(0, 20, 0);
  TS_ASSERT_EQUALS(10, i.MinDistanceToBoundary(pt));
  pt.x = 20;
  pt.y = 0;
  TS_ASSERT_EQUALS(10, i.MinDistanceToBoundary(pt));
  pt.y = 20;
  double d(200);
  d = sqrt(d);
  TS_ASSERT_EQUALS(d, i.MinDistanceToBoundary(pt));
  pt.x = 5;
  pt.y = 1;
  TS_ASSERT_EQUALS(1, i.MinDistanceToBoundary(pt));
  std::vector<Pt3d> in{{1, 1, 0}, {2, 1, 0}, {2, 2, 0}, {1, 2, 0}};
  v2d.push_back(in);
  in = {{8, 8, 0}, {9, 8, 0}, {9, 9, 0}, {8, 9, 0}};
  v2d.push_back(in);
  i.Setup(pts, v2d);
  pt.x = 3;
  pt.y = 2;
  TS_ASSERT_EQUALS(1, i.MinDistanceToBoundary(pt));
  pt.x = 7;
  pt.y = 8;
  TS_ASSERT_EQUALS(1, i.MinDistanceToBoundary(pt));
} // GmPolygonUnitTests::testMinDistanceToBoundary
//------------------------------------------------------------------------------
/// \brief tests intersection of polygons
//------------------------------------------------------------------------------
void GmPolygonUnitTests::testIntersection()
{
  BSHP<xms::GmPolygon> p1 = xms::GmPolygon::New();
  BSHP<xms::GmPolygon> p2 = xms::GmPolygon::New();

  xms::VecPt3d out1 = {{0.0, 0.0}, {0.0, 1.0}, {1.0, 1.0}, {1.0, 0.0}, {0.0, 0.0}};
  xms::VecPt3d2d in;
  p1->Setup(out1, in);
  xms::VecPt3d out2 = {{-1.0, -1.0}, {-1.0, 2.0}, {2.0, 2.0}, {2.0, -1.0}, {-1.0, -1.0}};
  p2->Setup(out2, in);
  std::vector<BSHP<xms::GmPolygon>> output;
  p1->Intersection(*p2, output);
  TS_ASSERT(output.size() == 1);
  if (output.size() != 1)
    return;
  xms::VecPt3d base = out1;
  xms::VecPt3d outputPts;
  xms::VecPt3d2d outputPtsInPoly;
  output[0]->GetPoints(outputPts, outputPtsInPoly);
  double tol(1e-12);
  TS_ASSERT_DELTA_VECPT3D(base, outputPts, tol);

  out2 = {{0.5, -1.0}, {0.5, 2.0}, {2.0, 2.0}, {2.0, -1.0}, {0.5, -1.0}};
  p2->Setup(out2, in);
  p1->Intersection(*p2, output);
  TS_ASSERT(output.size() == 1);
  if (output.size() != 1)
    return;
  base = {{0.5, 1.0}, {1.0, 1.0}, {1.0, 0.0}, {0.5, 0.0}, {0.5, 1.0}};
  output[0]->GetPoints(outputPts, outputPtsInPoly);
  tol = 1e-6;
  TS_ASSERT_DELTA_VECPT3D(base, outputPts, tol);

} // GmPolygonUnitTests::testIntersection
//------------------------------------------------------------------------------
/// \brief tests union of polygons
//------------------------------------------------------------------------------
void GmPolygonUnitTests::testUnion()
{
  BSHP<xms::GmPolygon> p1 = xms::GmPolygon::New();
  BSHP<xms::GmPolygon> p2 = xms::GmPolygon::New();

  xms::VecPt3d out1 = {{0.0, 0.0}, {0.0, 1.0}, {1.0, 1.0}, {1.0, 0.0}, {0.0, 0.0}};
  xms::VecPt3d2d in;
  p1->Setup(out1, in);
  xms::VecPt3d out2 = {{-1.0, -1.0}, {-1.0, 2.0}, {2.0, 2.0}, {2.0, -1.0}, {-1.0, -1.0}};
  p2->Setup(out2, in);
  std::vector<BSHP<xms::GmPolygon>> output;
  p1->Union(*p2, output);
  TS_ASSERT(output.size() == 1);
  if (output.size() != 1)
    return;
  xms::VecPt3d base = out2;
  xms::VecPt3d outputPts;
  xms::VecPt3d2d outputPtsInPoly;
  output[0]->GetPoints(outputPts, outputPtsInPoly);
  double tol(1e-12);
  TS_ASSERT_DELTA_VECPT3D(base, outputPts, tol);

  out2 = {{0.5, -1.0}, {0.5, 2.0}, {2.0, 2.0}, {2.0, -1.0}, {0.5, -1.0}};
  p2->Setup(out2, in);
  p1->Union(*p2, output);
  TS_ASSERT(output.size() == 1);
  if (output.size() != 1)
    return;
  base = {{0.5, 1.0}, {0.5, 2.0}, {2.0, 2.0}, {2.0, -1.0}, {0.5, -1.0},
          {0.5, 0.0}, {0.0, 0.0}, {0.0, 1.0}, {0.5, 1.0}};
  output[0]->GetPoints(outputPts, outputPtsInPoly);
  tol = 1e-6;
  TS_ASSERT_DELTA_VECPT3D(base, outputPts, tol);

  out2 = {{2.0, 0.0}, {2.0, 1.0}, {3.0, 1.0}, {3.0, 0.0}, {2.0, 0.0}};
  p2->Setup(out2, in);
  p1->Union(*p2, output);
  TS_ASSERT(output.size() == 2);
  output[0]->GetPoints(outputPts, outputPtsInPoly);
  base = out1;
  tol = 1e-12;
  TS_ASSERT_DELTA_VECPT3D(base, outputPts, tol);
  output[1]->GetPoints(outputPts, outputPtsInPoly);
  base = out2;
  TS_ASSERT_DELTA_VECPT3D(base, outputPts, tol);
} // GmPolygonUnitTests::testUnion
////////////////////////////////////////////////////////////////////////////////
/// \brief
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/// \brief Returns the test group.
/// \return CxxTest::TestGroup reference.
//------------------------------------------------------------------------------
// virtual
#ifndef CXXTEST4
const CxxTest::TestGroup& GmPolygonIntermediateTests::group()
{
  return *CxxTest::TestGroup::GetGroup(CxxTest::TG_INTERMEDIATE);
} // GmPolygonIntermediateTests::group
#endif
//------------------------------------------------------------------------------
/// \brief Test lots of points for timing purposes. Only in release, not debug.
/// \see PointInPolyTests::testSpeed
/// \verbatim
// 30 -       45--44  41--40  37--36  33--32  29------28
//    |        |   |   |   |   |   |   |   |   |       |
// 25 -       46  43--42  39--38  35--34  31--30  26--27
//    |        |                                   |
// 20 -       47---48  2---3---4---5---6---7---8  25--24
//    |            |   |                       |       |
// 15 -       50---49  1  18---17 14---13 10---9  22--23
//    |        |       |   |   |   |   |   |       |
// 10 -       51       0---19 16---15 12---11     21--20
//    |        |                                       |
//  5 -       52   2---3   6---7  10--11  14--15  18--19
//    |        |   |   |   |   |   |   |   |   |   |
//  0 -        0---1   4---5   8---9  12--13  16--17
//
//             |---|---|---|---|---|---|---|---|---|---|
//             0   5  10  15  20  25  30  35  40  45  50  55
/// \endverbatim
//------------------------------------------------------------------------------
void GmPolygonIntermediateTests::testSpeed()
{
#if BOOST_OS_WINDOWS
#ifndef _DEBUG
  GmPointInPolyTester_GmPolygon tester;
  tester.DoTest();
#endif
#endif
} // GmPolygonIntermediateTests::testSpeed

#endif // CXX_TEST
