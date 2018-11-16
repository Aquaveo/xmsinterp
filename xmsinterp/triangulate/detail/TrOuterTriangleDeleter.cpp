//------------------------------------------------------------------------------
/// \file
/// \ingroup triangulate_detail
/// \copyright (C) Copyright Aquaveo 2018. Distributed under FreeBSD License
/// (See accompanying file LICENSE or https://aqaveo.com/bsd/license.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------

// 1. Precompiled header

// 2. My own header
#include <xmsinterp/triangulate/detail/TrOuterTriangleDeleter.h>

// 3. Standard library headers

// 4. External library headers

// 5. Shared code headers
#include <xmscore/stl/set.h>
#include <xmscore/stl/vector.h>
#include <xmscore/misc/Observer.h>
#include <xmscore/misc/XmError.h>
#include <xmscore/misc/boost_defines.h> // BSHP
#include <xmsinterp/triangulate/TrTin.h>
#include <xmsinterp/triangulate/triangles.h>

// 6. Non-shared code headers

//----- Forward declarations ---------------------------------------------------

//----- External globals -------------------------------------------------------

//----- Namespace declaration --------------------------------------------------

namespace xms
{
//----- Constants / Enumerations -----------------------------------------------

//----- Classes / Structs ------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
class TrOuterTriangleDeleterImpl : public TrOuterTriangleDeleter
{
public:
  /// \brief Flags used triangles
  enum BoundaryEnum {
    BE_UNVISITED = 0,
    BE_VISITED = 1,
    BE_OUTSIDE = 2,
    BE_INSIDE = 4,
    BE_ONBOUNDARY = 8
  };

  TrOuterTriangleDeleterImpl();
  virtual ~TrOuterTriangleDeleterImpl();

public:
  virtual void Delete(const VecInt2d& a_polys, BSHP<TrTin> a_tin) override;
  /// \brief Set the observer to use for feedback while processing.
  /// \param a_: The obserer.
  virtual void SetObserver(BSHP<Observer> a_) override { m_observer = a_; }

private:
  void FlagTrianglesAlongPolygon(const VecInt& a_poly, VecInt& a_flags);
  void MarkNeighbors(VecInt& a_flags);

private:
  VecInt2d m_polys;          ///< Polygons (boundary and holes) of tin
  BSHP<TrTin> m_tin;         ///< tin
  BSHP<Observer> m_observer; ///< Observer
};                           // class TrOuterTriangleDeleterImpl

//----- Internal functions -----------------------------------------------------

//----- Class / Function definitions -------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// \class TrOuterTriangleDeleterImpl
/// \brief Used to delete tin triangles that are outside given polygons. The
/// polygons may include holes - polygons inside polygons.
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/// \brief constructor
//------------------------------------------------------------------------------
TrOuterTriangleDeleterImpl::TrOuterTriangleDeleterImpl()
: TrOuterTriangleDeleter()
, m_polys()
, m_tin()
{
}
//------------------------------------------------------------------------------
/// \brief
//------------------------------------------------------------------------------
TrOuterTriangleDeleterImpl::~TrOuterTriangleDeleterImpl()
{
}
//------------------------------------------------------------------------------
/// \brief Deletes triangles in \a a_tin that are outside a_polys or inside if
///        the poly is a hole.
///
///        Polygons should be CW for outside poly and CCW for inside poly.
///        Last point is the first point repeated.
/// \param[in] a_polys:   Polygon(s) (0-based indices of points in a_tin).
/// \param[in,out] a_tin: The tin to be modified.
//------------------------------------------------------------------------------
void TrOuterTriangleDeleterImpl::Delete(const VecInt2d& a_polys, BSHP<TrTin> a_tin)
{
  m_polys = a_polys;
  m_tin = a_tin;

  // Make sure polygons are closed
  for (int i = 0; i < (int)m_polys.size(); ++i)
  {
    if (m_polys[i].front() != m_polys[i].back())
    {
      XM_ASSERT(false);
      m_polys[i].push_back(m_polys[i].front());
    }
  }

  // Flag triangles on border
  VecInt flags(m_tin->NumTriangles(), BE_UNVISITED);
  for (size_t i = 0; i < m_polys.size(); ++i)
  {
    FlagTrianglesAlongPolygon(m_polys[i], flags);
  }
  if (m_observer)
    m_observer->ProgressStatus(0.4);

  // Flag all triangles working out from the ones already flagged
  MarkNeighbors(flags);
  if (m_observer)
    m_observer->ProgressStatus(0.8);

  // Create the set of triangles to delete
  SetInt trisToDelete;
  for (size_t i = 0; i < flags.size(); ++i)
  {
    if (flags[i] & BE_OUTSIDE)
    {
      trisToDelete.insert((int)i);
    }
  }

  m_tin->DeleteTriangles(trisToDelete);
  if (m_observer)
    m_observer->ProgressStatus(1.0);
} // TrOuterTriangleDeleterImpl::Delete
//------------------------------------------------------------------------------
/// \brief Flag triangles along polygon as inside or outside the polygon.
/// \param a_poly: Indices of polygon points. Closed (last = first).
/// \param a_flags: BoundaryEnum flags size of number of triangles.
//------------------------------------------------------------------------------
void TrOuterTriangleDeleterImpl::FlagTrianglesAlongPolygon(const VecInt& a_poly, VecInt& a_flags)
{
  VecInt::const_iterator it0 = a_poly.begin();
  VecInt::const_iterator it1 = it0;
  ++it1;
  int outTri, inTri;
  for (; it1 != a_poly.end(); ++it0, ++it1)
  {
    // outTri = m_tin->TriangleAdjacentToEdge(*it0, *it1);
    // inTri = m_tin->TriangleAdjacentToEdge(*it1, *it0);
    inTri = m_tin->TriangleAdjacentToEdge(*it0, *it1);
    outTri = m_tin->TriangleAdjacentToEdge(*it1, *it0);
    if (outTri != -1)
    {
      // A polygon with an internal arc is a little different. We have adjacent
      // triangles to 2 edges that will be marked as "inside".
      if (!(a_flags[outTri] & BE_INSIDE))
        a_flags[outTri] = BE_OUTSIDE | BE_ONBOUNDARY;
    }
    if (inTri != -1)
    {
      a_flags[inTri] = BE_INSIDE | BE_ONBOUNDARY;
    }
  }

} // TrOuterTriangleDeleterImpl::FlagTrianglesAlongPolygon
//------------------------------------------------------------------------------
/// \brief Flag all triangles as inside or outside the polygon, starting with
///        those already flagged and working out. Compare to myiMarkNeighbors.
/// \param a_flags: BoundaryEnum flags size of num triangles with triangles
///                 along polygon border already set.
//------------------------------------------------------------------------------
void TrOuterTriangleDeleterImpl::MarkNeighbors(VecInt& a_flags)
{
  XM_ENSURE_TRUE_VOID_NO_ASSERT(!m_tin->Triangles().empty());

  // Create stack size of num triangles
  int numTri = m_tin->NumTriangles();
  VecInt stack(numTri, -1);

  bool done = false;
  int lastTri = 0;
  while (!done)
  {
    // find a visited elem with unvisited neighbors
    int tri = lastTri;
    bool found = false;
    while (tri < numTri && tri != -1 && !found)
    {
      if (a_flags[tri] & BE_ONBOUNDARY && !(a_flags[tri] & BE_VISITED))
      {
        for (int id = 0; id < 3 && !found; ++id)
        {
          int adjTri = m_tin->AdjacentTriangle(tri, id);
          if (adjTri != -1 && a_flags[adjTri] == BE_UNVISITED)
          {
            found = true;
            lastTri = tri + 1;
          }
        }
      }
      if (!found)
      {
        ++tri;
      }
    }

    // all have been flagged, we're done
    if (!found)
    {
      done = true;
      // AKZ - flag unvisited as out
      for (size_t i = 0; i < a_flags.size(); ++i)
      {
        if (a_flags[i] == BE_UNVISITED)
        {
          a_flags[i] = BE_OUTSIDE;
        }
      }
    }
    else
    {
      int size = 0;
      // push the first tri onto the stack
      stack[size++] = tri;
      for (int i = 0; i < size; i++)
      {
        int currTri = stack[i];
        a_flags[currTri] |= BE_VISITED;
        // find the unflagged adjacents and push onto stack
        int id = 0;
        do
        {
          int adjTri = m_tin->AdjacentTriangle(currTri, id);
          if (adjTri != -1 && a_flags[adjTri] == BE_UNVISITED)
          {
            // push adjacent onto stack
            stack[size++] = adjTri;
            if (a_flags[currTri] & BE_INSIDE)
              a_flags[adjTri] = BE_INSIDE;
            else
              a_flags[adjTri] = BE_OUTSIDE;
          }
          id = trIncrementIndex(id);
        } while (id != 0);
      }
    }
  }

} // TrOuterTriangleDeleterImpl::MarkNeighbors

////////////////////////////////////////////////////////////////////////////////
/// \class TrOuterTriangleDeleter
/// \see TrOuterTriangleDeleterImpl
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/// \brief Creates a TrOuterTriangleDeleterImpl object.
/// \return TrOuterTriangleDeleter.
//------------------------------------------------------------------------------
BSHP<TrOuterTriangleDeleter> TrOuterTriangleDeleter::New()
{
  BSHP<TrOuterTriangleDeleter> deleter(new TrOuterTriangleDeleterImpl);
  return deleter;
} // TrOuterTriangleDeleter::New
//------------------------------------------------------------------------------
/// \brief
//------------------------------------------------------------------------------
TrOuterTriangleDeleter::TrOuterTriangleDeleter()
{
} // TrOuterTriangleDeleter::TrOuterTriangleDeleter
//------------------------------------------------------------------------------
/// \brief
//------------------------------------------------------------------------------
TrOuterTriangleDeleter::~TrOuterTriangleDeleter()
{
} // TrOuterTriangleDeleter::~TrOuterTriangleDeleter

} // namespace xms

#if CXX_TEST
////////////////////////////////////////////////////////////////////////////////
// UNIT TESTS
////////////////////////////////////////////////////////////////////////////////

#include <xmsinterp/triangulate/detail/TrOuterTriangleDeleter.t.h>

#include <xmscore/testing/TestTools.h>
#include <xmsinterp/triangulate/TrTin.t.h>
#include <xmscore/misc/carray.h>

////////////////////////////////////////////////////////////////////////////////
/// \class TrOuterTriangleDeleterUnitTests
/// \brief Tests for TrOuterTriangleDeleter
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/// \brief Tests TrOuterTriangleDeleter.
/// \verbatim
//  Before
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
//
//  After
//  10-  17-----18------19------20------21
//    |  |\  8  /|\ 11  /|\ 22  / \ 26  /
//    |  | \   / | \   / | \   /   \   /
//    |  |  \ /  |  \ /30|23\ / 27  \ /
//   5-  |6 13 12|9 14   |  15------16
//    |  |  / \  |  /|\  |  / \ 25  /|
//    |  | /   \ | / | \ | /   \   / |
//    |  |/  7  \|/  |  \|/     \ /29|
//   0-  9------10   |3 11      12   |
//    |  |\ 13  /    |  / \     / \  |
//    |  | \   /     | /   \   /   \ |
//    |  |  \ /      |/ 16  \ / 21  \|
//  -5-  | 0 5-------6-------7-------8
//    |  |  / \  4  / \ 14  / \ 19  / \
//    |  | /   \   /   \   /   \   /   \
//    |  |/  1  \ /  2  \ / 20  \ / 17  \
// -10-  0-------1-------2-------3-------4
//
//       |-------|-------|-------|-------|
//       0      10      20      30      40
/// \endverbatim
//------------------------------------------------------------------------------
void TrOuterTriangleDeleterUnitTests::test1()
{
  BSHP<xms::TrTin> tin = trBuildTestTin7();

  // Set up polygons
  xms::VecInt2d polys;
  int outPoly[] = {0, 9, 17, 18, 19, 20, 21, 16, 8, 4, 3, 2, 1, 0};
  int inPoly1[] = {10, 5, 6, 14, 10};
  int inPoly2[] = {11, 7, 12, 15, 11};
  polys.push_back(xms::VecInt(&outPoly[0], &outPoly[XM_COUNTOF(outPoly)]));
  polys.push_back(xms::VecInt(&inPoly1[0], &inPoly1[XM_COUNTOF(inPoly1)]));
  polys.push_back(xms::VecInt(&inPoly2[0], &inPoly2[XM_COUNTOF(inPoly2)]));

  BSHP<xms::TrOuterTriangleDeleter> deleter = xms::TrOuterTriangleDeleter::New();
  deleter->Delete(polys, tin);

  // Verify triangles after
  int trisA[] = {0,  5,  9,  5,  0,  1,  1,  2,  6,  14, 6,  11, 1,  6,  5,  9,  13, 17, 13,
                 9,  10, 17, 13, 18, 10, 14, 18, 18, 14, 19, 10, 18, 13, 9,  5,  10, 6,  2,
                 7,  7,  11, 6,  3,  4,  8,  3,  8,  7,  7,  2,  3,  12, 7,  8,  19, 15, 20,
                 15, 19, 11, 15, 12, 16, 20, 16, 21, 16, 20, 15, 12, 8,  16, 11, 19, 14};
  xms::VecInt trisAfter(&trisA[0], &trisA[XM_COUNTOF(trisA)]);
  TS_ASSERT_EQUALS_VEC(trisAfter, tin->Triangles());

} // TrOuterTriangleDeleterUnitTests::test1

#endif // CXX_TEST
