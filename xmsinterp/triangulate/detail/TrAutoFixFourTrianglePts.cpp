//------------------------------------------------------------------------------
/// \file
/// \ingroup triangulate_detail
/// \copyright (C) Copyright Aquaveo 2018. Distributed under the xmsng
///  Software License, Version 1.0. (See accompanying file
///  LICENSE_1_0.txt or copy at http://www.aquaveo.com/xmsng/LICENSE_1_0.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------

// 1. Precompiled header

// 2. My own header
#include <xmsinterp/triangulate/detail/TrAutoFixFourTrianglePts.h>

// 3. Standard library headers
#include <map>

// 4. External library headers

// 5. Shared code headers
#include <xmscore/math/math.h>
#include <xmscore/misc/XmError.h>
#include <xmscore/misc/Observer.h>
#include <xmsinterp/triangulate/TrTin.h>

// 6. Non-shared code headers

//----- Forward declarations ---------------------------------------------------

//----- External globals -------------------------------------------------------

//----- Namespace declaration --------------------------------------------------

namespace xms
{
//----- Constants / Enumerations -----------------------------------------------

//----- Classes / Structs ------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
class TrAutoFixFourTrianglePtsImpl : public TrAutoFixFourTrianglePts
{
public:
  TrAutoFixFourTrianglePtsImpl();
  virtual ~TrAutoFixFourTrianglePtsImpl();

public:
  virtual void Fix(BSHP<TrTin> a_tin) override;
  //------------------------------------------------------------------------------
  /// \brief Set the observer to use for feedback while processing.
  /// \param a_: The observer.
  //------------------------------------------------------------------------------
  virtual void SetObserver(BSHP<Observer> a_) override { m_observer = a_; }
  //------------------------------------------------------------------------------
  /// \brief Set the indexes of points that can not be deleted
  /// \param[in] a_ptIdx : The indexes of points that can't be deleted
  //------------------------------------------------------------------------------
  virtual void SetUndeleteablePtIdxs(VecInt& a_ptIdx) override
  {
    m_noDelete.clear();
    m_noDelete.insert(a_ptIdx.begin(), a_ptIdx.end());
  }

  void FillAdjTrisIfNeeded();
  void GetPtsAndTrisToDelete();
  std::pair<int, int> EdgeOppositePt(int a_ptIdx, int a_tri, VecInt& a_tris);
  void MakeTwoNewTriangles(VecPt3d& a_pts, std::map<int, int> a_edges, int a_tris[2][3]);
  void ReplaceTriangle(int& a_tIdx, int a_newTri[3], VecInt& a_tris);
  void RenumberPts();
  void RenumberTris();
  void RemovePts();
  void RemoveTris();
  void ClearAdjTrisIfCreated();

private:
private:
  BSHP<TrTin> m_tin;         ///< tin
  BSHP<Observer> m_observer; ///< Observer
  bool m_removeAdjTris;      ///< flag to remove adjacent tris when done
  SetInt m_ptsToDelete;      ///< points in the TIN that will be deleted
  SetInt m_trisToDelete;     ///< triangles that will be deleted
  SetInt m_noDelete;         ///< indexes of points that can't be deleted
};                           // class TrOuterTriangleDeleterImpl

//----- Internal functions -----------------------------------------------------

//----- Class / Function definitions -------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// \class TrAutoFixFourTrianglePtsImpl
/// \brief Used to delete points that are connected to 4 triangles and then
/// retriangulate the void.
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/// \brief constructor
//------------------------------------------------------------------------------
TrAutoFixFourTrianglePtsImpl::TrAutoFixFourTrianglePtsImpl()
: TrAutoFixFourTrianglePts()
, m_tin()
, m_observer()
, m_removeAdjTris(false)
, m_ptsToDelete()
, m_trisToDelete()
{
} // TrAutoFixFourTrianglePtsImpl::TrAutoFixFourTrianglePtsImpl
//------------------------------------------------------------------------------
/// \brief
//------------------------------------------------------------------------------
TrAutoFixFourTrianglePtsImpl::~TrAutoFixFourTrianglePtsImpl()
{
}
//------------------------------------------------------------------------------
/// \brief Removes any points in the tin that are connected to only 4 triangles.
/// These points usually cause poorly formed elements in the mesher.
/// \param[in,out] a_tin: The tin to be modified.
//------------------------------------------------------------------------------
void TrAutoFixFourTrianglePtsImpl::Fix(BSHP<TrTin> a_tin)
{
  m_tin = a_tin;
  FillAdjTrisIfNeeded();
  GetPtsAndTrisToDelete();
  RenumberPts();
  RenumberTris();
  RemovePts();
  RemoveTris();
  ClearAdjTrisIfCreated();
} // TrOuterTriangleDeleterImpl::Fix
//------------------------------------------------------------------------------
/// \brief Makes sure the adjacent triangles exist in the TIN
//------------------------------------------------------------------------------
void TrAutoFixFourTrianglePtsImpl::FillAdjTrisIfNeeded()
{
  VecInt2d& adjTris(m_tin->TrisAdjToPts());
  if (adjTris.empty())
  {
    m_tin->BuildTrisAdjToPts();
    m_removeAdjTris = true;
  }
} // TrAutoFixFourTrianglePtsImpl::FillAdjTrisIfNeeded
//------------------------------------------------------------------------------
/// \brief Finds and removes the points connected to 4 triangles
//------------------------------------------------------------------------------
void TrAutoFixFourTrianglePtsImpl::GetPtsAndTrisToDelete()
{
  VecInt2d& adjTris(m_tin->TrisAdjToPts());
  VecInt& tris(m_tin->Triangles());
  VecPt3d& pts(m_tin->Points());
  int npts = static_cast<int>(adjTris.size());
  auto itEnd = m_noDelete.end();
  for (int i = 0; i < npts; ++i)
  {
    auto it = m_noDelete.find(i);
    if (4 == adjTris[i].size() && it == itEnd)
    {
      m_ptsToDelete.insert(i);
      m_trisToDelete.insert(adjTris[i][2]);
      m_trisToDelete.insert(adjTris[i][3]);
      std::map<int, int> mapii;
      for (int t = 0; t < 4; ++t)
      {
        mapii.insert(EdgeOppositePt(i, adjTris[i][t], tris));
      }
      int newTris[2][3];
      MakeTwoNewTriangles(pts, mapii, newTris);
      ReplaceTriangle(adjTris[i][0], newTris[0], tris);
      ReplaceTriangle(adjTris[i][1], newTris[1], tris);
    }
  }
} // TrAutoFixFourTrianglePtsImpl::RemovePts
//------------------------------------------------------------------------------
/// \brief Finds and removes the points connected to 4 triangles
/// \param[in] a_ptIdx : A point on a triangle
/// \param[in] a_tri : The index to the triangle
/// \param[in] a_tris : Array of triangles
/// \return a pair of ints that are the point indexes to the points on the
/// edge opposite a_ptIdx
//------------------------------------------------------------------------------
std::pair<int, int> TrAutoFixFourTrianglePtsImpl::EdgeOppositePt(int a_ptIdx,
                                                                 int a_tri,
                                                                 VecInt& a_tris)
{
  int tIdx = a_tri * 3;
  int* tPts = &a_tris[tIdx];
  std::pair<int, int> rval(tPts[0], tPts[1]);
  if (tPts[0] == a_ptIdx)
  {
    rval.first = tPts[1];
    rval.second = tPts[2];
  }
  else if (tPts[1] == a_ptIdx)
  {
    rval.first = tPts[2];
    rval.second = tPts[0];
  }
  return rval;
} // TrAutoFixFourTrianglePtsImpl::EdgeOppositePt
//------------------------------------------------------------------------------
/// \brief Makes 2 new triangles from the 4 edges passed into this method
/// \param[in] a_pts : Array of point locations in the TIN
/// \param[in] a_edges : A map of edges that surround the point that is being
/// removed.
/// \param[out] a_tris : Array of point indexes that will define the new
/// triangles
//------------------------------------------------------------------------------
void TrAutoFixFourTrianglePtsImpl::MakeTwoNewTriangles(VecPt3d& a_pts,
                                                       std::map<int, int> a_edges,
                                                       int a_tris[2][3])
{
  int bound[4];
  auto e = a_edges.begin();
  for (int t = 0; t < 4; ++t)
  {
    bound[t] = e->first;
    e = a_edges.find(e->second);
  }
  // is 0,2 closer or 1,3 closer
  Pt3d &p0(a_pts[bound[0]]), &p1(a_pts[bound[1]]), &p2(a_pts[bound[2]]), &p3(a_pts[bound[3]]);
  double d2_02 = MdistSq(p0.x, p0.y, p2.x, p2.y);
  double d2_13 = MdistSq(p1.x, p1.y, p3.x, p3.y);
  if (d2_02 < d2_13)
  {
    a_tris[0][0] = bound[0];
    a_tris[0][1] = bound[1];
    a_tris[0][2] = bound[2];
    a_tris[1][0] = bound[0];
    a_tris[1][1] = bound[2];
    a_tris[1][2] = bound[3];
  }
  else
  {
    a_tris[0][0] = bound[0];
    a_tris[0][1] = bound[1];
    a_tris[0][2] = bound[3];
    a_tris[1][0] = bound[1];
    a_tris[1][1] = bound[2];
    a_tris[1][2] = bound[3];
  }
} // TrAutoFixFourTrianglePtsImpl::MakeTwoNewTriangles
//------------------------------------------------------------------------------
/// \brief Makes 2 new triangles from the 4 edges passed into this method
/// \param[in] a_tIdx : Index to a triangle that will be replaced
/// \param[in] a_newTri : Point indexes that define a new triangle
/// \param[in,out] a_tris : The triangles in the TIN
//------------------------------------------------------------------------------
void TrAutoFixFourTrianglePtsImpl::ReplaceTriangle(int& a_tIdx, int a_newTri[3], VecInt& a_tris)
{
  int tIdx = a_tIdx * 3;
  for (int i = 0; i < 3; ++i)
    a_tris[tIdx + i] = a_newTri[i];
} // TrAutoFixFourTrianglePtsImpl::ReplaceTriangle
//------------------------------------------------------------------------------
/// \brief Renumbers the point indexes in the triangles
//------------------------------------------------------------------------------
void TrAutoFixFourTrianglePtsImpl::RenumberPts()
{
  VecInt& tris(m_tin->Triangles());
  VecPt3d& pts(m_tin->Points());

  VecInt oldToNewPt(pts.size());
  auto it = m_ptsToDelete.begin();
  auto itEnd = m_ptsToDelete.end();
  int cnt(0), npts(static_cast<int>(pts.size()));
  for (int i = 0; i < npts; ++i)
  {
    if (it != itEnd && i == *it)
    {
      cnt++;
      it++;
    }
    oldToNewPt[i] = i - cnt;
  }

  for (auto& t : tris)
    t = oldToNewPt[t];
} // TrAutoFixFourTrianglePtsImpl::RenumberPts
//------------------------------------------------------------------------------
/// \brief Renumbers the triangle indexes in the adjacent triangles
//------------------------------------------------------------------------------
void TrAutoFixFourTrianglePtsImpl::RenumberTris()
{
  if (m_removeAdjTris)
    return;

  int ntri = static_cast<int>(m_tin->Triangles().size() / 3);
  VecInt oldToNewTris(ntri);
  auto it = m_trisToDelete.begin();
  auto itEnd = m_trisToDelete.end();
  int cnt(0);
  for (int i = 0; i < ntri; ++i)
  {
    if (it != itEnd && i == *it)
    {
      cnt++;
      it++;
    }
    oldToNewTris[i] = i - cnt;
  }

  VecInt2d& adjTris(m_tin->TrisAdjToPts());
  for (auto& v : adjTris)
    for (auto& t : v)
      t = oldToNewTris[t];
} // TrAutoFixFourTrianglePtsImpl::RenumberTris
//------------------------------------------------------------------------------
/// \brief Removes points from the points vector of the TIN
//------------------------------------------------------------------------------
void TrAutoFixFourTrianglePtsImpl::RemovePts()
{
  VecPt3d& pts(m_tin->Points());
  auto it = m_ptsToDelete.rbegin();
  auto itEnd = m_ptsToDelete.rend();
  for (; it != itEnd; ++it)
    pts.erase(pts.begin() + *it);
} // TrAutoFixFourTrianglePtsImpl::RemovePts
//------------------------------------------------------------------------------
/// \brief Removes triangles from the triangles vector in the TIN
//------------------------------------------------------------------------------
void TrAutoFixFourTrianglePtsImpl::RemoveTris()
{
  SetInt delTris;
  for (auto& t : m_trisToDelete)
  {
    int tIdx = t * 3;
    delTris.insert(tIdx + 0);
    delTris.insert(tIdx + 1);
    delTris.insert(tIdx + 2);
  }

  VecInt& tris(m_tin->Triangles());
  auto it = delTris.rbegin();
  auto itEnd = delTris.rend();
  for (; it != itEnd; ++it)
    tris.erase(tris.begin() + *it);
} // TrAutoFixFourTrianglePtsImpl::RemoveTris
//------------------------------------------------------------------------------
/// \brief Clears the adjacent triangles if this class created them
//------------------------------------------------------------------------------
void TrAutoFixFourTrianglePtsImpl::ClearAdjTrisIfCreated()
{
  VecInt2d& adjTris(m_tin->TrisAdjToPts());
  adjTris.clear();
  if (m_removeAdjTris)
    return;
  m_tin->BuildTrisAdjToPts();
} // TrAutoFixFourTrianglePtsImpl::ClearAdjTrisIfCreated

////////////////////////////////////////////////////////////////////////////////
/// \class TrAutoFixFourTrianglePts
/// \see TrAutoFixFourTrianglePtsImpl
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/// \brief Creates a TrAutoFixFourTrianglePtsImpl object.
/// \return TrAutoFixFourTrianglePts.
//------------------------------------------------------------------------------
BSHP<TrAutoFixFourTrianglePts> TrAutoFixFourTrianglePts::New()
{
  BSHP<TrAutoFixFourTrianglePts> fixer(new TrAutoFixFourTrianglePtsImpl);
  return fixer;
} // TrAutoFixFourTrianglePts::New
//------------------------------------------------------------------------------
/// \brief
//------------------------------------------------------------------------------
TrAutoFixFourTrianglePts::TrAutoFixFourTrianglePts()
{
}
TrAutoFixFourTrianglePts::~TrAutoFixFourTrianglePts()
{
}

} // namespace xms

#if CXX_TEST
////////////////////////////////////////////////////////////////////////////////
// UNIT TESTS
////////////////////////////////////////////////////////////////////////////////

using namespace xms;
#include <xmsinterp/triangulate/detail/TrAutoFixFourTrianglePts.t.h>

#include <xmscore/testing/TestTools.h>
#include <xmsinterp/triangulate/TrTriangulatorPoints.h>

////////////////////////////////////////////////////////////////////////////////
/// \class TrAutoFixFourTrianglePtsUnitTests
/// \brief Tests for TrAutoFixFourTrianglePts
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/// \brief Tests TrAutoFixFourTrianglePts.
//------------------------------------------------------------------------------
void TrAutoFixFourTrianglePtsUnitTests::test1()
{
  BSHP<VecPt3d> pts(new VecPt3d());
  *pts = {{0, 0}, {10, 0}, {0, 10}, {10, 10}, {5, 5}};
  BSHP<TrTin> tin = TrTin::New();
  tin->SetPoints(pts);
  TrTriangulatorPoints client(*pts, tin->Triangles(), &tin->TrisAdjToPts());
  client.Triangulate();

  TrAutoFixFourTrianglePtsImpl p;
  p.Fix(tin);

  TS_ASSERT_EQUALS(4, tin->Points().size());

} // TrOuterTriangleDeleterTests::test1

#endif // CXX_TEST
