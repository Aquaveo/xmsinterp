//------------------------------------------------------------------------------
/// \file
/// \ingroup interpolate_detail
/// \copyright (C) Copyright Aquaveo 2018. Distributed under FreeBSD License
/// (See accompanying file LICENSE or https://aqaveo.com/bsd/license.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------

// 1. Precompiled header

// 2. My header
#include <xmsinterp/interpolate/detail/InterpNatNeigh.h>

// 3. Standard Library Headers

// 4. External Library Headers
#pragma warning(push)
#pragma warning(disable : 4512) // boost code: no assignment operator
#include <boost/geometry.hpp>
#include <boost/geometry/index/rtree.hpp>
#include <boost/unordered_map.hpp>
#pragma warning(pop)

// 5. Shared Headers
#include <xmscore/math/math.h>
#include <xmscore/misc/XmError.h>
#include <xmscore/stl/utility.h>
#include <xmsgrid/geometry/geoms.h>
#include <xmsgrid/geometry/GmTriSearch.h>
#include <xmsinterp/interpolate/detail/NodalFunc.h>
#include <xmscore/misc/xmstype.h>

// 6. Non-shared Headers

//----- Forward declarations ---------------------------------------------------

//----- External globals -------------------------------------------------------

//----- Namespace declaration --------------------------------------------------
namespace xms
{
//----- Constants / Enumerations -----------------------------------------------

//----- Classes / Structs ------------------------------------------------------
/// \brief convenience struct for natural neighbor interpolation
struct nnOuterEdgeStruct
{
public:
  nnOuterEdgeStruct() { m_triIdx = m_edge[0] = m_edge[1] = XM_NONE; }
  int m_triIdx;   ///< index of this triangle
  int m_edge[2];  ///< pts that make up edge in CC order
  xms::Pt3d m_cc; ///< circumcenter of triangle made by interp pt with edge
};
//----- Internal functions -----------------------------------------------------

//----- Class / Function definitions -------------------------------------------
class InterpNatNeighImpl : public InterpNatNeigh
{
public:
  InterpNatNeighImpl(const std::vector<xms::Pt3d>& a_pts,
                     const std::vector<int>& a_tris,
                     const std::vector<float>& a_scalar,
                     GmTriSearch* a_triSearch);

  virtual float InterpToPt(const xms::Pt3d& a_pt) override;
  virtual void GetNeighbors(int a_ptIdx, std::vector<int>& a_neigh) override;
  virtual void SetNodalFunc(BSHP<NodalFunc> a_) override;
  virtual void RecalcNodalFunc() override;
  virtual void SetBlendWeights(bool a_) override;
  virtual std::string ToString() override;

  //
  void FillEdgeMap();
  void FillCenterVec();
  void EdgesFromTri(int a_triIdx, std::pair<int, int> a_edges[3]);
  void NeighTriFromTriIdx(int a_triIdx, std::vector<int>& a_tris);
  void GetNatNeighTriangles(const xms::Pt3d& a_pt, std::vector<int>& a_tris);
  double BlendFunc(double a_x);
  double ScalarFromNodalFunc(int a_ptIdx, const xms::Pt3d& a_loc);

  double HaleNnInterp(const xms::Pt3d& a_pt);
  void HaleNnVisitNeighbors(int a_tIdx,
                            const xms::Pt3d& a_pt,
                            std::vector<nnOuterEdgeStruct>& a_outerEdges,
                            std::map<int, double>& a_weights);
  int AdjacentTriangle(std::pair<int, int>& a_edge, int a_triIdx);
  bool PtInTriCircumCircle(const xms::Pt3d& a_pt, int a_tri);
  void HaleNnAddOuterEdge(std::vector<nnOuterEdgeStruct>& a_,
                          int a_tIdx,
                          int a_ptIdx0,
                          int a_ptIdx1,
                          const xms::Pt3d& a_pt);
  void HaleNnSortOuterEdges(std::vector<nnOuterEdgeStruct>& a_);
  void HaleNnOuterEdgesToWeights(std::vector<nnOuterEdgeStruct>& a_outerEdges,
                                 std::map<int, double>& a_weights);
  void NormalizeWeights(std::map<int, double>& a_weights);
  void BlendWeights(std::map<int, double>& a_weights);
  double ScalarFromWeights(std::map<int, double>& a_weights, const xms::Pt3d& a_pt);

private:
  /// typedef for long template
  typedef boost::unordered_map<std::pair<int, int>, std::pair<int, int>> MapEdges;

  const std::vector<xms::Pt3d>& m_pts; ///< points interpolating from
  const std::vector<int>& m_tris;      ///< triangles interpolating from
  const std::vector<float>& m_scalar;  ///< scalar interpolating from
  double m_tol;                        ///< tolerance for geometric floating point comparisons
  MapEdges m_edges;                    ///< map of triangle edges
  std::vector<xms::Pt3d> m_centers;    ///< circumcircle centers of triangles
  BSHP<NodalFunc> m_nf;                ///< Nodal function (constant, gradient plane, quadratic)
  GmTriSearch* m_triSearch;            ///< Spatial index for searching triangles
  bool m_blendWeights;                 ///< flag for blending interpolation weights
};

namespace
{
//------------------------------------------------------------------------------
/// \brief
//------------------------------------------------------------------------------
static std::pair<int, int> iMakePair(int a_first, int a_second)
{
  if (a_first < a_second)
    return std::make_pair(a_first, a_second);
  return std::make_pair(a_second, a_first);
} // iMakePair
} // unnamed namespace
//------------------------------------------------------------------------------
/// \brief Creates a Natural Neighbor Interpolation class
/// \param[in] a_pts Locations that are interpolated from
/// \param[in] a_tris Triangles connecting a_pts and defining neighbors
/// \param[in] a_scalar Scalars that are interpolated from
/// \param[in] a_triSearch Spatial index for searching triangles
/// \return A shared pointer to an instance of this class.
//------------------------------------------------------------------------------
BSHP<InterpNatNeigh> InterpNatNeigh::New(const std::vector<xms::Pt3d>& a_pts,
                                         const std::vector<int>& a_tris,
                                         const std::vector<float>& a_scalar,
                                         GmTriSearch* a_triSearch)
{
  BSHP<InterpNatNeighImpl> r(new InterpNatNeighImpl(a_pts, a_tris, a_scalar, a_triSearch));
  return r;
} // InterpNatNeigh::InterpNatNeigh
//------------------------------------------------------------------------------
/// \brief Constructor
//------------------------------------------------------------------------------
InterpNatNeigh::InterpNatNeigh()
{
} // InterpNatNeigh::InterpNatNeigh
//------------------------------------------------------------------------------
/// \brief Destructor
//------------------------------------------------------------------------------
InterpNatNeigh::~InterpNatNeigh()
{
} // InterpNatNeigh::~InterpNatNeigh

////////////////////////////////////////////////////////////////////////////////
/// \class InterpNatNeighImpl
/// \brief Performs natural neighbor interpolation to a location.
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/// \brief Consructor
/// \param[in] a_pts Locations that are interpolated from
/// \param[in] a_tris Triangles connecting a_pts and defining neighbors
/// \param[in] a_scalar Scalars that are interpolated from
/// \param[in] a_triSearch Spatial index for searching triangles
//------------------------------------------------------------------------------
InterpNatNeighImpl::InterpNatNeighImpl(const std::vector<xms::Pt3d>& a_pts,
                                       const std::vector<int>& a_tris,
                                       const std::vector<float>& a_scalar,
                                       GmTriSearch* a_triSearch)
: m_pts(a_pts)
, m_tris(a_tris)
, m_scalar(a_scalar)
, m_tol(1e-9)
, m_centers()
, m_nf()
, m_triSearch(a_triSearch)
, m_blendWeights(false)
{
  FillEdgeMap();
  FillCenterVec();
} // InterpNatNeighImpl::~InterpNatNeighImpl
//------------------------------------------------------------------------------
/// \brief Interpolates to a_pt
/// \param[in] a_pt The location to interpolate to
/// \return The interpolated value at a_pt
//------------------------------------------------------------------------------
float InterpNatNeighImpl::InterpToPt(const xms::Pt3d& a_pt)
{
  return (float)(HaleNnInterp(a_pt));
} // InterpNatNeighImpl::InterpToPt
//------------------------------------------------------------------------------
/// \brief Finds neighboring triangles for the pt at a_ptIdx index in m_pts
/// \param[in] a_ptIdx Index to the point where we want to find its neighbor
/// triangles
/// \param[out] a_neigh Vector of triangle indexes
//------------------------------------------------------------------------------
void InterpNatNeighImpl::GetNeighbors(int a_ptIdx, std::vector<int>& a_neigh)
{
  const xms::Pt3d& p(m_pts[a_ptIdx]);
  a_neigh.resize(0);
  std::vector<int> tris;
  GetNatNeighTriangles(p, tris);

  std::set<int> ptIdxs;
  // now check to see if the point is inside the cirumcircle
  for (size_t i = 0; i < tris.size(); ++i)
  {
    for (int j = 0; j < 3; ++j)
      ptIdxs.insert(m_tris[tris[i] + j]);
  }
  ptIdxs.erase(a_ptIdx);
  a_neigh.assign(ptIdxs.begin(), ptIdxs.end());
} // InterpNatNeighImpl::GetNeighbors
//------------------------------------------------------------------------------
/// \brief Set the precomputed nodal function
/// \param a_ The nodal function class.
//------------------------------------------------------------------------------
void InterpNatNeighImpl::SetNodalFunc(BSHP<NodalFunc> a_)
{
  m_nf = a_;
} // InterpNatNeighImpl::SetNodalFunc
//------------------------------------------------------------------------------
/// \brief Recalculates the nodal function. This happens when the scalars change
//------------------------------------------------------------------------------
void InterpNatNeighImpl::RecalcNodalFunc()
{
  if (m_nf)
    m_nf->ComputeNodalFuncs();
} // InterpNatNeighImpl::RecalcNodalFunc
//------------------------------------------------------------------------------
/// \brief Turns on a flag for using a blending function when calculating weights
/// \param[in] a_ flag to turn on weight blending
//------------------------------------------------------------------------------
void InterpNatNeighImpl::SetBlendWeights(bool a_)
{
  m_blendWeights = a_;
} // InterpNatNeighImpl::SetBlendWeights
//------------------------------------------------------------------------------
/// \brief serializes the class to a string
/// \return The string.
//------------------------------------------------------------------------------
std::string InterpNatNeighImpl::ToString()
{
  std::stringstream ss;
  VecToStream(ss, m_pts, "pts");
  VecToStream(ss, m_tris, "tris");
  VecToStream(ss, m_scalar, "scalar");
  ss << m_tol << "=tol ";
  // typedef boost::unordered_map<std::pair<int, int>, std::pair<int, int>> MapEdges;
  for (MapEdges::iterator it = m_edges.begin(); it != m_edges.end(); ++it)
  {
    ss << it->first.first << " " << it->first.second << " " << it->second.first << " "
       << it->second.second << " ";
  }
  ss << "=edges ";
  VecToStream(ss, m_centers, "centers");
  ss << m_blendWeights << "=blendWeights ";
  return ss.str();
} // InterpNatNeighImpl::ToString
//------------------------------------------------------------------------------
/// \brief Creates a map of edges for natural neighbor calculations
//------------------------------------------------------------------------------
void InterpNatNeighImpl::FillEdgeMap()
{
  if (m_tris.empty())
    return;
#if _DEBUG
  std::pair<int, int> pr;
#endif
  for (size_t i = 0; i < m_tris.size(); i += 3)
  {
    int k = (int)i;
    std::pair<int, int> tp[3];
    EdgesFromTri((int)i, tp);
    for (int j = 0; j < 3; ++j)
    {
      if (m_edges.find(tp[j]) == m_edges.end())
      {
        m_edges[tp[j]].first = k;
        m_edges[tp[j]].second = XM_NONE;
      }
      else
        m_edges[tp[j]].second = k;
#if _DEBUG
      pr = m_edges[tp[j]];
#endif
    }
  }
} // InterpNatNeighImpl::FillEdgeMap
//------------------------------------------------------------------------------
/// \brief calculate the circumcircle centers for all triangles
//------------------------------------------------------------------------------
void InterpNatNeighImpl::FillCenterVec()
{
  if (m_pts.empty() || m_tris.empty())
    return;

  xms::Pt3d pt, ptMin, ptMax;
  ptMin = ptMax = m_pts[0];
  for (size_t i = 1; i < m_pts.size(); ++i)
  {
    ptMin.x = Mmin(m_pts[i].x, ptMin.x);
    ptMin.y = Mmin(m_pts[i].y, ptMin.y);
    ptMax.x = Mmax(m_pts[i].x, ptMax.x);
    ptMax.y = Mmax(m_pts[i].y, ptMax.y);
  }
  m_tol = gmXyDistance(ptMin, ptMax) / 1e9;
  m_centers.assign(m_tris.size() / 3, xms::Pt3d());
  for (size_t i = 0, cnt = 0; i < m_tris.size(); i += 3, ++cnt)
  {
    const xms::Pt3d &p0(m_pts[m_tris[i]]), &p1(m_pts[m_tris[i + 1]]), &p2(m_pts[m_tris[i + 2]]);
    // calculate the center (x,y) and radius squared (z) of the circumcenter
    // of the triangle
    gmCircumcircleWithTol(&p0, &p1, &p2, &pt.x, &pt.y, &pt.z, m_tol);
    m_centers[cnt] = pt;
  }
} // InterpNatNeighImpl::FillCenterVec
//------------------------------------------------------------------------------
/// \brief fill an array of edges from a triangle index
/// \param[in] a_triIdx Index of the triangle
/// \param[out] a_edges Array of triangle edges filled by this method
//------------------------------------------------------------------------------
void InterpNatNeighImpl::EdgesFromTri(int a_triIdx, std::pair<int, int> a_edges[3])
{
  a_edges[0] = iMakePair(m_tris[a_triIdx], m_tris[a_triIdx + 1]);
  a_edges[1] = iMakePair(m_tris[a_triIdx + 1], m_tris[a_triIdx + 2]);
  a_edges[2] = iMakePair(m_tris[a_triIdx + 2], m_tris[a_triIdx]);
} // InterpNatNeighImpl::EdgesFromTri
//------------------------------------------------------------------------------
/// \brief Gets the adjacent triangles to a_triIdx
/// \param[in] a_triIdx Index of the triangle
/// \param[out] a_tris Indexes of adjacent triangles
//------------------------------------------------------------------------------
void InterpNatNeighImpl::NeighTriFromTriIdx(int a_triIdx, std::vector<int>& a_tris)
{
  a_tris.resize(0);
  std::pair<int, int> edges[3];
  EdgesFromTri(a_triIdx, edges);
  std::set<int> setTris;
  for (int i = 0; i < 3; ++i)
  {
    const std::pair<int, int>& pTri = m_edges[edges[i]];
    setTris.insert(pTri.first);
    if (XM_NONE != pTri.second)
      setTris.insert(pTri.second);
  }
  setTris.erase(a_triIdx);
  a_tris.assign(setTris.begin(), setTris.end());
} // InterpNatNeighImpl::NeighTriFromTriIdx
//------------------------------------------------------------------------------
/// \brief  Given a pt this will find the "natural neighbor" triangles. First,
/// we find the triangle that the pt is in. Then we visit the neighbor
/// triangles to see if the point is inside of its circumcircle. If it is then
/// we will also visit the neighbors of that triangle and so on.
/// \param[in] a_pt The location to find the natural neighbor triangles
/// \param[out] a_tris The triangle indexes that are natural neighbors to a_pt
//------------------------------------------------------------------------------
void InterpNatNeighImpl::GetNatNeighTriangles(const xms::Pt3d& a_pt, std::vector<int>& a_tris)
{
  a_tris.resize(0);
  if (!m_triSearch)
    return;
  int tIdx = m_triSearch->TriContainingPt(a_pt);
  if (XM_NONE == tIdx)
    return;

  a_tris.push_back(tIdx);
  std::vector<int> visited(m_tris.size(), 0);
  visited[tIdx] = 1;
  for (size_t j = 0; j < a_tris.size(); ++j)
  {
    tIdx = a_tris[j];
    std::vector<int> vals;
    NeighTriFromTriIdx(tIdx, vals);
    xms::Pt3d center;
    double rSquared, distSquared;
    std::set<int> ptIdxs;
    // now check to see if the point is inside the circumcircle
    for (size_t i = 0; i < vals.size(); ++i)
    {
      size_t tri = vals[i];
      if (visited[tri])
        continue;
      visited[tri] = 1;
      distSquared = gmXyDistanceSquared(m_centers[tri / 3], a_pt);
      rSquared = m_centers[tri / 3].z;
      if (distSquared <= rSquared)
        a_tris.push_back((int)tri);
    }
  }
} // InterpNatNeighImpl::GetNatNeighTriangles
//------------------------------------------------------------------------------
/// \brief Blending function to smooth interpolation weights refactored from GMS
/// \param x interpolation weight
/// \return The blended value.
//------------------------------------------------------------------------------
double InterpNatNeighImpl::BlendFunc(double x)
{
  return ((-2.0 * x * x * x) + (3.0 * x * x));
} // InterpNatNeighImpl::BlendFunc
//------------------------------------------------------------------------------
/// \brief calculates the scalar using the nodal function
/// \param[in] a_ptIdx Index to point in m_pts
/// \param[in] a_loc Location being interpolated to
/// \return the scalar adjusted by the nodal function
//------------------------------------------------------------------------------
double InterpNatNeighImpl::ScalarFromNodalFunc(int a_ptIdx, const xms::Pt3d& a_loc)
{
  double rval((double)m_scalar[a_ptIdx]);
  // get value from the nodal function
  if (m_nf)
    rval = m_nf->ScalarFromPtIdx(a_ptIdx, a_loc);
  return rval;
} // InterpNatNeighImpl::ScalarFromNodalFunc
//------------------------------------------------------------------------------
/// \brief Implementation of natural neighbor interpolation taken from this
/// paper
/// A stable and fast implementation of natural neighbor interpolation
/// Luming Liang & Dave Hale
/// Center for Wave Phenomena, Colorado School of Mines,Golden, CO 80401, USA
/// April 20, 2010
/// \param[in] a_pt The location being interpolated to
/// \return The interpolated value at a_pt
//------------------------------------------------------------------------------
double InterpNatNeighImpl::HaleNnInterp(const xms::Pt3d& a_pt)
{
  double rval(0);
  if (!m_triSearch)
    return rval;
  int tIdx = m_triSearch->TriContainingPt(a_pt);
  if (XM_NONE == tIdx)
    return rval;

  std::map<int, double> weights;
  std::vector<nnOuterEdgeStruct> outerEdges;
  // search through all neighbors
  HaleNnVisitNeighbors(tIdx, a_pt, outerEdges, weights);
  // sort outer edges into a loop
  HaleNnSortOuterEdges(outerEdges);
  // process outer edges
  HaleNnOuterEdgesToWeights(outerEdges, weights);

  NormalizeWeights(weights);
  if (m_blendWeights)
    BlendWeights(weights);
  rval = ScalarFromWeights(weights, a_pt);
  return rval;
} // InterpNatNeighImpl::HaleNnInterp
//------------------------------------------------------------------------------
/// \brief Visit the neighbors to a_pt and calculate the interpolation weight
/// for the natural neighbors
/// \param[in] a_tIdx Starting triangle index
/// \param[in] a_pt Location used to find natural neighbors
/// \param[out] a_outerEdges Outer edges for a_pt
/// \param[out] a_weights Interpolation weight for neighbor triangle
//------------------------------------------------------------------------------
void InterpNatNeighImpl::HaleNnVisitNeighbors(int a_tIdx,
                                              const xms::Pt3d& a_pt,
                                              std::vector<nnOuterEdgeStruct>& a_outerEdges,
                                              std::map<int, double>& a_weights)
{
  std::set<std::pair<int, int>> processedEdges;
  std::vector<int> tris(1, a_tIdx);
  for (size_t t = 0; t < tris.size(); ++t)
  { // visit adjacent triangles
    std::pair<int, int> edge;
    int tIdx = tris[t];
    for (int i = 0; i < 3; ++i)
    {
      int ptIdx = m_tris[tIdx + i];
      int nextTriPtIdx = i == 2 ? 0 : i + 1;
      int ptIdx1 = m_tris[tIdx + nextTriPtIdx];
      edge = iMakePair(ptIdx, ptIdx1);
      // already processed this edge
      if (processedEdges.find(edge) != processedEdges.end())
        continue;

      // see if there is an adjacent triangle and if it is a natural neighbor
      int adjTri = AdjacentTriangle(edge, tIdx);
      if (XM_NONE == adjTri || !PtInTriCircumCircle(a_pt, adjTri))
      { // no adjtri or not a natural neighbor. This is an outer edge.
        HaleNnAddOuterEdge(a_outerEdges, tIdx, ptIdx, ptIdx1, a_pt);
        continue;
      }

      // adjacent triangle is a natural neighbor. Calculate area contribution.
      // cross the circumcenters of the 2 triangles.
      const xms::Pt3d &tCC(m_centers[tIdx / 3]), adjCC(m_centers[adjTri / 3]);
      double cross = (tCC.x * adjCC.y) - (tCC.y * adjCC.x);
      a_weights[ptIdx] -= cross;
      a_weights[ptIdx1] += cross;

      tris.push_back(adjTri);
      // add to processed edges
      processedEdges.insert(edge);
    }
  }
} // InterpNatNeighImpl::HaleNnVisitNeighbors
//------------------------------------------------------------------------------
/// \brief Gets the triangle adjacent to a_edge
/// \param[in] a_edge A triangle edge
/// \param[in] a_triIdx Triangle index
/// \return the index to the adjacent triangle. If there is no adjacent triangle
/// then the return value is XM_NONE.
//------------------------------------------------------------------------------
int InterpNatNeighImpl::AdjacentTriangle(std::pair<int, int>& a_edge, int a_triIdx)
{
  int rval = XM_NONE;
  boost::unordered_map<std::pair<int, int>, std::pair<int, int>>::iterator it(m_edges.find(a_edge));
  if (it != m_edges.end())
  {
    rval = it->second.first;
    if (a_triIdx == rval)
      rval = it->second.second;
  }
  return rval;
} // InterpNatNeighImpl::AdjacentTriangle
//------------------------------------------------------------------------------
/// \brief Determines if a point is inside of a triangle circumcircle
/// \param[in] a_pt The location to test
/// \param[in] a_tri Index to the triangle
/// \return true if the point is inside the circumcircle
//------------------------------------------------------------------------------
bool InterpNatNeighImpl::PtInTriCircumCircle(const xms::Pt3d& a_pt, int a_tri)
{
  const xms::Pt3d& cc = m_centers[a_tri / 3];
  double d2 = gmXyDistanceSquared(a_pt, cc);
  if (d2 < cc.z)
    return true;
  return false;
} // InterpNatNeighImpl::PtInTriCircumCircle
//------------------------------------------------------------------------------
/// \brief Adds a nnOuterEdgeStruct to a vector
/// \param[out] a_ Vector of outer edges
/// \param[in] a_tIdx Triangle index
/// \param[in] a_ptIdx0 Point index for first point on edge of triangle a_tIdx
/// \param[in] a_ptIdx1 Point index for second point on edge of triangle a_tIdx
/// \param[in] a_pt Location used to find this outer edge
//------------------------------------------------------------------------------
void InterpNatNeighImpl::HaleNnAddOuterEdge(std::vector<nnOuterEdgeStruct>& a_,
                                            int a_tIdx,
                                            int a_ptIdx0,
                                            int a_ptIdx1,
                                            const xms::Pt3d& a_pt)
{
  nnOuterEdgeStruct n;
  n.m_triIdx = a_tIdx;
  n.m_edge[0] = a_ptIdx0;
  n.m_edge[1] = a_ptIdx1;
  gmCircumcircleWithTol(&m_pts[a_ptIdx0], &m_pts[a_ptIdx1], &a_pt, &n.m_cc.x, &n.m_cc.y, &n.m_cc.z,
                        m_tol);
  a_.push_back(n);
} // InterpNatNeighImpl::HaleNnAddOuterEdge
//------------------------------------------------------------------------------
/// \brief Sorts the outer edges
/// \param[out] a_ Vector of outer edges
//------------------------------------------------------------------------------
void InterpNatNeighImpl::HaleNnSortOuterEdges(std::vector<nnOuterEdgeStruct>& a_)
{
  std::vector<nnOuterEdgeStruct> e(a_);
  for (size_t i = 1; i < a_.size(); ++i)
  {
    for (size_t j = 1; j < e.size(); ++j)
    {
      if (e[j].m_edge[0] == a_[i - 1].m_edge[1])
        a_[i] = e[j];
    }
  }
} // InterpNatNeighImpl::HaleNnSortOuterEdges
//------------------------------------------------------------------------------
/// \brief Calculates weights from the outer edges
/// \param[in] a_outerEdges The outer edges
/// \param[out] a_weights Weight calculated for each outer edge
//------------------------------------------------------------------------------
void InterpNatNeighImpl::HaleNnOuterEdgesToWeights(std::vector<nnOuterEdgeStruct>& a_outerEdges,
                                                   std::map<int, double>& a_weights)
{
  for (size_t i = 0; i < a_outerEdges.size(); ++i)
  { // get the current edge and the next edge
    nnOuterEdgeStruct& curr(a_outerEdges[i]);
    size_t nextIdx = i + 1 == a_outerEdges.size() ? 0 : i + 1;
    nnOuterEdgeStruct& next(a_outerEdges[nextIdx]);

    int ptIdx = curr.m_edge[1];
    // cross the original tri cc with the tri cc formed by curr edge and the
    // interpolation point
    {
      xms::Pt3d &o1(m_centers[curr.m_triIdx / 3]), &c1(curr.m_cc);
      double cross = (o1.x * c1.y) - (o1.y * c1.x);
      a_weights[ptIdx] += cross;
      a_weights[curr.m_edge[0]] -= cross;
    }

    // cross tri cc formed by curr edge and interp pt with tri cc formed by
    // next edge and interp pt
    {
      xms::Pt3d &o1(curr.m_cc), &c1(next.m_cc);
      double cross = (o1.x * c1.y) - (o1.y * c1.x);
      a_weights[ptIdx] += cross;
    }
  }
} // InterpNatNeighImpl::HaleNnOuterEdgesToWeights
//------------------------------------------------------------------------------
/// \brief Normalizes the weights so that they sum to 1.0
/// \param[out] a_weights The interpolation weights
//------------------------------------------------------------------------------
void InterpNatNeighImpl::NormalizeWeights(std::map<int, double>& a_weights)
{
  double sum(0);
  std::map<int, double>::iterator it(a_weights.begin());
  for (; it != a_weights.end(); ++it)
    sum += it->second;
  for (it = a_weights.begin(); it != a_weights.end(); ++it)
    it->second /= sum;
} // InterpNatNeighImpl::NormalizeWeights
//------------------------------------------------------------------------------
/// \brief Blends the weights using BlendFunc. Smooths the interpolation.
/// \param[out] a_weights The interpolation weights
//------------------------------------------------------------------------------
void InterpNatNeighImpl::BlendWeights(std::map<int, double>& a_weights)
{
  double sum(0);
  std::map<int, double>::iterator it(a_weights.begin());
  for (; it != a_weights.end(); ++it)
  {
    it->second = BlendFunc(it->second);
    sum += it->second;
  }
  for (it = a_weights.begin(); it != a_weights.end(); ++it)
    it->second /= sum;
} // InterpNatNeighImpl::NormalizeWeights
//------------------------------------------------------------------------------
/// \brief Computes a scalar at a_pt based on the interpolation weights
/// \param[in] a_weights The interpolation weights
/// \param[in] a_pt The location interpolated to.
/// \return The interpolated value at a_pt.
//------------------------------------------------------------------------------
double InterpNatNeighImpl::ScalarFromWeights(std::map<int, double>& a_weights,
                                             const xms::Pt3d& a_pt)
{
  double rval(0);
  std::map<int, double>::iterator it(a_weights.begin());
  for (; it != a_weights.end(); ++it)
  {
    rval += it->second * ScalarFromNodalFunc(it->first, a_pt);
  }
  return rval;
} // InterpNatNeighImpl::NormalizeWeights

} // namespace xms

#ifdef CXX_TEST
////////////////////////////////////////////////////////////////////////////////

#include <xmsinterp/interpolate/detail/InterpNatNeigh.t.h>

#include <xmscore/testing/TestTools.h>

namespace
{
static void iGetPtsTris(std::vector<xms::Pt3d>& a_pts,
                        std::vector<int>& a_tris,
                        std::vector<float>& a_scalar)
{
  a_pts = {{26, 74, 5},  {15, 31, 8},  {60, -3, 4}, {78, 78, 7},
           {56, 64, 10}, {75, 45, 11}, {43, 22, 2}};
  a_tris = {0, 4, 3, 0, 1, 4, 4, 5, 3, 4, 6, 5, 4, 1, 6, 1, 2, 6, 2, 5, 6};
  a_scalar = {5.0f, 8.0f, 4.0f, 7.0f, 10.0f, 11.0f, 2.0f};
} // iGetPtsTris
} // unnamed namespace

// namespace xms {
using namespace xms;
////////////////////////////////////////////////////////////////////////////////
/// \class InterpNatNeighUnitTests
/// \brief Tester for the InterpNatNeigh class
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/// \brief testing class creation
//------------------------------------------------------------------------------
void InterpNatNeighUnitTests::testCreateClass()
{
  std::vector<xms::Pt3d> pts;
  std::vector<int> tris;
  std::vector<float> scalar;
  BSHP<InterpNatNeigh> p = InterpNatNeigh::New(pts, tris, scalar, nullptr);
} // InterpNatNeighUnitTests::testCreateClass
//------------------------------------------------------------------------------
/// \brief testing getting the natural neighbor triangles
//------------------------------------------------------------------------------
void InterpNatNeighUnitTests::testGetTris()
{
  xms::Pt3d loc(46, 27, 0);
  BSHP<std::vector<xms::Pt3d>> pts(new std::vector<xms::Pt3d>());
  BSHP<std::vector<int>> tris(new std::vector<int>());
  std::vector<int> tIdxs;
  std::vector<float> scalar;
  iGetPtsTris(*pts, *tris, scalar);
  BSHP<GmTriSearch> ts = GmTriSearch::New();
  ts->TrisToSearch(pts, tris);
  InterpNatNeighImpl p(*pts, *tris, scalar, ts.get());
  p.GetNatNeighTriangles(loc, tIdxs);
  std::vector<int> nBase = {9, 12, 18, 3};
  TS_ASSERT_EQUALS_VEC(nBase, tIdxs);
} // InterpNatNeighUnitTests::testGetTris
//------------------------------------------------------------------------------
/// \brief testing getting the natural neighbor triangles
//------------------------------------------------------------------------------
void InterpNatNeighUnitTests::testGetNeighbors()
{
  BSHP<std::vector<xms::Pt3d>> pts(new std::vector<xms::Pt3d>());
  BSHP<std::vector<int>> tris(new std::vector<int>());
  std::vector<int> tIdxs;
  std::vector<float> scalar;
  iGetPtsTris(*pts, *tris, scalar);
  BSHP<GmTriSearch> ts = GmTriSearch::New();
  ts->TrisToSearch(pts, tris);
  InterpNatNeighImpl p(*pts, *tris, scalar, ts.get());
  p.GetNeighbors(6, tIdxs);
  std::vector<int> nBase = {1, 2, 4, 5};
  TS_ASSERT_EQUALS_VEC(nBase, tIdxs);
} // InterpNatNeighUnitTests::testGetNeighbors
//------------------------------------------------------------------------------
/// \brief testing sorting outer edges for the Hale NN method
//------------------------------------------------------------------------------
void InterpNatNeighUnitTests::testHaleNnSortOuterEdges()
{
  std::vector<nnOuterEdgeStruct> edges(5);
  edges[0].m_edge[0] = 5;
  edges[0].m_edge[1] = 1;
  edges[1].m_edge[0] = 3;
  edges[1].m_edge[1] = 4;
  edges[2].m_edge[0] = 2;
  edges[2].m_edge[1] = 3;
  edges[3].m_edge[0] = 4;
  edges[3].m_edge[1] = 5;
  edges[4].m_edge[0] = 1;
  edges[4].m_edge[1] = 2;
  BSHP<std::vector<xms::Pt3d>> pts(new std::vector<xms::Pt3d>());
  BSHP<std::vector<int>> tris(new std::vector<int>());
  std::vector<int> tIdxs;
  std::vector<float> scalar;
  iGetPtsTris(*pts, *tris, scalar);
  BSHP<GmTriSearch> ts = GmTriSearch::New();
  ts->TrisToSearch(pts, tris);
  InterpNatNeighImpl p(*pts, *tris, scalar, ts.get());
  p.HaleNnSortOuterEdges(edges);
  int base[5][2] = {{5, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 5}};
  for (size_t i = 0; i < edges.size(); ++i)
  {
    TS_ASSERT_EQUALS(base[i][0], edges[i].m_edge[0]);
  }
} // InterpNatNeighUnitTests::testHaleNnSortOuterEdges
//------------------------------------------------------------------------------
/// \brief testing the Hale NN interpolation
//------------------------------------------------------------------------------
void InterpNatNeighUnitTests::testHaleNnInterp()
{
  BSHP<std::vector<xms::Pt3d>> pts(new std::vector<xms::Pt3d>());
  BSHP<std::vector<int>> tris(new std::vector<int>());
  std::vector<int> tIdxs;
  std::vector<float> scalar;
  iGetPtsTris(*pts, *tris, scalar);
  BSHP<GmTriSearch> ts = GmTriSearch::New();
  ts->TrisToSearch(pts, tris);
  InterpNatNeighImpl p(*pts, *tris, scalar, ts.get());
  std::vector<xms::Pt3d> iPts = {
    {36, 42, 0}, {39, 17, 0}, {64, 27, 0}, {55, 41, 0},
    {72, 64, 0}, {52, 73, 0}, {33, 60, 0}, {65.9404706, 54.0595293, 0}};

  std::vector<double> iVals(iPts.size(), 0);
  for (size_t i = 0; i < iPts.size(); ++i)
    iVals[i] = p.HaleNnInterp(iPts[i]);
  std::vector<double> nBase = {6.0669168950546712, 4.6727605118829985, 7.1629642928188586,
                               6.9812628044431957, 8.9941520467836238, 6.9374999999999991,
                               6.6593131032057444, 10.058329876949339};
  TS_ASSERT_DELTA_VEC(nBase, iVals, 1e-10);
} // InterpNatNeighUnitTests::testHaleNnInterp

//} // namespace xms
#endif // CXX_TEST
