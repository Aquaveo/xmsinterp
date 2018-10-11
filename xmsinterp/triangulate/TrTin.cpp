//------------------------------------------------------------------------------
/// \file
/// \ingroup triangulate
/// \copyright (C) Copyright Aquaveo 2018. Distributed under FreeBSD License
/// (See accompanying file LICENSE or https://aqaveo.com/bsd/license.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------

// 1. Precompiled header

// 2. My own header
#include <xmsinterp/triangulate/TrTin.h>

// 3. Standard library headers
#include <cfloat>
#include <numeric>
#include <ostream>
#include <sstream>

// 4. External library headers
#include <boost/bind.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/unordered_set.hpp>

// 5. Shared code headers
#include <xmscore/misc/StringUtil.h>
#include <xmscore/stl/set.h>
#include <xmscore/stl/vector.h>
#include <xmsinterp/geometry/geoms.h>
#include <xmscore/misc/boost_defines.h>
#include <xmsinterp/triangulate/triangles.h>
#include <xmscore/misc/XmError.h>
#include <xmscore/misc/xmstype.h>
#include <xmscore/misc/XmConst.h>

// 6. Non-shared code headers

//----- Namespace declaration --------------------------------------------------

namespace xms
{
//----- Forward declarations ---------------------------------------------------

//----- External globals -------------------------------------------------------

//----- Constants / Enumerations -----------------------------------------------

//----- Classes / Structs ------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
class TrTinImpl : public TrTin
{
  /// \cond
  friend class boost::serialization::access;
  /// \endcond
public:
  TrTinImpl();
  virtual ~TrTinImpl();

  // Setters
  virtual void SetPoints(BSHP<VecPt3d> a_pts) override;
  virtual void SetTriangles(BSHP<VecInt> a_tris) override;
  virtual void SetTrianglesAdjacentToPoints(BSHP<VecInt2d> a_trisAdjToPts) override;
  virtual void SetGeometry(BSHP<VecPt3d> a_pts,
                           BSHP<VecInt> a_tris,
                           BSHP<VecInt2d> a_trisAdjToPts) override;

  // Getters
  virtual VecPt3d& Points() override;
  virtual VecInt& Triangles() override;
  virtual VecInt2d& TrisAdjToPts() override;

  virtual const VecPt3d& Points() const override;
  virtual const VecInt& Triangles() const override;
  virtual const VecInt2d& TrisAdjToPts() const override;

  virtual BSHP<VecPt3d> PointsPtr() override;
  virtual BSHP<VecInt> TrianglesPtr() override;

  virtual int NumPoints() const override;
  virtual int NumTriangles() const override;

  // "tr" equivalent functions (trTriangleFromEdge, trIndex etc)

  // Non modifiers
  virtual bool TriangleFromEdge(int a_pt1,
                                int a_pt2,
                                int& a_tri,
                                int& a_idx1,
                                int& a_idx2) const override;
  virtual int TriangleAdjacentToEdge(int a_pt1, int a_pt2) const override;
  virtual int LocalIndex(int a_tri, int a_pt) const override;
  virtual int GlobalIndex(int a_triIdx, int a_localVtxIdx) const override;
  virtual bool VerticesAreAdjacent(int a_pt1, int a_pt2) const override;
  virtual int CommonEdgeIndex(int a_tri, int a_adjTri) const override;
  virtual int AdjacentTriangle(int a_triIdx, int a_edgeIdx) const override;
  virtual Pt3d TriangleCentroid(int a_tri) const override;
  virtual double TriangleArea(int a_tri) const override;
  // virtual int FirstBoundaryPoint () const;
  virtual int NextBoundaryPoint(int a_point) const override;
  virtual int PreviousBoundaryPoint(int a_point) const override;
  virtual void GetBoundaryPoints(VecInt& a_boundaryPoints) const override;
  virtual void GetBoundaryPolys(VecInt2d& a_polys) const override;
  virtual bool GetExtents(Pt3d& a_mn, Pt3d& a_mx) const override;
  virtual void ExportTinFile(std::ostream& a_os) const override;

  // Modifiers
  virtual bool SwapEdge(int a_triA, int a_triB, bool a_checkAngle = true) override;
  virtual void DeleteTriangles(const SetInt& a_trisToDelete) override;
  virtual void DeletePoints(const SetInt& a_points) override;
  virtual bool OptimizeTriangulation() override;
  virtual bool RemoveLongThinTrianglesOnPerimeter(const double a_ratio) override;
  virtual void Clear() override;
  virtual void BuildTrisAdjToPts() override;

  virtual std::string ToString() const override;
  virtual void FromString(const std::string&) override;
  template <typename Archive>
  void serialize(Archive& archive, const unsigned int version);

private:
  void InsertAdjacentTriangle(int a_pt, int a_tri);
  void DeleteAdjacentTriangle(int a_pt, int a_tri);
  bool TriIndexFound(const int& a_triPt) const;
  bool PointIndexFound(const Pt3d& a_point) const;
  bool AdjacentIndexFound(const VecInt& a_point) const;
  bool CheckAndSwap(int a_triA, int a_triB, bool a_propagate, const VecInt& a_flags);
  bool PointIsInCircumcircle(int a_tri1, int a_tri2, int id);
  void BuildTrisAdjToPtsConst() const;
  void CheckTriangle(const int a_tri, const int a_index, const double a_ratio,
    VecInt &a_flags, SetInt &a_trisToDelete) const;
  int AdjacentTriangleIndex(const int a_currTri, const int a_adjTri) const;

  BSHP<VecPt3d> m_pts;                  ///< tin points
  BSHP<VecInt> m_tris;                  ///< triangles, 0-based indices to m_pts, grouped by 3s
  BSHP<VecInt2d> m_trisAdjToPts;        ///< triangles adjacent to points. 1st dim = size of m_pts
  boost::unordered_set<int> m_toDelete; ///< Used only when deleting stuff

}; // class TrTinImpl
} // namespace xms

BOOST_CLASS_EXPORT(xms::TrTinImpl); ///< Cause boost
// BOOST_CLASS_VERSION(xms::TrTinImpl, 1) // only if using version

namespace xms
{
//----- Internal functions -----------------------------------------------------

//----- Class / Function definitions -------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// \class TrTinImpl
/// \brief Class to encapsulate a tin made simply of arrays of points,
///        triangles and adjacency information. Also methods to manipulate it.
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/// \brief constructor
//------------------------------------------------------------------------------
TrTinImpl::TrTinImpl()
: m_pts(new VecPt3d())
, m_tris(new VecInt())
, m_trisAdjToPts(new VecInt2d())
, m_toDelete()
{
}
//------------------------------------------------------------------------------
/// \brief destructor
//------------------------------------------------------------------------------
TrTinImpl::~TrTinImpl()
{
}
//------------------------------------------------------------------------------
/// \brief Sets the tin points.
/// \param[in] a_pts: The points.
//------------------------------------------------------------------------------
void TrTinImpl::SetPoints(BSHP<VecPt3d> a_pts)
{
  m_pts = a_pts;
}
//------------------------------------------------------------------------------
/// \brief Sets the tin triangles.
/// \param[in] a_tris: 0-based indices of each triangle's points (grouped by 3s)
//------------------------------------------------------------------------------
void TrTinImpl::SetTriangles(BSHP<VecInt> a_tris)
{
  m_tris = a_tris;
}
//------------------------------------------------------------------------------
/// \brief Sets the adjacency info of triangles adjacent to points.
/// \param[in] a_trisAdjToPts: 0-based indices of triangles adjacent to points.
//------------------------------------------------------------------------------
void TrTinImpl::SetTrianglesAdjacentToPoints(BSHP<VecInt2d> a_trisAdjToPts)
{
  m_trisAdjToPts = a_trisAdjToPts;
}
//------------------------------------------------------------------------------
/// \brief Set all the tin geometry at once (points, triangles, adjacency).
/// \param[in] a_pts:          The tin points.
/// \param[in] a_tris:  0-based indices of triangle points (grouped by 3s)
/// \param[in] a_trisAdjToPts: 0-based indices of triangles adjacent to points.
//------------------------------------------------------------------------------
void TrTinImpl::SetGeometry(BSHP<VecPt3d> a_pts, BSHP<VecInt> a_tris, BSHP<VecInt2d> a_trisAdjToPts)
{
  m_pts = a_pts;
  m_tris = a_tris;
  m_trisAdjToPts = a_trisAdjToPts;
} // TrTinImpl::SetGeometry
//------------------------------------------------------------------------------
/// \brief Return the tin points.
/// \return The points in the tin.
//------------------------------------------------------------------------------
VecPt3d& TrTinImpl::Points()
{
  return *m_pts.get();
}
//------------------------------------------------------------------------------
/// \brief Return 0-based indices of triangle points (grouped by 3s).
/// \return Triangles as 0-based indices of triangle points (grouped by 3s).
//------------------------------------------------------------------------------
VecInt& TrTinImpl::Triangles()
{
  return *m_tris.get();
}
//------------------------------------------------------------------------------
/// \brief Returns triangles adjacent to points (0-based).
/// \return 2D vector with triangles adjacent to points, size of num points.
//------------------------------------------------------------------------------
VecInt2d& TrTinImpl::TrisAdjToPts()
{
  return *m_trisAdjToPts.get();
}
//------------------------------------------------------------------------------
/// \brief Return the tin points.
/// \return The points in the tin.
//------------------------------------------------------------------------------
const VecPt3d& TrTinImpl::Points() const
{
  return *m_pts.get();
}
//------------------------------------------------------------------------------
/// \brief Return 0-based indices of triangle points (grouped by 3s).
/// \return Triangles as 0-based indices of triangle points (grouped by 3s).
//------------------------------------------------------------------------------
const VecInt& TrTinImpl::Triangles() const
{
  return *m_tris.get();
}
//------------------------------------------------------------------------------
/// \brief Returns triangles adjacent to points (0-based).
/// \return 2D vector with triangles adjacent to points, size of num points.
//------------------------------------------------------------------------------
const VecInt2d& TrTinImpl::TrisAdjToPts() const
{
  return *m_trisAdjToPts.get();
}
//------------------------------------------------------------------------------
/// \brief Return the pointer to tin points.
/// \return Shared pointer to the points vector.
//------------------------------------------------------------------------------
BSHP<VecPt3d> TrTinImpl::PointsPtr()
{
  return m_pts;
}
//------------------------------------------------------------------------------
/// \brief Return the pointer to tin triangles.
/// \return Shared pointer to the triangle points vector.
//------------------------------------------------------------------------------
BSHP<VecInt> TrTinImpl::TrianglesPtr()
{
  return m_tris;
}
//------------------------------------------------------------------------------
/// \brief Return the number of points.
/// \return Number of points.
//------------------------------------------------------------------------------
int TrTinImpl::NumPoints() const
{
  return (int)(*m_pts).size();
}
//------------------------------------------------------------------------------
/// \brief Return the number of triangles.
/// \return Number of triangles.
//------------------------------------------------------------------------------
int TrTinImpl::NumTriangles() const
{
  return (int)(*m_tris).size() / 3;
}
//------------------------------------------------------------------------------
/// \brief Finds the triangle with the edge defined by a_pt1 and a_pt2
///        and the local index of those points. Compare to trTinTriFromEdge.
/// \param[in] a_pt1: First edge point index (0-based).
/// \param[in] a_pt2: Second edge point index (0-based).
/// \param[out] a_tri: Index of triangle or XM_NONE if not found.
/// \param[out] a_localPt1: Local pt (0-2) of a_pt1 in a_tri.
/// \param[out] a_localPt2: Local pt (0-2) of a_pt2 in a_tri.
/// \return true if found.
//------------------------------------------------------------------------------
bool TrTinImpl::TriangleFromEdge(int a_pt1,
                                 int a_pt2,
                                 int& a_tri,
                                 int& a_localPt1,
                                 int& a_localPt2) const
{
  a_localPt1 = a_localPt2 = 0;

  // For each triangle adjacent to a_pt1
  VecInt::const_iterator adj = (*m_trisAdjToPts)[a_pt1].begin();
  for (; adj != (*m_trisAdjToPts)[a_pt1].end(); ++adj)
  {
    a_localPt1 = LocalIndex(*adj, a_pt1);
    a_localPt2 = trIncrementIndex(a_localPt1);
    if ((*m_tris)[(*adj * 3) + a_localPt2] == a_pt2)
    {
      a_tri = *adj;
      return true;
    }
  }
  a_tri = XM_NONE;
  return false;
} // TrTinImpl::TriangleFromEdge
//------------------------------------------------------------------------------
/// \brief Returns the triangle adjacent to the edge defined by a_pt1 and
///        a_pt2. Compare to trTriangleAdjacentToEdge.
///
/// Gives triangle on the right side of the edge.
/// \param a_pt1: First edge point index (0-based).
/// \param a_pt2: Second edge point index (0-based).
/// \return The triangle adjacent to the edge.
//------------------------------------------------------------------------------
int TrTinImpl::TriangleAdjacentToEdge(int a_pt1, int a_pt2) const
{
  int localIdx1, localIdx2;

  // For each triangle adjacent to a_pt1
  VecInt::const_iterator adj = (*m_trisAdjToPts)[a_pt1].begin();
  bool found = false;
  for (; adj != (*m_trisAdjToPts)[a_pt1].end(); ++adj)
  {
    localIdx1 = LocalIndex(*adj, a_pt1);
    localIdx2 = trDecrementIndex(localIdx1);
    if ((*m_tris)[(*adj * 3) + localIdx2] == a_pt2)
    {
      found = true;
      break;
    }
  }
  return (found ? *adj : XM_NONE);
} // TrTinImpl::TriangleAdjacentToEdge
//------------------------------------------------------------------------------
/// \brief Returns index (0-2) of point within triangle given global index.
///        Compare to trIndex.
/// \param a_tri: Triangle index (0-based).
/// \param a_pt: Global point index (0-based).
/// \return The local index (0-2) of the point a_pt in the triangle a_tri or
///         XM_NONE if a_pt is not on a_tri.
//------------------------------------------------------------------------------
int TrTinImpl::LocalIndex(int a_tri, int a_pt) const
{
  int t = a_tri * 3;
  for (int i = 0; i < 3; ++i)
  {
    if ((*m_tris)[t + i] == a_pt)
      return i;
  }
  return XM_NONE;
} // TrTinImpl::LocalIndex
//------------------------------------------------------------------------------
/// \brief Return true if vertices form the edge of a triangle. Compare to
///        vrVerticesAreAdjacent.
/// \param a_pt1: First point index (0-based).
/// \param a_pt2: Second point index (0-based).
/// \return true if points are adjacent.
//------------------------------------------------------------------------------
bool TrTinImpl::VerticesAreAdjacent(int a_pt1, int a_pt2) const
{
  // Look through the triangles adjacent to a_pt1
  for (size_t i = 0; i < (*m_trisAdjToPts)[a_pt1].size(); ++i)
  {
    int trisIdx = (*m_trisAdjToPts)[a_pt1][i] * 3; // Index in m_tris
    // See if a_pt2 is part of this triangle by checking all three points.
    // vrVerticesAreAdjacent would call trIndex then trIncrementIndex and
    // trDecrementIndex but I think this is simpler and maybe faster.
    for (int j = 0; j < 3; ++j)
    {
      if ((*m_tris)[trisIdx + j] == a_pt2)
      {
        return true;
      }
    }
  }
  return false;
} // TrTinImpl::VerticesAreAdjacent
//------------------------------------------------------------------------------
/// \brief Swap edges if triangles combine to form convex quad. Compare to
///        trSwapEdge.
///
/// a_triA and a_triB must be adjacent triangles.
///
///                b2 * top
///                  / \
///                 /   \   a_triB
///             b3 / --> \ b1
///           lft *-------* rgt
///                \ <-- / a3
///                 \   /   a_triA
///                  \ /
///               btm * a2
///
/// \param a_triA: First triangle.
/// \param a_triB: Second triangle.
/// \param a_checkAngle: If true, won't swap if very thin triangle would be
///                      created.
/// \return true if swap was successful.
//------------------------------------------------------------------------------
bool TrTinImpl::SwapEdge(int a_triA, int a_triB, bool a_checkAngle /*true*/)
{
  XM_ENSURE_TRUE_NO_ASSERT(a_triA >= 0 && a_triB >= 0, false);

  int a1, a2, a3, b1, b2, b3;
  int top, btm, lft, rgt;
  Pt3d toppt, btmpt, lftpt, rgtpt;

  b3 = CommonEdgeIndex(a_triB, a_triA);
  b2 = trDecrementIndex(b3);
  b1 = trIncrementIndex(b3);
  a3 = LocalIndex(a_triA, GlobalIndex(a_triB, b1));
  a1 = trIncrementIndex(a3);
  a2 = trIncrementIndex(a1);
  top = GlobalIndex(a_triB, b2);
  lft = GlobalIndex(a_triB, b3);
  rgt = GlobalIndex(a_triB, b1);
  btm = GlobalIndex(a_triA, a2);
  toppt = (*m_pts)[top];
  lftpt = (*m_pts)[lft];
  rgtpt = (*m_pts)[rgt];
  btmpt = (*m_pts)[btm];

  // make sure triangles are good
  double area1 = trArea(toppt, lftpt, btmpt);
  double area2 = trArea(btmpt, rgtpt, toppt);

  if (area1 > 0.0 && area2 > 0.0)
  {
    bool swap = true;
    if (a_checkAngle)
    {
      double ang1 = gmAngleBetweenEdges(rgtpt, btmpt, toppt);
      double ang2 = gmAngleBetweenEdges(btmpt, toppt, rgtpt);
      double ang3 = gmAngleBetweenEdges(toppt, btmpt, lftpt);
      double ang4 = gmAngleBetweenEdges(lftpt, toppt, btmpt);
      const static double min_ang = 0.01 * (XM_PI / 180.0);
      const static double max_ang = 179.99 * (XM_PI / 180.0);
      if (ang1 < min_ang || ang1 > max_ang || ang2 < min_ang || ang2 > max_ang || ang3 < min_ang ||
          ang3 > max_ang || ang4 < min_ang || ang4 > max_ang)
      {
        swap = false;
      }
    }
    if (swap)
    {
      // update the adjacent tin ptrs
      DeleteAdjacentTriangle(lft, a_triB);
      DeleteAdjacentTriangle(rgt, a_triA);
      InsertAdjacentTriangle(top, a_triA /*, a3*/);
      InsertAdjacentTriangle(btm, a_triB /*, b3*/);

      // update the verts
      (*m_tris)[(a_triA * 3) + a3] = top;
      (*m_tris)[(a_triB * 3) + b3] = btm;

      // See if we created a bad triangle
      if (!gmCounterClockwiseTri((*m_pts)[(*m_tris)[a_triA * 3]],
                                 (*m_pts)[(*m_tris)[(a_triA * 3) + 1]],
                                 (*m_pts)[(*m_tris)[(a_triA * 3) + 2]]) ||
          !gmCounterClockwiseTri((*m_pts)[(*m_tris)[a_triB * 3]],
                                 (*m_pts)[(*m_tris)[(a_triB * 3) + 1]],
                                 (*m_pts)[(*m_tris)[(a_triB * 3) + 2]]))
      {
        std::stringstream msg;
        msg << "Swapping triangles: " << a_triA << " and " << a_triB
            << " created invalid triangles (ordered cw instead of ccw).";
        XM_LOG(xmlog::error, msg.str());
      }
      return true;
    }
  }
  return false;
} // TrTinImpl::SwapEdge
//------------------------------------------------------------------------------
/// \brief Swap edges if triangles combine to form convex quad. Compare to
///        trCheckAndSwap.
///
/// a_triA and a_triB must be adjacent triangles.
///
///                b2 * top
///                  / \
///                 /   \   a_triB
///             b3 / --> \ b1
///           lft *-------* rgt
///                \ <-- / a3
///                 \   /   a_triA
///                  \ /
///               btm * a2
///
/// \param a_triA: First triangle.
/// \param a_triB: Second triangle.
/// \param a_propagate: If true, recursively checks adjacent triangles, which
///                     will happen even if false if adjacent triangles flag is
///                     not zero.
/// \param a_flags: Flags to keep track of visited triangles.
/// \return true if swap was successful.
//------------------------------------------------------------------------------
bool TrTinImpl::CheckAndSwap(int a_triA, int a_triB, bool a_propagate, const VecInt& a_flags)
{
  XM_ENSURE_TRUE_NO_ASSERT(a_triA >= 0 && a_triB >= 0, false);

  bool change = false;
  int tri1id2 = 0;
  int tri2id0 = 0, tri2id1 = 0, tri2id2 = 0;

  if (a_triB != XM_NONE)
  {
    tri2id2 = CommonEdgeIndex(a_triB, a_triA);
    tri2id1 = trDecrementIndex(tri2id2);
    change = PointIsInCircumcircle(a_triA, a_triB, tri2id1);
  }

  if (change)
  {
    tri2id0 = trIncrementIndex(tri2id2);
    tri1id2 = LocalIndex(a_triA, GlobalIndex(a_triB, tri2id0));

    SwapEdge(a_triA, a_triB, false);
    int adjTri = AdjacentTriangle(a_triA, tri1id2);
    if (a_propagate || (!a_propagate && adjTri != XM_NONE && a_flags[adjTri]))
    {
      CheckAndSwap(a_triA, adjTri, a_propagate, a_flags);
    }
    adjTri = AdjacentTriangle(a_triA, tri2id0);
    if (a_propagate || (!a_propagate && adjTri != XM_NONE && a_flags[adjTri]))
    {
      CheckAndSwap(a_triB, adjTri, a_propagate, a_flags);
    }
  }

  return false;
} // TrTinImpl::CheckAndSwap
//------------------------------------------------------------------------------
/// \brief Returns \a true if a_localPt of a_tri2 is inside a_tri1's
///        circumcircle.
///
/// SPECIAL CASE - we want all square elements to have a "diagonal"
/// that is connected from lower left to upper right corner, so
/// if a_tri1 and a_tri2 form a square or rectangle, it will return TRUE if the
/// "diagonal" edge is from upper left to lower right corner and
/// FALSE if the "diagonal" edge is from lower left to upper right
/// corner.
///
/// \param a_tri1: Triangle (0-based) adjacent to a_tri2.
/// \param a_tri2: Triangle (0-based) adjacent to a_tri1.
/// \param a_localPt: Local index (0-2) of the point in a_tri2 that is opposite
///              a_tri1.
/// \return \a true if \a a_localPt of \a a_tri2 is inside \a a_tri1's
///         circumcircle, else false.
/// \pre    Triangles must be adjacent
/// \verbatim
///
///        *------------*
///        |          / |
///        | a_tri1 /   |
///        |      /     |
///        |    /       |
///        |  /  a_tri2 |
///        *------------*
///                      a_localPt
/// \endverbatim
//------------------------------------------------------------------------------
bool TrTinImpl::PointIsInCircumcircle(int a_tri1, int a_tri2, int a_localPt)
{
  int t = a_tri1 * 3;
  return (gmPtInCircumcircle((*m_pts)[GlobalIndex(a_tri2, a_localPt)], &(*m_pts)[t]) == PT_IN);
} // TrTinImpl::PointIsInCircumcircle
//------------------------------------------------------------------------------
/// \brief Return index of common edge between triangle and neighbor. Edge
///        index is 0-2 based on a_tri. Compare to trCommonEdgeIndex.
///
///  a_triIdx must be adjacent to neighbor.
///
/// \param a_tri: Triangle index (0-based).
/// \param a_adjTri: Adjacent triangle index (0-based).
/// \return Index of edge (0-2) in a_tri between a_tri and a_adTri, or
///         XM_NONE if not adjacent.
//------------------------------------------------------------------------------
int TrTinImpl::CommonEdgeIndex(int a_tri, int a_adjTri) const
{
  bool found = false;
  int edge = 0;
  while (!found)
  {
    int adjtri = AdjacentTriangle(a_tri, edge);
    if (adjtri == a_adjTri)
      found = true;
    else
      edge = trIncrementIndex(edge);
    if (edge == 0)
      break;
  }
  if (!found)
    edge = XM_NONE;
  return edge;
} // TrTinImpl::CommonEdgeIndex
//------------------------------------------------------------------------------
/// \brief Returns the triangle adjacent to a_triIdx across a_edgeIdx (0-2).
///        Compare to trAdjacentTriangle.
/// Example: a_edgeIdx 2 returns triangle adjacent to edge c.
/// \verbatim
///              2
///             / \
///          c /   \ b
///           /     \
///          0-------1
///              a
/// \endverbatim
/// \param a_triIdx: Triangle index (0-based).
/// \param a_edgeIdx: Local edge index (0-2) in a_tri.
/// \return Triangle adjacent to a_tri along it's a_edge or XM_NONE if there is no
///         triangle adjacent to the edge.
//------------------------------------------------------------------------------
int TrTinImpl::AdjacentTriangle(int a_triIdx, int a_edgeIdx) const
{
  int t = a_triIdx * 3;
  int edgePt1 = (*m_tris)[t + a_edgeIdx];
  int edgePt2 = (*m_tris)[t + trIncrementIndex(a_edgeIdx)];

  int adjTri;
  int idx1, idx2;
  if (TriangleFromEdge(edgePt2, edgePt1, adjTri, idx1, idx2))
  {
    return adjTri;
  }
  return XM_NONE;
} // TrTinImpl::AdjacentTriangle
//------------------------------------------------------------------------------
/// \brief Calculate and return the centroid of a triangle.
/// \param a_tri: Triangle index (0-based).
/// \return The centroid.
//------------------------------------------------------------------------------
xms::Pt3d TrTinImpl::TriangleCentroid(int a_tri) const
{
  int t = a_tri * 3;
  Pt3d centroid((*m_pts)[(*m_tris)[t]]);
  centroid += (*m_pts)[(*m_tris)[t + 1]];
  centroid += (*m_pts)[(*m_tris)[t + 2]];
  return (centroid / (double)3);
} // TrTinImpl::TriangleCentroid
//------------------------------------------------------------------------------
/// \brief Calculate and return the area of a triangle.
/// \param a_tri: Triangle index (0-based).
/// \return The area.
//------------------------------------------------------------------------------
double TrTinImpl::TriangleArea(int a_tri) const
{
  int t = a_tri * 3;
  return trArea((*m_pts)[(*m_tris)[t]], (*m_pts)[(*m_tris)[t + 1]], (*m_pts)[(*m_tris)[t + 2]]);

} // TrTinImpl::TriangleArea
#if 0 // FirstBoundaryPoint has been commented out because it wasn't used
      // Also, what if you have holes? Consider using GetBoundaryPoints and
      // GetBoundaryPolys. The code has been left here in case we reconsider.
//------------------------------------------------------------------------------
/// \brief Returns the first encountered point on a boundary. Compare to
///        tiFirstBoundaryVertex.
///
/// \return The point index or XM_NONE if not found.
//------------------------------------------------------------------------------
int TrTinImpl::FirstBoundaryPoint () const
{
  XM_ENSURE_TRUE_NO_ASSERT(!(*m_trisAdjToPts).empty(), XM_NONE);

  int nPoints = NumPoints();
  int firstTri, tri, localIndex, prevtri(XM_NONE);
  for (int p = 0; p < nPoints; ++p)
  {
    firstTri = tri = (*m_trisAdjToPts)[p][0];
    do {
      localIndex = LocalIndex(tri, p);
      prevtri = tri;
      tri = AdjacentTriangle(tri, localIndex);
    } while (tri != firstTri && tri != XM_NONE);
    if (tri == XM_NONE) {
      return p;
    }
  }
  return XM_NONE;
} // TrTinImpl::FirstBoundaryPoint
#endif
//------------------------------------------------------------------------------
/// \brief Returns the next point CW from point on the boundary. CCW if in
///        an inside hole. Compare to trNextBoundaryVertex (or
///        trPreviousBoundaryVertex since order here is CW, not CCW).
///
/// Point must be on the boundary.
/// \param[in] a_point: The starting point.
/// \return The point index or XM_NONE if not found.
//------------------------------------------------------------------------------
int TrTinImpl::NextBoundaryPoint(int a_point) const
{
  XM_ENSURE_TRUE_NO_ASSERT(a_point >= 0 && a_point <= (int)(*m_pts).size(), XM_NONE);
  XM_ENSURE_TRUE_NO_ASSERT(!(*m_trisAdjToPts)[a_point].empty(), XM_NONE);

  // Get a starting tri
  int firstTri, tri, prevtri(XM_NONE), bpoint(XM_NONE);
  int localIndex;
  firstTri = tri = (*m_trisAdjToPts)[a_point][0];

  // use a "do" loop not a "while" loop here
  do
  {
    localIndex = trDecrementIndex(LocalIndex(tri, a_point));
    prevtri = tri;
    tri = AdjacentTriangle(tri, localIndex);
  } while (tri != firstTri && tri != XM_NONE);

  // if tri is null, there is a boundary vertex
  if (tri == XM_NONE)
  {
    bpoint = GlobalIndex(prevtri, localIndex);
  }
  return bpoint;

} // TrTinImpl::NextBoundaryPoint
//------------------------------------------------------------------------------
/// \brief Returns the next point CCW from point on the boundary. CW if in
///        an inside hole. Compare to trPreviousBoundaryVertex (or
///        trNextBoundaryVertex since order here is CW, not CCW).
///
/// Point must be on the boundary.
/// \param[in] a_point: The starting point.
/// \return The point index or XM_NONE if not found.
//------------------------------------------------------------------------------
int TrTinImpl::PreviousBoundaryPoint(int a_point) const
{
  XM_ENSURE_TRUE_NO_ASSERT(a_point >= 0 && a_point <= (int)(*m_pts).size(), XM_NONE);
  XM_ENSURE_TRUE_NO_ASSERT(!(*m_trisAdjToPts)[a_point].empty(), XM_NONE);

  // Get a starting tri
  int firstTri, tri, prevtri(XM_NONE), bpoint(XM_NONE);
  int localIndex;
  firstTri = tri = (*m_trisAdjToPts)[a_point][0];

  // use a "do" loop not a "while" loop here
  do
  {
    localIndex = LocalIndex(tri, a_point);
    prevtri = tri;
    tri = AdjacentTriangle(tri, localIndex);
  } while (tri != firstTri && tri != XM_NONE);

  // if tri is null, there is a boundary vertex
  if (tri == XM_NONE)
  {
    bpoint = GlobalIndex(prevtri, trIncrementIndex(localIndex));
  }
  return bpoint;
} // TrTinImpl::PreviousBoundaryPoint
//------------------------------------------------------------------------------
/// \brief Gives the 0-based indices of all points on any boundary, in no
///        particular order.
/// \param[out] a_boundaryPoints: The 0-based indices of all points on the
/// boundary.
//------------------------------------------------------------------------------
void TrTinImpl::GetBoundaryPoints(VecInt& a_boundaryPoints) const
{
  // Make sure adjacencies exist first
  if (TrisAdjToPts().empty())
  {
    BuildTrisAdjToPtsConst();
  }

  VecInt& tris = *m_tris;
  SetInt setPoints;
  size_t numTri = m_tris->size() / 3;
  int t = 0;
  for (size_t tri = 0; tri < numTri; ++tri, t += 3)
  {
    for (int i = 0; i < 3; ++i)
    {
      if (AdjacentTriangle((int)tri, i) == XM_NONE)
      {
        setPoints.insert(tris[t + i]);
        setPoints.insert(tris[t + trIncrementIndex(i)]);
      }
    }
  }

  a_boundaryPoints.clear();
  std::copy(setPoints.begin(), setPoints.end(), std::back_inserter(a_boundaryPoints));

} // TrTinImpl::GetBoundaryPoints
//------------------------------------------------------------------------------
/// \brief Gets exterior boundary and all interior voids as polygons of 0-based
///        point indices. First point is not repeated as the last point.
/// \param[out] a_polys: Outer polygon is CW, inner polys are CCW. First point
///                      is repeated at the end.
//------------------------------------------------------------------------------
void TrTinImpl::GetBoundaryPolys(VecInt2d& a_polys) const
{
  // Get all the points on any boundary
  VecInt points;
  GetBoundaryPoints(points);

  // Convert to a set
  boost::unordered_set<int> pointSet(points.begin(), points.end());

  // Put points into polygons
  a_polys.clear();
  while (!pointSet.empty())
  {
    // Get the new first point
    // using minimum element so result is consistent for different architectures
    auto it = std::min_element(pointSet.begin(), pointSet.end());

    // Find adjacent points on the border until we return to the first point,
    // removing the points from the set of boundary points
    int first = *it;
    a_polys.push_back(VecInt());
    a_polys.back().push_back(first);
    pointSet.erase(it);
    int next = NextBoundaryPoint(first);
    while (next != first)
    {
      a_polys.back().push_back(next);
      it = pointSet.find(next);
      if (it == pointSet.end())
      {
        XM_LOG(xmlog::warning,
               "Unable to get boundary polygon from meshed points. Check the input polygon.");
        a_polys.clear();
        return;
      }
      pointSet.erase(it);
      next = NextBoundaryPoint(next);
    }
    a_polys.back().push_back(a_polys.back().front()); // Repeat first point as the last point
  }
} // TrTinImpl::GetBoundaryPolys
//------------------------------------------------------------------------------
/// \brief Computes the extents (min, max) of the tin.
/// \param[out] a_mn: Minimum xyz.
/// \param[out] a_mx: Maximum xyz.
/// \return true if there are any points and the extents were computed.
//------------------------------------------------------------------------------
bool TrTinImpl::GetExtents(Pt3d& a_mn, Pt3d& a_mx) const
{
  if (!(*m_pts).empty())
  {
    const VecPt3d& pts = (*m_pts);
    a_mn = XM_DBL_HIGHEST;
    a_mx = XM_DBL_LOWEST;
    for (size_t i = 0; i < pts.size(); ++i)
    {
      gmAddToExtents(pts[i], a_mn, a_mx);
    }
    return true;
  }
  return false;
} // TrTinImpl::GetExtents
//------------------------------------------------------------------------------
/// \brief Export in the .tin file format. Useful for debugging.
/// \param[out] a_os: The stream to export to.
//------------------------------------------------------------------------------
void TrTinImpl::ExportTinFile(std::ostream& a_os) const
{
  a_os << "TIN\n";
  a_os << "BEGT\n";

  // Points
  VecPt3d& pts = (*m_pts);
  a_os << "VERT " << pts.size() << "\n";
  for (size_t i = 0; i < pts.size(); ++i)
  {
    a_os << STRstd(pts[i].x) << "\t" << STRstd(pts[i].y) << "\t" << STRstd(pts[i].z) << "\n";
  }

  // Triangles
  VecInt& tris = (*m_tris);
  a_os << "TRI " << NumTriangles() << "\n";
  for (size_t i = 0; i < tris.size(); i += 3)
  {
    a_os << (tris[i] + 1) << "\t" << (tris[i + 1] + 1) << "\t" << (tris[i + 2] + 1) << "\n";
  }

  a_os << "ENDT\n";
} // TrTinImpl::ExportTinFile
//------------------------------------------------------------------------------
/// \brief Adds a_tri as an adjacent triangle to a_pt and updates
///        m_trisAdjToPts.
/// \param[in] a_pt: The point that a_tri is adjacent to.
/// \param[in] a_tri: The triangle adjacent to a_pt to add to list of adjacents.
//------------------------------------------------------------------------------
void TrTinImpl::InsertAdjacentTriangle(int a_pt, int a_tri)
{
  XM_ENSURE_TRUE_VOID(a_pt >= 0 && a_tri >= 0);

  (*m_trisAdjToPts)[a_pt].push_back(a_tri);
} // TrTinImpl::InsertAdjacentTriangle
//------------------------------------------------------------------------------
/// \brief Removes a_tri from a_pt's adjacent triangles. Compare to
///        vrDeleteAdjacentTrianglePtr.
/// \param[in] a_pt: The point from which we are removing an adjacent triangle.
/// \param[in] a_tri: The triangle we are removing as an adjacent from point.
//------------------------------------------------------------------------------
void TrTinImpl::DeleteAdjacentTriangle(int a_pt, int a_tri)
{
  VecInt::iterator adjTri = (*m_trisAdjToPts)[a_pt].begin();
  for (; adjTri != (*m_trisAdjToPts)[a_pt].end(); ++adjTri)
  {
    if (*adjTri == a_tri)
    {
      (*m_trisAdjToPts)[a_pt].erase(adjTri);
      break;
    }
  }
} // TrTinImpl::DeleteAdjacentTriangle
//------------------------------------------------------------------------------
/// \brief Returns index into m_pts of a_localPt which is 0-2.
/// \param a_triIdx = Triangle index (0-based).
/// \param a_localPt = Local index (0-2) of point within the triangle.
/// \return Global index into m_pts of a_localPt.
//------------------------------------------------------------------------------
int TrTinImpl::GlobalIndex(int a_triIdx, int a_localPt) const
{
  XM_ENSURE_TRUE((a_triIdx * 3) + a_localPt < static_cast<int>((*m_tris).size()), XM_NONE);
  return (*m_tris)[(a_triIdx * 3) + a_localPt];
} // TrTinImpl::GlobalIndex
//------------------------------------------------------------------------------
/// \brief Predicate used in remove_if to get the index of the current item
///        in the vector.
///
/// \see http://stackoverflow.com/questions/23122555/
/// This could be a lamda defined where it's used if we could figure out how
/// to get that to work.
/// \param[in] a_triPt: Index of a point on a triangle.
/// \return true if found, else false.
//------------------------------------------------------------------------------
bool TrTinImpl::TriIndexFound(const int& a_triPt) const
{
  int index = (int)(&a_triPt - &*(*m_tris).begin());
  return m_toDelete.find(index) != m_toDelete.end();
} // TrTinImpl::TriIndexFound
//------------------------------------------------------------------------------
/// \brief Predicate used in remove_if to get the index of the current item
///        in the vector.
///
/// \see http://stackoverflow.com/questions/23122555/
/// This could be a lamda defined where it's used if we could figure out how
/// to get that to work.
/// \param[in] a_point: A point in a tin.
/// \return true if found, else false.
//------------------------------------------------------------------------------
bool TrTinImpl::PointIndexFound(const Pt3d& a_point) const
{
  int index = (int)(&a_point - &*(*m_pts).begin());
  return m_toDelete.find(index) != m_toDelete.end();
} // TrTinImpl::PointIndexFound
//------------------------------------------------------------------------------
/// \brief Predicate used in remove_if to get the index of the current item
///        in the vector.
///
/// \see http://stackoverflow.com/questions/23122555/
/// This could be a lamda defined where it's used if we could figure out how
/// to get that to work.
/// \param[in] a_adj: Triangles adjacent to a point.
/// \return true if found, else false.
//------------------------------------------------------------------------------
bool TrTinImpl::AdjacentIndexFound(const VecInt& a_adj) const
{
  int index = (int)(&a_adj - &*(*m_trisAdjToPts).begin());
  return m_toDelete.find(index) != m_toDelete.end();
} // TrTinImpl::AdjacentIndexFound
//------------------------------------------------------------------------------
/// \brief Deletes the triangles specified in \a a_trisToDelete and updates
///        and renumbers triangle adjacency info.
/// \param[in] a_trisToDelete: Triangles to be deleted.
//------------------------------------------------------------------------------
void TrTinImpl::DeleteTriangles(const SetInt& a_trisToDelete)
{
  int oldNumTris = NumTriangles();

  // Add indices of triangles in m_tris to a hash table
  for (SetInt::iterator it = a_trisToDelete.begin(); it != a_trisToDelete.end(); ++it)
  {
    XM_ENSURE_TRUE_VOID_NO_ASSERT((*it * 3) + 2 < static_cast<int>((*m_tris).size()));
    m_toDelete.insert((*it * 3));
    m_toDelete.insert((*it * 3) + 1);
    m_toDelete.insert((*it * 3) + 2);
  }

  // Erase triangles from the m_tris vector using m_toDelete set.
  // This would be better as a lamda but I couldn't get it to work.
  (*m_tris).erase(std::remove_if((*m_tris).begin(), (*m_tris).end(),
                                 boost::bind(&TrTinImpl::TriIndexFound, this, _1)),
                  (*m_tris).end());
  m_toDelete.clear();

  // Remove adjacent triangles from m_trisAdjToPts
  for (VecInt2d::iterator itPoint = (*m_trisAdjToPts).begin(); itPoint != (*m_trisAdjToPts).end();
       ++itPoint)
  {
    for (VecInt::iterator itTri = itPoint->begin(); itTri != itPoint->end();)
    {
      if (a_trisToDelete.find(*itTri) != a_trisToDelete.end())
      {
        itTri = itPoint->erase(itTri);
      }
      else
      {
        ++itTri;
      }
    }
  }

  // Renumber triangles in m_trisAdjToPts
  // Create a oldToNewIdxs
  VecInt oldToNewIdxs(oldNumTris);                        // Size of old, will have new idxs
  std::iota(oldToNewIdxs.begin(), oldToNewIdxs.end(), 0); // Init 0 to old size
  SetInt::iterator it = a_trisToDelete.begin();
  int offset = 0;
  for (int i = 0; i < oldNumTris; ++i)
  {
    if (it != a_trisToDelete.end() && i >= *it)
    {
      offset++;
      ++it;
    }
    else
    {
      oldToNewIdxs[i] -= offset;
    }
  }
  for (VecInt2d::iterator itPoint = (*m_trisAdjToPts).begin(); itPoint != (*m_trisAdjToPts).end();
       ++itPoint)
  {
    for (VecInt::iterator itAdj = itPoint->begin(); itAdj != itPoint->end(); ++itAdj)
    {
      *itAdj = oldToNewIdxs[*itAdj];
    }
  }

} // TrTinImpl::DeleteTriangles
//------------------------------------------------------------------------------
/// \brief Deletes the points and any attached triangles, updates adjacency and
///        renumbers things.
/// \param[in] a_points: The set of points to delete.
//------------------------------------------------------------------------------
void TrTinImpl::DeletePoints(const SetInt& a_points)
{
  // Add a_points to a hash table which is used by the predicate
  m_toDelete.clear();
  m_toDelete.insert(a_points.begin(), a_points.end());

  // Erase points from the m_pts vector using m_toDelete set.
  // This would be better as a lamda but I couldn't get it to work.
  (*m_pts).erase(std::remove_if((*m_pts).begin(), (*m_pts).end(),
                                boost::bind(&TrTinImpl::PointIndexFound, this, _1)),
                 (*m_pts).end());

  // Create a set of all triangles adjacent to all points to be deleted
  SetInt trianglesToDelete;
  if (!(*m_trisAdjToPts).empty() && !(*m_tris).empty())
  {
    SetInt::const_iterator it = a_points.begin();
    for (; it != a_points.end(); ++it)
    {
      trianglesToDelete.insert((*m_trisAdjToPts)[*it].begin(), (*m_trisAdjToPts)[*it].end());
    }
  }

  if (!(*m_trisAdjToPts).empty())
  {
    // Erase points from the m_trisAdjToPts vector using m_toDelete set.
    // This would be better as a lamda but I couldn't get it to work.
    (*m_trisAdjToPts)
      .erase(std::remove_if((*m_trisAdjToPts).begin(), (*m_trisAdjToPts).end(),
                            boost::bind(&TrTinImpl::AdjacentIndexFound, this, _1)),
             (*m_trisAdjToPts).end());
  }
  m_toDelete.clear();

  // Remove all triangles attached to the deleted points
  if (!(*m_trisAdjToPts).empty() && !(*m_tris).empty())
  {
    DeleteTriangles(trianglesToDelete);
    trRenumberOnDelete(a_points, (*m_tris));
  }

} // TrTinImpl::DeletePoints
//------------------------------------------------------------------------------
/// \brief Swaps triangle edges until they are a Delauney triangulation.
/// \return True if modified.
//------------------------------------------------------------------------------
bool TrTinImpl::OptimizeTriangulation()
{
  bool modified = false;
  int nTri = NumTriangles();
  VecInt flags(nTri, false);

  bool meshaltered;
  int id;
  int adjtri;

  do
  {
    meshaltered = false;
    for (int tri = 0; tri < nTri; ++tri)
    {
      if (flags[tri])
      {
        id = 0;
        for (int i = 0; i <= 2; i++)
        {
          // get neighboring element
          adjtri = AdjacentTriangle(tri, id);
          if (adjtri != XM_NONE && flags[adjtri])
          {
            // swap if needed and propagate
            if (CheckAndSwap(tri, adjtri, false, flags))
            {
              meshaltered = true;
            }
          }
          id = trIncrementIndex(id);
        }
      }
    }
    if (meshaltered)
      modified = true;
  } while (meshaltered);

  return true;
} // TrTinImpl::OptimizeTriangulation
//------------------------------------------------------------------------------
/// \brief finds the index of adjacent triangle that points to the current
///        triangle
/// \param[in] a_currTri: The index of the triangle to check.
/// \param[in] a_adjTri: The index of the adjacent triangle.
/// \return The index of the adjacent triangle that points to the current
///         triangle or XM_NONE if the triangles are not adjacent
//------------------------------------------------------------------------------
int TrTinImpl::AdjacentTriangleIndex(const int a_currTri, const int a_adjTri) const
{
  if (a_currTri == XM_NONE || a_adjTri == XM_NONE)
    return XM_NONE; // hold onto your butts
  int index = 0;
  while (index < 3)
  {
    if (AdjacentTriangle(a_adjTri, index) == a_currTri)
      return index;
    else
      ++index;
  }
  return XM_NONE; // hold onto your butts
} // TrTinImpl::AdjacentTriangleIndex
//------------------------------------------------------------------------------
/// \brief If triangle is long and thin (index edge is long compared to sum of
///        other two edges) the triangle is marked for deletion and the
///        triangles adjacent to the other two edges are checked.
/// \param[in] a_tri: The index of the triangle to check.
/// \param[in] a_index: The index of the triangle's edge to check.
/// \param[in] a_ratio: The ratio of one edge length compared to the sum of the
///            other two edge lengths.  If the length ratio of a triangle is
///            greater then or equal to this given ratio, the triangle is marked
///            for deletion.
/// \param[in,out] a_flags: Flags for each of the triangles.
/// \param[out] a_trisToDelete: Triangles to be deleted that are long and thin.
//------------------------------------------------------------------------------
void TrTinImpl::CheckTriangle(const int a_tri, const int a_index,
  const double a_ratio, VecInt &a_flags, SetInt &a_trisToDelete) const
{
  if (a_tri != XM_NONE)
  {
    a_flags[a_tri] |= a_index;
    const int indexPlus1 = trIncrementIndex(a_index);
    const int indexPlus2 = trIncrementIndex(indexPlus1);
    int t = a_tri * 3;
    Pt3d p1((*m_pts)[(*m_tris)[t + a_index]]);
    Pt3d p2((*m_pts)[(*m_tris)[t + indexPlus1]]);
    Pt3d p3((*m_pts)[(*m_tris)[t + indexPlus2]]);
    double l12 = gmXyDistance(p1, p2);
    double l23 = gmXyDistance(p2, p3);
    double l31 = gmXyDistance(p3, p1);
    double lengthRatio = l12 / (l23 + l31);
    if (lengthRatio >= a_ratio)
    {
      a_trisToDelete.insert(a_tri);
      // check the two adjacent triangles
      int adjTri = AdjacentTriangle(a_tri, indexPlus1);
      if (adjTri != XM_NONE && !(a_flags[adjTri] & AdjacentTriangleIndex(a_tri, adjTri)))
        CheckTriangle(adjTri, AdjacentTriangleIndex(a_tri, adjTri), a_ratio,
          a_flags, a_trisToDelete);
      adjTri = AdjacentTriangle(a_tri, indexPlus2);
      if (adjTri != XM_NONE && !(a_flags[adjTri] & AdjacentTriangleIndex(a_tri, adjTri)))
        CheckTriangle(adjTri, AdjacentTriangleIndex(a_tri, adjTri), a_ratio,
          a_flags, a_trisToDelete);
    }
  }
} // TrTinImpl::CheckTriangle
//------------------------------------------------------------------------------
/// \brief Removes long thin triangles on the boundary of the TIN.
/// \param[in] a_ratio: The ratio of one edge length compared to the sum of the
///            other two edge lengths.  If the length ratio of a triangle is
///            greater then or equal to this given ratio, the triangle is deleted.
/// \return true on success.
//------------------------------------------------------------------------------
bool TrTinImpl::RemoveLongThinTrianglesOnPerimeter(const double a_ratio)
{
  // set flags so that a triangle does not get checked more than once
  int nTri = NumTriangles();
  VecInt flags(nTri, 4);
  SetInt trisToDelete;
  for (int tri = 0; tri < nTri; ++tri)
  {
    int i = 0;
    while (i <= 2)
    {
      if (AdjacentTriangle(tri, i) == XM_NONE)
      {
        // No adjacent triangle on edge i.  currtriangle is on outside edge
        CheckTriangle(tri, i, a_ratio, flags, trisToDelete);
        // Don't stop.  Must check all edges as 2 may be on boundary and
        // the first one might not have met the criteria but the 2nd might.
      }
      ++i;
    }
  }
  DeleteTriangles(trisToDelete);
  return true;
} // TrTinImpl::RemoveLongThinTrianglesOnPerimeter
//------------------------------------------------------------------------------
/// \brief Delete the memory.
//------------------------------------------------------------------------------
void TrTinImpl::Clear()
{
  if (m_pts)
    m_pts->clear();
  if (m_tris)
    m_tris->clear();
  if (m_trisAdjToPts)
    m_trisAdjToPts->clear();
} // TrTinImpl::Clear
//------------------------------------------------------------------------------
/// \brief Build array of triangles adjacent to points.
//------------------------------------------------------------------------------
void TrTinImpl::BuildTrisAdjToPts()
{
  BuildTrisAdjToPtsConst(); // code moved to const version
} // TrTinImpl::BuildTrisAdjToPts
//------------------------------------------------------------------------------
/// \brief Use boost archive to get the TrTin as text.
/// \return The string.
//------------------------------------------------------------------------------
std::string TrTinImpl::ToString() const
{
  std::stringstream ss;
  {
    boost::archive::text_oarchive oa(ss);
    oa << *this;
  }
  return ss.str();
} // TrTinImpl::ToString
//------------------------------------------------------------------------------
/// \brief Use boost archive to turn the text into a TrTin.
/// \param[in] a_text: The string.
//------------------------------------------------------------------------------
void TrTinImpl::FromString(const std::string& a_text)
{
  std::stringstream ss(a_text);
  {
    boost::archive::text_iarchive ia(ss);
    ia >> *this;
  }
} // TrTinImpl::FromString
//------------------------------------------------------------------------------
/// \brief Boost serialize function.
/// \param archive: The archive.
/// \param version: Version number.
//------------------------------------------------------------------------------
template <typename Archive>
void TrTinImpl::serialize(Archive& archive, const unsigned int version)
{
  (void)version; // Because Doxygen complained when commented out above.
  archive& boost::serialization::base_object<TrTin>(*this); // does nothing

  VecPt3d& p(*m_pts);
  VecInt& t(*m_tris);
  VecInt2d& t2(*m_trisAdjToPts);

  archive& p;
  archive& t;
  archive& t2;
} // TrTinImpl::serialize
//------------------------------------------------------------------------------
/// \brief A const function used only internally and needed to modify
///        m_trisAdjToPts which is mutable.
//------------------------------------------------------------------------------
void TrTinImpl::BuildTrisAdjToPtsConst() const
{
  // You would think m_trisAdjToPts would need to be mutable, but it doesn't
  VecInt2d& trisAdjToPts = *m_trisAdjToPts;
  const VecInt& tris = *m_tris;
  trisAdjToPts.assign(m_pts->size(), VecInt());
  size_t numTri = tris.size() / 3;
  int t = 0;
  for (size_t tri = 0; tri < numTri; ++tri)
  {
    trisAdjToPts[tris[t++]].push_back((int)tri);
    trisAdjToPts[tris[t++]].push_back((int)tri);
    trisAdjToPts[tris[t++]].push_back((int)tri);
  }
  stShrinkCapacity(trisAdjToPts);
} // TrTinImpl::BuildTrisAdjToPtsConst

////////////////////////////////////////////////////////////////////////////////
/// \class TrTin
/// \see TrTinImpl
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/// \brief constructor
//------------------------------------------------------------------------------
TrTin::TrTin()
{
}
//------------------------------------------------------------------------------
/// \brief destructor
//------------------------------------------------------------------------------
TrTin::~TrTin()
{
}
//------------------------------------------------------------------------------
/// \brief Create a TrTinImpl object.
/// \return TrTin.
//------------------------------------------------------------------------------
BSHP<TrTin> TrTin::New()
{
  return BDPC<TrTin>(BSHP<TrTinImpl>(new TrTinImpl()));
} // TrTin::TrTin
//------------------------------------------------------------------------------
/// \brief Boost serialize function.
//------------------------------------------------------------------------------
// template<typename Archive>
// void TrTin::serialize(Archive& archive, const unsigned int version)
//{
//  TrTinImpl *t = dynamic_cast<TrTinImp*>(this);
//  if (t)
//  {
//    t->serialize(archive, version);
//  }
//} // TrTinImpl::serialize

//----- Global functions -------------------------------------------------------

//------------------------------------------------------------------------------
/// \brief Renumbers items in \a a_vec as if deleting items in \a a_deleting.
///
/// Used to renumber m_tris when deleting points. Subtracts 1 from each point
/// index that is greater than or equal to the point index that was deleted.
/// Goes from greatest number being deleted to least so greater numbers get
/// reduced more with each pass.
/// \param[in] a_deleting: Set of numbered things in a_vec being deleted.
/// \param[in,out] a_vec: Vector of numbered things that gets renumbered.
//------------------------------------------------------------------------------
void trRenumberOnDelete(const SetInt& a_deleting, VecInt& a_vec)
{
  for (SetInt::reverse_iterator itDeleting = a_deleting.rbegin(); itDeleting != a_deleting.rend();
       ++itDeleting)
  {
    int pt = *itDeleting;
    for (VecInt::iterator itTriPt = a_vec.begin(); itTriPt != a_vec.end(); ++itTriPt)
    {
      if (*itTriPt >= pt)
      {
        --(*itTriPt);
      }
    }
  }
} // trRenumberOnDelete

} // namespace xms

#if CXX_TEST
////////////////////////////////////////////////////////////////////////////////
// UNIT TESTS
////////////////////////////////////////////////////////////////////////////////

#include <xmsinterp/triangulate/TrTin.t.h>

#include <xmscore/testing/TestTools.h>
#include <xmscore/misc/carray.h>
#include <xmsinterp/triangulate/TrTriangulatorPoints.h>

//----- Namespace declaration --------------------------------------------------

// namespace xms {
using namespace xms;

namespace
{
//------------------------------------------------------------------------------
/// \brief For testing, converts an array to a vector of Pt3d.
//------------------------------------------------------------------------------
static VecPt3d iArrayToVecPt3d(double* a_array, int a_size)
{
  VecPt3d v(a_size / 2);
  for (int i = 0; i < a_size; i += 2)
  {
    v[i / 2].x = a_array[i];
    v[i / 2].y = a_array[i + 1];
  }
  return v;
} // iArrayToVecPt3d

} // namespace unnamed

//------------------------------------------------------------------------------
/// \brief Builds a simple TIN with a hole in the middle.
/// \verbatim
///
///  15.0        9
///             /|\
///           /  |  \
///         /  8 | 9  \
///  10.0  6-----7-----8
///        |\  5 |\  6 |\
///        |  \  |  \  |  \
///        | 4  \|    \| 7  \
///   5.0  2-----3-----4-----5
///         \  0 |\  2 | 3  /
///           \  |  \  |  /
///             \| 1  \|/
///   0.0        0-----1
///
///      0.0   5.0   10.0  15.0
///
/// \endverbatim
/// \return The tin.
//------------------------------------------------------------------------------
BSHP<TrTin> trBuildTestTin()
{
  BSHP<TrTin> tin = TrTin::New();

  tin->Points() = {{5, 0, 0},  {10, 0, 0}, {0, 5, 0},  {5, 5, 0},   {10, 5, 0},
                   {15, 5, 0}, {0, 10, 0}, {5, 10, 0}, {10, 10, 0}, {5, 15, 0}};
  tin->Triangles() = {0, 3, 2, 0, 1, 3, 1, 4, 3, 1, 5, 4, 2, 3, 6,
                      3, 7, 6, 4, 8, 7, 4, 5, 8, 6, 7, 9, 7, 8, 9};
  tin->BuildTrisAdjToPts();
  return tin;
} // trBuildTestTin
//------------------------------------------------------------------------------
/// \brief Builds a simple TIN for testing.
/// \verbatim
///
///  10.0 6
///       |\
///       |  \
///       |    \
///       |      \
///       |   5    \
///       |          \
///   5.0 4-----------5
///       |\    4    /|\
///       |  \     /  |  \
///       |    \ /    |    \
///       |  2  3  3  |      \
///       |    / \    |        \
///       |  /     \  |    1     \
///       |/    0    \|            \
///   0.0 0-----------1-------------2
///      0.0   2.5   5.0          10.0
///
/// \endverbatim
/// \return The tin.
//------------------------------------------------------------------------------
BSHP<TrTin> trBuildTestTin2()
{
  BSHP<TrTin> tin = TrTin::New();

  tin->Points() = {{0, 0, 0}, {5, 0, 0}, {10, 0, 0}, {2.5, 2.5, 0},
                   {0, 5, 0}, {5, 5, 0}, {0, 10, 0}};
  int tris[] = {0, 1, 3, 1, 2, 5, 0, 3, 4, 1, 5, 3, 4, 3, 5, 4, 5, 6};
  tin->Triangles().assign(&tris[0], &tris[XM_COUNTOF(tris)]);
  tin->BuildTrisAdjToPts();
  return tin;
} // trBuildTestTin2
//------------------------------------------------------------------------------
/// \brief Builds a simple TIN for testing.
/// \verbatim
///
///  40   36-37--38--39--40--41--42--43--44
///       |\16|\28|\24|\54|51/|52/|\61|\59|
///       |17\|18\|25\|26\|/47|/50|53\|60\|
///  30   27-28--29--30--31--32--33--34--35
///       |\14|\23|\22|\63|48/|49/|\58|\57|
///       |15\|19\|20\|27\|/35|/46|62\|55\|
///  20   18-19--20--21--22--23--24--25--26
///       |\ 3|29/|21/|\30|\32|\45|\41|56/|
///       |1 \|/ 7|/10|13\|33\|34\|42\|/44|
///  10   9--10--11--12--13--14--15--16--17
///       |2 /|\12|8 /|9 /|\37|\36|\38|43/|
///       |/ 0|4 \|/ 5|/ 6|11\|31\|39\|/40|
///   0   0---1---2---3---4---5---6---7---8
///       0  10  20  30  40  50  60  70  80
///
/// (most triangles not shown)
/// \endverbatim
/// \return The tin.
//------------------------------------------------------------------------------
BSHP<TrTin> trBuildTestTin6()
{
  BSHP<TrTin> tin = TrTin::New();
  const double kYMax = 40;
  const double kXMax = 80;
  for (int y = 0; y <= kYMax; y += 10)
  {
    for (int x = 0; x <= kXMax; x += 10)
    {
      tin->Points().push_back(Pt3d(x, y, 0.0));
    }
  }

  TrTriangulatorPoints triangulator(tin->Points(), tin->Triangles(), &tin->TrisAdjToPts());
  triangulator.Triangulate();
  return tin;
} // trBuildTestTin6
//------------------------------------------------------------------------------
/// \brief Builds a simple TIN for testing.
/// \verbatim
///
//  10-  17-----18------19------20------21
//    |  |\  8  /|\ 11  /|\ 22  / \ 26  /|
//    |  | \   / | \   / | \   /   \   //|
//    |  |  \ /  |  \ /30|23\ / 27  \ / /|
//   5-  |6 13 12|9 14   |  15------16  /|
//    |  |  / \  |  /|\  |  / \ 25  /|28/|
//    |  | /   \ | / | \ | /   \   / | / |
//    |  |/  7  \|/  |  \|/ 24  \ /29| / |
//   0-  9------10 10|3 11------12   | / |
//    |  |\ 13  / \  |  / \ 15  / \  |/  |
//    |  | \   /   \ | /   \   /   \ |/  |
//    |  |  \ /  5  \|/ 16  \ / 21  \|/  |
//  -5-  | 0 5-------6-------7-------8 18|
//    |  |  / \  4  / \ 14  / \ 19  / \  |
//    |  | /   \   /   \   /   \   /   \ |
//    |  |/  1  \ /  2  \ / 20  \ / 17  \|
// -10-  0-------1-------2-------3-------4
//
//       |-------|-------|-------|-------|
//       0      10      20      30      40
/// \endverbatim
/// \return The tin.
//------------------------------------------------------------------------------
BSHP<TrTin> trBuildTestTin7()
{
  double meshPtsA[] = {0,  -10, 10, -10, 20, -10, 30, -10, 40, -10, 5,  -5, 15, -5, 25,
                       -5, 35,  -5, 0,   0,  10,  0,  20,  0,  30,  0,  5,  5,  15, 5,
                       25, 5,   35, 5,   0,  10,  10, 10,  20, 10,  30, 10, 40, 10};
  BSHP<TrTin> tin = TrTin::New();
  tin->Points() = iArrayToVecPt3d(meshPtsA, XM_COUNTOF(meshPtsA));
  TrTriangulatorPoints triangulator(tin->Points(), tin->Triangles(), &tin->TrisAdjToPts());
  triangulator.Triangulate();
  return tin;
} // trBuildTestTin7
//------------------------------------------------------------------------------
/// \brief Builds a simple TIN with a hole in the middle and a concavity.
/// \verbatim
///
///  15.0        9----10----11
///             /|\ 10 | 11 /
///           /  |  \  |  /
///         /  8 | 9  \|/
///  10.0  6-----7-----8
///        |\  5 |\  6 |\
///        |  \  |  \  |  \
///        | 4  \|    \| 7  \
///   5.0  2-----3-----4-----5
///         \  0 |\  2 | 3  /
///           \  |  \  |  /
///             \| 1  \|/
///   0.0        0-----1
///
///      0.0   5.0   10.0  15.0
///
/// \endverbatim
/// \return The tin.
//------------------------------------------------------------------------------
BSHP<TrTin> trBuildTestTin8()
{
  BSHP<TrTin> tin = TrTin::New();

  tin->Points() = {{5, 0, 0},  {10, 0, 0}, {0, 5, 0},   {5, 5, 0},  {10, 5, 0},  {15, 5, 0},
                   {0, 10, 0}, {5, 10, 0}, {10, 10, 0}, {5, 15, 0}, {10, 15, 0}, {15, 15, 0}};
  tin->Triangles() = {0, 3, 2, 0, 1, 3, 1, 4, 3, 1, 5, 4, 2, 3,  6, 3, 7,  6,
                      4, 8, 7, 4, 5, 8, 6, 7, 9, 7, 8, 9, 8, 10, 9, 8, 11, 10};
  tin->BuildTrisAdjToPts();
  return tin;
} // trBuildTestTin8
//------------------------------------------------------------------------------
/// \brief Builds a TIN with long thin triangles for testing.
/// \verbatim
///
///  10.0 7
///       |\\
///       |  \ \
///       |    \  \
///       |      \  \
///       |   6    \ 7 \
///       |          \   \
///   5.0 4-----------5---6
///       |\    4    /|\   \
///       |  \     /  |  \ 5 \
///       |    \ /    |    \  \
///       |  2  3  3  |      \ \
///       |    / \    |        \ \
///       |  /     \  |    1     \\
///       |/    0    \|            \
///   0.0 0-----------1-------------2
///      0.0   2.5   5.0          10.0
///
/// \endverbatim
/// \return The tin.
//------------------------------------------------------------------------------
BSHP<TrTin> trBuildTestTin9()
{
  BSHP<TrTin> tin = TrTin::New();

  tin->Points() = {{0, 0, 0}, {5, 0, 0}, {10, 0, 0}, {2.5, 2.5, 0},
                   {0, 5, 0}, {5, 5, 0}, {6, 5, 0}, {0, 10, 0}};
  int tris[] = {0, 1, 3, 1, 2, 5, 0, 3, 4, 1, 5, 3, 4, 3, 5, 5, 2, 6, 4, 5, 7, 5, 6, 7};
  tin->Triangles().assign(&tris[0], &tris[XM_COUNTOF(tris)]);
  tin->BuildTrisAdjToPts();
  return tin;
} // trBuildTestTin9

////////////////////////////////////////////////////////////////////////////////
/// \class TrTinUnitTests
/// \brief Tests for TrTin
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/// \brief Tests a bunch of the TrTin methods.
/// \verbatim

// 10-        3-----------4
//   |       /0\2       1/2\
//   |      /   \   2   /   \
//   |     /     \     /     \
//   |    /   0   \   /   1   \
//   |   /1       2\0/0       1\
//  0-  0-----------1-----------2
//
//      |-----|-----|-----|-----|
//      0     5    10    15    20
//
// m_tris          =  3,0,1,  1,2,4,  1,4,3
// m_trisAdjToPts  = [0][0,1,2][1][0,2][1,2]

/// \endverbatim
//------------------------------------------------------------------------------
void TrTinUnitTests::test1()
{
  // Create memory
  BSHP<VecPt3d> pts(new VecPt3d());
  BSHP<VecInt> tris(new VecInt());
  BSHP<VecInt2d> trisAdjToPts(new VecInt2d());

  // Set up to our example tin
  *pts = {{0, 0, 0}, {10, 0, 0}, {20, 0, 0}, {5, 10, 0}, {15, 10, 0}};
  *tris = {3, 0, 1, 1, 2, 4, 1, 4, 3};
  *trisAdjToPts = {{0}, {0, 1, 2}, {1}, {0, 2}, {1, 2}};

  BSHP<TrTin> tin = TrTin::New();
  tin->SetGeometry(pts, tris, trisAdjToPts);

  // TriangleFromEdge

  int tri;
  int idx1, idx2;
  bool rv;
  rv = tin->TriangleFromEdge(1, 3, tri, idx1, idx2);
  TS_ASSERT_EQUALS(rv, true);
  TS_ASSERT_EQUALS(tri, 0);
  TS_ASSERT_EQUALS(idx1, 2);
  TS_ASSERT_EQUALS(idx2, 0);
  rv = tin->TriangleFromEdge(3, 1, tri, idx1, idx2);
  TS_ASSERT_EQUALS(rv, true);
  TS_ASSERT_EQUALS(tri, 2);
  TS_ASSERT_EQUALS(idx1, 2);
  TS_ASSERT_EQUALS(idx2, 0);
  rv = tin->TriangleFromEdge(1, 0, tri, idx1, idx2);
  TS_ASSERT_EQUALS(rv, false);

  // TriangleAdjacentToEdge

  tri = tin->TriangleAdjacentToEdge(1, 3);
  TS_ASSERT_EQUALS(tri, 2);
  tri = tin->TriangleAdjacentToEdge(3, 1);
  TS_ASSERT_EQUALS(tri, 0);
  tri = tin->TriangleAdjacentToEdge(1, 0);
  TS_ASSERT_EQUALS(tri, 0);

  // LocalIndex

  TS_ASSERT_EQUALS(tin->LocalIndex(0, 0), 1);
  TS_ASSERT_EQUALS(tin->LocalIndex(0, 1), 2);
  TS_ASSERT_EQUALS(tin->LocalIndex(0, 3), 0);
  TS_ASSERT_EQUALS(tin->LocalIndex(0, 4), XM_NONE);

  // GlobalIndex

  TS_ASSERT_EQUALS(tin->GlobalIndex(0, 0), 3);
  TS_ASSERT_EQUALS(tin->GlobalIndex(0, 1), 0);
  TS_ASSERT_EQUALS(tin->GlobalIndex(0, 2), 1);
  TS_ASSERT_EQUALS(tin->GlobalIndex(1, 0), 1);
  TS_ASSERT_EQUALS(tin->GlobalIndex(1, 1), 2);
  TS_ASSERT_EQUALS(tin->GlobalIndex(1, 2), 4);
  TS_ASSERT_EQUALS(tin->GlobalIndex(2, 0), 1);
  TS_ASSERT_EQUALS(tin->GlobalIndex(2, 1), 4);
  TS_ASSERT_EQUALS(tin->GlobalIndex(2, 2), 3);
  bool asserting = xmAsserting();
  xmAsserting() = false;
  TS_ASSERT_EQUALS(tin->GlobalIndex(3, 2), XM_NONE);
  TS_ASSERT_EQUALS(tin->GlobalIndex(2, 3), XM_NONE);
  xmAsserting() = asserting;

  // VerticesAreAdjacent

  TS_ASSERT_EQUALS(tin->VerticesAreAdjacent(0, 1), true);
  TS_ASSERT_EQUALS(tin->VerticesAreAdjacent(1, 0), true);
  TS_ASSERT_EQUALS(tin->VerticesAreAdjacent(0, 2), false);

  // CommonEdgeIndex

  TS_ASSERT_EQUALS(tin->CommonEdgeIndex(0, 2), 2);
  TS_ASSERT_EQUALS(tin->CommonEdgeIndex(2, 0), 2);
  TS_ASSERT_EQUALS(tin->CommonEdgeIndex(1, 2), 2);
  TS_ASSERT_EQUALS(tin->CommonEdgeIndex(2, 1), 0);
  TS_ASSERT_EQUALS(tin->CommonEdgeIndex(0, 1), XM_NONE);

  // AdjacentTriangle

  TS_ASSERT_EQUALS(tin->AdjacentTriangle(0, 0), XM_NONE);
  TS_ASSERT_EQUALS(tin->AdjacentTriangle(0, 1), XM_NONE);
  TS_ASSERT_EQUALS(tin->AdjacentTriangle(0, 2), 2);
  TS_ASSERT_EQUALS(tin->AdjacentTriangle(1, 0), XM_NONE);
  TS_ASSERT_EQUALS(tin->AdjacentTriangle(1, 1), XM_NONE);
  TS_ASSERT_EQUALS(tin->AdjacentTriangle(1, 2), 2);
  TS_ASSERT_EQUALS(tin->AdjacentTriangle(2, 0), 1);
  TS_ASSERT_EQUALS(tin->AdjacentTriangle(2, 1), XM_NONE);
  TS_ASSERT_EQUALS(tin->AdjacentTriangle(2, 2), 0);

  // TriangleCentroid

  const double kDelta = 1e-5;
  TS_ASSERT_DELTA_PT3D(tin->TriangleCentroid(0), Pt3d(5, 3.333333333, 0), kDelta);
  TS_ASSERT_DELTA_PT3D(tin->TriangleCentroid(1), Pt3d(15, 3.333333333, 0), kDelta);
  TS_ASSERT_DELTA_PT3D(tin->TriangleCentroid(2), Pt3d(10, 6.66666667, 0), kDelta);

  // TriangleArea

  TS_ASSERT_DELTA(tin->TriangleArea(0), 50.0, kDelta);
  TS_ASSERT_DELTA(tin->TriangleArea(1), 50.0, kDelta);
  TS_ASSERT_DELTA(tin->TriangleArea(2), 50.0, kDelta);

  // NextBoundaryPoint

  TS_ASSERT_EQUALS(tin->NextBoundaryPoint(0), 3);
  TS_ASSERT_EQUALS(tin->NextBoundaryPoint(3), 4);
  TS_ASSERT_EQUALS(tin->NextBoundaryPoint(4), 2);
  TS_ASSERT_EQUALS(tin->NextBoundaryPoint(2), 1);
  TS_ASSERT_EQUALS(tin->NextBoundaryPoint(1), 0);

  // PreviousBoundaryPoint

  TS_ASSERT_EQUALS(tin->PreviousBoundaryPoint(0), 1);
  TS_ASSERT_EQUALS(tin->PreviousBoundaryPoint(1), 2);
  TS_ASSERT_EQUALS(tin->PreviousBoundaryPoint(2), 4);
  TS_ASSERT_EQUALS(tin->PreviousBoundaryPoint(4), 3);
  TS_ASSERT_EQUALS(tin->PreviousBoundaryPoint(3), 0);

  // GetBoundaryPoints is tested elsewhere
  // GetBoundaryPolys is tested elsewhere

  // GetExtents

  Pt3d mn, mx;
  TS_ASSERT_EQUALS(tin->GetExtents(mn, mx), true);
  TS_ASSERT_DELTA_PT3D(mn, Pt3d(0, 0, 0), kDelta);
  TS_ASSERT_DELTA_PT3D(mx, Pt3d(20, 10, 0), kDelta);

  // Below here we've modified the tin -----------------------------------------

  // SwapEdge

  rv = tin->SwapEdge(0, 2);
  TS_ASSERT_EQUALS(rv, true);
  TS_ASSERT_EQUALS(tin->Triangles()[0], 3);
  TS_ASSERT_EQUALS(tin->Triangles()[1], 0);
  TS_ASSERT_EQUALS(tin->Triangles()[2], 4);
  TS_ASSERT_EQUALS(tin->Triangles()[3], 1);
  TS_ASSERT_EQUALS(tin->Triangles()[4], 2);
  TS_ASSERT_EQUALS(tin->Triangles()[5], 4);
  TS_ASSERT_EQUALS(tin->Triangles()[6], 1);
  TS_ASSERT_EQUALS(tin->Triangles()[7], 4);
  TS_ASSERT_EQUALS(tin->Triangles()[8], 0);
  const VecInt2d& trisAdjToPtsRef = tin->TrisAdjToPts();
  TS_ASSERT_EQUALS((VecInt{0, 2}), trisAdjToPtsRef[0]);
  TS_ASSERT_EQUALS((VecInt{1, 2}), trisAdjToPtsRef[1]);
  TS_ASSERT_EQUALS((VecInt{1}), trisAdjToPtsRef[2]);
  TS_ASSERT_EQUALS((VecInt{0}), trisAdjToPtsRef[3]);
  TS_ASSERT_EQUALS((VecInt{1, 2, 0}), trisAdjToPtsRef[4]);

  // DeleteTriangles is tested elsewhere
  // DeletePoints is tested elsewhere
  // OptimizeTriangulation is tested elsewhere
  // Clear is tested below

  // BuildTrisAdjToPts

  tin->TrisAdjToPts().clear();
  tin->BuildTrisAdjToPts();
  TS_ASSERT_EQUALS(*trisAdjToPts, tin->TrisAdjToPts());

  // Clear

  tin->Clear();
  TS_ASSERT_EQUALS(tin->Points().empty(), true);
  TS_ASSERT_EQUALS(tin->Triangles().empty(), true);
  TS_ASSERT_EQUALS(tin->TrisAdjToPts().empty(), true);

} // TrTinUnitTests::test1
//------------------------------------------------------------------------------
/// \brief
/// \verbatim
// Before
//
//  20-   6------7------8
//    |   |\     |\     |
//    |   |\\ 3  |\\ 7  |
//    |   |\ \   |\ \   |
//    |   | \ \  | \ \  |
//    |   | \  \ | \  \ |
//    |   |  \  \|  \  \|
//  10-   3 1 \2 4 5 \6 5
//    |   |\  \  |\  \  |
//    |   | \  \ | \  \ |
//    |   |  \ \ |  \ \ |
//    |   |   \ \|   \ \|
//    |   | 0  \\| 4  \\|
//    |   |     \|     \|0
//  0 -   0------1------2
//
//        |------|------|
//        0     10     20

// After
//  20-   6------7------8
//    |   |\     |\     |
//    |   | \  3 | \  7 |
//    |   |  \   |  \   |
//    |   |   \  |   \  |
//    |   | 2  \ | 6  \ |
//    |   |     \|     \|
//  10-   3------4------5
//    |   |\     |\     |
//    |   | \  1 | \  5 |
//    |   |  \   |  \   |
//    |   |   \  |   \  |
//    |   | 0  \ | 4  \ |
//    |   |     \|     \|
//  0 -   0------1------2
//
//        |------|------|
//        0     10     20
/// \endverbatim
//------------------------------------------------------------------------------
void TrTinUnitTests::testOptimizeTriangulation()
{
  // Create memory
  BSHP<VecPt3d> pts(new VecPt3d());
  BSHP<VecInt> tris(new VecInt());
  BSHP<VecInt2d> trisAdjToPts(new VecInt2d());

  // Set up to our example tin
  *pts = {{0, 0, 0},   {10, 0, 0}, {20, 0, 0},  {0, 10, 0}, {10, 10, 0},
          {20, 10, 0}, {0, 20, 0}, {10, 20, 0}, {20, 20, 0}};
  *tris = {0, 1, 3, 1, 6, 3, 1, 4, 6, 4, 7, 6, 1, 2, 4, 2, 7, 4, 2, 5, 7, 5, 8, 7};
  *trisAdjToPts = {{0},    {0, 1, 2, 4}, {4, 5, 6},    {0, 1}, {2, 4, 5, 3},
                   {6, 7}, {1, 2, 3},    {3, 5, 6, 7}, {7}};

  BSHP<TrTin> tin = TrTin::New();
  tin->SetGeometry(pts, tris, trisAdjToPts);

  // Test GetBoundaryPoints
  VecInt boundaryPoints;
  tin->GetBoundaryPoints(boundaryPoints);
  TS_ASSERT_EQUALS((VecInt{0, 1, 2, 3, 5, 6, 7, 8}), boundaryPoints);

  // Optimize
  TS_ASSERT(tin->OptimizeTriangulation());
  VecInt trisAfter = {0, 1, 3, 1, 6, 3, 1, 4, 6, 4, 7, 6, 1, 2, 4, 2, 7, 4, 2, 5, 7, 5, 8, 7};
  TS_ASSERT_EQUALS_VEC(trisAfter, tin->Triangles());

  // Test GetBoundaryPoints
  tin->GetBoundaryPoints(boundaryPoints);
  TS_ASSERT_EQUALS((VecInt{0, 1, 2, 3, 5, 6, 7, 8}), boundaryPoints);

} // TrTinUnitTests::testOptimizeTriangulation
//------------------------------------------------------------------------------
/// \brief
/// \verbatim
// 20-  5-----------6-----------7   20-  5-----------6-----------7
//   |  |\         / \         /|     |  |\         /|\         /|
//   |  | \   1   /   \   6   / |     |  | \   1   / | \   6   / |
//   |  |  \     /     \     /  |     |  |  \     /  |  \     /  |
//   |  |   \   /   7   \   /   |     |  |   \   /   |   \   /   |
//   |  |    \ /         \ /    |     |  |    \ /    |    \ /    |
// 10-  |  2  3-----------4   4 |   10-  |  2  3  3  |  7  4   4 |
//   |  |    / \         / \    |     |  |    / \    |    / \    |
//   |  |   /   \   3   /   \   |     |  |   /   \   |   /   \   |
//   |  |  /     \     /     \  |     |  |  /     \  |  /     \  |
//   |  | /   0   \   /   5   \ |     |  | /   0   \ | /   5   \ |
//   |  |/         \ /         \|     |  |/         \|/         \|
//  0-  0-----------1-----------2    0-  0-----------1-----------2
//
//      |-----|-----|-----|-----|        |-----|-----|-----|-----|
//      0     5    10    15    20        0     5    10    15    20
/// \endverbatim
//------------------------------------------------------------------------------
void TrTinUnitTests::testSwap()
{
  // Create tin and get some convenience variables
  BSHP<TrTin> tin = TrTin::New();
  VecInt& tris = tin->Triangles();
  VecInt2d& trisAdjToPts = tin->TrisAdjToPts();

  // Set up to our example tin points
  tin->Points() = {{0, 0, 0},   {10, 0, 0}, {20, 0, 0},  {5, 10, 0},
                   {15, 10, 0}, {0, 20, 0}, {10, 20, 0}, {20, 20, 0}};

  // Triangulate the points
  TrTriangulatorPoints client(tin->Points(), tin->Triangles(), &trisAdjToPts);
  client.Triangulate();

  // See that things are as expected before the swap
  int trisB[] = {0, 1, 3, 5, 3, 6, 3, 5, 0, 1, 4, 3, 4, 2, 7, 2, 4, 1, 7, 6, 4, 4, 6, 3};
  VecInt trisBefore(&trisB[0], &trisB[24]);
  TS_ASSERT_EQUALS(trisBefore, tris);

  VecInt2d trisAdjToPtsBefore = {{0, 2},          {0, 3, 5}, {4, 5},    {0, 1, 2, 3, 7},
                                 {3, 4, 5, 6, 7}, {1, 2},    {1, 6, 7}, {4, 6}};

  TS_ASSERT_EQUALS(trisAdjToPtsBefore, trisAdjToPts);

  // Swap
  bool rv = tin->SwapEdge(3, 7);
  TS_ASSERT_EQUALS(rv, true);

  // See that things are as expected after the swap
  int trisA[] = {0, 1, 3, 5, 3, 6, 3, 5, 0, 1, 6, 3, 4, 2, 7, 2, 4, 1, 7, 6, 4, 4, 6, 1};
  VecInt trisAfter(&trisA[0], &trisA[24]);
  TS_ASSERT_EQUALS(trisAfter, tris);

  VecInt2d trisAdjToPtsAfter = {{0, 2},       {0, 3, 5, 7}, {4, 5},       {0, 1, 2, 3},
                                {4, 5, 6, 7}, {1, 2},       {1, 6, 7, 3}, {4, 6}};
  TS_ASSERT_EQUALS(trisAdjToPtsAfter, trisAdjToPts);

  // Test GetBoundaryPoints
  VecInt boundaryPoints;
  tin->GetBoundaryPoints(boundaryPoints);
  TS_ASSERT_EQUALS((VecInt{0, 1, 2, 5, 6, 7}), boundaryPoints);

} // TrTinUnitTests::testSwap
#if 0 // FirstBoundaryPoint has been commented out because it wasn't used
//------------------------------------------------------------------------------
/// \brief Tests TrTin::FirstBoundaryPoint
//------------------------------------------------------------------------------
void TrTinTests::testFirstBoundaryPoint ()
{
  

  // Example 1
  {
    BSHP<TrTin> tin = trBuildTestTin();
    TS_ASSERT_EQUALS(tin->FirstBoundaryPoint(), 0);
  }

  // Example 2
// 20-  5-----------6-----------7
//   |  |\         / \         /|
//   |  | \   1   /   \   6   / |
//   |  |  \     /     \     /  |
//   |  |   \   /   7   \   /   |
//   |  |    \ /         \ /    |
// 10-  |  2  0-----------1   4 |
//   |  |    / \         / \    |
//   |  |   /   \   3   /   \   |
//   |  |  /     \     /     \  |
//   |  | /   0   \   /   5   \ |
//   |  |/         \ /         \|
//  0-  2-----------3-----------4
//
//      |-----|-----|-----|-----|
//      0     5    10    15    20

  {
    BSHP<TrTin> tin = TrTin::New();
    tin->Points() = {{5,10,0},{15,10,0},{0,0,0},{10,0,0},
                     {20,0,0},{0,20,0},{10,20,0},{20,20,0}};
    TrTriangulatorPoints client(tin->Points(), tin->Triangles(),
                                &tin->TrisAdjToPts());
    client.Triangulate();
    TS_ASSERT_EQUALS(tin->FirstBoundaryPoint(), 2);
  }

} // TrTinTests::testFirstBoundaryPoint
#endif
//------------------------------------------------------------------------------
/// \brief Tests TrTin::GetBoundaryPoints and TrTin::GetBoundaryPolys.
/// \verbatim

//  40   34-35--36--37--38--39--40--41--42
//       |\15|\23|\19|\44|41/|42/|\51|\49|
//       |16\|17\|20\|21\|/37|/40|43\|50\|
//  30   25-26--27--28--29--30--31--32--33
//       |\13|    \18|\52|38/|39/ \48|\47|
//       |14\|      \|22\|/29|/     \|45\|
//  20   18-19      20--21--22      23--24
//       |\ 3|      /|\24|\26|\      |46/|
//       |1 \|    / 9|12\|27\|28\    |/36|
//  10   9--10--11--12--13--14--15--16--17
//       |2 /|\11|7 /|8 /|\31|\30|\32|35/|
//       |/ 0|4 \|/ 5|/ 6|10\|25\|33\|/34|
//   0   0---1---2---3---4---5---6---7---8
//       0  10  20  30  40  50  60  70  80

/// \endverbatim
//------------------------------------------------------------------------------
void TrTinUnitTests::testBoundaries()
{
  BSHP<TrTin> tin = trBuildTestTin6();
  SetInt toDelete = {20, 24};
  tin->DeletePoints(toDelete);
  VecInt2d boundaries;

  // GetBoundaryPoints

  VecInt pts;
  tin->GetBoundaryPoints(pts);
  int ptsA[] = {0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 15, 16, 17, 18, 19, 20,
                22, 23, 24, 25, 26, 27, 31, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42};
  VecInt expectedPts(&ptsA[0], &ptsA[XM_COUNTOF(ptsA)]);
  TS_ASSERT_EQUALS(expectedPts, pts);

  // GetBoundaryPolys

  tin->GetBoundaryPolys(boundaries);

  VecInt2d expected = {
    {0, 9, 18, 25, 34, 35, 36, 37, 38, 39, 40, 41, 42, 33, 24, 17, 8, 7, 6, 5, 4, 3, 2, 1, 0},
    {10, 11, 20, 27, 26, 19, 10},
    {15, 16, 23, 31, 22, 15}};
  TS_ASSERT_EQUALS_VEC2D(expected, boundaries);
} // TrTinUnitTests::testBoundaries
//------------------------------------------------------------------------------
/// \brief Tests TrTin::DeleteTriangles
//------------------------------------------------------------------------------
void TrTinUnitTests::testDeleteTriangles()
{
  BSHP<TrTin> tin = trBuildTestTin6();
  TS_ASSERT_EQUALS(tin->NumTriangles(), 64);
  TS_ASSERT_EQUALS(tin->NumPoints(), 45);

  const VecInt2d& trisAdjToPts = tin->TrisAdjToPts();

  // Verify adjacency
  TS_ASSERT_EQUALS((VecInt{7, 19, 20, 21, 23, 29}), trisAdjToPts[20]);
  TS_ASSERT_EQUALS((VecInt{0, 1, 2, 3, 4, 7, 12, 29}), trisAdjToPts[10]);
  TS_ASSERT_EQUALS((VecInt{7, 8, 10, 12, 21}), trisAdjToPts[11]);
  TS_ASSERT_EQUALS((VecInt{10, 13, 20, 21, 22, 27, 30}), trisAdjToPts[21]);
  TS_ASSERT_EQUALS((VecInt{18, 20, 22, 23, 25, 28}), trisAdjToPts[29]);
  TS_ASSERT_EQUALS((VecInt{14, 16, 17, 18, 19, 23}), trisAdjToPts[28]);
  TS_ASSERT_EQUALS((VecInt{3, 14, 15, 19, 29}), trisAdjToPts[19]);

  // Delete the triangles around point 20
  SetInt toDelete = {7, 19, 20, 21, 23, 29};
  tin->DeleteTriangles(toDelete);

  TS_ASSERT_EQUALS(tin->NumTriangles(), 58); // 64 - 6
  TS_ASSERT_EQUALS(tin->NumPoints(), 45);

  // Make sure we renumbered the adjacency array properly
  int mx = -1;
  for (size_t i = 0; i < trisAdjToPts.size(); ++i)
  {
    for (size_t j = 0; j < trisAdjToPts[i].size(); ++j)
    {
      if (trisAdjToPts[i][j] > mx)
        mx = trisAdjToPts[i][j];
    }
  }
  TS_ASSERT_EQUALS(mx, 57); // 58 - 1

  ///  40   36-37--38--39--40--41--42--43--44
  ///       |\15|\23|\19|\48|45/|46/|\55|\53|
  ///       |16\|17\|20\|21\|/41|/44|47\|54\|
  ///  30   27-28--29--30--31--32--33--34--35
  ///       |\13|    \18|\57|42/|43/|\52|\51|
  ///       |14\|      \|22\|/29|/40|56\|49\|
  ///  20   18-19  20  21--22--23--24--25--26
  ///       |\ 3|      /|\24|\26|\39|\35|50/|
  ///       |1 \|    / 9|12\|27\|28\|36\|/38|
  ///  10   9--10--11--12--13--14--15--16--17
  ///       |2 /|\11|7 /|8 /|\31|\30|\32|37/|
  ///       |/ 0|4 \|/ 5|/ 6|10\|25\|33\|/34|
  ///   0   0---1---2---3---4---5---6---7---8
  ///       0  10  20  30  40  50  60  70  80
  // Verify adjacency
  TS_ASSERT(tin->TrisAdjToPts()[20].empty());
  TS_ASSERT_EQUALS((VecInt{0, 1, 2, 3, 4, 11}), trisAdjToPts[10]);
  TS_ASSERT_EQUALS((VecInt{7, 9, 11}), trisAdjToPts[11]);
  TS_ASSERT_EQUALS((VecInt{9, 12, 18, 22, 24}), trisAdjToPts[21]);
  TS_ASSERT_EQUALS((VecInt{17, 18, 20, 23}), trisAdjToPts[29]);
  TS_ASSERT_EQUALS((VecInt{13, 15, 16, 17}), trisAdjToPts[28]);
  TS_ASSERT_EQUALS((VecInt{3, 13, 14}), trisAdjToPts[19]);
} // TrTinUnitTests::testDeleteTriangles
//------------------------------------------------------------------------------
/// \brief Tests TrTin::DeletePoints
//------------------------------------------------------------------------------
void TrTinUnitTests::testDeletePoints()
{
  BSHP<TrTin> tin = trBuildTestTin6();
  TS_ASSERT_EQUALS(tin->NumTriangles(), 64);
  TS_ASSERT_EQUALS(tin->NumPoints(), 45);

  SetInt toDelete = {20, 24};
  tin->DeletePoints(toDelete);

  TS_ASSERT_EQUALS(tin->NumTriangles(), 53);
  TS_ASSERT_EQUALS(tin->NumPoints(), 43);

  ///  40   34-35--36--37--38--39--40--41--42
  ///       |\15|\23|\19|\44|41/|42/|\51|\49|
  ///       |16\|17\|20\|21\|/37|/40|43\|50\|
  ///  30   25-26--27--28--29--30--31--32--33
  ///       |\13|    \18|\52|38/|39/ \48|\47|
  ///       |14\|      \|22\|/29|/     \|45\|
  ///  20   18-19      20--21--22      23--24
  ///       |\ 3|      /|\24|\26|\      |46/|
  ///       |1 \|    / 9|12\|27\|28\    |/36|
  ///  10   9--10--11--12--13--14--15--16--17
  ///       |2 /|\11|7 /|8 /|\31|\30|\32|35/|
  ///       |/ 0|4 \|/ 5|/ 6|10\|25\|33\|/34|
  ///   0   0---1---2---3---4---5---6---7---8
  ///       0  10  20  30  40  50  60  70  80

  // Verify adjacency
  const VecInt2d& trisAdjToPts = tin->TrisAdjToPts();

  TS_ASSERT_EQUALS((VecInt{0, 1, 2, 3, 4, 11}), trisAdjToPts[10]);
  TS_ASSERT_EQUALS((VecInt{7, 9, 11}), trisAdjToPts[11]);
  TS_ASSERT_EQUALS((VecInt{9, 12, 18, 22, 24}), trisAdjToPts[20]);
  TS_ASSERT_EQUALS((VecInt{17, 18, 20, 23}), trisAdjToPts[27]);
  TS_ASSERT_EQUALS((VecInt{13, 15, 16, 17}), trisAdjToPts[26]);
  TS_ASSERT_EQUALS((VecInt{3, 13, 14}), trisAdjToPts[19]);

  TS_ASSERT_EQUALS((VecInt{28, 30, 32, 33}), trisAdjToPts[15]);
  TS_ASSERT_EQUALS((VecInt{32, 35, 36, 46}), trisAdjToPts[16]);
  TS_ASSERT_EQUALS((VecInt{45, 46, 48}), trisAdjToPts[23]);
  TS_ASSERT_EQUALS((VecInt{39, 40, 43, 48}), trisAdjToPts[31]);
  TS_ASSERT_EQUALS((VecInt{26, 28, 29, 39}), trisAdjToPts[22]);
} // TrTinUnitTests::testDeletePoints
//------------------------------------------------------------------------------
/// \brief Tests TrTin::RemoveLongThinTrianglesOnPerimeter
//------------------------------------------------------------------------------
void TrTinUnitTests::testRemoveLongThinTrianglesOnPerimeter()
{
  BSHP<TrTin> tin = trBuildTestTin9();
  TS_ASSERT_EQUALS(tin->NumTriangles(), 8);
  TS_ASSERT_EQUALS(tin->NumPoints(), 8);

  tin->RemoveLongThinTrianglesOnPerimeter(0.75);

  TS_ASSERT_EQUALS(tin->NumTriangles(), 6);
  TS_ASSERT_EQUALS(tin->NumPoints(), 8);
} // TrTinUnitTests::testRemoveLongThinTrianglesOnPerimeter

  //} // namespace xms

#endif // CXX_TEST
