//------------------------------------------------------------------------------
/// \file
/// \brief Provides functionality to do spatial queries on a set of triangles.
/// Uses the boost RTree for queries.
/// \ingroup geometry
/// \copyright (C) Copyright Aquaveo 2018. Distributed under the xmsng
///  Software License, Version 1.0. (See accompanying file
///  LICENSE_1_0.txt or copy at http://www.aquaveo.com/xmsng/LICENSE_1_0.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------

// 1. Precompiled header

// 2. My header
#include <xmsinterp/geometry/GmTriSearch.h>

// 3. Standard Library Headers

// 4. External Library Headers
#pragma warning(push)
#pragma warning(disable : 4512) // boost code: no assignment operator
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/index/rtree.hpp>
#include <boost/unordered_map.hpp>
#include <boost/iterator/counting_iterator.hpp>
#pragma warning(pop)

// 5. Shared Headers
#include <xmscore/math/math.h>
#include <xmscore/stl/utility.h>
#include <xmsinterp/geometry/GmBoostTypes.h>
#include <xmsinterp/geometry/geoms.h>
#include <xmsinterp/geometry/GmPtSearch.h>
#include <xmscore/misc/xmstype.h>
#include <xmscore/misc/XmError.h>
#include <xmscore/misc/XmConst.h>

// 6. Non-shared Headers

//----- Forward declarations ---------------------------------------------------

//----- External globals -------------------------------------------------------

//----- Namespace declaration --------------------------------------------------
namespace xms
{
namespace bg = boost::geometry;
namespace bgi = bg::index;

typedef size_t value;
typedef bgi::quadratic<16> qRtree;

//----- Constants / Enumerations -----------------------------------------------

//----- Classes / Structs ------------------------------------------------------

class GmTriSearchImpl;

#define BARYTOL 1e-6 ///< This is an odd duck tolerance

/// \brief provides indexing for the spatial index
class idx_tri
{
public:
  typedef const GmBstBox3d result_type; ///< created to follow boost examples
  /// \ brief constructor
  /// \param[in] a_pts Point locations
  /// \param[in] a_tris Triangles referencing point indexes
  /// \param[in] a_tol Tolerance for expanding triangle extents
  explicit idx_tri(const Pt3d* a_pts, const int* a_tris, double a_tol)
  : m_pts(a_pts)
  , m_tris(a_tris)
  , m_tol(a_tol)
  {
  }
  /// \brief calculates triangle extents based on a triangle index
  /// \param i: index.
  /// \return result_type.
  result_type operator()(size_t i) const
  {
    i *= 3;
    const Pt3d& p0(m_pts[m_tris[i]]);
    const Pt3d& p1(m_pts[m_tris[i + 1]]);
    const Pt3d& p2(m_pts[m_tris[i + 2]]);
    GmBstBox3d b;
    b.max_corner() = b.min_corner() = p0;
    if (b.min_corner().x > p1.x)
      b.min_corner().x = p1.x;
    if (b.min_corner().x > p2.x)
      b.min_corner().x = p2.x;
    if (b.max_corner().x < p1.x)
      b.max_corner().x = p1.x;
    if (b.max_corner().x < p2.x)
      b.max_corner().x = p2.x;

    if (b.min_corner().y > p1.y)
      b.min_corner().y = p1.y;
    if (b.min_corner().y > p2.y)
      b.min_corner().y = p2.y;
    if (b.max_corner().y < p1.y)
      b.max_corner().y = p1.y;
    if (b.max_corner().y < p2.y)
      b.max_corner().y = p2.y;
    b.min_corner() -= m_tol;
    b.max_corner() += m_tol;
    return b;
  }
  const Pt3d* m_pts; ///< point locations
  const int* m_tris; ///< triangles referencing point indexes
  double m_tol;      ///< tolerance used to expand extents of triangle
};

/// \brief Structure for Barycentric coordinate calculations
struct BarycentricVals
{
  Pt3d orig;      ///< original point location
  double coef[6]; ///< barycentric coefficients
  int dir;        ///< direction based on the xyz's of the points making the triangle
};

////////////////////////////////////////////////////////////////////////////////
/// \brief Implementation of GmTriSearch
class GmTriSearchImpl : public GmTriSearch
{
public:
  GmTriSearchImpl();
  virtual ~GmTriSearchImpl();
  virtual void TrisToSearch(BSHP<std::vector<Pt3d>> a_pts, BSHP<std::vector<int>> a_tris) override;
  virtual void SetPtActivity(DynBitset& a_activity) override;
  virtual void SetTriActivity(DynBitset& a_activity) override;
  /// \brief Activity of the points based on the triangle activity
  /// \return boost::dynamic_bitset of size_t
  virtual DynBitset GetPtActivity() override { return PointActivityFromTriActivity(); }
  bool ActiveTri(int a_idx);
  virtual int TriContainingPt(const Pt3d& a_pt) override;
  virtual void TriEnvelopsContainingPt(const Pt3d& a_pt, std::vector<int>& a_tris) override;
  virtual void TriEnvelopesOverlap(const Pt3d& a_pMin,
                                   const Pt3d& a_pMax,
                                   std::vector<int>& a_tris) override;

  virtual bool InterpWeights(const Pt3d& a_pt,
                             std::vector<int>& a_idxs,
                             std::vector<double>& a_wts) override;
  virtual bool InterpWeightsTriangleIdx(const Pt3d& a_pt,
                                        int& a_triangleIdx,
                                        std::vector<int>& a_idxs,
                                        std::vector<double>& a_wts) override;

  virtual std::string ToString() const override;

  void CreateRTree();

  void PointIdxesFromTriIdx(int a_triIdx, int a_ptIdxes[3]);
  int FindTriangle(const Pt3d& a_pt, int ix[3], Pt3d& weights);
  void GetTriBarycentricVals(int a_idx, int a_ix[3], BarycentricVals& a_b);

  DynBitset PointActivityFromTriActivity();
  BSHP<GmPtSearch> CreatePtSearch();

  Pt3d m_min; ///< mininum extents of all points
  Pt3d m_max; ///< maximum extents of all points

  BSHP<std::vector<Pt3d>> m_pts; ///< point locations
  BSHP<std::vector<int>> m_tris; ///< triangles that reference point indexes
  DynBitset m_triActivity;       ///< activity of the triangles

  /// we create a cache of these for faster interp the second time we hit the
  /// triangle
  boost::unordered_map<size_t, BarycentricVals> m_cache;
  /// spatial index of the triangle extents
  bgi::rtree<value, qRtree, idx_tri>* m_rTree;
};

//----- Internal functions -----------------------------------------------------

//------------------------------------------------------------------------------
/// \brief Converts from Cartesian coordinates to Barycentric coordinates (local
/// triangle coords).
/// \param a_pt1: Point 1.
/// \param a_pt2: Point 2.
/// \param a_pt3: Point 3.
/// \param a_b: Barycentric values.
//------------------------------------------------------------------------------
void iCartToBary(const Pt3d& a_pt1, const Pt3d& a_pt2, const Pt3d& a_pt3, BarycentricVals& a_b)
{
  Pt3d a(a_pt1.x, a_pt1.y, 0.0);
  Pt3d b(a_pt2.x, a_pt2.y, 0.0);
  Pt3d c(a_pt3.x, a_pt3.y, 0.0);
  Pt3d norm(0, 0, 1);
  gmBaryPrepare(&a, &b, &c, &norm, &a_b.orig, a_b.coef, &a_b.dir, true);
} // iCartToBary
//------------------------------------------------------------------------------
/// \brief Calculates weights at the 3 points of a triangle given the location
/// "a_p" and the Barycentric coords of the triangle.
/// \param a_p: A point
/// \param a_b: Barycentric values.
/// \param weights: Weights.
//------------------------------------------------------------------------------
void iGetBarycentricCoords(const Pt3d& a_p, BarycentricVals& a_b, Pt3d& weights)
{
  Pt3d p(a_p.x, a_p.y, 0.0);
  gmCartToBary(&p, &a_b.orig, a_b.coef, a_b.dir, &weights);
} // iGetBarycentricCoords

//----- Class / Function definitions -------------------------------------------

//------------------------------------------------------------------------------
/// \brief Creates an TriSearch class
/// \return GmTriSearch.
//------------------------------------------------------------------------------
BSHP<GmTriSearch> GmTriSearch::New()
{
  BSHP<GmTriSearch> ptr(new GmTriSearchImpl());
  return ptr;
} // TriSearch::New
//------------------------------------------------------------------------------
/// \brief Constructor
//------------------------------------------------------------------------------
GmTriSearch::GmTriSearch()
{
} // TriSearch::TriSearch
//------------------------------------------------------------------------------
/// \brief Destructor
//------------------------------------------------------------------------------
GmTriSearch::~GmTriSearch()
{
} // TriSearch::~TriSearch

////////////////////////////////////////////////////////////////////////////////
/// \class GmTriSearchImpl
/// \brief Provides functionality to do spatial queries on a set of triangles.
/// Uses the boost RTree for queries.
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/// \brief Constructor
//------------------------------------------------------------------------------
GmTriSearchImpl::GmTriSearchImpl()
: m_min(XM_FLT_HIGHEST)
, m_max(XM_FLT_LOWEST)
, m_pts(new std::vector<Pt3d>())
, m_tris(new std::vector<int>())
, m_triActivity()
, m_rTree(nullptr)
{
} // GmTriSearchImpl::GmTriSearchImpl
//------------------------------------------------------------------------------
/// \brief Destructor
//------------------------------------------------------------------------------
GmTriSearchImpl::~GmTriSearchImpl()
{
  if (m_rTree)
    delete (m_rTree);
} // GmTriSearchImpl::~GmTriSearchImpl
//------------------------------------------------------------------------------
/// \brief Adds the triangles to the class
/// \param a_pts Array of point locations.
/// \param a_tris Array of triangles that references the a_pts array. This
/// array will have a size that is a multiple of 3. The first 3 locations in
/// array represent the first triangle and will have indices that correspond to
/// locations in the a_pts array.
//------------------------------------------------------------------------------
void GmTriSearchImpl::TrisToSearch(BSHP<std::vector<Pt3d>> a_pts, BSHP<std::vector<int>> a_tris)
{
  // remove existing rtree and cache
  if (m_rTree)
    delete (m_rTree);
  m_cache.clear();

  m_pts = a_pts;
  m_tris = a_tris;
  CreateRTree();
} // GmTriSearchImpl::TrisToSearch
//------------------------------------------------------------------------------
/// \brief Creates the RTree of the triangles
//------------------------------------------------------------------------------
void GmTriSearchImpl::CreateRTree()
{
  if (m_pts->empty() || m_tris->empty())
    return;

  m_min = XM_FLT_HIGHEST;
  m_max = XM_FLT_LOWEST;
  for (size_t i = 0; i < m_pts->size(); ++i)
  {
    Pt3d& pt = (*m_pts)[i];
    if (pt.x < m_min.x)
      m_min.x = pt.x;
    if (pt.y < m_min.y)
      m_min.y = pt.y;
    if (pt.z < m_min.z)
      m_min.z = pt.z;
    if (pt.x > m_max.x)
      m_max.x = pt.x;
    if (pt.y > m_max.y)
      m_max.y = pt.y;
    if (pt.z > m_max.z)
      m_max.z = pt.z;
  }
  double tol = sqr(m_max.x - m_min.x) + sqr(m_max.y - m_min.y);
  tol = sqrt(tol) / 1e9;

  qRtree rtree;
  size_t nTri = m_tris->size() / 3;
  idx_tri getter(&(*m_pts)[0], &(*m_tris)[0], tol);
  m_rTree = new bgi::rtree<size_t, qRtree, idx_tri>(
    boost::counting_iterator<size_t>(0), boost::counting_iterator<size_t>(nTri), rtree, getter);
} // GmTriSearchImpl::CreateRTree
//------------------------------------------------------------------------------
/// \brief Modifies the activity bitset of the class.
/// \param a_activity bitset of the activity of the points
//------------------------------------------------------------------------------
void GmTriSearchImpl::SetPtActivity(DynBitset& a_activity)
{
  XM_ENSURE_TRUE_NO_ASSERT(a_activity.size() == m_pts->size());

  // set triangle activity from point activity
  m_triActivity.clear();
  m_triActivity.resize(m_tris->size(), 1);
  for (size_t i = 0; i < m_tris->size(); i += 3)
  {
    if (!a_activity.test((*m_tris)[i]) || !a_activity.test((*m_tris)[i + 1]) ||
        !a_activity.test((*m_tris)[i + 2]))
    {
      m_triActivity[i] = m_triActivity[i + 1] = m_triActivity[i + 2] = 0;
    }
  }
} // GmTriSearchImpl::SetPtActivity
//------------------------------------------------------------------------------
/// \brief Modifies the activity bitset of the class.
/// \param a_activity bitset of the activity of the points
//------------------------------------------------------------------------------
void GmTriSearchImpl::SetTriActivity(DynBitset& a_activity)
{
  if (a_activity.empty())
  {
    m_triActivity = a_activity;
    return;
  }

  XM_ENSURE_TRUE_NO_ASSERT(a_activity.size() == m_tris->size() / 3);

  m_triActivity.reset();
  m_triActivity.resize(m_tris->size(), 1);
  size_t idx;
  for (size_t i = 0; i < a_activity.size(); ++i)
  {
    idx = i * 3;
    m_triActivity[idx + 0] = a_activity[i];
    m_triActivity[idx + 1] = a_activity[i];
    m_triActivity[idx + 2] = a_activity[i];
  }
} // GmTriSearchImpl::SetTriActivity
//------------------------------------------------------------------------------
/// \brief Tests if a triangle is active. If any of the points of the tri
/// are not active then the tri is not active
/// \param a_idx Triangle index
/// \return true if the triangle a_idx is active.
//------------------------------------------------------------------------------
bool GmTriSearchImpl::ActiveTri(int a_idx)
{
  if (m_triActivity.empty())
    return true;
  if (!m_triActivity.test(a_idx))
    return false;
  return true;
} // GmTriSearchImpl::ActiveTri
//------------------------------------------------------------------------------
/// \brief Find the triangle containing the point
/// \param a_pt Location used to find a triangle.
/// \return Index of triangle containing a_pt. If XM_NONE is returned then no
/// triangle contained the point.
//------------------------------------------------------------------------------
int GmTriSearchImpl::TriContainingPt(const Pt3d& a_pt)
{
  int ix[3];
  Pt3d weights;
  return FindTriangle(a_pt, ix, weights);
} // GmTriSearchImpl::TriContainingPt
//------------------------------------------------------------------------------
/// \brief Find all triangle whose envelop contains the point.
/// \param a_pt Location used to find a triangle.
/// \param a_tris The indices to triangles whose envelop contains the point
//------------------------------------------------------------------------------
void GmTriSearchImpl::TriEnvelopsContainingPt(const Pt3d& a_pt, VecInt& a_tris)
{
  int idx;
  a_tris.resize(0);
  Pt3d p(a_pt.x, a_pt.y, 0);
  std::vector<value> vals;
  m_rTree->query(bgi::intersects(p), std::back_inserter(vals));
  for (size_t i = 0; i < vals.size(); ++i)
  {
    idx = (int)(vals[i] * 3);
    a_tris.push_back(idx);
  }
} // GmTriSearchImpl::TriEnvelopsContainingPt
//------------------------------------------------------------------------------
/// \brief Find all triangle whose envelop contains the point.
/// \param a_pMin min x,y location of the box
/// \param a_pMax max x,y location of the box
/// \param a_tris The indices to triangles whose envelop contains the point
//------------------------------------------------------------------------------
void GmTriSearchImpl::TriEnvelopesOverlap(const Pt3d& a_pMin, const Pt3d& a_pMax, VecInt& a_tris)
{
  GmBstBox3d bx;
  bx.max_corner() = a_pMax;
  bx.max_corner().z = 0;
  bx.min_corner() = a_pMin;
  bx.min_corner().z = 0;
  int idx;
  a_tris.resize(0);
  std::vector<value> vals;
  // m_rTree->query(bgi::overlaps(bx), std::back_inserter(vals));
  m_rTree->query(bgi::intersects(bx), std::back_inserter(vals));
  for (size_t i = 0; i < vals.size(); ++i)
  {
    idx = (int)(vals[i] * 3);
    a_tris.push_back(idx);
  }
} // GmTriSearchImpl::TriContainingPt
//------------------------------------------------------------------------------
/// \brief Use the stored triangles to get interpolation weights for a point.
/// Returns false if the point is outside the triangles.
/// \param a_pt Location that is interpolated to.
/// \param a_idxs Vector of triangle point indices filled in by this method.
/// \param a_wts Vector of triangle point weights filled in by this method.
/// \return false if a_pt is not inside of any triangle
//------------------------------------------------------------------------------
bool GmTriSearchImpl::InterpWeights(const Pt3d& a_pt, VecInt& a_idxs, VecDbl& a_wts)
{
  int triangleIdx;
  bool result = InterpWeightsTriangleIdx(a_pt, triangleIdx, a_idxs, a_wts);
  return result;
} // GmTriSearchImpl::InterpWeights
//------------------------------------------------------------------------------
/// \brief Use the stored triangles to get interpolation weights for a point.
/// Returns false if the point is outside the triangles.
/// \param a_pt Location that is interpolated to.
/// \param a_triangleIdx Found triangles index in triangle array
///                      (zero based count*3).
/// \param a_idxs Vector of triangle point indices filled in by this method.
/// \param a_wts Vector of triangle point weights filled in by this method.
/// \return false if a_pt is not inside of any triangle
//------------------------------------------------------------------------------
bool GmTriSearchImpl::InterpWeightsTriangleIdx(const Pt3d& a_pt, int& a_triangleIdx,
                                               VecInt& a_idxs, VecDbl& a_wts)
{
  int ix[3];
  Pt3d weights;
  a_triangleIdx = FindTriangle(a_pt, ix, weights);
  if (a_triangleIdx != XM_NONE)
  {
    a_wts.resize(0);
    a_wts.push_back(weights.x);
    a_wts.push_back(weights.y);
    a_wts.push_back(weights.z);
    a_idxs.assign(ix, ix + 3);
    return true;
  }
  else
  {
    a_idxs.clear();
    a_wts.clear();
  }
  return false;
} // GmTriSearchImpl::InterpWeightsTriangleIdx
//------------------------------------------------------------------------------
/// \brief Write the internals to a string.
/// \return The string.
//------------------------------------------------------------------------------
std::string GmTriSearchImpl::ToString() const
{
  std::stringstream ss;
  ss << m_min << "=min" << m_max << "=max"
     << "\n";

  if (m_pts)
    VecToStream(ss, *m_pts, "pts");
  if (m_tris)
    VecToStream(ss, *m_tris, "tris");
  VecToStream(ss, m_triActivity, "triActivity");
  return ss.str();
} // GmTriSearchImpl::ToString
//------------------------------------------------------------------------------
/// \brief Fills in the point indices based on the triangle index.
/// \param a_triIdx The triangle index in the m_tris vector.
/// \param a_ptIdxes Array of point indices filled by this method. The indices
/// correspond to locations in the m_pts vector.
//------------------------------------------------------------------------------
void GmTriSearchImpl::PointIdxesFromTriIdx(int a_triIdx, int a_ptIdxes[3])
{
  a_ptIdxes[0] = a_ptIdxes[1] = a_ptIdxes[2] = -1;
  if (a_triIdx < 0 || a_triIdx + 2 >= (int)m_tris->size())
  {
    std::stringstream msg;
    msg << "Index out of range. File: " << __FILE__ << ", Line:" << __LINE__;
    XM_LOG(xmlog::debug, msg.str());
    return;
  }
  a_ptIdxes[0] = (*m_tris)[a_triIdx + 0];
  a_ptIdxes[1] = (*m_tris)[a_triIdx + 1];
  a_ptIdxes[2] = (*m_tris)[a_triIdx + 2];
} // GmTriSearchImpl::PointIdxesFromTriIdx
//------------------------------------------------------------------------------
/// \brief Find the triangle containing the location a_pt.
/// \param a_pt The location to interpolate to.
/// \param ix The point indices from the triangle containing a_pt.
/// \param weights The weights associated with the points ix.
/// \return index to the triangle containg a_pt.
//------------------------------------------------------------------------------
int GmTriSearchImpl::FindTriangle(const Pt3d& a_pt, int ix[3], Pt3d& weights)
{
  if (!m_tris || m_tris->empty() || !m_pts || m_pts->empty())
  {
    XM_LOG(xmlog::error, "Unable to find triangle; no points or triangles exist.");
    return XM_NONE;
  }
  Pt3d p(a_pt.x, a_pt.y, 0);
  std::vector<value> vals;
  m_rTree->query(bgi::intersects(p), std::back_inserter(vals));
  if (vals.empty())
    return XM_NONE;

  size_t idx;
  for (size_t i = 0; i < vals.size(); ++i)
  {
    idx = vals[i] * 3;
    // if any point is inactive then skip this triangle
    if (!ActiveTri((int)idx))
      continue;
    ix[0] = (*m_tris)[idx];
    ix[1] = (*m_tris)[idx + 1];
    ix[2] = (*m_tris)[idx + 2];

    GmBstPoly3d poly;
    for (size_t j = 0; j < 3; ++j)
    {
      bg::exterior_ring(poly).push_back((*m_pts)[ix[j]]);
    }
    bg::exterior_ring(poly).push_back((*m_pts)[ix[0]]);
    if (bg::covered_by(a_pt, poly))
    {
      BarycentricVals b;
      GetTriBarycentricVals((int)idx, ix, b);
      iGetBarycentricCoords(p, b, weights);
      return (int)idx;
    }
  }
  // additional checks with some tolerancing
  for (size_t i = 0; i < vals.size(); ++i)
  {
    idx = vals[i] * 3;
    // if any point is inactive then skip this triangle
    if (!ActiveTri((int)idx))
      continue;
    ix[0] = (*m_tris)[idx];
    ix[1] = (*m_tris)[idx + 1];
    ix[2] = (*m_tris)[idx + 2];
    BarycentricVals b;
    GetTriBarycentricVals((int)idx, ix, b);
    iGetBarycentricCoords(p, b, weights);
    // if (GTEQ_TOL(weights.x, 0, BARYTOL) && GTEQ_TOL(weights.y, 0, BARYTOL) &&
    //    GTEQ_TOL(weights.z, 0, BARYTOL)) {
    if (weights.x >= -BARYTOL && weights.y >= -BARYTOL && weights.z >= -BARYTOL)
    {
      return (int)idx;
    }
  }

  return XM_NONE;
} // GmTriSearchImpl::FindTriangle
//------------------------------------------------------------------------------
/// \brief Gets the barycentric values for a triangle
/// \param a_idx Triangle index
/// \param a_ix Point indexes that make up triangle
/// \param a_b The barycentric values
//------------------------------------------------------------------------------
void GmTriSearchImpl::GetTriBarycentricVals(int a_idx, int a_ix[3], BarycentricVals& a_b)
{
  if (m_cache.find(a_idx) != m_cache.end())
  {
    a_b = m_cache[a_idx];
  }
  else
  {
    iCartToBary((*m_pts)[a_ix[0]], (*m_pts)[a_ix[1]], (*m_pts)[a_ix[2]], a_b);
    m_cache[a_idx] = a_b;
  }
} // GmTriSearchImpl::GetTriBarycentricVals
//------------------------------------------------------------------------------
/// \brief Gets the activity of the points from the triangles
/// \return bitset of the activity of the points.
//------------------------------------------------------------------------------
DynBitset GmTriSearchImpl::PointActivityFromTriActivity()
{
  DynBitset r;
  if (m_triActivity.empty())
    return r;

  // all points inactive, if any triangle attached to point is active then
  // the point will be active
  r.resize(m_pts->size(), 0);
  for (size_t i = 0; i < m_tris->size(); i += 3)
  {
    if (ActiveTri((int)i))
    {
      r[(*m_tris)[i + 0]] = 1;
      r[(*m_tris)[i + 1]] = 1;
      r[(*m_tris)[i + 2]] = 1;
    }
  }
  return r;
} // GmTriSearchImpl::PointActivityFromTriActivity
//------------------------------------------------------------------------------
/// \brief creates the rtree of the points
/// \return GmPtSearch.
//------------------------------------------------------------------------------
BSHP<GmPtSearch> GmTriSearchImpl::CreatePtSearch()
{
  BSHP<GmPtSearch> pts = GmPtSearch::New(true);
  pts->PtsToSearch(m_pts);
  DynBitset b = PointActivityFromTriActivity();
  pts->SetActivity(b);
  return pts;
} // GmTriSearchImpl::CreatePtSearch

} // namespace xms

#ifdef CXX_TEST
////////////////////////////////////////////////////////////////////////////////

#include <xmsinterp/geometry/GmTriSearch.t.h>

#include <xmscore/testing/TestTools.h>

#include <xmsinterp/triangulate/TrTin.h>
#include <xmsinterp/triangulate/TrTin.t.h>

using namespace xms;

////////////////////////////////////////////////////////////////////////////////
/// \class TriSearchUnitTests
/// \brief tester for the TriSearch class
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/// \brief tests creating the class
//------------------------------------------------------------------------------
void TriSearchUnitTests::testCreateClass()
{
  boost::shared_ptr<GmTriSearch> pts = GmTriSearch::New();
  TS_ASSERT(pts);
}
//------------------------------------------------------------------------------
/// \brief tests calculation of interpolation weights from triangles
//------------------------------------------------------------------------------
void TriSearchUnitTests::testInterpWeights()
{
  boost::shared_ptr<GmTriSearch> tris = GmTriSearch::New();
  BSHP<std::vector<Pt3d>> p(new std::vector<Pt3d>());
  *p = {{0, 0, 0}, {1, 1, 1}, {1, 0, 2}, {0, 1, 2}, {.5, 1.5, 1}};
  BSHP<std::vector<int>> t(new std::vector<int>());
  *t = {0, 2, 1, 0, 1, 3, 3, 1, 4};
  tris->TrisToSearch(p, t);

  std::vector<int> idxs, baseIdx;
  std::vector<double> wt, baseWt;
  Pt3d pt(.5, .2, 0);
  TS_ASSERT(tris->InterpWeights(pt, idxs, wt));
  baseIdx = {0, 2, 1};
  baseWt = {.5, .3, .2};
  TS_ASSERT_EQUALS_VEC(baseIdx, idxs);
  TS_ASSERT_DELTA_VEC(baseWt, wt, 1e-7);

  int triangleIdx;
  TS_ASSERT(tris->InterpWeightsTriangleIdx({.25, .75, 0}, triangleIdx, idxs, wt));
  TS_ASSERT_EQUALS(3, triangleIdx);
  baseIdx = {0, 1, 3};
  TS_ASSERT_EQUALS_VEC(baseIdx, idxs);
  baseWt = {.25, .25, .5};
  TS_ASSERT_DELTA_VEC(baseWt, wt, 1e-7);

  TS_ASSERT(!tris->InterpWeightsTriangleIdx({0, 1.25, 0}, triangleIdx, idxs, wt));
  TS_ASSERT_EQUALS(XM_NONE, triangleIdx);
  TS_ASSERT_EQUALS_VEC(VecInt(), idxs);
  TS_ASSERT_DELTA_VEC(VecDbl(), wt, 1e-7);
} // TriSearchUnitTests::testInterpWeights
//------------------------------------------------------------------------------
/// \brief tests setting the point activity
//------------------------------------------------------------------------------
void TriSearchUnitTests::testPtActivity()
{
  BSHP<GmTriSearch> tris = GmTriSearch::New();
  BSHP<std::vector<Pt3d>> p(new std::vector<Pt3d>());
  *p = {{0, 0, 0}, {1, 1, 1}, {1, 0, 2}, {0, 1, 2}, {.5, 1.5, 1}};
  BSHP<std::vector<int>> t(new std::vector<int>());
  *t = {0, 2, 1, 0, 1, 3, 3, 1, 4};
  tris->TrisToSearch(p, t);

  DynBitset act_wrongSize(6);
  // won't do anything
  tris->SetPtActivity(act_wrongSize);

  Pt3d pt(.5, .2, 0);
  TS_ASSERT_EQUALS(0, tris->TriContainingPt(pt));

  DynBitset act(5);
  act.flip();
  act.set(1, false);
  tris->SetPtActivity(act);

  TS_ASSERT_EQUALS(XM_NONE, tris->TriContainingPt(pt));
} // TriSearchUnitTests::testPtActivity
//------------------------------------------------------------------------------
/// \brief tests setting the triangle activity
//------------------------------------------------------------------------------
void TriSearchUnitTests::testTriActivity()
{
  BSHP<GmTriSearch> tris = GmTriSearch::New();
  BSHP<std::vector<Pt3d>> p(new std::vector<Pt3d>());
  *p = {{0, 0, 0}, {1, 1, 1}, {1, 0, 2}, {0, 1, 2}, {.5, 1.5, 1}};
  BSHP<std::vector<int>> t(new std::vector<int>());
  *t = {0, 2, 1, 0, 1, 3, 3, 1, 4};
  tris->TrisToSearch(p, t);

  DynBitset act_wrongSize(4);
  // won't do anything
  tris->SetTriActivity(act_wrongSize);

  Pt3d pt(.5, .2, 0), pt2(.5, .5, 0);
  TS_ASSERT_EQUALS(0, tris->TriContainingPt(pt));
  // pt2 is right on the boundary between first two triangles
  TS_ASSERT_EQUALS(0, tris->TriContainingPt(pt2));

  DynBitset act(3);
  act.flip();
  act.set(0, false);
  tris->SetTriActivity(act);

  TS_ASSERT_EQUALS(XM_NONE, tris->TriContainingPt(pt));
  TS_ASSERT_EQUALS(3, tris->TriContainingPt(pt2));

  // setting to empty activity turns on everything
  act.clear();
  tris->SetTriActivity(act);
  TS_ASSERT_EQUALS(0, tris->TriContainingPt(pt));
  TS_ASSERT_EQUALS(0, tris->TriContainingPt(pt2));
} // TriSearchUnitTests::testPtActivity
//------------------------------------------------------------------------------
/// \brief A particular test case from SMS that was included
//------------------------------------------------------------------------------
void TriSearchUnitTests::testSmsCase1()
{
  Pt3d pt(-31.459823375717541, 29.927133417260336, 0);
  GmTriSearchImpl tris;
  BSHP<std::vector<Pt3d>> p(new std::vector<Pt3d>());
  *p = {{-20.150000000000002, 46.579999999999998, 7},
        {-41.100000000000001, 30.370000000000001, 8},
        {-19.550000000000001, 29.379999999999999, 9}};
  BSHP<std::vector<int>> t(new std::vector<int>());
  *t = {2, 0, 1};
  tris.TrisToSearch(p, t);
  TS_ASSERT_EQUALS(0, tris.TriContainingPt(pt));
} // TriSearchUnitTests::testCase1
//------------------------------------------------------------------------------
/// \brief tests a point that touches a triangle
//------------------------------------------------------------------------------
void TriSearchUnitTests::testTouch()
{
  Pt3d pt(.5, .5, 0);
  GmTriSearchImpl tris;
  BSHP<std::vector<Pt3d>> p(new std::vector<Pt3d>());
  *p = {{0, 0, 7}, {1, 0, 8}, {1, 1, 9}};
  BSHP<std::vector<int>> t(new std::vector<int>());
  *t = {2, 0, 1};
  tris.TrisToSearch(p, t);
  TS_ASSERT_EQUALS(0, tris.TriContainingPt(pt));
} // TriSearchUnitTests::testTouch
//------------------------------------------------------------------------------
/// \brief tests overlaping triangles
//------------------------------------------------------------------------------
void TriSearchUnitTests::testTriEnvelopesOverlap()
{
  BSHP<TrTin> t = trBuildTestTin();
  GmTriSearchImpl ts;
  BSHP<VecInt> tPtr(new VecInt());
  tPtr->reserve(t->Triangles().size());
  for (const auto& tt : t->Triangles())
    tPtr->push_back((int)tt);
  ts.TrisToSearch(t->PointsPtr(), tPtr);

  Pt3d pMin(9, 4, 0), pMax(15, 10, 0);
  VecInt tIdxes;
  ts.TriEnvelopesOverlap(pMin, pMax, tIdxes);
  std::sort(tIdxes.begin(), tIdxes.end());
  VecInt baseIdxes = {3, 6, 9, 18, 21, 27};
  TS_ASSERT_EQUALS_VEC(baseIdxes, tIdxes);
} // TriSearchUnitTests::testTriEnvelopesOverlap

#endif // CXX_TEST
