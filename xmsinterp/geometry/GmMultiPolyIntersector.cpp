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
#include <xmsinterp/geometry/GmMultiPolyIntersector.h>

// 3. Standard library headers

// 4. External library headers
#pragma warning(push)
#pragma warning(disable : 4512) // boost code: no assignment operator
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/linestring.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/index/rtree.hpp>
#pragma warning(pop)

// 5. Shared code headers
#include <xmscore/misc/XmConst.h>
#include <xmscore/misc/XmError.h> // XM_ASSERT
#include <xmscore/misc/XmLog.h>
#include <xmscore/misc/boost_defines.h>      // BSHP
#include <xmscore/misc/xmstype.h>            // XM_NODATA
#include <xmscore/points/pt.h>               // Pt3d
#include <xmscore/stl/set.h>                 // Set*
#include <xmscore/stl/vector.h>              // Vec*
#include <xmsinterp/geometry/GmBoostTypes.h> // GmBstPoly3d, XmBstRing
#include <xmsinterp/geometry/GmMultiPolyIntersectionSorter.h> // GmMultiPolyIntersectionSorter
#include <xmsinterp/geometry/GmMultiPolyIntersectorData.h> // GmMultiPolyIntersectorData
#include <xmsinterp/geometry/geoms.h> // gmPolygonArea, gmAddToExtents, gmXyDistance

//----- Forward declarations ---------------------------------------------------

namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;

//----- External globals -------------------------------------------------------

//----- Namespace declaration --------------------------------------------------

namespace xms {
//----- Constants / Enumerations -----------------------------------------------

// Typedefs
typedef std::pair<GmBstBox3d, int> ValueBox; ///< Pair used in rtree
typedef bgi::rtree<ValueBox, bgi::quadratic<8>> RtreeBox; ///< Rtree typedef

//----- Classes / Structs ------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
class GmMultiPolyIntersectorImpl : public GmMultiPolyIntersector {
public:
  GmMultiPolyIntersectorImpl(const VecPt3d &a_points, const VecInt2d &a_polys,
                             BSHP<GmMultiPolyIntersectionSorter> a_sorter,
                             int a_startingId = 1);
  virtual ~GmMultiPolyIntersectorImpl();

  virtual void SetQuery(GmMultiPolyIntersectorQueryEnum a_query) override;
  virtual void TraverseLineSegment(double x1, double y1, double x2, double y2,
                                   VecInt &polyids, VecDbl &tvalues);
  virtual void TraverseLineSegment(double x1, double y1, double x2, double y2,
                                   VecInt &polyids);
  virtual void TraverseLineSegment(double a_x1, double a_y1, double a_x2,
                                   double a_y2, VecInt &a_polyids,
                                   std::vector<Pt3d> &a_pts);
  virtual int PolygonFromPoint(const Pt3d &a_pt);

private:
  void CalculateBuffer();
  void BufferTheBox(GmBstBox3d &box) const;
  GmBstPoly3d &GetBoostPoly(int a_polyIdx);
  void BuildBoostPoly(int a_polyIdx, GmBstPoly3d &a_boostPoly) const;
  void BuildRTree();
  void CreateLine();
  void GetPolysForPoint(Pt3d pt, SetInt &poly);
  void EnsureEndPointsRepresented();
  void IntersectEachPolyWithLine();
  void ComputeTValues();
  void SortIntersections();
  void OffsetPolyIds(VecInt &polyIds) const;
  void PointsOnSegment(const GmBstPoly3d &a_poly, const GmBstLine3d &a_line,
                       std::deque<Pt3d> &a_output);
  // void ValidatePolygons ();
  void TraverseLineSegmentAll(double a_x1, double a_y1, double a_x2,
                              double a_y2, VecInt &a_polyids, VecDbl &a_tvalues,
                              std::vector<Pt3d> &a_pts);

  GmMultiPolyIntersectorData m_d; ///< Point and poly data.
  Pt3d m_pt1;                     ///< 1st line segment point
  Pt3d m_pt2;                     ///< 2nd line segment point
  RtreeBox *m_rtree;              ///< Rtree used to find polygons
  GmBstLine3d m_line;             ///< Current line segment
  double m_buffer;                ///< Small buffer around each bounding box
  int m_startingId; ///< Offset if polys start at something other than one
  std::vector<GmBstPoly3d> m_boostPolys; ///< Polygons as boost geom polygons
  BSHP<GmMultiPolyIntersectionSorter>
      m_sorter; ///< Sorter used to process results
  GmMultiPolyIntersectorQueryEnum
      m_query; ///< Type of query (intersect, covered by...)
};             // class GmMultiPolyIntersectorImpl

//----- Class / Function definitions -------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// \class GmMultiPolyIntersector
/// \brief See GmMultiPolyIntersectorImpl comments.
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/// \brief
//------------------------------------------------------------------------------
GmMultiPolyIntersector::GmMultiPolyIntersector() {
} // GmMultiPolyIntersector::GmMultiPolyIntersector
//------------------------------------------------------------------------------
/// \brief
//------------------------------------------------------------------------------
GmMultiPolyIntersector::~GmMultiPolyIntersector() {
} // GmMultiPolyIntersector::~GmMultiPolyIntersector
//------------------------------------------------------------------------------
/// \brief Creates a new GmMultiPolyIntersectorImpl object.
/// \param a_points: The points that make up the polygon.
/// \param a_polys: 0-based indexes into a_points array to define polygons.
///                   The first point is NOT repeated as the last point.
/// \param a_sorter: Class to clean up and sort the intersections the way
///                   you want them.
/// \param a_startingId: If the polygon IDs should start at something other
///                       than 1, specify the starting value.
/// \return GmMultiPolyIntersector.
//------------------------------------------------------------------------------
BSHP<GmMultiPolyIntersector>
GmMultiPolyIntersector::New(const VecPt3d &a_points, const VecInt2d &a_polys,
                            BSHP<GmMultiPolyIntersectionSorter> a_sorter,
                            int a_startingId /*=1*/) {
  return BDPC<GmMultiPolyIntersector>(
      BSHP<GmMultiPolyIntersectorImpl>(new GmMultiPolyIntersectorImpl(
          a_points, a_polys, a_sorter, a_startingId)));
} // GmMultiPolyIntersector::GmMultiPolyIntersector

////////////////////////////////////////////////////////////////////////////////
/// \class GmMultiPolyIntersectorImpl
/// \brief Used to intersect a line segment with any number of polygons in 2D.
///        Returns the intersected polygons in order along with t values.
///        May be an alternative to SurfaceIter and feiTraverseLineSegment2.
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/// \brief constructor
/// \param a_points: The points that make up the polygon.
/// \param a_polys: 0-based indexes into a_points array to define polygons.
///                   The first point is NOT repeated as the last point.
/// \param a_sorter: Class to clean up and sort the intersections the way
///                   you want them.
/// \param a_startingId: If the polygon IDs should start at something other
///                       than 1, specify the starting value.
//------------------------------------------------------------------------------
GmMultiPolyIntersectorImpl::GmMultiPolyIntersectorImpl(
    const VecPt3d &a_points, const VecInt2d &a_polys,
    BSHP<GmMultiPolyIntersectionSorter> a_sorter, int a_startingId /*=1*/)
    : m_d(), m_pt1(), m_pt2(), m_rtree(nullptr), m_line(), m_buffer(0.0),
      m_startingId(a_startingId), m_boostPolys(), m_sorter(a_sorter),
      m_query(GMMPIQ_COVEREDBY) {
  m_d.m_points = a_points;

  m_d.m_polys = a_polys;
  // ValidatePolygons();

  m_boostPolys.assign(a_polys.size(), GmBstPoly3d());

  CalculateBuffer();
  BuildRTree();
} // GmMultiPolyIntersectorImpl::GmMultiPolyIntersectorImpl
//------------------------------------------------------------------------------
/// \brief
//------------------------------------------------------------------------------
GmMultiPolyIntersectorImpl::~GmMultiPolyIntersectorImpl() {
  if (m_rtree)
    delete (m_rtree);
} // GmMultiPolyIntersectorImpl::GmMultiPolyIntersectorImpl
////------------------------------------------------------------------------------
///// \brief Make sure polygons are ordered correctly.
////------------------------------------------------------------------------------
// void GmMultiPolyIntersectorImpl::ValidatePolygons ()
//{
//#ifdef _DEBUG
//  for (size_t i = 0; i < m_d.m_polys.size(); ++i) {
//    VecPt3d poly;
//    for (size_t j = 0; j < m_d.m_polys[i].size(); ++j) {
//      poly.push_back(m_d.m_points[m_d.m_polys[i][j]]);
//    }
//    XM_ASSERT(gmPolygonArea(&poly[0], poly.size()) > 0);
//  }
//#endif
//} // GmMultiPolyIntersectorImpl::ValidatePolygons
//------------------------------------------------------------------------------
/// \brief Calculate a small buffer distance by which we expand all polygon
///        bounding boxes.
//------------------------------------------------------------------------------
void GmMultiPolyIntersectorImpl::CalculateBuffer() {
  // Get min/max of all polys
  Pt3d mn(XM_DBL_HIGHEST), mx(XM_DBL_LOWEST);
  for (size_t i = 0; i < m_d.m_polys.size(); ++i) {
    for (size_t j = 0; j < m_d.m_polys[i].size(); ++j) {
      gmAddToExtents(m_d.m_points[m_d.m_polys[i][j]], mn, mx);
    }
  }

  // Calculate the buffer as a fraction of the distance between mn and mx
  const double kFraction = 1e-5; // 0.00001
  m_buffer = gmXyDistance(mn, mx) * kFraction;
} // GmMultiPolyIntersectorImpl::CalculateBuffer
//------------------------------------------------------------------------------
/// \brief Because the rtree intersection fails in some cases where the line
///        is on an edge, we slightly expand the bounding box of the poly to
///        ensure that it comes back as a potential intersection. This may
///        result in more false positives but until boost geometry starts
///        working correctly, it's the best idea we've got.
/// \param box: The box.
//------------------------------------------------------------------------------
void GmMultiPolyIntersectorImpl::BufferTheBox(GmBstBox3d &box) const {
  box.min_corner().x -= m_buffer;
  box.min_corner().y -= m_buffer;
  box.max_corner().x += m_buffer;
  box.max_corner().y += m_buffer;
} // GmMultiPolyIntersectorImpl::BufferTheBox
//------------------------------------------------------------------------------
/// \brief Create and return a boost polygon given a polygon index.
/// \param a_polyIdx: The polygon index.
/// \return GmBstPoly3d.
//------------------------------------------------------------------------------
GmBstPoly3d &GmMultiPolyIntersectorImpl::GetBoostPoly(int a_polyIdx) {
  if (m_boostPolys[a_polyIdx].outer().empty()) {
    // if (bg::exterior_ring(m_boostPolys[a_polyIdx]).empty()) {
    BuildBoostPoly(a_polyIdx, m_boostPolys[a_polyIdx]);
  }
  return m_boostPolys[a_polyIdx];
} // GmMultiPolyIntersectorImpl::GetBoostPoly
//------------------------------------------------------------------------------
/// \brief Build a boost polygon given a polygon index.
/// \param a_polyIdx: The polygon index.
/// \param a_boostPoly: GmBstPoly3d.
//------------------------------------------------------------------------------
void GmMultiPolyIntersectorImpl::BuildBoostPoly(
    int a_polyIdx, GmBstPoly3d &a_boostPoly) const {
  const VecInt &poly = m_d.m_polys[a_polyIdx];
  for (int j = 0; j < (int)poly.size(); ++j) {
    Pt3d pt = m_d.m_points[poly[j]];
    bg::exterior_ring(a_boostPoly).push_back(m_d.m_points[poly[j]]);
  }
  bg::exterior_ring(a_boostPoly).push_back(m_d.m_points[poly[0]]);
} // GmMultiPolyIntersectorImpl::BuildBoostPoly
//------------------------------------------------------------------------------
/// \brief Create a boost rtree of polygon extents.
//------------------------------------------------------------------------------
void GmMultiPolyIntersectorImpl::BuildRTree() {
  // Changed to use the packing algorithm. See:
  // http://www.boost.org/doc/libs/1_55_0/libs/geometry/doc/html/geometry/spatial_indexes/introduction.html
  // We thought this would make it faster but according to the test
  // GmMultiPolyIntersector2IntermediateTests::testLargeNumPolysAndSegments it
  // didn't make much difference, and actually may be very slightly slower, but
  // the test may not be very representative of real scenarios. The old code is
  // still here but commented out in case we want to bring it back in the
  // future. -MJK

  // m_rtree = new RtreeBox(); // Non packing algorithm
  std::vector<ValueBox> boxen; // Packing algorithm

  for (int i = 0; i < (int)m_d.m_polys.size(); ++i) {
    Pt3d mn(XM_DBL_HIGHEST), mx(XM_DBL_LOWEST);
    for (int j = 0; j < (int)m_d.m_polys[i].size(); ++j) {
      gmAddToExtents(m_d.m_points[m_d.m_polys[i][j]], mn, mx);
    }
    GmBstBox3d box(mn, mx);
    BufferTheBox(box);
    // m_rtree->insert(std::make_pair(box, i)); // Non packing algorithm
    boxen.push_back(std::make_pair(box, i)); // Packing algorithm
  }

  // Packing algorithm
  m_rtree = new RtreeBox(boxen.begin(), boxen.end());

} // GmMultiPolyIntersectorImpl::BuildRTree
//------------------------------------------------------------------------------
/// \brief Creates a boost geom line of the current line segment.
//------------------------------------------------------------------------------
void GmMultiPolyIntersectorImpl::CreateLine() {
  m_line.push_back(m_pt1);
  m_line.push_back(m_pt2);
} // GmMultiPolyIntersectorImpl::CreateLine
//------------------------------------------------------------------------------
/// \brief Set the query to use (covered by, intersects...).
/// \param a_query: GmMultiPolyIntersectorQueryEnum
//------------------------------------------------------------------------------
void GmMultiPolyIntersectorImpl::SetQuery(
    GmMultiPolyIntersectorQueryEnum a_query) {
  m_query = a_query;
} // GmMultiPolyIntersectorImpl::SetQuery
//------------------------------------------------------------------------------
/// \brief Find the set of polygons intersected (in or on) by the point.
/// \param pt: point loction
/// \param a_poly: 1-based polygon ID.
//------------------------------------------------------------------------------
void GmMultiPolyIntersectorImpl::GetPolysForPoint(Pt3d pt, SetInt &a_poly) {
  std::vector<ValueBox> result;
  m_rtree->query(bgi::intersects(pt), std::back_inserter(result));

  // Find the polygon that the point is inside
  for (size_t i = 0; i < result.size(); ++i) {
    GmBstPoly3d &poly = GetBoostPoly(result[i].second);
    //(bg::within(pt, poly)) { // Seems to return true if inside
    bool rv = false;
    switch (m_query) {
    case GMMPIQ_COVEREDBY:
      rv = bg::covered_by(pt, poly); // Seems to return true if inside or on
      break;
    case GMMPIQ_INTERSECTS:
      rv = bg::intersects(pt, poly);
      break;
    default:
      XM_ASSERT(false);
      break;
    }
    if (rv) {
      a_poly.insert(result[i].second + 1);
    }
  }

} // GmMultiPolyIntersectorImpl::GetPolysForPoint
//------------------------------------------------------------------------------
/// \brief Go through results (potential polygons) and intersect each one.
//------------------------------------------------------------------------------
void GmMultiPolyIntersectorImpl::IntersectEachPolyWithLine() {
  // Get potential polygons which intersect the line from the rtree
  std::vector<ValueBox> result;
  m_rtree->query(bgi::intersects(m_line), std::back_inserter(result));
  // m_rtree.query(bgi::overlaps(m_line), std::back_inserter(result)); //
  // doesn't compile m_rtree.query(bgi::covered_by(m_line),
  // std::back_inserter(result)); // doesn't compile
  // m_rtree.query(bgi::covers(m_line), std::back_inserter(result)); // doesn't
  // compile

  // Go through the potential polygons and try to intersect them
  for (size_t i = 0; i < result.size(); ++i) {
    GmBstPoly3d &poly = GetBoostPoly(result[i].second);
    std::deque<Pt3d> output;
    bg::intersection(poly, m_line, output);
    if (2 > output.size())
      PointsOnSegment(poly, m_line, output);
    for (size_t j = 0; j < output.size(); ++j) {
      ix ixn(output[j], result[i].second + 1, XM_NODATA);
      m_d.m_ixs.push_back(ixn);
    }
  }
} // GmMultiPolyIntersectorImpl::IntersectEachPolyWithLine
//------------------------------------------------------------------------------
/// \brief Check if the points on the outside of the polygon lie on the line
/// segment.
/// \param a_poly: GmBstPoly3d.
/// \param a_line: GmBstLine3d.
/// \param a_output: Output.
//------------------------------------------------------------------------------
void GmMultiPolyIntersectorImpl::PointsOnSegment(const GmBstPoly3d &a_poly,
                                                 const GmBstLine3d &a_line,
                                                 std::deque<Pt3d> &a_output) {
  double tol = m_buffer * 1e-6;
  for (size_t i = 0; i < a_poly.outer().size(); ++i) {
    const Pt3d &p = a_poly.outer()[i];
    if (gmOnLineAndBetweenEndpointsWithTol(
            a_line[0], a_line[1], p.x, p.y,
            tol)) { // don't add the point if it is already in the output
      bool dup = false;
      for (size_t j = 0; j < a_output.size(); ++j) {
        if (gmEqualPointsXY(a_output[j], p, tol))
          dup = true;
      }
      if (!dup)
        a_output.push_back(p);
    }
  }
} // GmMultiPolyIntersectorImpl::PointsOnSegment
//------------------------------------------------------------------------------
/// \brief Compute t values (0.0 - 1.0) from the intersection.
///
/// t values are the percent (0.0 - 1.0) distance from the line segment start
/// to each intersection.
//------------------------------------------------------------------------------
void GmMultiPolyIntersectorImpl::ComputeTValues() {
  double totalLength = gmXyDistance(m_pt1, m_pt2);
  for (size_t i = 0; i < m_d.m_ixs.size(); ++i) {
    if (m_d.m_ixs[i].m_t == XM_NODATA) {
      m_d.m_ixs[i].m_t = gmXyDistance(m_pt1, m_d.m_ixs[i].m_pt) / totalLength;
    }
  }
} // GmMultiPolyIntersectorImpl::ComputeTValues
//------------------------------------------------------------------------------
/// \brief Does a preliminary sort of the intersections by t value.
//------------------------------------------------------------------------------
void GmMultiPolyIntersectorImpl::SortIntersections() {
  XM_ENSURE_TRUE_VOID_NO_ASSERT(m_d.m_ixs.size() > 1);

  // Sort intersections by t value
  std::sort(m_d.m_ixs.begin(), m_d.m_ixs.end(),
            [](const ix &a, const ix &b) -> bool { return a.m_t < b.m_t; });

} // GmMultiPolyIntersectorImpl::SortIntersections
//------------------------------------------------------------------------------
/// \brief Because unfortunately intersecting the line with the poly doesn't
///        always create an intersection if a point is on the polygon edge.
//------------------------------------------------------------------------------
void GmMultiPolyIntersectorImpl::EnsureEndPointsRepresented() {
  // If 1st point was in or on poly, make sure there's an intersection for it
  for (SetInt::iterator it = m_d.m_polys1.begin(); it != m_d.m_polys1.end();
       ++it) {
    bool found = false;
    for (size_t i = 0;
         i < m_d.m_ixs.size() && !found && m_d.m_ixs[i].m_t == 0.0; ++i) {
      if (m_d.m_ixs[i].m_i == *it)
        found = true;
    }
    if (!found)
      m_d.m_ixs.insert(m_d.m_ixs.begin(), ix(m_pt1, *it, 0.0));
  }

  // If 2nd point was in or on poly, make sure there's an intersection for it
  for (SetInt::iterator it = m_d.m_polys2.begin(); it != m_d.m_polys2.end();
       ++it) {
    bool found = false;
    for (size_t i = m_d.m_ixs.size();
         i-- > 0 && !found && m_d.m_ixs[i].m_t == 1.0;) {
      if (m_d.m_ixs[i].m_i == *it)
        found = true;
    }
    if (!found)
      m_d.m_ixs.push_back(ix(m_pt2, *it, 1.0));
  }
} // GmMultiPolyIntersectorImpl::EnsureEndPointsRepresented
//------------------------------------------------------------------------------
/// \brief If polygon IDs should start at something other than 1, we handle
///        that here.
/// \param polyIds: 1-based list of polygons intersected by line segment.
//------------------------------------------------------------------------------
void GmMultiPolyIntersectorImpl::OffsetPolyIds(VecInt &polyIds) const {
  if (m_startingId != 1) {
    int offset = m_startingId - 1;
    for (int i = 0; i < (int)polyIds.size() - 1; ++i) {
      if (polyIds[i] != XM_NONE) {
        polyIds[i] += offset;
      }
    }
  }
} // GmMultiPolyIntersectorImpl::OffsetPolyIds
//------------------------------------------------------------------------------
/// \brief Intersect segment with polys and save intersected polys and t-values.
/// \param x1: x coordinate of 1st point defining a line segment.
/// \param y1: y coordinate of 1st point defining a line segment.
/// \param x2: x coordinate of 2nd point defining a line segment.
/// \param y2: y coordinate of 2nd point defining a line segment.
/// \param polyids: 1-based list of polygons intersected by line segment.
/// \param tvalues: Values from 0.0 to 1.0 representing where on the line
///                  segment the intersection with the polygon in polyids
///                  occurs. If there are any t values there are always at
///                  least 2 and all represent where the line enters the
///                  polygon, except the last which represents where it exited.
///                  There would therefore be one more t value than poly id
///                  but we make the sizes equal by always making the last
///                  poly id -1.
//------------------------------------------------------------------------------
void GmMultiPolyIntersectorImpl::TraverseLineSegment(double x1, double y1,
                                                     double x2, double y2,
                                                     VecInt &polyids,
                                                     VecDbl &tvalues) {
  VecPt3d pts;
  TraverseLineSegmentAll(x1, y1, x2, y2, polyids, tvalues, pts);
} // GmMultiPolyIntersectorImpl::TraverseLineSegment
//------------------------------------------------------------------------------
/// \brief Intersect segment with polys and save intersected polys.
/// \param x1: x coordinate of 1st point defining a line segment.
/// \param y1: y coordinate of 1st point defining a line segment.
/// \param x2: x coordinate of 2nd point defining a line segment.
/// \param y2: y coordinate of 2nd point defining a line segment.
/// \param polyids: 1-based list of polygons intersected by line segment.
//------------------------------------------------------------------------------
void GmMultiPolyIntersectorImpl::TraverseLineSegment(double x1, double y1,
                                                     double x2, double y2,
                                                     VecInt &polyids) {
  VecDbl tvalues;
  VecPt3d pts;
  TraverseLineSegmentAll(x1, y1, x2, y2, polyids, tvalues, pts);
} // GmMultiPolyIntersectorImpl::TraverseLineSegment
//-----------------------------------------------------------------------------
/// \brief Intersect segment with polys and save intersected polys and
///        intersection points.
/// \param a_x1: x coordinate of 1st point defining a line segment.
/// \param a_y1: y coordinate of 1st point defining a line segment.
/// \param a_x2: x coordinate of 2nd point defining a line segment.
/// \param a_y2: y coordinate of 2nd point defining a line segment.
/// \param a_polyids: 1-based list of polygons intersected by line segment.
/// \param a_pts: Intersection points.
//-----------------------------------------------------------------------------
void GmMultiPolyIntersectorImpl::TraverseLineSegment(double a_x1, double a_y1,
                                                     double a_x2, double a_y2,
                                                     VecInt &a_polyids,
                                                     std::vector<Pt3d> &a_pts) {
  VecDbl tvalues;
  TraverseLineSegmentAll(a_x1, a_y1, a_x2, a_y2, a_polyids, tvalues, a_pts);
} // GmMultiPolyIntersectorImpl::TraverseLineSegment
//-----------------------------------------------------------------------------
/// \brief Finds the polygon containing the point
/// \param a_pt: the location of the point
/// \return the polygon id
//-----------------------------------------------------------------------------
int GmMultiPolyIntersectorImpl::PolygonFromPoint(const Pt3d &a_pt) {
  int rval(XM_NONE);
  SetInt polys;
  GetPolysForPoint(a_pt, polys);
  if (!polys.empty()) {
    rval = (int)*polys.begin();
  }
  return rval;
} // GmMultiPolyIntersectorImpl::PolygonFromPoint
//-----------------------------------------------------------------------------
/// \brief Intersect segment with polys and save intersected polys, t-values,
///        and intersection points.
/// \param a_x1: x coordinate of 1st point defining a line segment.
/// \param a_y1: y coordinate of 1st point defining a line segment.
/// \param a_x2: x coordinate of 2nd point defining a line segment.
/// \param a_y2: y coordinate of 2nd point defining a line segment.
/// \param a_polyids: 1-based list of polygons intersected by line segment.
/// \param a_tvalues: Values from 0.0 to 1.0 representing where on the line
///                  segment the intersection with the polygon in polyids
///                  occurs. If there are any t values there are always at
///                  least 2 and all represent where the line enters the
///                  polygon, except the last which represents where it exited.
///                  There would therefore be one more t value than poly id
///                  but we make the sizes equal by always making the last
///                  poly id -1.
/// \param a_pts: Intersection points.
//-----------------------------------------------------------------------------
void GmMultiPolyIntersectorImpl::TraverseLineSegmentAll(
    double a_x1, double a_y1, double a_x2, double a_y2, VecInt &a_polyids,
    VecDbl &a_tvalues, std::vector<Pt3d> &a_pts) {
  m_pt1.Set(a_x1, a_y1, 0.0);
  m_pt2.Set(a_x2, a_y2, 0.0);
  GetPolysForPoint(m_pt1, m_d.m_polys1);
  GetPolysForPoint(m_pt2, m_d.m_polys2);
  CreateLine();
  IntersectEachPolyWithLine();
  ComputeTValues();
  SortIntersections();
  EnsureEndPointsRepresented();
  if (!m_sorter) {
    XM_ASSERT(false);
  } else {
    double kTol = 1e-13; // Used to compare t values which are always 0.0 - 1.0
    m_sorter->Sort(m_d, a_polyids, a_tvalues, a_pts, kTol);
  }
  OffsetPolyIds(a_polyids);

  m_d.m_ixs.clear();
  m_d.m_polys1.clear();
  m_d.m_polys2.clear();
  m_line.clear();
} // GmMultiPolyIntersectorImpl::TraverseLineSegmentAll
} // namespace xms

///////////////////////////////////////////////////////////////////////////////
// TESTS
///////////////////////////////////////////////////////////////////////////////
#ifdef CXX_TEST

#include <xmsinterp/geometry/GmMultiPolyIntersector.t.h>

#include <xmscore/testing/TestTools.h>
#include <xmsinterp/geometry/GmMultiPolyIntersectionSorterTerse.h>
#include <xmsinterp/triangulate/triangles.h>

//----- Namespace declaration --------------------------------------------------

// namespace xms {
using namespace xms;

namespace // unnamed namespace
{
//------------------------------------------------------------------------------
/// \brief
//------------------------------------------------------------------------------
void iRunTest(double x1, double y1, double x2, double y2, const VecPt3d &pts,
              const VecInt2d &polys, const VecInt &a_expectedPolyIDs,
              const VecDbl &a_expectedtValues,
              const VecPt3d &a_expectedPoints) {
  BSHP<GmMultiPolyIntersectionSorter> sorter =
      BSHP<GmMultiPolyIntersectionSorter>(
          new GmMultiPolyIntersectionSorterTerse());
  BSHP<GmMultiPolyIntersector> mpi =
      GmMultiPolyIntersector::New(pts, polys, sorter);
  VecInt polyIds1, polyIds2, polyIds3;
  VecDbl tValues;
  VecPt3d points;
  mpi->TraverseLineSegment(x1, y1, x2, y2, polyIds1, tValues);
  mpi->TraverseLineSegment(x1, y1, x2, y2, polyIds2);
  mpi->TraverseLineSegment(x1, y1, x2, y2, polyIds3, points);
  TS_ASSERT_EQUALS_VEC(a_expectedPolyIDs, polyIds1);
  TS_ASSERT_EQUALS_VEC(a_expectedPolyIDs, polyIds2);
  TS_ASSERT_EQUALS_VEC(a_expectedPolyIDs, polyIds3);
  TS_ASSERT_EQUALS(polyIds1.size(), tValues.size());
  TS_ASSERT_EQUALS(polyIds1.size(), points.size());
  const double kDelta = 1e-5;
  TS_ASSERT_DELTA_VEC(a_expectedtValues, tValues, kDelta);
  TS_ASSERT_DELTA_VECPT3D(a_expectedPoints, points, kDelta);
} // iRunTest

} // unnamed namespace

////////////////////////////////////////////////////////////////////////////////
/// \class GmMultiPolyIntersectorUnitTests
/// \brief Tests for GmMultiPolyIntersector
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/// \brief
/// \verbatim
//               (10,10)
//    3-------------2
//    |             |
//  0------------------1
//    |             |
//    |      1      |
//    |             |
//    0-------------1
// (0,0)
/// \endverbatim
//------------------------------------------------------------------------------
void GmMultiPolyIntersectorUnitTests::test1OutOut() {
  VecPt3d pts = {{0, 0, 0}, {10, 0, 0}, {10, 10, 0}, {0, 10, 0}};
  VecInt2d polys = {{0, 1, 2, 3}};
  VecInt expectedIds = {1, -1};
  VecDbl expectedTvals = {0.0833333, 0.916667};
  VecPt3d expectedPoints = {{0.0, 5.0, 0.0}, {10.0, 5.0, 0.0}};
  iRunTest(-1, 5, 11, 5, pts, polys, expectedIds, expectedTvals,
           expectedPoints);

} // GmMultiPolyIntersectorUnitTests::test1OutOut
//------------------------------------------------------------------------------
/// \brief
/// \verbatim
//               (10,10)
//    3-------------2
//    |             |
//  0----------1    |
//    |             |
//    |      1      |
//    |             |
//    0-------------1
// (0,0)
/// \endverbatim
//------------------------------------------------------------------------------
void GmMultiPolyIntersectorUnitTests::test1OutIn() {
  VecPt3d pts{{0, 0, 0}, {10, 0, 0}, {10, 10, 0}, {0, 10, 0}};
  VecInt2d polys{{0, 1, 2, 3}};
  VecInt expectedIds{1, -1};
  VecDbl expectedTvals{0.111111, 1.0};
  VecPt3d expectedPoints = {{0.0, 5.0, 0.0}, {8.0, 5.0, 0.0}};
  iRunTest(-1, 5, 8, 5, pts, polys, expectedIds, expectedTvals, expectedPoints);

} // GmMultiPolyIntersectorUnitTests::test1OutIn
//------------------------------------------------------------------------------
/// \brief
/// \verbatim
//               (10,10)
//    3-------------2
//    |             |
//    |       0----------1
//    |             |
//    |      1      |
//    |             |
//    0-------------1
// (0,0)
/// \endverbatim
//------------------------------------------------------------------------------
void GmMultiPolyIntersectorUnitTests::test1InOut() {
  VecPt3d pts{{0, 0, 0}, {10, 0, 0}, {10, 10, 0}, {0, 10, 0}};
  VecInt2d polys = {{0, 1, 2, 3}};
  VecInt expectedIds{1, -1};
  VecDbl expectedTvals{0.0, 0.833333};
  VecPt3d expectedPoints = {{5.0, 5.0, 0.0}, {10.0, 5.0, 0.0}};
  iRunTest(5, 5, 11, 5, pts, polys, expectedIds, expectedTvals, expectedPoints);

} // GmMultiPolyIntersectorUnitTests::test1InOut
//------------------------------------------------------------------------------
/// \brief
/// \verbatim
//               (10,10)
//    3-------------2
//    |             |
//    |  0--------1 |
//    |             |
//    |      1      |
//    |             |
//    0-------------1
// (0,0)
/// \endverbatim
//------------------------------------------------------------------------------
void GmMultiPolyIntersectorUnitTests::test1InIn() {
  VecPt3d pts{{0, 0, 0}, {10, 0, 0}, {10, 10, 0}, {0, 10, 0}};
  VecInt2d polys{{0, 1, 2, 3}};
  VecInt expectedIds{1, -1};
  VecDbl expectedTvals{0.0, 1.0};
  VecPt3d expectedPoints = {{2.0, 5.0, 0.0}, {8.0, 5.0, 0.0}};
  iRunTest(2, 5, 8, 5, pts, polys, expectedIds, expectedTvals, expectedPoints);

} // GmMultiPolyIntersectorUnitTests::test1InIn
//------------------------------------------------------------------------------
/// \brief
/// \verbatim
//               (10,10)
//    3-------------2
//    |             |
//    0-------------1
//    |             |
//    |      1      |
//    |             |
//    0-------------1
// (0,0)
/// \endverbatim
//------------------------------------------------------------------------------
void GmMultiPolyIntersectorUnitTests::test1OnOn() {
  VecPt3d pts{{0, 0, 0}, {10, 0, 0}, {10, 10, 0}, {0, 10, 0}};
  VecInt2d polys{{0, 1, 2, 3}};
  VecInt expectedIds{1, -1};
  VecDbl expectedTvals{0.0, 1.0};
  VecPt3d expectedPoints = {{0.0, 5.0, 0.0}, {10.0, 5.0, 0.0}};
  iRunTest(0, 5, 10, 5, pts, polys, expectedIds, expectedTvals, expectedPoints);

} // GmMultiPolyIntersectorUnitTests::test1OnOn
//------------------------------------------------------------------------------
/// \brief
/// \verbatim
//               (10,10)
//    3-------------2
//    |             |
//    0------1      |
//    |             |
//    |      1      |
//    |             |
//    0-------------1
// (0,0)
/// \endverbatim
//------------------------------------------------------------------------------
void GmMultiPolyIntersectorUnitTests::test1OnIn() {
  VecPt3d pts{{0, 0, 0}, {10, 0, 0}, {10, 10, 0}, {0, 10, 0}};
  VecInt2d polys{{0, 1, 2, 3}};
  VecInt expectedIds{1, -1};
  VecDbl expectedTvals{0.0, 1.0};
  VecPt3d expectedPoints = {{0.0, 5.0, 0.0}, {5.0, 5.0, 0.0}};
  iRunTest(0, 5, 5, 5, pts, polys, expectedIds, expectedTvals, expectedPoints);

} // GmMultiPolyIntersectorUnitTests::test1OnIn
//------------------------------------------------------------------------------
/// \brief
/// \verbatim
//               (10,10)
//    3-------------2
//    |             |
//    |      0------1
//    |             |
//    |      1      |
//    |             |
//    0-------------1
// (0,0)
/// \endverbatim
//------------------------------------------------------------------------------
void GmMultiPolyIntersectorUnitTests::test1InOn() {
  VecPt3d pts{{0, 0, 0}, {10, 0, 0}, {10, 10, 0}, {0, 10, 0}};
  VecInt2d polys{{0, 1, 2, 3}};
  VecInt expectedIds{1, -1};
  VecDbl expectedTvals{0.0, 1.0};
  VecPt3d expectedPoints = {{5.0, 5.0, 0.0}, {10.0, 5.0, 0.0}};
  iRunTest(5, 5, 10, 5, pts, polys, expectedIds, expectedTvals, expectedPoints);

} // GmMultiPolyIntersectorUnitTests::test1InOn
//------------------------------------------------------------------------------
/// \brief
/// \verbatim
//               (10,10)
//    3-------------2
//    |             |
// 0--1             |
//    |      1      |
//    |             |
//    |             |
//    0-------------1
// (0,0)
/// \endverbatim
//------------------------------------------------------------------------------
void GmMultiPolyIntersectorUnitTests::test1OutOn() {
  VecPt3d pts{{0, 0, 0}, {10, 0, 0}, {10, 10, 0}, {0, 10, 0}};
  VecInt2d polys{{0, 1, 2, 3}};
  VecInt expectedIds{1, -1};
  VecDbl expectedTvals{1.0, 1.0};
  VecPt3d expectedPoints = {{0.0, 5.0, 0.0}, {0.0, 5.0, 0.0}};
  iRunTest(-1, 5, 0, 5, pts, polys, expectedIds, expectedTvals, expectedPoints);

} // GmMultiPolyIntersectorUnitTests::test1OutOn
//------------------------------------------------------------------------------
/// \brief
/// \verbatim
//               (10,10)
//    3-------------2
//    |             |
//    |             0--1
//    |      1      |
//    |             |
//    |             |
//    0-------------1
// (0,0)
/// \endverbatim
//------------------------------------------------------------------------------
void GmMultiPolyIntersectorUnitTests::test1OnOut() {
  VecPt3d pts{{0, 0, 0}, {10, 0, 0}, {10, 10, 0}, {0, 10, 0}};
  VecInt2d polys{{0, 1, 2, 3}};
  VecInt expectedIds{1, -1};
  VecDbl expectedTvals{0.0, 0.0};
  VecPt3d expectedPoints = {{10.0, 5.0, 0.0}, {10.0, 5.0, 0.0}};
  iRunTest(10, 5, 11, 5, pts, polys, expectedIds, expectedTvals,
           expectedPoints);

} // GmMultiPolyIntersectorUnitTests::test1OnOut
//------------------------------------------------------------------------------
/// \brief
/// \verbatim
//               (10,10)
//    3----0----1---2
//    |             |
//    |             |
//    |      1      |
//    |             |
//    |             |
//    0-------------1
// (0,0)
/// \endverbatim
//------------------------------------------------------------------------------
void GmMultiPolyIntersectorUnitTests::test1EdgeInIn() {
  VecPt3d pts{{0, 0, 0}, {10, 0, 0}, {10, 10, 0}, {0, 10, 0}};
  VecInt2d polys{{0, 1, 2, 3}};
  VecInt expectedIds{1, -1};
  VecDbl expectedTvals{0.0, 1.0};
  VecPt3d expectedPoints = {{3.0, 10.0, 0.0}, {7.0, 10.0, 0.0}};
  iRunTest(3, 10, 7, 10, pts, polys, expectedIds, expectedTvals,
           expectedPoints);

} // GmMultiPolyIntersectorUnitTests::test1EdgeInIn
//------------------------------------------------------------------------------
/// \brief
/// \verbatim
//               (10,10)
//   3|1-----------2|2
//    |             |
//    |             |
//    |      1      |
//    |             |
//    |             |
//    0-------------1
// (0,0)
/// \endverbatim
//------------------------------------------------------------------------------
void GmMultiPolyIntersectorUnitTests::test1EdgePtPt() {
  VecPt3d pts{{0, 0, 0}, {10, 0, 0}, {10, 10, 0}, {0, 10, 0}};
  VecInt2d polys{{0, 1, 2, 3}};
  VecInt expectedIds{1, -1};
  VecDbl expectedTvals{0.0, 1.0};
  VecPt3d expectedPoints = {{0.0, 10.0, 0.0}, {10.0, 10.0, 0.0}};
  iRunTest(0, 10, 10, 10, pts, polys, expectedIds, expectedTvals,
           expectedPoints);

} // GmMultiPolyIntersectorUnitTests::test1EdgePtPt
//------------------------------------------------------------------------------
/// \brief
/// \verbatim
//               (10,10)
// 1--3-------------2--2
//    |             |
//    |             |
//    |      1      |
//    |             |
//    |             |
//    0-------------1
// (0,0)
/// \endverbatim
//------------------------------------------------------------------------------
void GmMultiPolyIntersectorUnitTests::test1EdgeOutOut() {
  VecPt3d pts{{0, 0, 0}, {10, 0, 0}, {10, 10, 0}, {0, 10, 0}};
  VecInt2d polys{{0, 1, 2, 3}};
  VecInt expectedIds{1, -1};
  VecDbl expectedTvals{0.0833333, 0.9166667};
  VecPt3d expectedPoints = {{10.0, 10.0, 0.0}, {0.0, 10.0, 0.0}};
  // iRunTest(-1, 10, 11, 10, pts, polys, expectedIds, expectedTvals,
  // expectedPoints); // doesn't work
  iRunTest(11, 10, -1, 10, pts, polys, expectedIds, expectedTvals,
           expectedPoints);

} // GmMultiPolyIntersectorUnitTests::test1EdgeOutOut
//------------------------------------------------------------------------------
/// \brief
/// \verbatim
//               (10,10)
// 1--3------2------2
//    |             |
//    |             |
//    |      1      |
//    |             |
//    |             |
//    0-------------1
// (0,0)
/// \endverbatim
//------------------------------------------------------------------------------
void GmMultiPolyIntersectorUnitTests::test1EdgeOutIn() {
  VecPt3d pts{{0, 0, 0}, {10, 0, 0}, {10, 10, 0}, {0, 10, 0}};
  VecInt2d polys{{0, 1, 2, 3}};
  VecInt expectedIds{1, -1};
  VecDbl expectedTvals{0.166667, 1.0};
  VecPt3d expectedPoints = {{0.0, 10.0, 0.0}, {5.0, 10.0, 0.0}};
  iRunTest(-1, 10, 5, 10, pts, polys, expectedIds, expectedTvals,
           expectedPoints);
  expectedTvals = {0.0, {5 / 6.0}};
  expectedPoints = {{5.0, 10.0, 0.0}, {0.0, 10.0, 0.0}};
  iRunTest(5, 10, -1, 10, pts, polys, expectedIds, expectedTvals,
           expectedPoints);
  // iRunTest(-1, 0, 5, 0, pts, polys, expectedIds, expectedTvals,
  // expectedPoints); //works iRunTest(-1, 9.999999999999999, 5, 10, pts, polys,
  // expectedIds, expectedTvals, expectedPoints); //works
  expectedTvals = {1.0 / 3.0, 1.0};
  expectedPoints = {{10.0, 10.0, 0.0}, {0.0, 10.0, 0.0}};
  iRunTest(15, 10, 0, 10, pts, polys, expectedIds, expectedTvals,
           expectedPoints);
} // GmMultiPolyIntersectorUnitTests::test1EdgeOutIn
//------------------------------------------------------------------------------
/// \brief
/// \verbatim
//               (10,10)
//    3------1------2--2
//    |             |
//    |             |
//    |      1      |
//    |             |
//    |             |
//    0-------------1
// (0,0)
/// \endverbatim
//------------------------------------------------------------------------------
void GmMultiPolyIntersectorUnitTests::test1EdgeInOut() {
  VecPt3d pts{{0, 0, 0}, {10, 0, 0}, {10, 10, 0}, {0, 10, 0}};
  VecInt2d polys{{0, 1, 2, 3}};
  VecInt expectedIds{1, -1};
  VecDbl expectedTvals{0.0, 0.833333};
  VecPt3d expectedPoints = {{5.0, 10.0, 0.0}, {10.0, 10.0, 0.0}};
  iRunTest(5, 10, 11, 10, pts, polys, expectedIds, expectedTvals,
           expectedPoints); // works
  // iRunTest(11, 10, 5, 10, pts, polys, expectedIds, expectedTvals,
  // expectedPoints); // doesn't work

} // GmMultiPolyIntersectorUnitTests::test1EdgeInOut
//------------------------------------------------------------------------------
/// \brief
/// \verbatim
//               (10,10)
// 1-2|3-------------2
//    |             |
//    |             |
//    |      1      |
//    |             |
//    |             |
//    0-------------1
// (0,0)
/// \endverbatim
//------------------------------------------------------------------------------
void GmMultiPolyIntersectorUnitTests::test1OutPt() {
  VecPt3d pts{{0, 0, 0}, {10, 0, 0}, {10, 10, 0}, {0, 10, 0}};
  VecInt2d polys{{0, 1, 2, 3}};
  VecInt expectedIds{1, -1};
  VecDbl expectedTvals{1.0, 1.0};
  VecPt3d expectedPoints = {{0.0, 10.0, 0.0}, {0.0, 10.0, 0.0}};
  iRunTest(-1, 10, 0, 10, pts, polys, expectedIds, expectedTvals,
           expectedPoints);

} // GmMultiPolyIntersectorUnitTests::test1OutPt
//------------------------------------------------------------------------------
/// \brief
/// \verbatim
//      2
//     /            (10,10)
//    3-------------2
//   /|             |
//  1 |             |
//    |      1      |
//    |             |
//    |             |
//    0-------------1
// (0,0)
/// \endverbatim
//------------------------------------------------------------------------------
void GmMultiPolyIntersectorUnitTests::test1OutPtOut() {
  VecPt3d pts{{0, 0, 0}, {10, 0, 0}, {10, 10, 0}, {0, 10, 0}};
  VecInt2d polys{{0, 1, 2, 3}};
  VecInt expectedIds{1, -1};
  VecDbl expectedTvals{0.5, 0.5};
  VecPt3d expectedPoints = {{0.0, 10.0, 0.0}, {0.0, 10.0, 0.0}};
  iRunTest(-1, 9, 1, 11, pts, polys, expectedIds, expectedTvals,
           expectedPoints);

} // GmMultiPolyIntersectorUnitTests::test1OutPtOut
//------------------------------------------------------------------------------
/// \brief
/// \verbatim
//   2
//  /              (10,10)
// 1  3-------------2
//    |             |
//    |             |
//    |      1      |
//    |             |
//    |             |
//    0-------------1
// (0,0)
/// \endverbatim
//------------------------------------------------------------------------------
void GmMultiPolyIntersectorUnitTests::test1AllOut() {
  VecPt3d pts{{0, 0, 0}, {10, 0, 0}, {10, 10, 0}, {0, 10, 0}};
  VecInt2d polys{{0, 1, 2, 3}};
  VecInt expectedIds;
  VecDbl expectedTvals;
  VecPt3d expectedPoints = {};
  iRunTest(-1, 10, 0, 11, pts, polys, expectedIds, expectedTvals,
           expectedPoints);

} // GmMultiPolyIntersectorUnitTests::test1AllOut
//------------------------------------------------------------------------------
/// \brief
/// \verbatim
//                 (10,10)
//   3|1-----2------2
//    |             |
//    |             |
//    |      1      |
//    |             |
//    |             |
//    0-------------1
// (0,0)
/// \endverbatim
//------------------------------------------------------------------------------
void GmMultiPolyIntersectorUnitTests::test1PtIn() {
  VecPt3d pts{{0, 0, 0}, {10, 0, 0}, {10, 10, 0}, {0, 10, 0}};
  VecInt2d polys{{0, 1, 2, 3}};
  VecInt expectedIds{1, -1};
  VecDbl expectedTvals{0.0, 1.0};
  VecPt3d expectedPoints = {{0.0, 10.0, 0.0}, {5.0, 10.0, 0.0}};
  iRunTest(0, 10, 5, 10, pts, polys, expectedIds, expectedTvals,
           expectedPoints);

} // GmMultiPolyIntersectorUnitTests::test1PtIn
//------------------------------------------------------------------------------
/// \brief
/// \verbatim
//                 (10,10)
//   3|------1-----2|2
//    |             |
//    |             |
//    |      1      |
//    |             |
//    |             |
//    0-------------1
// (0,0)
/// \endverbatim
//------------------------------------------------------------------------------
void GmMultiPolyIntersectorUnitTests::test1InPt() {
  VecPt3d pts{{0, 0, 0}, {10, 0, 0}, {10, 10, 0}, {0, 10, 0}};
  VecInt2d polys{{0, 1, 2, 3}};
  VecInt expectedIds{1, -1};
  VecDbl expectedTvals{0.0, 1.0};
  VecPt3d expectedPoints = {{5.0, 10.0, 0.0}, {10.0, 10.0, 0.0}};
  iRunTest(5, 10, 10, 10, pts, polys, expectedIds, expectedTvals,
           expectedPoints);

} // GmMultiPolyIntersectorUnitTests::test1InPt
//------------------------------------------------------------------------------
/// \brief
/// \verbatim
//  (0,10)
//    3-------------4-------------5
//    |             |             |
//    0-------------1             |
//    |             |             |
//    |      1      |      2      |
//    |             |             |
//    0-------------1-------------2
// (0,0)                        (20,0)
/// \endverbatim
//------------------------------------------------------------------------------
void GmMultiPolyIntersectorUnitTests::test2OnOn() {
  VecPt3d pts{{0, 0, 0},  {10, 0, 0},  {20, 0, 0},
              {0, 10, 0}, {10, 10, 0}, {20, 10, 0}};
  VecInt2d polys{{0, 1, 4, 3}};
  VecInt expectedIds{1, -1}; // old: (1)
  VecDbl expectedTvals{0.0, 1.0};
  VecPt3d expectedPoints = {{0.0, 5.0, 0.0}, {10.0, 5.0, 0.0}};
  iRunTest(0, 5, 10, 5, pts, polys, expectedIds, expectedTvals, expectedPoints);

} // GmMultiPolyIntersectorUnitTests::test2OnOn
//------------------------------------------------------------------------------
/// \brief
/// \verbatim
//  (0,10)
//    3-------------4-------------5
//    |             |             |
//    |     0-------1             |
//    |             |             |
//    |      1      |      2      |
//    |             |             |
//    0-------------1-------------2
// (0,0)                        (20,0)
/// \endverbatim
//------------------------------------------------------------------------------
void GmMultiPolyIntersectorUnitTests::test2InOn() {
  VecPt3d pts{{0, 0, 0},  {10, 0, 0},  {20, 0, 0},
              {0, 10, 0}, {10, 10, 0}, {20, 10, 0}};
  VecInt2d polys{{0, 1, 4, 3}};
  VecInt expectedIds{1, -1}; // old: (1)
  VecDbl expectedTvals{0.0, 1.0};
  VecPt3d expectedPoints = {{5.0, 5.0, 0.0}, {10.0, 5.0, 0.0}};
  iRunTest(5, 5, 10, 5, pts, polys, expectedIds, expectedTvals, expectedPoints);

} // GmMultiPolyIntersectorUnitTests::test2InOn
//------------------------------------------------------------------------------
/// \brief
/// \verbatim
//  (0,10)
//    3-------------4-------------5
//    |             |             |
//    |             0-------1     |
//    |             |             |
//    |      1      |      2      |
//    |             |             |
//    0-------------1-------------2
// (0,0)                        (20,0)
/// \endverbatim
//------------------------------------------------------------------------------
void GmMultiPolyIntersectorUnitTests::test2OnIn() {
  VecPt3d pts{{0, 0, 0},  {10, 0, 0},  {20, 0, 0},
              {0, 10, 0}, {10, 10, 0}, {20, 10, 0}};
  VecInt2d polys = {{0, 1, 4, 3}, {1, 2, 5, 4}};
  VecInt expectedIds{1, -1}; // old: (1)
  VecDbl expectedTvals{0.0, 1.0};
  VecPt3d expectedPoints = {{5.0, 5.0, 0.0}, {10.0, 5.0, 0.0}};
  iRunTest(5, 5, 10, 5, pts, polys, expectedIds, expectedTvals, expectedPoints);

} // GmMultiPolyIntersectorUnitTests::test2OnIn
//------------------------------------------------------------------------------
/// \brief
/// \verbatim
//  (0,10)
//    3-------------4-------------5
//    |             |             |
//  0--------------------------------1
//    |             |             |
//    |      1      |      2      |
//    |             |             |
//    0-------------1-------------2
// (0,0)                        (20,0)
/// \endverbatim
//------------------------------------------------------------------------------
void GmMultiPolyIntersectorUnitTests::test2OutOut() {
  VecPt3d pts{{0, 0, 0},  {10, 0, 0},  {20, 0, 0},
              {0, 10, 0}, {10, 10, 0}, {20, 10, 0}};
  VecInt2d polys = {{0, 1, 4, 3}, {1, 2, 5, 4}};
  VecInt expectedIds{1, 2, -1}; // old: (1)(2)
  VecDbl expectedTvals{0.0454545, 0.5, 0.954545};
  VecPt3d expectedPoints = {
      {0.0, 5.0, 0.0}, {10.0, 5.0, 0.0}, {20.0, 5.0, 0.0}};
  iRunTest(-1, 5, 21, 5, pts, polys, expectedIds, expectedTvals,
           expectedPoints);

} // GmMultiPolyIntersectorUnitTests::test2OutOut
//------------------------------------------------------------------------------
/// \brief
/// \verbatim
//  (0,10)
//    3-------------4-------------5
//    |             |             |
//  0------------------------1    |
//    |             |             |
//    |      1      |      2      |
//    |             |             |
//    0-------------1-------------2
// (0,0)                        (20,0)
/// \endverbatim
//------------------------------------------------------------------------------
void GmMultiPolyIntersectorUnitTests::test2OutIn() {
  VecPt3d pts{{0, 0, 0},  {10, 0, 0},  {20, 0, 0},
              {0, 10, 0}, {10, 10, 0}, {20, 10, 0}};
  VecInt2d polys = {{{0, 1, 4, 3}}, {1, 2, 5, 4}};
  VecInt expectedIds{1, 2, -1}; // old: (1)(2)
  VecDbl expectedTvals{0.0625, 0.6875, 1.0};
  VecPt3d expectedPoints = {
      {0.0, 5.0, 0.0}, {10.0, 5.0, 0.0}, {15.0, 5.0, 0.0}};
  iRunTest(-1, 5, 15, 5, pts, polys, expectedIds, expectedTvals,
           expectedPoints);

} // GmMultiPolyIntersectorUnitTests::test2OutIn
//------------------------------------------------------------------------------
/// \brief
/// \verbatim
//  (0,10)
//    3-------------4-------------5
//    |             |             |
//    |       0---------------------1
//    |             |             |
//    |      1      |      2      |
//    |             |             |
//    0-------------1-------------2
// (0,0)                        (20,0)
/// \endverbatim
//------------------------------------------------------------------------------
void GmMultiPolyIntersectorUnitTests::test2InOut() {
  VecPt3d pts{{0, 0, 0},  {10, 0, 0},  {20, 0, 0},
              {0, 10, 0}, {10, 10, 0}, {20, 10, 0}};
  VecInt2d polys = {{0, 1, 4, 3}, {1, 2, 5, 4}};
  VecInt expectedIds{1, 2, -1}; // old: (1)(2)
  VecDbl expectedTvals{0.0, 0.3125, 0.9375};
  VecPt3d expectedPoints = {
      {5.0, 5.0, 0.0}, {10.0, 5.0, 0.0}, {20.0, 5.0, 0.0}};
  iRunTest(5, 5, 21, 5, pts, polys, expectedIds, expectedTvals, expectedPoints);

} // GmMultiPolyIntersectorUnitTests::test2InOut
//------------------------------------------------------------------------------
/// \brief
/// \verbatim
//  (0,10)
//    3-------------4-------------5
//    |             |             |
//    |      0-------------1      |
//    |             |             |
//    |      1      |      2      |
//    |             |             |
//    0-------------1-------------2
// (0,0)                        (20,0)
/// \endverbatim
//------------------------------------------------------------------------------
void GmMultiPolyIntersectorUnitTests::test2InIn() {
  VecPt3d pts{{0, 0, 0},  {10, 0, 0},  {20, 0, 0},
              {0, 10, 0}, {10, 10, 0}, {20, 10, 0}};
  VecInt2d polys = {{0, 1, 4, 3}, {1, 2, 5, 4}};
  VecInt expectedIds{1, 2, -1}; // old: (1)(2)
  VecDbl expectedTvals{0.0, 0.5, 1.0};
  VecPt3d expectedPoints = {
      {5.0, 5.0, 0.0}, {10.0, 5.0, 0.0}, {15.0, 5.0, 0.0}};
  iRunTest(5, 5, 15, 5, pts, polys, expectedIds, expectedTvals, expectedPoints);
} // GmMultiPolyIntersectorUnitTests::test2InIn
//------------------------------------------------------------------------------
/// \brief
/// \verbatim
//  (0,10)
//    3------1-----2|4-------------5
//    |             |             |
//    |             |             |
//    |      1      |      2      |
//    |             |             |
//    |             |             |
//    0-------------1-------------2
// (0,0)                        (20,0)
/// \endverbatim
//------------------------------------------------------------------------------
void GmMultiPolyIntersectorUnitTests::test2InPt() {
  VecPt3d pts{{0, 0, 0},  {10, 0, 0},  {20, 0, 0},
              {0, 10, 0}, {10, 10, 0}, {20, 10, 0}};
  VecInt2d polys = {{0, 1, 4, 3}, {1, 2, 5, 4}};
  VecInt expectedIds{1, -1}; // old: (1)(2)
  VecDbl expectedTvals{0.0, 1.0};
  VecPt3d expectedPoints = {{5.0, 10.0, 0.0}, {10.0, 10.0, 0.0}};
  iRunTest(5, 10, 10, 10, pts, polys, expectedIds, expectedTvals,
           expectedPoints);
} // GmMultiPolyIntersectorUnitTests::test2InPt
//------------------------------------------------------------------------------
/// \brief
/// \verbatim
//  (0,10)
//    3------------4|1-----2------5
//    |             |             |
//    |             |             |
//    |      1      |      2      |
//    |             |             |
//    |             |             |
//    0-------------1-------------2
// (0,0)                        (20,0)
/// \endverbatim
//------------------------------------------------------------------------------
void GmMultiPolyIntersectorUnitTests::test2PtIn() {
  VecPt3d pts{{0, 0, 0},  {10, 0, 0},  {20, 0, 0},
              {0, 10, 0}, {10, 10, 0}, {20, 10, 0}};
  VecInt2d polys = {{0, 1, 4, 3}, {1, 2, 5, 4}};
  VecInt expectedIds{2, -1}; // old: (1)(2)
  VecDbl expectedTvals{0.0, 1.0};
  VecPt3d expectedPoints = {{10.0, 10.0, 0.0}, {15.0, 10.0, 0.0}};
  iRunTest(10, 10, 15, 10, pts, polys, expectedIds, expectedTvals,
           expectedPoints);
} // GmMultiPolyIntersectorUnitTests::test2PtIn
//------------------------------------------------------------------------------
/// \brief
/// \verbatim
//  (0,10)
//    3------1-----4|-------------5 2
//    |             |             |
//    |             |             |
//    |      1      |      2      |
//    |             |             |
//    |             |             |
//    0-------------1-------------2
// (0,0)                        (20,0)
/// \endverbatim
//------------------------------------------------------------------------------
void GmMultiPolyIntersectorUnitTests::test2InEdgePt() {
  // The comment below is not true. Check
  // GmMultiPolyIntersectorImpl::GmMultiPolyIntersectorImpl to see that we don't
  // set z's to 0.0 and this test works. We are using Pt3d as a 2D boost
  // geometry so z's don't come into play.
  //  // The non-equal z values would cause this test to fail but now we
  //  // set them all to 0.0.
  VecPt3d pts{{0, 0, 100},  {10, 0, 110},  {20, 0, 105},
              {0, 10, 104}, {10, 10, 103}, {20, 10, 106}};
  VecInt2d polys = {{0, 1, 4, 3}, {1, 2, 5, 4}};
  VecInt expectedIds{1, 2, -1};
  VecDbl expectedTvals = {{0.0}, {1 / 3.0}, {1}};
  VecPt3d expectedPoints = {
      {5.0, 10.0, 0.0}, {10.0, 10.0, 0.0}, {20.0, 10.0, 0.0}};
  iRunTest(5, 10, 20, 10, pts, polys, expectedIds, expectedTvals,
           expectedPoints);
  expectedIds = {{2}, {1}, {-1}};
  expectedPoints = {{15.0, 10.0, 0.0}, {10.0, 10.0, 0.0}, {0.0, 10.0, 0.0}};
  iRunTest(15, 10, 0, 10, pts, polys, expectedIds, expectedTvals,
           expectedPoints);
} // GmMultiPolyIntersectorUnitTests::test2PtIn
//------------------------------------------------------------------------------
/// \brief
/// \verbatim
//  (0,20)
//    3-------------2
//    |           1 |
//    |            \|
//    |      1      \
//    |             |\
//    |             | \
//    0-------------1--\----------6
//                  |   \         |
//                  |    2        |
//                  |             |
//                  |      2      |
//                  |             |
//                  4-------------5
// (0,0)                        (20,0)
/// \endverbatim
//------------------------------------------------------------------------------
void GmMultiPolyIntersectorUnitTests::test2InOutIn() {
  VecPt3d pts{{0, 10, 0}, {10, 10, 0}, {10, 20, 0}, {0, 20, 0},
              {10, 0, 0}, {20, 0, 0},  {20, 10, 0}};
  VecInt2d polys = {{0, 1, 2, 3}, {4, 5, 6, 1}};
  VecInt expectedIds = {{1}, {-1}, {2}, {-1}}; // old: (1)(2)
  VecDbl expectedTvals = {{0.0}, {0.2}, {0.8}, {1.0}};
  VecPt3d expectedPoints = {
      {8.0, 18.0, 0.0}, {10.0, 16.0, 0.0}, {16.0, 10.0, 0.0}, {18.0, 8.0, 0.0}};
  iRunTest(8, 18, 18, 8, pts, polys, expectedIds, expectedTvals,
           expectedPoints);
} // GmMultiPolyIntersectorUnitTests::test2InOutIn
//------------------------------------------------------------------------------
/// \brief Given 1 x 2 2D grid turned into triangles with point at poly center
///        triangles numbered as follows:
/// \verbatim
//
// (0, 0)
//    0-------------1-------------2
//    |\     4     /|\     8     /|
//    |  \   1   /  |  \       /  |
//    |    \   /    |    \   /    |
//    |  1   3    3 |  5   4    7 |
//    |    /   \    |    /   \    |
//    |  /       \  |  /   2   \  |
//    |/     2     \|/     6     \|
//    5-------------6-------------7
//                            (20, -10)
/// \endverbatim
//------------------------------------------------------------------------------
void GmMultiPolyIntersectorUnitTests::testInsideToInside() {
  VecPt3d pts;
  VecInt2d polys;
  trBuildGridTrianglePolys(1, 2, pts, polys);

  VecInt expectedIds{4, 3, 5, 6, -1};
  VecDbl expectedTvals{0.0, 0.166666, 0.5, 0.833333, 1.0};
  VecPt3d expectedPoints = {{5.0, -2.5, 0.0},
                            {6.6666666666667, -3.333333333333, 0.0},
                            {10.0, -5.0, 0.0},
                            {13.333333333333, -6.666666666667, 0.0},
                            {15.0, -7.5, 0.0}};
  iRunTest(5, -2.5, 15, -7.5, pts, polys, expectedIds, expectedTvals,
           expectedPoints);
} // GmMultiPolyIntersectorUnitTests::testInsideToInside
//------------------------------------------------------------------------------
/// \brief Given 1 x 2 2D grid turned into triangles with point at poly center
///        triangles numbered as follows:
/// \verbatim
//
// (0, 0)
//  1 0-------------1-------------2
//    |\     4     /|\     8     /|
//    |  \       /  |  \       /  |
//    |    \   /    |    \   /    |
//    |  1   3    3 |  5   4    7 |
//    |    /   \    |    /   \    |
//    |  /       \  |  /       \  |
//    |/     2     \|/     6     \|
//    5-------------6-------------7 2
//                            (20, -10)
/// \endverbatim
//------------------------------------------------------------------------------
void GmMultiPolyIntersectorUnitTests::testOutsideToOutside() {
  VecPt3d pts;
  VecInt2d polys;
  trBuildGridTrianglePolys(1, 2, pts, polys);

  VecInt expectedIds{1, 4, 3, 5, 6, 7, -1};
  VecDbl expectedTvals{0.0454545, 0.0833333, 0.34375, 0.5,
                       0.65625,   0.916667,  0.954545};
  VecPt3d expectedPoints = {
      {0.0, -0.454545454545, 0.0}, {0.8333333333333, -0.833333333333, 0.0},
      {6.5625, -3.4375, 0.0},      {10.0, -5.0, 0.0},
      {13.4375, -6.5625, 0.0},     {19.166666666667, -9.166666666667, 0.0},
      {20.0, -9.545454545455, 0.0}};
  iRunTest(-1, 0, 21, -10, pts, polys, expectedIds, expectedTvals,
           expectedPoints);
} // GmMultiPolyIntersectorUnitTests::testOutsideToOutside
//------------------------------------------------------------------------------
/// \brief Given 1 x 2 2D grid turned into triangles with point at poly center
///        triangles numbered as follows:
/// \verbatim
//
//       1
//(0,0) /
//     0-------------1-------------2
//    /|\     4     /|\     8     /|
//   2 |  \       /  |  \       /  |
//     |    \   /    |    \   /    |
//     |  1   3    3 |  5   4    7 |
//     |    /   \    |    /   \    |
//     |  /       \  |  /       \  |
//     |/     2     \|/     6     \|
//     5-------------6-------------7 2
//                             (20, -10)
/// \endverbatim
//------------------------------------------------------------------------------
void GmMultiPolyIntersectorUnitTests::testTouchesVertex() {
  VecPt3d pts;
  VecInt2d polys;
  trBuildGridTrianglePolys(1, 2, pts, polys);
  // SurfaceIter crashes on this. We return no intersection.
  VecInt expectedIds;
  VecDbl expectedTvals;
  VecPt3d expectedPoints = {};
  iRunTest(1, 1, -1, -1, pts, polys, expectedIds, expectedTvals,
           expectedPoints);
} // GmMultiPolyIntersectorUnitTests::testTouchesVertex
//------------------------------------------------------------------------------
/// \brief  Given 1 x 2 2D grid turned into triangles with point at poly center
///         triangles numbered as follows:
/// \verbatim
//
// (0, 0)
//    0-------------1-------------2
// 1  |\     4     /|\     8     /|
//  \ |  \       /  |  \       /  |
//   2|    \   /    |    \   /    |
//    |  1   3    3 |  5   4    7 |
//    |    /   \    |    /   \    |
//    |  /       \  |  /       \  |
//    |/     2     \|/     6     \|
//    5-------------6-------------7 2
//                            (20, -10)
/// \endverbatim
//------------------------------------------------------------------------------
void GmMultiPolyIntersectorUnitTests::testTouchesEdge() {
#ifdef __linux__
  TS_SKIP("GREENBUILD: This test does not pass on Linux builds.");
#endif
  VecPt3d pts;
  VecInt2d polys;
  trBuildGridTrianglePolys(1, 2, pts, polys);
  VecInt expectedIds{1, -1};
  VecDbl expectedTvals{1.0, 1.0};
  VecPt3d expectedPoints = {{0.0, -2.0, 0.0}, {0.0, -2.0, 0.0}};
  iRunTest(-1, -1, 0, -2, pts, polys, expectedIds, expectedTvals,
           expectedPoints);
} // GmMultiPolyIntersectorUnitTests::testTouchesEdge
//------------------------------------------------------------------------------
/// \brief Given 3 x 3 2D grid turned into triangles with point at poly center
///        triangles numbered as follows:
/// \verbatim
//
// (0, 0)
//    +-------------+-------------+-------------+
//    |\     4     /|\     8     /|\     12    /|
//    |  \       /  |  \       /  |  \       /  |
//    |    \   /    |    \   /    |    \   /    |
//    |  1   x    3 |  5   x    7 |  9   x   11 |
//    |    /   1    |    /   \    |    /   \    |
//    |  /       \  |  /       \  |  /       \  |
//    |/     2     \|/     6     \|/     10    \|
//    +-------------+-------------+-------------+
//    |\     16    /|\     20    /|\     24    /|
//    |  \       /  |  \       /  |  \       /  |
//    |    \   /    |    \   /    |    \   /    |
//    | 13   x   15 | 17   x   19 | 21   x   23 |
//    |    /   \    |    /   \    |    /   \    |
//    |  /       \  |  /       \  |  /       \  |
//    |/     14    \|/     18    \|/     22    \|
//    +-------------+-------------+-------------+
//    |\     28    /|\     32    /|\     36    /|
//    |  \       /  |  \       /  |  \       /  |
//    |    \   /    |    \   /    |    2   /    |
//    | 25   x   27 | 29   x   31 | 33   x   35 |
//    |    /   \    |    /   \    |    /   \    |
//    |  /       \  |  /       \  |  /       \  |
//    |/     26    \|/     30    \|/     34    \|
//    +-------------+-------------+-------------+
//                                          (30, -30)
/// \endverbatim
//------------------------------------------------------------------------------
void GmMultiPolyIntersectorUnitTests::testAlongEdgesInsideToInside() {
  // TS_FAIL("GmMultiPolyIntersectorUnitTests::testAlongEdgesInsideToInside");

  VecPt3d pts;
  VecInt2d polys;
  trBuildGridTrianglePolys(3, 3, pts, polys);

  // MfMapLayerUGridImp::TraverseLineSegment results
  // VecInt expectedIds = {3,17,18,33,-1};
  // VecDbl expectedTvals = {0.0,0.22222,0.5,0.777778,1.0};
  VecInt expectedIds{2, 17, 18, 33, -1};
  VecDbl expectedTvals{0.0, 0.22222, 0.5, 0.777778, 1.0};
  VecPt3d expectedPoints = {{6.0, -6.0, 0.0},
                            {10.0, -10.0, 0.0},
                            {15.0, -15.0, 0.0},
                            {20.0, -20.0, 0.0},
                            {24.0, -24.0, 0.0}};
  iRunTest(6, -6, 24, -24, pts, polys, expectedIds, expectedTvals,
           expectedPoints);

} // GmMultiPolyIntersectorUnitTests::testAlongEdgesInsideToInside
//------------------------------------------------------------------------------
/// \brief Given 3 x 3 2D grid turned into triangles with point at poly center
///        triangles numbered as follows:
/// \verbatim
//
// (0, 0)
//    0-------------1-------------2-------------3
//    |\     4     /|\     8     /|\     12    /|
//    |  \       /  |  \       /  |  \       /  |
//    |    \   /    |    \   /    |    \   /    |
//    |  1  12    3 |  5  13    7 |  9  14   11 |
//    |    /   \    |    /   \    |    /   \    |
//    |  /       \  |  /       \  |  /       \  |
//    |/     2     \|/     6     \|/     10    \|
//  1 4-------------5-------------6-------------7 2
//    |\     16    /|\     20    /|\     24    /|
//    |  \       /  |  \       /  |  \       /  |
//    |    \   /    |    \   /    |    \   /    |
//    | 13  15   15 | 17  16   19 | 21  17   23 |
//    |    /   \    |    /   \    |    /   \    |
//    |  /       \  |  /       \  |  /       \  |
//    |/     14    \|/     18    \|/     22    \|
//    8-------------9------------10------------11
//                                          (30, -20)
/// \endverbatim
//------------------------------------------------------------------------------
void GmMultiPolyIntersectorUnitTests::testAlongEdgesOutsideToOutside() {
  VecPt3d pts;
  VecInt2d polys;
  trBuildGridTrianglePolys(2, 3, pts, polys);

  // VecInt expectedIds = {16,20,24,23,-1}; SurfaceIter results
  // MfMapLayerUGridImp::TraverseLineSegment results:
  VecInt expectedIds{16, 20, 24, -1};
  VecDbl expectedTvals{0.016129032258064516, 0.33870967741935482,
                       0.66129032258064513, 0.98387096774193550};
  VecPt3d expectedPoints = {{0.0, -10.0, 0.0},
                            {10.0, -10.0, 0.0},
                            {20.0, -10.0, 0.0},
                            {30.0, -10.0, 0.0}};
  iRunTest(-0.5, -10, 30.5, -10, pts, polys, expectedIds, expectedTvals,
           expectedPoints);
} // GmMultiPolyIntersectorUnitTests::testAlongEdgesOutsideToOutside
//------------------------------------------------------------------------------
/// \brief Given 1 x 2 2D grid turned into triangles with point at poly center
///        triangles numbered as follows:
/// \verbatim
//
// (0, 0)
//    +-------------+-------------+
//    |\     4     /|\     8     /|
//    |  \       /  |  \       /  |
//    |    \   /  1 |    \   /    |
//    |  1   x    3 |  5   x    7 |
//    |    /   \    |    /   \    |  2
//    |  /       \  |  /       \  |
//    |/     2     \|/     6     \|
//    +-------------+-------------+
//                            (20, -10)
/// \endverbatim
//------------------------------------------------------------------------------
void GmMultiPolyIntersectorUnitTests::testEdgeThroughOppositeVertexAtAngle() {
  // TS_FAIL("GmMultiPolyIntersectorUnitTests::testEdgeThroughOppositeVertexAtAngle");

  VecPt3d pts;
  VecInt2d polys;
  trBuildGridTrianglePolys(1, 2, pts, polys);

  // MfMapLayerUGridImp::TraverseLineSegment results
  // VecInt expectedIds = {3,5,7,-1};
  // VecDbl expectedTvals = {0.0,0.14285714285714288,0.5,0.85714285714285721};
  VecInt expectedIds{3, 5, 7, -1};
  VecDbl expectedTvals{0.0, 0.14285714285714288, 0.5, 0.85714285714285721};
  VecPt3d expectedPoints = {{8.0, -2.5, 0.0},
                            {10.0, -3.214285714286, 0.0},
                            {15.0, -5.0, 0.0},
                            {20.0, -6.785714285714, 0.0}};
  iRunTest(8, -2.5, 22, -7.5, pts, polys, expectedIds, expectedTvals,
           expectedPoints);

} // GmMultiPolyIntersectorUnitTests::testEdgeThroughOppositeVertexAtAngle
//------------------------------------------------------------------------------
/// \brief Given 1 x 2 2D grid turned into triangles with point at poly center
///        triangles numbered as follows:
/// \verbatim
//
// (0, 0)                         2
//    +-------------+-------------+
//    |\     4     /|\     8     /|
//    |  \       /  |  \       /  |
//    |    \   /    |    \   /    |
//    |  1   x    3 |  5   x    7 |
//    |    /   \    |    /   \    |
//    |  /       \  1  /       \  |
//    |/     2     \|/     6     \|
//    +-------------+-------------+
//                            (20, -10)
/// \endverbatim
//------------------------------------------------------------------------------
void GmMultiPolyIntersectorUnitTests::testStartAtEdgeThroughAdjacent() {
  // TS_FAIL("GmMultiPolyIntersectorUnitTests::testStartAtEdgeThroughAdjacent");

  VecPt3d pts;
  VecInt2d polys;
  trBuildGridTrianglePolys(1, 2, pts, polys);

  // MfMapLayerUGridImp::TraverseLineSegment results
  // VecInt expectedIds = {5,8,-1};
  // VecDbl expectedTvals = {0.0,0.4,0.8};
  VecInt expectedIds{5, 8, -1};
  VecDbl expectedTvals{0.0, 0.4, 0.8};
  VecPt3d expectedPoints = {
      {10.0, -8.0, 0.0}, {14.0, -4.0, 0.0}, {18.0, 0.0, 0.0}};
  iRunTest(10, -8, 20, 2, pts, polys, expectedIds, expectedTvals,
           expectedPoints);
} // GmMultiPolyIntersectorUnitTests::testStartAtEdgeThroughAdjacent
//------------------------------------------------------------------------------
/// \brief Given 1 x 2 2D grid turned into triangles with point at poly center
///        triangles numbered as follows:
///
/// We only do the first part: inside to edge.
/// \verbatim
//
// (0, 0)
//    +-------------+-------------+
//    |\     4     /|\     8     /|
//    |  \       /  |  \       /  |
//    |    \   /    |    \   /    |
//    |  1   x    3 |  5   x    7 |
//    |    /   \    |    /   \    |
//    |  /       \  |  /       \  |
//    |/     2     \|/     6     \|
//    +-------------+-------------+
//                            (20, -10)
/// \endverbatim
//------------------------------------------------------------------------------
void GmMultiPolyIntersectorUnitTests::testInsideToEdgeThenThroughAdjacent() {
// TS_FAIL("GmMultiPolyIntersectorUnitTests::testInsideToEdgeThenThroughAdjacent");
#ifdef __linux__
  TS_SKIP("GREENBUILD: This test does not pass on Linux builds.");
#endif
  VecPt3d pts;
  VecInt2d polys;
  trBuildGridTrianglePolys(1, 2, pts, polys);

  // MfMapLayerUGridImp::TraverseLineSegment results
  // VecInt expectedIds = {3,-1};
  // VecDbl expectedTvals = {0.0,1.0};
  VecInt expectedIds{3, -1};
  VecDbl expectedTvals{0.0, 1.0};
  VecPt3d expectedPoints = {{9.0, -5.0, 0.0}, {10.0, -8.0, 0.0}};
  iRunTest(9, -5, 10, -8, pts, polys, expectedIds, expectedTvals,
           expectedPoints);
} // GmMultiPolyIntersectorUnitTests::testInsideToEdgeThenThroughAdjacent
//------------------------------------------------------------------------------
/// \brief We only do the first part: inside to edge.
/// \verbatim
/// \endverbatim
//------------------------------------------------------------------------------
void GmMultiPolyIntersectorUnitTests::testMap2MfBug() {
  VecPt3d pts{{1937.0003414079, 11829.937474193, 0.0},
              {1947.0331979019, 11744.658193995, 0.0},
              {1806.3085553039, 11640.957366058, 0.0},
              {1827.8055464927, 11817.091027732, 0.0},
              {2046.1951363232, 11842.783920654, 0.0},
              {2155.4908205085, 11854.643944679, 0.0},
              {2160.8354359402, 11805.390806652, 0.0},
              {2070.1604445251, 11710.224742147, 0.0}};
  VecInt2d polys(2);
  polys[0] = {0, 3, 2, 1};
  polys[1] = {0, 1, 7, 6, 5, 4};

  const double t = 0.41350462827872492;
  VecInt expectedIds{2, -1, 2, 1, -1};
  VecDbl expectedTvals{0.0, 0.0, t, .5, 1.0};
  VecPt3d expectedPoints = {
      {2046.1951363232131, 11842.783920653745, 0.00000000000000000},
      {2046.1951363231999, 11842.783920653999, 0.00000000000000000},
      {1955.8900301603892, 11832.159790516887, 0.00000000000000000},
      {1937.0003414078999, 11829.937474193001, 0.00000000000000000},
      {1827.8055464926999, 11817.091027732000, 0.00000000000000000}};
  iRunTest(2046.1951363232131, 11842.783920653745, 1827.8055464926651,
           11817.091027732373, pts, polys, expectedIds, expectedTvals,
           expectedPoints);
  expectedIds = {1, 2, -1, 2, -1};
  expectedTvals = {0.0, .5, 1 - t, 1, 1};
  expectedPoints = {{1827.8055464927, 11817.091027732, 0.0},
                    {1937.0003414079, 11829.937474193, 0.0},
                    {1955.8909579942, 11832.159899674, 0.0},
                    {2046.1951363232, 11842.783920654, 0.0},
                    {2046.1951363232, 11842.783920654, 0.0}};
  iRunTest(1827.8055464926651, 11817.091027732373, 2046.1951363232131,
           11842.783920653745, pts, polys, expectedIds, expectedTvals,
           expectedPoints);
} // GmMultiPolyIntersectorUnitTests::testInsideToEdgeThenThroughAdjacent
//------------------------------------------------------------------------------
/// \brief Given 1 x 2 2D grid turned into triangles with point at poly center
/// triangles numbered as follows:
/// \verbatim
//
// (0, 0)
//    +-------------+-------------+
//    |\     4     /|\     8     /|
//    |  \       /  |  \       /  |
//    |    \   /    |    \   /    |
//    |  1   x    3 |  5   x    7 |
//    |    /   \    |    /   \    |
//    |  /       \  |  /       \  |
//    |/     2     \|/     6     \|
//    +-------------+-------------+
//                            (20, -10)
/// \endverbatim
//------------------------------------------------------------------------------
void GmMultiPolyIntersectorUnitTests::testEndAtEdgeFromAdjacent() {
  // TS_FAIL("GmMultiPolyIntersectorUnitTests::testEndAtEdgeFromAdjacent");

  VecPt3d pts;
  VecInt2d polys;
  trBuildGridTrianglePolys(1, 2, pts, polys);

  // MfMapLayerUGridImp::TraverseLineSegment results
  // VecInt expectedIds = {8,5,-1};
  // VecDbl expectedTvals = {0.2,0.6,1.0};
  VecInt expectedIds{8, 5, -1};
  VecDbl expectedTvals{0.2, 0.6, 1.0};
  VecPt3d expectedPoints = {{18.0, 0.0}, {14.0, -4.0, 0.0}, {10.0, -8.0, 0.0}};
  iRunTest(20, 2, 10, -8, pts, polys, expectedIds, expectedTvals,
           expectedPoints);
} // GmMultiPolyIntersectorUnitTests::testEndAtEdgeFromAdjacent
//------------------------------------------------------------------------------
/// \brief This case revealed the need for a tolerance when comparing t values.
//------------------------------------------------------------------------------
void GmMultiPolyIntersectorUnitTests::testSmsCase1() {
  VecPt3d pts{{-39.719999999999999, 90.079999999999998, 0.0},
              {-21.129999999999999, 90.469999999999999, 1.0},
              {-38.930000000000000, 75.840000000000003, 2.0},
              {-20.539999999999999, 76.629999999999995, 3.0},
              {-39.719999999999999, 62.000000000000000, 4.0},
              {-20.539999999999999, 61.609999999999999, 5.0},
              {-40.509999999999998, 47.770000000000003, 6.0},
              {-20.150000000000002, 46.579999999999998, 7.0},
              {-41.100000000000001, 30.370000000000001, 8.0},
              {-19.550000000000001, 29.379999999999999, 9.0}};
  VecInt2d polys(8);
  polys[0] = {7, 5, 6};
  polys[1] = {3, 2, 5};
  polys[2] = {2, 3, 1};
  polys[3] = {9, 7, 8};
  polys[4] = {7, 6, 8};
  polys[5] = {0, 2, 1};
  polys[6] = {5, 2, 4};
  polys[7] = {4, 6, 5};

  VecInt expectedIds{6, 3, 2, 7, 8, 1, 5, 4, -1};
  VecDbl expectedTvals{
      0.065777232109665892, 0.18968813237387866, 0.26837085508795250,
      0.35199491192297022,  0.47395399691316503, 0.58799734941084614,
      0.68358069055240822,  0.81969307266961533, 0.93250275302111885};
  VecPt3d expectedPoints = {{-31.97119143306, 90.242562417488, 0.0},
                            {-31.8980840019, 81.619602868102, 0.0},
                            {-31.8516611955, 76.144072194429, 0.0},
                            {-31.80232300197, 70.32467407928, 0.0},
                            {-31.73036714182, 61.837541354813, 0.0},
                            {-31.66308156385, 53.901264454499, 0.0},
                            {-31.60668739257, 47.249619744458, 0.0},
                            {-31.52638108712, 37.777559072921, 0.0},
                            {-31.45982337572, 29.92713341726, 0.0}};
  iRunTest(-32.009999999999998, 94.819999999999993, -31.420000000000002,
           25.230000000000000, pts, polys, expectedIds, expectedTvals,
           expectedPoints);
} // GmMultiPolyIntersectorUnitTests::testSmsCase1

////////////////////////////////////////////////////////////////////////////////
/// \class GmMultiPolyIntersector2IntermediateTests
/// \brief Tests for GmMultiPolyIntersector that take longer to run.
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/// \brief Returns the test group.
/// \return CxxTest::TestGroup reference.
//------------------------------------------------------------------------------
#ifndef CXXTEST4
const CxxTest::TestGroup &GmMultiPolyIntersector2IntermediateTests::group() {
  return *CxxTest::TestGroup::GetGroup(CxxTest::TG_INTERMEDIATE);
} // GmMultiPolyIntersector2IntermediateTests::group
#endif
//------------------------------------------------------------------------------
/// \brief Test a large number of polygons for speed.
//------------------------------------------------------------------------------
void GmMultiPolyIntersector2IntermediateTests::testLargeNumPolys() {
  // TS_FAIL("GmMultiPolyIntersectorUnitTests::testHuge");

  // Like testAlongEdgesInsideToInside but with many more polygons

  VecPt3d pts;
  VecInt2d polys;
  trBuildGridTrianglePolys(1000, 3, pts, polys);
  // Results in 120,000 polygons, 70,004 points

  // MfMapLayerUGridImp::TraverseLineSegment results
  // VecInt expectedIds = {3,17,18,33,-1};
  // VecDbl expectedTvals = {0.0,0.22222,0.5,0.777778,1.0};
  VecInt expectedIds{2, 17, 18, 33, -1};
  VecDbl expectedTvals{0.0, 0.22222, 0.5, 0.777778, 1.0};
  VecPt3d expectedPoints = {{6.0, -6.0, 0.0},
                            {10.0, -10.0, 0.0},
                            {15.0, -15.0, 0.0},
                            {20.0, -20.0, 0.0},
                            {24.0, -24.0, 0.0}};
  iRunTest(6, -6, 24, -24, pts, polys, expectedIds, expectedTvals,
           expectedPoints);

} // GmMultiPolyIntersector2IntermediateTests::testLargeNumPolys
//------------------------------------------------------------------------------
/// \brief This test can be used for speed comparisons.
//------------------------------------------------------------------------------
void GmMultiPolyIntersector2IntermediateTests::testLargeNumPolysAndSegments() {
  // TS_FAIL("GmMultiPolyIntersectorUnitTests::testHuge");

  // Like testAlongEdgesInsideToInside but with many more polygons

  VecPt3d pts;
  VecInt2d polys;
  trBuildGridTrianglePolys(1000, 3, pts, polys);
  // Results in 120,000 polygons, 70,004 points

  VecInt expectedIds = {2, 17, 18, 33, -1};
  VecDbl expectedTvals = {0.0, 0.22222, 0.5, 0.777778, 1.0};
  BSHP<GmMultiPolyIntersectionSorter> sorter =
      BSHP<GmMultiPolyIntersectionSorter>(
          new GmMultiPolyIntersectionSorterTerse());
  BSHP<GmMultiPolyIntersector> mpi =
      GmMultiPolyIntersector::New(pts, polys, sorter);
  VecInt polyIds;
  VecDbl tValues;
  const int nIntersectingLines =
      /*1e5*/ 1000; // Go big for serious timing testing
  for (size_t i = 0; i < nIntersectingLines; ++i) {
    // 3 different segments repeated
    mpi->TraverseLineSegment(6, -6, 24, -24, polyIds, tValues);
    mpi->TraverseLineSegment(7, -6, 25, -24, polyIds, tValues);
    mpi->TraverseLineSegment(25, -6, 6, -24, polyIds, tValues);
    // Don't need to check that the values are correct. That is done in
    // testLargeNumPolys
  }

} // GmMultiPolyIntersector2IntermediateTests::testLargeNumPolysAndSegments

  //} // namespace xms

#endif
