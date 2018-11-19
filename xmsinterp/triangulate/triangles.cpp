//------------------------------------------------------------------------------
/// \file
/// \brief Functions dealing with triangles.
/// \ingroup triangulate
/// \copyright (C) Copyright Aquaveo 2018. Distributed under FreeBSD License
/// (See accompanying file LICENSE or https://aqaveo.com/bsd/license.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------

#include <xmsinterp/geometry/geoms.h>
#include <xmscore/stl/vector.h>
#include <xmsinterp/triangulate/triangles.h>

//----- Constants / Enumerations -----------------------------------------------

//----- Forward declarations ---------------------------------------------------

//----- External globals -------------------------------------------------------

//----- Namespace declaration --------------------------------------------------

namespace xms
{
//----- Global variables -------------------------------------------------------

//----- File globals -----------------------------------------------------------

//----- Internal function prototypes -------------------------------------------

//----- Functions --------------------------------------------------------------

//------------------------------------------------------------------------------
/// \brief Return the signed planar area of the triangle (CCW Positive).
///
/// This uses the formula:
/// (-x2*y1 + x3*y1 + x1*y2 - x3*y2 - x1*y3 + x2*y3) / 2
/// reworked to ensure precision is not lost with large numbers by
/// moving the triangle to the origin by subtracting the first vertex's
/// location from all vertices.
/// \param[in] a_pt1: First point on triangle.
/// \param[in] a_pt2: Second point on triangle.
/// \param[in] a_pt3: Third point on triangle.
/// \return The area.
//------------------------------------------------------------------------------
double trArea(const Pt3d& a_pt1, const Pt3d& a_pt2, const Pt3d& a_pt3)
{
  return (((a_pt2.x - a_pt1.x) * (a_pt3.y - a_pt1.y)) -
          ((a_pt3.x - a_pt1.x) * (a_pt2.y - a_pt1.y))) *
         0.5;
} // trArea
//------------------------------------------------------------------------------
/// \brief Create something like this:
/// \verbatim
/// (0, 0)
///    0-------------1-------------2
///    |\     4     /|\     8     /|
///    |  \       /  |  \       /  |
///    |    \   /    |    \   /    |
///    |  1   3    3 |  5   4    7 |
///    |    /   \    |    /   \    |
///    |  /       \  |  /       \  |
///    |/     2     \|/     6     \|
///    5-------------6-------------7 2
///                            (20, -10)
/// \endverbatim
/// \param[in] rows: Number of rows
/// \param[in] cols: Number of columns
/// \param[out] a_points: The points.
/// \param[out] a_polys: The polygons.
//------------------------------------------------------------------------------
void trBuildGridTrianglePolys(int rows, int cols, VecPt3d& a_points, VecInt2d& a_polys)
{
  trBuildGridPolys(rows, cols, a_points, a_polys);

  VecPt3d points = a_points;

  // add cell centers
  for (size_t i = 0; i < a_polys.size(); ++i)
  {
    VecPt3d pts;
    VecInt& poly = a_polys[i];
    size_t numPts = poly.size();
    pts.reserve(numPts);
    for (size_t j = 0; j < numPts; ++j)
    {
      pts.push_back(a_points[poly[j]]);
    }
    Pt3d centroid = gmComputeCentroid(pts);
    points.push_back(centroid);
  }

  // Build triangle polys
  int firstCellCenter = (int)a_points.size();
  VecInt2d trianglePolys;

  for (size_t cellIdx = 0; cellIdx < a_polys.size(); ++cellIdx)
  {
    VecInt newPoly(3, 0);
    VecInt& poly = a_polys[cellIdx];
    size_t numPts = poly.size();
    for (size_t pt = 0; pt < numPts; ++pt)
    {
      newPoly[0] = poly[pt];
      newPoly[1] = poly[(pt + 1) % numPts];
      newPoly[2] = firstCellCenter + (int)cellIdx;
      trianglePolys.push_back(newPoly);
    }
  }

  a_points = points;
  a_polys = trianglePolys;
} // trBuildGridTrianglePolys
//------------------------------------------------------------------------------
/// \brief Build something like this:
/// \verbatim
///  (0, 0)
///    0-------------1-------------2
///    |             |             |
///    |             |             |
///    |      1      |      2      |
///    |             |             |
///    |             |             |
///    3-------------4-------------5
///                             (20,-10)
/// \endverbatim
/// \param[in] rows: Number of rows
/// \param[in] cols: Number of columns
/// \param[out] pts: The points.
/// \param[out] polys: The polygons.
//------------------------------------------------------------------------------
void trBuildGridPolys(int rows, int cols, VecPt3d& pts, VecInt2d& polys)
{
  pts.clear();
  for (int i = 0; i <= rows; ++i)
  {
    for (int j = 0; j <= cols; ++j)
    {
      pts.push_back(Pt3d(j * 10.0, -i * 10.0, 0.0));
    }
  }

  polys.assign(rows * cols, VecInt());
  int count = 0;
  int node = 0;
  int nodeNextRow = cols + 1;
  for (int i = 0; i < rows; ++i)
  {
    for (int j = 0; j < cols; ++j)
    {
      VecInt& poly = polys[count++];
      poly.push_back(node++);
      poly.push_back(nodeNextRow++);
      poly.push_back(nodeNextRow);
      poly.push_back(node);
    }
    ++node;
    ++nodeNextRow;
  }
} // trBuildGridPolys
} // namespace xms
