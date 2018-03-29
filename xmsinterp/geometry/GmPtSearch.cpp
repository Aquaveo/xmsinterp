//------------------------------------------------------------------------------
/// \file
/// \ingroup geometry
/// \copyright (C) Copyright Aquaveo 2018. Distributed under the xmsng
///  Software License, Version 1.0. (See accompanying file
///  LICENSE_1_0.txt or copy at http://www.aquaveo.com/xmsng/LICENSE_1_0.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------

// 1. Precompiled header

// 2. My header
#include <xmsinterp/geometry/GmPtSearch.h>

// 3. Standard Library Headers

// 4. External Library Headers
#pragma warning(push)
#pragma warning(disable : 4512) // boost code: no assignment operator
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/register/point.hpp>
#include <boost/geometry/index/rtree.hpp>
#include <boost/iterator/counting_iterator.hpp>
#pragma warning(pop)

// 5. Shared Headers
#include <xmscore/points/pt.h>
#include <xmscore/stl/utility.h>
#include <xmscore/misc/XmError.h>
#include <xmscore/misc/DynBitset.h>
#include <xmscore/misc/XmConst.h>

// 6. Non-shared Headers

//----- Forward declarations ---------------------------------------------------

//----- External globals -------------------------------------------------------

//----- Namespace declaration --------------------------------------------------
namespace
{
class bPt
{
public:
  bPt()
  : x(0)
  , y(0)
  , z(0)
  {
  }
  bPt(double a_x, double a_y, double a_z)
  : x(a_x)
  , y(a_y)
  , z(a_z)
  {
  }
  bPt(const xms::Pt3d& a_)
  : x(a_.x)
  , y(a_.y)
  , z(a_.z)
  {
  }
  bPt& operator=(const xms::Pt3d a_)
  {
    x = a_.x;
    y = a_.y;
    z = a_.z;
    return (*this);
  }

  double x, y, z;
};
} // unnamed namespace
/// boost macro for registering bPt as a 3D point. Most of our use of the
/// spatial indexes is in 2D. However, we do use the point searching in 3D
/// if the user constructs the object with 2d specified as false.
BOOST_GEOMETRY_REGISTER_POINT_3D(bPt, double, cs::cartesian, x, y, z);
using namespace xms;
namespace xms
{
namespace bg = boost::geometry;
namespace bgi = bg::index;

/// box
typedef bg::model::box<bPt> box;
/// value
typedef size_t value;
/// qRtree
typedef bgi::quadratic<8> qRtree;

//----- Constants / Enumerations -----------------------------------------------

//----- Classes / Structs ------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
/// \brief a class used with the boost::geometry::index::satisfies function
class fSatisfies
{
public:
  /// \brief Constructor
  /// \param[in] a_nVals Number of points
  fSatisfies(size_t a_nVals)
  : m_bits(a_nVals, 0)
  {
  }

  DynBitset m_bits; ///< bitset that is the size of the points

  /// \brief Determines if a point can be returned. If the bit is not set yet.
  /// \param a_: ???
  /// \return true or false.
  template <typename value>
  bool operator()(value const& a_) const
  {
    if (m_bits.test(a_))
    {
      return false;
    }
    return true;
  } // operator()
};

////////////////////////////////////////////////////////////////////////////////
/// \brief class for indexing the points
class idx_pt
{
public:
  typedef const bPt result_type; ///< created to follow boost examples

  /// \brief constructor
  /// \param[in] a_ Array of point locations
  /// \param[in] a_2d 2d or 3d points
  explicit idx_pt(const Pt3d* a_, bool a_2d)
  : m_(a_)
  , m_2d(a_2d)
  {
    m_v.reset();
  }

  /// \brief constructor
  /// \param[in] a_ Array of point locations
  /// \param[in] a_2d 2d or 3d points
  explicit idx_pt(BSHP<std::vector<Pt3d>> a_, bool a_2d)
  : m_(0)
  , m_2d(a_2d)
  , m_v(a_)
  {
  }

  /// \brief operator to return the point location based on an index
  /// \param i: index.
  /// \return result_type
  result_type operator()(size_t i) const
  {
    if (m_)
      return bPt(m_[i].x, m_[i].y, m_2d ? 0 : m_[i].z);
    else
      return bPt((*m_v)[i].x, (*m_v)[i].y, m_2d ? 0 : (*m_v)[i].z);
  }

  const Pt3d* m_;              ///< array of point locations
  bool m_2d;                   ///< flag specifying 2d searching only
  BSHP<std::vector<Pt3d>> m_v; ///< vector of point locations
};

////////////////////////////////////////////////////////////////////////////////
/// \brief Implementation of GmPtSearch. Generic class for searching location
/// data. Uses a boost R-tree to query a set of locations. Works for 2D and 3D.
class GmPtSearchImpl : public GmPtSearch
{
public:
  GmPtSearchImpl(bool a_2dSearch);
  virtual ~GmPtSearchImpl();

  virtual void PtsToSearch(BSHP<std::vector<Pt3d>> a_pts);

  virtual void VectorThatGrowsToSearch(BSHP<std::vector<Pt3d>> a_);
  virtual bool AddPtToVectorIfUnique(const Pt3d& a_, double a_tol, int& a_ptIdx) override;

  virtual void NearestPtsToPt(const Pt3d& a_pt,
                              int a_numPtsToFind,
                              bool a_quad_oct_Search,
                              std::vector<int>& a_nearest) const;

  virtual void NearestPtsToPtInRtree(int a_ptIdx,
                                     const Pt3d& a_pt,
                                     int a_numPtsToFind,
                                     bool a_quad_oct_Search,
                                     std::vector<int>& a_nearest) const;

  virtual bool PtInRTree(const Pt3d& a_pt, const double a_tol) override;

  virtual void PtsWithinDistanceToPtInRtree(int a_ptIdx,
                                            const Pt3d& a_pt,
                                            double a_dist,
                                            std::vector<int>& a_nearest) const;

  virtual void NearestPtsToPt(const Pt3d& a_pt,
                              int a_numPtsToFind,
                              bool a_quad_oct_Search,
                              std::vector<int>& a_nearest,
                              fSatisfies* a_fsat) const;

  virtual void SetActivity(DynBitset& a_activity);
  virtual DynBitset GetActivity() override;

  /// \brief Returns shared point to the point locations in the RTree
  /// \return VecPt3d.
  virtual const BSHP<VecPt3d> GetPointsPtr() const override { return m_bshpPt3d; }
  /// \brief Returns true if class only searches in 2D
  /// \return true or false.
  virtual bool Is2D() const override { return m_2dSearch; }
  virtual std::string ToString() const override;

  void UpdateMinMax(const Pt3d* a_pts, size_t a_npts);
  void CreateOctants(const Pt3d& a_pt, std::vector<box>& a_boxes) const;

  bool m_2dSearch;                            ///< flag specifying 2d searching only
  Pt3d m_min;                                 ///< minimum extents of points
  Pt3d m_max;                                 ///< maximum extents of points
  DynBitset m_activity;                       ///< point activity
  bgi::rtree<value, qRtree, idx_pt>* m_rTree; ///< spatial index for searching the points
  BSHP<VecPt3d> m_bshpPt3d;                   ///< Vector of point locations
};                                            // class GmPtSearchImpl

//----- Internal functions -----------------------------------------------------

//----- Class / Function definitions -------------------------------------------

//------------------------------------------------------------------------------
/// \brief Creates an PtSearch class
/// \param a_2dSearch Flag indicating that the point is consider 2d (xy) not
/// 3d (xyz).
/// \return A shared pointer to an instance of GmPtSearch
//------------------------------------------------------------------------------
BSHP<GmPtSearch> GmPtSearch::New(bool a_2dSearch)
{
  BSHP<GmPtSearch> ptr(new GmPtSearchImpl(a_2dSearch));
  return ptr;
} // PtSearch::New
//------------------------------------------------------------------------------
/// \brief
//------------------------------------------------------------------------------
GmPtSearch::GmPtSearch()
{
} // PtSearch::PtSearch
//------------------------------------------------------------------------------
/// \brief
//------------------------------------------------------------------------------
GmPtSearch::~GmPtSearch()
{
} // PtSearch::~PtSearch

////////////////////////////////////////////////////////////////////////////////
/// \class GmPtSearchImpl
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/// \brief Constructor
/// \param[in] a_2dSearch Flag to specify that searching is in 2D (x,y) only.
//------------------------------------------------------------------------------
GmPtSearchImpl::GmPtSearchImpl(bool a_2dSearch)
: m_2dSearch(a_2dSearch)
, m_min(XM_FLT_HIGHEST)
, m_max(XM_FLT_LOWEST)
, m_activity()
, m_rTree(nullptr)
{
} // GmPtSearchImpl::GmPtSearchImpl
//------------------------------------------------------------------------------
/// \brief
//------------------------------------------------------------------------------
GmPtSearchImpl::~GmPtSearchImpl()
{
  if (m_rTree)
    delete (m_rTree);
} // GmPtSearchImpl::~GmPtSearchImpl
//------------------------------------------------------------------------------
/// \brief Adds the point locations to the class
/// \param[in] a_pts Array of point locations.
//------------------------------------------------------------------------------
void GmPtSearchImpl::PtsToSearch(BSHP<std::vector<Pt3d>> a_pts)
{
  if (m_rTree)
    delete (m_rTree);

  m_bshpPt3d = a_pts;
  qRtree rtree;
  idx_pt getter(a_pts, m_2dSearch);
  m_rTree = new bgi::rtree<size_t, qRtree, idx_pt>(
    boost::counting_iterator<size_t>(0), boost::counting_iterator<size_t>(m_bshpPt3d->size()),
    rtree, getter);

  UpdateMinMax(&(*m_bshpPt3d)[0], m_bshpPt3d->size());
} // GmPtSearchImpl::PtsToSearch
//------------------------------------------------------------------------------
/// \brief Updates the m_min, m_max variables
/// \param a_pts Array of point locations.
/// \param a_nPts Number of values in a_pts array.
//------------------------------------------------------------------------------
void GmPtSearchImpl::UpdateMinMax(const Pt3d* a_pts, size_t a_nPts)
{
  for (size_t i = 0; i < a_nPts; ++i)
  {
    Pt3d pt = a_pts[i];
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
    if (m_2dSearch)
      pt.z = 0;
  }
  m_min -= 1;
  m_max += 1;
} // GmPtSearchImpl::UpdateMinMax
//------------------------------------------------------------------------------
/// \brief Adds the point locations to the class
/// \param a_ Shared pointer to array of point locations.
//------------------------------------------------------------------------------
void GmPtSearchImpl::VectorThatGrowsToSearch(BSHP<std::vector<Pt3d>> a_)
{
  if (m_rTree)
    delete (m_rTree);
  m_bshpPt3d = a_;
  qRtree rtree;
  idx_pt getter(a_, m_2dSearch);
  m_rTree = new bgi::rtree<size_t, qRtree, idx_pt>(boost::counting_iterator<size_t>(0),
                                                   boost::counting_iterator<size_t>(a_->size()),
                                                   rtree, getter);

  if (!m_bshpPt3d->empty())
    UpdateMinMax(&(*m_bshpPt3d)[0], m_bshpPt3d->size());
} // GmPtSearchImpl::VectorThatGrowsToSearch
//------------------------------------------------------------------------------
/// \brief Adds the point to the R-tree if the point location is unique based
/// on the passed in tolerance. If a point exists at this location then return
/// false. If the point is added then return true.
/// \param a_pt location to test
/// \param a_tol Tolerance used to compare point locations
/// \param a_ptIdx New point's index (if unique) or matching point's index.
/// \return true if unique, false if a_pt matched an existing point.
//------------------------------------------------------------------------------
bool GmPtSearchImpl::AddPtToVectorIfUnique(const Pt3d& a_pt, double a_tol, int& a_ptIdx)
{
  std::vector<int> n;
  PtsWithinDistanceToPtInRtree(-1, a_pt, a_tol, n);
  if (!n.empty())
  {
    a_ptIdx = *std::min_element(n.begin(), n.end());
    return false;
  }
  value val(m_bshpPt3d->size());
  a_ptIdx = (int)m_bshpPt3d->size();
  m_bshpPt3d->push_back(a_pt);
  m_rTree->insert(val);
  // update min/max
  if (a_pt.x < m_min.x)
    m_min.x = a_pt.x;
  if (a_pt.y < m_min.y)
    m_min.y = a_pt.y;
  if (a_pt.z < m_min.z)
    m_min.z = a_pt.z;
  if (a_pt.x > m_max.x)
    m_max.x = a_pt.x;
  if (a_pt.y > m_max.y)
    m_max.y = a_pt.y;
  if (a_pt.z > m_max.z)
    m_max.z = a_pt.z;
  // if (!m_bshpPt3d->empty()) UpdateMinMax(&(*m_bshpPt3d)[0], m_bshpPt3d->size());
  return true;
} // GmPtSearchImpl::AddPtToVectorIfUnique
//------------------------------------------------------------------------------
/// \brief Finds the nearest points to the input a_pt with an option to search
/// quadrants/octants
/// \param a_pt The location we are interested in.
/// \param a_numPtsToFind The number of nearest points to a_pt to find.
/// \param a_quad_oct_Search Whether to perform the nearest points search in
/// each quadrant (2d) or octant (3d).
/// \param a_nearest Vector that is filled in by this method.
//------------------------------------------------------------------------------
void GmPtSearchImpl::NearestPtsToPt(const Pt3d& a_pt,
                                    int a_numPtsToFind,
                                    bool a_quad_oct_Search,
                                    std::vector<int>& a_nearest) const
{
  fSatisfies *fs(0), fs1(1);
  if (!m_activity.empty())
  {
    fs1.m_bits = m_activity;
    fs = &fs1;
  }
  NearestPtsToPt(a_pt, a_numPtsToFind, a_quad_oct_Search, a_nearest, fs);
} // NearestPtsToPt
//------------------------------------------------------------------------------
/// \brief Finds the nearest points to the input a_pt with an option to search
/// quadrants/octants. The difference with this method is the addition of the
/// fSatisfies class. When performing a search in each quadrant/octant it is
/// possible for a point to be exactly on the boundary of a quadrant but we
/// don't want to include that point more than 1 time. The fSatisfies class
/// ensures that a point is only included 1 time.
/// \param a_pt The location we are interested in.
/// \param a_numPtsToFind The number of nearest points to a_pt to find.
/// \param a_quad_oct_Search Whether to perform the nearest points search in
/// each quadrant (2d) or octant (3d).
/// \param a_nearest Vector that is filled in by this method.
/// \param a_fsat Class used to ensure a point is include only 1 time in the
/// search results.
//------------------------------------------------------------------------------
void GmPtSearchImpl::NearestPtsToPt(const Pt3d& a_pt,
                                    int a_numPtsToFind,
                                    bool a_quad_oct_Search,
                                    std::vector<int>& a_nearest,
                                    fSatisfies* a_fsat) const
{
  std::vector<value> nearest;
  a_nearest.resize(0);
  if (!m_bshpPt3d || m_bshpPt3d->empty())
  {
    XM_LOG(xmlog::error, "Unable to find nearest points; no points exist.");
  }

  bPt tmpPt(a_pt);
  if (!a_quad_oct_Search)
  {
    if (a_fsat)
      m_rTree->query(bgi::satisfies(*a_fsat) && bgi::nearest(tmpPt, a_numPtsToFind),
                     std::back_inserter(nearest));
    else
      m_rTree->query(bgi::nearest(tmpPt, a_numPtsToFind), std::back_inserter(nearest));
    a_nearest.reserve(nearest.size());
    for (size_t i = 0; i < nearest.size(); ++i)
    {
      a_nearest.push_back((int)nearest[i]);
    }
  }
  else
  {
    fSatisfies *fPtr(a_fsat), tmpFsat(m_rTree->size());
    if (!fPtr)
      fPtr = &tmpFsat;
    std::vector<box> boxes;
    CreateOctants(a_pt, boxes);
    for (size_t i = 0; i < boxes.size(); ++i)
    {
      std::vector<value> foundPts;
      m_rTree->query(
        bgi::covered_by(boxes[i]) && bgi::satisfies(*fPtr) && bgi::nearest(tmpPt, a_numPtsToFind),
        std::back_inserter(foundPts));
      a_nearest.reserve(a_nearest.size() + foundPts.size());
      for (size_t j = 0; j < foundPts.size(); ++j)
      {
        a_nearest.push_back((int)foundPts[j]);
        fPtr->m_bits.set(foundPts[j]);
      }
    }
  }
  std::sort(a_nearest.begin(), a_nearest.end());
} // GmPtSearchImpl::NearestPtsToPt
//------------------------------------------------------------------------------
/// \brief Finds the nearest points to the input a_pt with an option to search
/// quadrants/octants. Similar to NearestPtsToPt but this method will always
/// pass the fSatisfies class to the RTree. This method is used to find the
/// nearest points to a point that is included in the RTree.
/// \param a_ptIdx The index of the point a_pt. a_pt is in the RTree and we
/// don't want it in the results.
/// \param a_pt The location we are interested in.
/// \param a_numPtsToFind The number of nearest points to a_pt to find.
/// \param a_quad_oct_Search Whether to perform the nearest points search in
/// each quadrant (2d) or octant (3d).
/// \param a_nearest Vector that is filled in by this method.
//------------------------------------------------------------------------------
void GmPtSearchImpl::NearestPtsToPtInRtree(int a_ptIdx,
                                           const Pt3d& a_pt,
                                           int a_numPtsToFind,
                                           bool a_quad_oct_Search,
                                           std::vector<int>& a_nearest) const
{
  fSatisfies fsat(m_rTree->size());
  if (!m_activity.empty())
    fsat.m_bits = m_activity;
  fsat.m_bits.set(a_ptIdx);
  NearestPtsToPt(a_pt, a_numPtsToFind, a_quad_oct_Search, a_nearest, &fsat);
} // GmPtSearchImpl::NearestPtsToPtInRtree
//------------------------------------------------------------------------------
/// \brief Checks if the point is in the Rtree within tolerance
/// \param a_pt The location being searched.
/// \param a_tol A distance used to define a box around a_pt to see if any
/// Rtree pts are inside that box.
/// \return true if the point is in the RTree.
//------------------------------------------------------------------------------
bool GmPtSearchImpl::PtInRTree(const Pt3d& a_pt, const double a_tol)
{
  std::vector<int> n;
  PtsWithinDistanceToPtInRtree(-1, a_pt, a_tol, n);
  return !n.empty();
} // GmPtSearchImpl::PtInRTree
//------------------------------------------------------------------------------
/// \brief Finds the nearest points to the input a_pt with an option to search
/// quadrants/octants. Similar to NearestPtsToPt but this method will always
/// pass the fSatisfies class to the RTree. This method is used to find the
/// nearest points to a point that is included in the RTree.
/// \param a_ptIdx The index of the point a_pt. a_pt is in the RTree and we
/// don't want it in the results.
/// \param a_ptIdx The index of the point in the Rtree
/// \param a_pt The location we are interested in.
/// \param a_distance The distance from the point where we want to find
/// additional points
/// \param a_nearest Vector that is filled in by this method.
//------------------------------------------------------------------------------
void GmPtSearchImpl::PtsWithinDistanceToPtInRtree(int a_ptIdx,
                                                  const Pt3d& a_pt,
                                                  double a_distance,
                                                  std::vector<int>& a_nearest) const
{
  fSatisfies fsat(m_rTree->size());
  if (!m_activity.empty())
    fsat.m_bits = m_activity;
  if (a_ptIdx > -1)
    fsat.m_bits.set(a_ptIdx);

  Pt3d bMin(a_pt - a_distance), bMax(a_pt + a_distance);
  if (m_2dSearch)
  {
    bMin.z = -1;
    bMax.z = 1;
  }
  bPt boxMin, boxMax;
  box aBox(bMin, bMax);

  std::vector<value> nearest;
  a_nearest.resize(0);

  m_rTree->query(bgi::satisfies(fsat) && bgi::covered_by(aBox), std::back_inserter(nearest));
  a_nearest.reserve(nearest.size());
  for (size_t i = 0; i < nearest.size(); ++i)
  {
    a_nearest.push_back((int)nearest[i]);
  }
} // GmPtSearchImpl::PtsWithinDistanceToPtInRtree
//------------------------------------------------------------------------------
/// \brief Sets activity on the points in the Rtree so that points can be
/// ignored when interpolating.
/// \param a_activity Bitset of the point activity. Must be the same size
/// as the number of points.
//------------------------------------------------------------------------------
void GmPtSearchImpl::SetActivity(DynBitset& a_activity)
{
  if (a_activity.size() == m_rTree->size())
  {
    m_activity = a_activity;
    m_activity.flip();
  }
} // SetActivity
//------------------------------------------------------------------------------
/// \brief Returns the point activity.
/// \return dynamic bitset of point activity.
//------------------------------------------------------------------------------
DynBitset GmPtSearchImpl::GetActivity()
{
  DynBitset act(m_activity);
  act.flip();
  return act;
} // GmPtSearchImpl::GetActivity
//------------------------------------------------------------------------------
/// \brief Write the internals to a string.
/// \return The string.
//------------------------------------------------------------------------------
std::string GmPtSearchImpl::ToString() const
{
  std::stringstream ss;
  ss << m_2dSearch << "=2dSearch " << m_min << "=min " << m_max << "=max "
     << "\n";

  VecToStream(ss, m_activity, "activity");

  if (m_bshpPt3d)
    VecToStream(ss, *m_bshpPt3d, "bshpPt3d");

  return ss.str();
} // GmPtSearchImpl::ToString
//------------------------------------------------------------------------------
/// \brief Creates octants (or quadrants for 2d) to be used in the rtree query
/// \param a_pt The location about which to create the quadrants/octants.
/// \param a_boxes Vector filled in by this method defining the quadrants/octants.
//------------------------------------------------------------------------------
void GmPtSearchImpl::CreateOctants(const Pt3d& a_pt, std::vector<box>& a_boxes) const
{
  a_boxes.resize(0);
  Pt3d bmin(m_min), bmax(m_max), pt(a_pt);
  box bound(bmin, bmax);
  if (!m_2dSearch)
    bound.min_corner().z = pt.z;
  // here are the quadrants that we create
  //  ---------
  //  | 1 | 2 |
  //  ---------
  //  | 3 | 4 |
  //  ---------
  box aBox(bound);
  // 1
  aBox.max_corner().x = pt.x;
  aBox.min_corner().y = pt.y;
  a_boxes.push_back(aBox);
  // 2
  aBox = bound;
  aBox.min_corner() = pt;
  if (m_2dSearch)
    aBox.min_corner().z = -1;
  a_boxes.push_back(aBox);
  // 3
  aBox = bound;
  aBox.max_corner() = pt;
  aBox.max_corner().z = m_max.z;
  if (m_2dSearch)
    aBox.max_corner().z = 1;
  a_boxes.push_back(aBox);
  // 4
  aBox = bound;
  aBox.min_corner().x = pt.x;
  aBox.max_corner().y = pt.y;
  a_boxes.push_back(aBox);
  if (m_2dSearch)
    return;
  // do the lower octants if this is 3D
  for (int i = 0; i < 4; ++i)
  {
    a_boxes.push_back(a_boxes[i]);
    a_boxes.back().max_corner().z = pt.z;
    a_boxes.back().min_corner().z = m_min.z;
  }
} // GmPtSearchImpl::CreateOctants

} // namespace xms

#ifdef CXX_TEST
////////////////////////////////////////////////////////////////////////////////

#include <xmsinterp/geometry/GmPtSearch.t.h>
#include <boost/assign.hpp>
#include <xmscore/testing/TestTools.h>

// namespace xms {
using namespace xms;

//------------------------------------------------------------------------------
/// \brief Get GMS tutorial data.
/// \param[out] a_pts: The points.
/// \param[out] a_scalar: The scalar values.
/// \param[out] a_iPts: ???
//------------------------------------------------------------------------------
void iGetPoints2(std::vector<Pt3d>& a_pts, std::vector<float>& a_scalar, std::vector<Pt3d>& a_iPts)
{
  a_pts = {{-75.0, -16.0, 0.0}, {-60.0, 32.0, 0.0},   {-34.0, 50.0, 0.0},   {-34.0, 27.0, 0.0},
           {-8.0, 40.0, 0.0},   {16.0, 38.0, 0.0},    {-25.0, 14.0, 43.64}, {10.0, 18.0, 44.16},
           {27.0, 26.0, 0.0},   {63.0, 35.0, 0.0},    {-32.0, 0.0, 59.04},  {-7.0, 7.0, 90.2},
           {26.0, 6.0, 67.2},   {75.0, 7.0, 0.0},     {-37.0, -15.0, 9.24}, {-7.0, -13.0, 71.0},
           {2.0, -3.0, 98.4},   {31.0, -15.0, 25.56}, {60.0, -13.0, 0.0},   {-50.0, -30.0, 0.0},
           {-30.0, -28.0, 0.0}, {43.0, -22.0, 0.0},   {-32.0, -50.0, 0.0},  {27.0, -37.0, 0.0},
           {60.0, -33.0, 0.0}};

  a_scalar.resize(a_pts.size());
  for (size_t i = 0; i < a_pts.size(); ++i)
    a_scalar[i] = (float)a_pts[i].z;
  a_iPts = {{-90, 60, 0}, {90, 60, 0}, {-90, -60, 0}, {90, -60, 0}, {60, -33, 0}};
} // iGetPoints
//------------------------------------------------------------------------------
/// macro for testing
#define _TS_ASSERT_POINTS_FOUND(a_file, a_line, a_required, a_optional, a_found) \
  iAssertPointsFound(a_file, a_line, a_required, a_optional, a_found)
/// macro for testing
#define TS_ASSERT_POINTS_FOUND(a_required, a_optional, a_found) \
  _TS_ASSERT_POINTS_FOUND(__FILE__, __LINE__, a_required, a_optional, a_found)
//------------------------------------------------------------------------------
/// \brief helper function for testing
/// \param a_file: file.
/// \param a_line: line.
/// \param a_required: ???
/// \param a_optional: ???
/// \param a_found: ???
//------------------------------------------------------------------------------
void iAssertPointsFound(const char* a_file,
                        int a_line,
                        const std::vector<int>& a_required,
                        const std::vector<int>& a_optional,
                        const std::vector<int>& a_found)
{
  // look for required points
  std::vector<bool> handled(a_found.size(), false);
  for (size_t i = 0; i < a_required.size(); ++i)
  {
    int idx = a_required[i];
    auto it = std::find(a_found.begin(), a_found.end(), idx);
    bool found = it != a_found.end();
    if (found)
    {
      handled[it - a_found.begin()] = found;
    }
    else
    {
      std::stringstream ss;
      ss << "Result failed to include " << idx;
      _TS_FAIL(a_file, a_line, ss.str().c_str());
    }
  }

  // ensure all other points are optional
  for (size_t i = 0; i < handled.size(); ++i)
  {
    if (!handled[i])
    {
      int idx = a_found[i];
      bool found = std::find(a_optional.begin(), a_optional.end(), idx) != a_optional.end();
      if (!found)
      {
        std::stringstream ss;
        ss << "Failed to find " << idx << " in optional points";
        _TS_FAIL(a_file, a_line, ss.str().c_str());
      }
    }
  }
} // iAssertPointsFound
////////////////////////////////////////////////////////////////////////////////
/// \class PtSearchUnitTests
/// \brief tester for the PtSearch class
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/// \brief tests creating the class
//------------------------------------------------------------------------------
void PtSearchUnitTests::testCreateClass()
{
  BSHP<GmPtSearch> pts = GmPtSearch::New(true);
  TS_ASSERT(pts);
}
//------------------------------------------------------------------------------
/// \brief helper function for testing
/// \param pts: Points.
/// \param a_2d: true if 2D.
//------------------------------------------------------------------------------
static void iSetupPts(std::vector<Pt3d>& pts, bool a_2d)
{
  pts.resize(0);
  pts.reserve(300);
  for (int i = 1; i < 11; ++i)
  {
    for (int j = 1; j < 11; ++j)
    {
      pts.push_back(Pt3d(i, j, 0));
      if (i % 2)
        pts.push_back(Pt3d(-i, j, 0));
      pts.push_back(Pt3d(i, -j, 0));
      if (i % 2)
        pts.push_back(Pt3d(-i, -j, 0));
    }
  }
  if (a_2d)
    return;
  std::vector<Pt3d> pts1(pts);
  for (size_t i = 0; i < pts1.size(); ++i)
    pts1[i].z = .1 * (double)(i + 1);
  pts.insert(pts.end(), pts1.begin(), pts1.end());
  for (size_t i = 0; i < pts1.size(); ++i)
    pts1[i].z = -.1 * (double)(i + 1);
  pts.insert(pts.end(), pts1.begin(), pts1.end());
} // iSetupPts
//------------------------------------------------------------------------------
/// \brief testing in 2d
//------------------------------------------------------------------------------
void PtSearchUnitTests::testTest2d()
{
  BSHP<std::vector<Pt3d>> pts(new std::vector<Pt3d>());
  iSetupPts(*pts, true);

  BSHP<GmPtSearch> iPts = GmPtSearch::New(true);
  iPts->PtsToSearch(pts);
  TS_ASSERT_EQUALS(true, iPts->Is2D());

  Pt3d aPt(.5, 0, 0);
  std::vector<int> foundPts, requiredPts, optionalPts;
  iPts->NearestPtsToPt(aPt, 8, false, foundPts);
  requiredPts = {0, 1, 2, 3, 4, 6, 40, 41};
  std::sort(foundPts.begin(), foundPts.end());
  TS_ASSERT_EQUALS(8, foundPts.size());
  TS_ASSERT_POINTS_FOUND(requiredPts, optionalPts, foundPts);

  iPts->NearestPtsToPt(aPt, 4, true, foundPts);
  requiredPts = {0, 1, 2, 3, 4, 5, 6, 7, 9, 11, 40, 41, 42, 43, 61, 63};
  std::sort(foundPts.begin(), foundPts.end());
  TS_ASSERT_EQUALS(16, foundPts.size());
  TS_ASSERT_POINTS_FOUND(requiredPts, optionalPts, foundPts);

  aPt.x = -3.6;
  aPt.y = 2.5;
  iPts->NearestPtsToPt(aPt, 8, false, foundPts);
  requiredPts = {61, 65, 69, 73, 121, 125, 129, 133};
  std::sort(foundPts.begin(), foundPts.end());
  TS_ASSERT_EQUALS(8, foundPts.size());
  TS_ASSERT_POINTS_FOUND(requiredPts, optionalPts, foundPts);

  iPts->NearestPtsToPt(aPt, 4, true, foundPts);
  requiredPts = {1, 5, 9, 61, 65, 69, 73, 77, 121, 125, 129, 133, 137, 181, 185, 189};
  std::sort(foundPts.begin(), foundPts.end());
  TS_ASSERT_EQUALS(16, foundPts.size());
  TS_ASSERT_POINTS_FOUND(requiredPts, optionalPts, foundPts);

  aPt.x = -1;
  aPt.y = 0;
  iPts->NearestPtsToPt(aPt, 4, true, foundPts);
  requiredPts = {0, 1, 2, 3, 4, 5, 6, 7, 9, 11, 40, 41, 61, 63, 65, 67};
  std::sort(foundPts.begin(), foundPts.end());
  TS_ASSERT_EQUALS(16, foundPts.size());
  TS_ASSERT_POINTS_FOUND(requiredPts, optionalPts, foundPts);

  iPts->NearestPtsToPt(aPt, 4, true, foundPts);
  requiredPts = {0, 1, 2, 3, 4, 5, 6, 7, 9, 11, 40, 41, 61, 63, 65, 67};
  std::sort(foundPts.begin(), foundPts.end());
  TS_ASSERT_EQUALS(16, foundPts.size());
  TS_ASSERT_POINTS_FOUND(requiredPts, optionalPts, foundPts);
} // PtSearchUnitTests::testTest2d
//------------------------------------------------------------------------------
/// \brief testing tutorial data from GMS in 2d
//------------------------------------------------------------------------------
void PtSearchUnitTests::testTest2dTutData()
{
  BSHP<std::vector<Pt3d>> pts(new std::vector<Pt3d>());
  std::vector<Pt3d> ipts;
  std::vector<float> scalar, s, vBase;
  iGetPoints2(*pts, scalar, ipts);

  BSHP<GmPtSearch> iPts = GmPtSearch::New(true);
  iPts->PtsToSearch(pts);

  std::vector<int> foundPts, requiredPts, optionalPts;
  iPts->NearestPtsToPt(ipts[0], 16, false, foundPts);
  requiredPts = {0, 1, 2, 3, 4, 5, 6, 7, 8, 10, 11, 14, 15, 16, 19, 20};
  std::sort(foundPts.begin(), foundPts.end());
  TS_ASSERT_EQUALS(16, foundPts.size());
  TS_ASSERT_POINTS_FOUND(requiredPts, optionalPts, foundPts);
} // PtSearchUnitTests::testTest2dTutData
//------------------------------------------------------------------------------
/// \brief testing in 3d
//------------------------------------------------------------------------------
void PtSearchUnitTests::testTest3d()
{
  BSHP<std::vector<Pt3d>> pts(new std::vector<Pt3d>());
  iSetupPts(*pts, false);

  BSHP<GmPtSearch> iPts = GmPtSearch::New(false);
  iPts->PtsToSearch(pts);

  Pt3d aPt(0, 0, 0);
  std::vector<int> foundPts, requiredPts, optionalPts;
  iPts->NearestPtsToPt(aPt, 16, false, foundPts);
  requiredPts = {0, 1, 2, 3, 300, 301, 302, 303, 600, 601, 602, 603};
  optionalPts = {4, 5, 6, 7, 40, 41};
  TS_ASSERT_EQUALS(16, foundPts.size());
  TS_ASSERT_POINTS_FOUND(requiredPts, optionalPts, foundPts);
}
//------------------------------------------------------------------------------
/// \brief helper function for testing
/// \param pts: Points.
//------------------------------------------------------------------------------
static void iSetupPtsOctant(std::vector<Pt3d>& pts)
{
  // points along the axis
  pts.push_back(Pt3d(1, 0, 0));
  pts.push_back(Pt3d(2, 0, 0));
  pts.push_back(Pt3d(-1, 0, 0));
  pts.push_back(Pt3d(-2, 0, 0));
  pts.push_back(Pt3d(0, 1, 0));
  pts.push_back(Pt3d(0, 2, 0));
  pts.push_back(Pt3d(0, -1, 0));
  pts.push_back(Pt3d(0, -2, 0));
  pts.push_back(Pt3d(0, 0, 1));
  pts.push_back(Pt3d(0, 0, 2));
  pts.push_back(Pt3d(0, 0, -1));
  pts.push_back(Pt3d(0, 0, -2));

  // points in the octants
  // positive z octants
  pts.push_back(Pt3d(1, 1, 2));
  pts.push_back(Pt3d(2, 1, 2));
  pts.push_back(Pt3d(1, 2, 2));
  pts.push_back(Pt3d(1, 1, 3));

  pts.push_back(Pt3d(-1, 1, 2));
  pts.push_back(Pt3d(-2, 1, 2));
  pts.push_back(Pt3d(-1, 2, 2));
  pts.push_back(Pt3d(-1, 1, 3));

  pts.push_back(Pt3d(1, -1, 2));
  pts.push_back(Pt3d(2, -1, 2));
  pts.push_back(Pt3d(1, -2, 2));
  pts.push_back(Pt3d(1, -1, 3));

  pts.push_back(Pt3d(-1, -1, 2));
  pts.push_back(Pt3d(-2, -1, 2));
  pts.push_back(Pt3d(-1, -2, 2));
  pts.push_back(Pt3d(-1, -1, 3));

  // negative z octants
  pts.push_back(Pt3d(1, 1, -2));
  pts.push_back(Pt3d(2, 1, -2));
  pts.push_back(Pt3d(1, 2, -2));
  pts.push_back(Pt3d(1, 1, -3));

  pts.push_back(Pt3d(-1, 1, -2));
  pts.push_back(Pt3d(-2, 1, -2));
  pts.push_back(Pt3d(-1, 2, -2));
  pts.push_back(Pt3d(-1, 1, -3));

  pts.push_back(Pt3d(1, -1, -2));
  pts.push_back(Pt3d(2, -1, -2));
  pts.push_back(Pt3d(1, -2, -2));
  pts.push_back(Pt3d(1, -1, -3));

  pts.push_back(Pt3d(-1, -1, -2));
  pts.push_back(Pt3d(-2, -1, -2));
  pts.push_back(Pt3d(-1, -2, -2));
  pts.push_back(Pt3d(-1, -1, -3));

} // iSetupPtsOctant
//------------------------------------------------------------------------------
/// \brief testing octant searching in 3d
//------------------------------------------------------------------------------
void PtSearchUnitTests::testTest3dOct()
{
  BSHP<std::vector<Pt3d>> pts(new std::vector<Pt3d>());
  iSetupPtsOctant(*pts);

  BSHP<GmPtSearch> iPts = GmPtSearch::New(false);
  iPts->PtsToSearch(pts);

  Pt3d aPt;
  std::vector<int> foundPts;
  iPts->NearestPtsToPt(aPt, 4, true, foundPts);
  std::vector<int> requiredPts = {0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 20, 21, 22,
                                  23, 24, 28, 29, 30, 31, 32, 36, 37, 38, 39, 40, 41, 42, 43};
  std::vector<int> optionalPts = {33, 34};
  std::sort(foundPts.begin(), foundPts.end());
  TS_ASSERT_EQUALS(32, foundPts.size());
  TS_ASSERT_POINTS_FOUND(requiredPts, optionalPts, foundPts);

  iPts->NearestPtsToPt(aPt, 4, true, foundPts);
  std::sort(foundPts.begin(), foundPts.end());
  TS_ASSERT_EQUALS(32, foundPts.size());
  TS_ASSERT_POINTS_FOUND(requiredPts, optionalPts, foundPts);
} // PtSearchUnitTests::testTest3d
//------------------------------------------------------------------------------
/// \brief testing searching with activity
//------------------------------------------------------------------------------
void PtSearchUnitTests::testActivity2d()
{
  BSHP<std::vector<Pt3d>> pts(new std::vector<Pt3d>());
  iSetupPts(*pts, true);

  BSHP<GmPtSearch> iPts = GmPtSearch::New(true);
  iPts->PtsToSearch(pts);

  DynBitset activity_wrongSize;
  // this should do nothing because it is sized wrong
  iPts->SetActivity(activity_wrongSize);

  Pt3d aPt(.5, 0, 0);
  std::vector<int> foundPts, requiredPts, optionalPts;
  iPts->NearestPtsToPt(aPt, 8, false, foundPts);
  requiredPts = {0, 1, 2, 3, 4, 6, 40, 41};
  std::sort(foundPts.begin(), foundPts.end());
  TS_ASSERT_EQUALS(8, foundPts.size());
  TS_ASSERT_POINTS_FOUND(requiredPts, optionalPts, foundPts);

  DynBitset act(300);
  act.flip();
  act.set(2, 0);
  act.set(40, 0);
  iPts->SetActivity(act);
  iPts->NearestPtsToPt(aPt, 8, false, foundPts);
  requiredPts = {0, 1, 3, 4, 6, 41, 42};
  optionalPts = {7, 43};
  std::sort(foundPts.begin(), foundPts.end());
  TS_ASSERT_EQUALS(8, foundPts.size());
  TS_ASSERT_POINTS_FOUND(requiredPts, optionalPts, foundPts);
} // PtSearchUnitTests::testActivity2d
//------------------------------------------------------------------------------
/// \brief testing searching with activity
//------------------------------------------------------------------------------
void PtSearchUnitTests::testActivity3d()
{
  BSHP<std::vector<Pt3d>> pts(new std::vector<Pt3d>());
  iSetupPtsOctant(*pts);

  BSHP<GmPtSearch> iPts = GmPtSearch::New(false);
  iPts->PtsToSearch(pts);

  DynBitset activity_wrongSize;
  // this should do nothing because it is sized wrong
  iPts->SetActivity(activity_wrongSize);

  Pt3d aPt;
  std::vector<int> foundPts;
  iPts->NearestPtsToPt(aPt, 4, true, foundPts);
  std::vector<int> requiredPts = {0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 20, 21, 22,
                                  23, 24, 28, 29, 30, 31, 32, 36, 37, 38, 39, 40, 41, 42, 43};
  std::vector<int> optionalPts = {33, 34};
  std::sort(foundPts.begin(), foundPts.end());
  TS_ASSERT_EQUALS(32, foundPts.size());
  TS_ASSERT_POINTS_FOUND(requiredPts, optionalPts, foundPts);

  DynBitset act(44);
  act.flip();
  act.set(2, 0);
  act.set(40, 0);
  iPts->SetActivity(act);
  iPts->NearestPtsToPt(aPt, 4, true, foundPts);
  requiredPts = {0,  1,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 20, 21, 22,
                 23, 24, 28, 29, 30, 31, 32, 36, 37, 38, 39, 41, 42, 43};
  optionalPts = {13, 14, 25, 33, 34};
  std::sort(foundPts.begin(), foundPts.end());
  TS_ASSERT_EQUALS(31, foundPts.size());
  TS_ASSERT_POINTS_FOUND(requiredPts, optionalPts, foundPts);
} // PtSearchUnitTests::testActivity3d
//------------------------------------------------------------------------------
/// \brief testing Point within a distance to a specified point
//------------------------------------------------------------------------------
void PtSearchUnitTests::testPtsWithinDist()
{
  BSHP<std::vector<Pt3d>> pts(new std::vector<Pt3d>());
  *pts = {{0, 0, 0}, {1, 0, 0}, {2, 0, 0}, {1, 1, 0}, {1, -1, 0}};

  GmPtSearchImpl p(true);
  p.PtsToSearch(pts);

  std::vector<int> n;
  p.PtsWithinDistanceToPtInRtree(1, (*pts)[1], .99, n);
  std::vector<int> base;
  TS_ASSERT_EQUALS_VEC(base, n);
  base = {0, 2, 3, 4};
  p.PtsWithinDistanceToPtInRtree(1, (*pts)[1], 1.0, n);
  TS_ASSERT_EQUALS_VEC(base, n);
  base = {1, 3, 4};
  p.PtsWithinDistanceToPtInRtree(2, (*pts)[2], 1.0, n);
  TS_ASSERT_EQUALS_VEC(base, n);
}
//------------------------------------------------------------------------------
/// \brief testing VectorThatGrows functionality
//------------------------------------------------------------------------------
void PtSearchUnitTests::testVectorThatGrows()
{
  BSHP<std::vector<Pt3d>> pts(new std::vector<Pt3d>());
  *pts = {{0, 0, 0}, {1, 0, 0}, {2, 0, 0}, {1, 1, 0}, {1, -1, 0}};

  GmPtSearchImpl p(true);
  p.VectorThatGrowsToSearch(pts);

  // add a point
  int ptIdx;
  Pt3d pt(0, 1, 0);
  TS_ASSERT_EQUALS(false, p.AddPtToVectorIfUnique(pt, 1, ptIdx));
  TS_ASSERT_EQUALS(0, ptIdx);
  TS_ASSERT_EQUALS(true, p.AddPtToVectorIfUnique(pt, 1e-9, ptIdx));
  TS_ASSERT_EQUALS(5, ptIdx);
} // PtSearchUnitTests::testVectorThatGrows

//} // namespace xms
#endif // CXX_TEST
