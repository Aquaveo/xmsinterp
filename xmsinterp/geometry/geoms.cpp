//------------------------------------------------------------------------------
/// \file
/// \brief Functions dealing with geometry.
/// \ingroup geometry
/// \copyright (C) Copyright Aquaveo 2018. Distributed under the xmsng
///  Software License, Version 1.0. (See accompanying file
///  LICENSE_1_0.txt or copy at http://www.aquaveo.com/xmsng/LICENSE_1_0.txt)
//------------------------------------------------------------------------------

// 1. Precompiled header

// 2. My own header
#include <xmsinterp/geometry/geoms.h>

// 3. Standard library headers
#include <cfloat>
#include <algorithm>

// 4. External library headers

// 5. Shared code headers
#include <xmscore/math/math.h> // GT_TOL, LT_TOL
#include <xmscore/misc/XmError.h>
#include <xmscore/points/pt.h> // Pt3d
#include <xmscore/stl/vector.h>
#include <xmsinterp/geometry/GmPolygon.h>
#include <xmscore/misc/xmstype.h> // XM_ZERO_TOL
#include <xmsinterp/triangulate/triangles.h>
#include <xmscore/misc/XmConst.h>

// 6. Non-shared code headers

//----- Namespace declaration --------------------------------------------------

namespace xms
{
//----- Constants / Enumerations -----------------------------------------------

//----- Forward declarations ---------------------------------------------------

//----- External globals -------------------------------------------------------

//----- Internal function prototypes -------------------------------------------

//------------------------------------------------------------------------------
/// \brief finds if a point is in or on a box in 2d
/// \param[in] a_bMin Min x,y of first box
/// \param[in] a_bMax Max x,y of first box
/// \param[in] a_pt point being tested
/// \return true if the point is in or on a box
//------------------------------------------------------------------------------
bool gmPointInOrOnBox2d(const Pt3d& a_bMin, const Pt3d& a_bMax, const Pt3d& a_pt)
{
  if (a_pt.x < a_bMin.x || a_pt.y < a_bMin.y || a_pt.x > a_bMax.x || a_pt.y > a_bMax.y)
    return false;
  return true;
} // gmBoxesOverlap2d
//------------------------------------------------------------------------------
/// \brief finds if 2 boxes overlap in 2d
/// \param[in] a_b1Min Min x,y of first box
/// \param[in] a_b1Max Max x,y of first box
/// \param[in] a_b2Min Min x,y of second box
/// \param[in] a_b2Max Max x,y of second box
/// \return true if the boxes overlap
//------------------------------------------------------------------------------
bool gmBoxesOverlap2d(const Pt3d& a_b1Min,
                      const Pt3d& a_b1Max,
                      const Pt3d& a_b2Min,
                      const Pt3d& a_b2Max)
{
  if (a_b1Max.x < a_b2Min.x)
    return false;
  if (a_b1Min.x > a_b2Max.x)
    return false;
  if (a_b1Max.y < a_b2Min.y)
    return false;
  if (a_b1Min.y > a_b2Max.y)
    return false;
  return true;
} // gmBoxesOverlap2d
//------------------------------------------------------------------------------
/// \brief Calculates the plane coefficients for a triangle. Given point
///        references calculate coefficents for plane (ax+by+cz+d=0).
/// \param[in] p1: First point.
/// \param[in] p2: Second point.
/// \param[in] p3: Third point.
/// \param[out] a: Coefficient a.
/// \param[out] b: Coefficient b.
/// \param[out] c: Coefficient c.
/// \param[out] d: Coefficient d.
//------------------------------------------------------------------------------
void gmCalculateNormalizedPlaneCoefficients(const Pt3d& p1,
                                            const Pt3d& p2,
                                            const Pt3d& p3,
                                            double* a,
                                            double* b,
                                            double* c,
                                            double* d)
{
  // call the other version
  gmCalculateNormalizedPlaneCoefficients(&p1, &p2, &p3, a, b, c, d);
} // gmCalculateNormalizedPlaneCoefficients
//------------------------------------------------------------------------------
/// \brief Calculates the plane coefficients for a triangle. Given point
///        pointers calculate coefficents for plane (ax+by+cz+d=0).
/// \param[in] p1: First point.
/// \param[in] p2: Second point.
/// \param[in] p3: Third point.
/// \param[out] a: Coefficient a.
/// \param[out] b: Coefficient b.
/// \param[out] c: Coefficient c.
/// \param[out] d: Coefficient d.
//------------------------------------------------------------------------------
void gmCalculateNormalizedPlaneCoefficients(const Pt3d* p1,
                                            const Pt3d* p2,
                                            const Pt3d* p3,
                                            double* a,
                                            double* b,
                                            double* c,
                                            double* d)
{
  double x1(p1->x), y1(p1->y), z1(p1->z);
  double x2(p2->x), y2(p2->y), z2(p2->z);
  double x3(p3->x), y3(p3->y), z3(p3->z);
  *a = (y1 * (z2 - z3) + y2 * (z3 - z1) + y3 * (z1 - z2));
  *b = (z1 * (x2 - x3) + z2 * (x3 - x1) + z3 * (x1 - x2));
  *c = (x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2));
  double mag(sqrt((*a) * (*a) + (*b) * (*b) + (*c) * (*c)));
  *a /= mag;
  *b /= mag;
  *c /= mag;
  *d = -(*a) * x1 - (*b) * y1 - (*c) * z1;
} // gmCalculateNormalizedPlaneCoefficients
//------------------------------------------------------------------------------
/// \brief Returns the z value halfway between the max and min z. Different
/// from the average z (where many points can skew the average).
/// \param a_points: vector of locations
/// \return Middle z value.
//------------------------------------------------------------------------------
double gmMiddleZ(const VecPt3d& a_points)
{
  // Get an average z value
  double zmin = XM_DBL_HIGHEST;
  double zmax = XM_DBL_LOWEST;
  for (size_t i = 0; i < a_points.size(); ++i)
  {
    double z = a_points[i].z;
    zmin = std::min(zmin, z);
    zmax = std::max(zmax, z);
  }
  return (zmin + zmax) / 2.0;
} // gmMiddleZ
//------------------------------------------------------------------------------
/// \brief Is a given point inside a circumcircle defined by three points?
/// \param pt: The point.
/// \param circumcirclePts: 3 points on the circumcircle.
/// \return PtInOutOrOn_enum
//------------------------------------------------------------------------------
PtInOutOrOn_enum gmPtInCircumcircle(const Pt3d& pt, Pt3d circumcirclePts[3])
{
  double xc, yc, r2;

  if (!gmCircumcircleWithTol(&circumcirclePts[0], &circumcirclePts[1], &circumcirclePts[2], &xc,
                             &yc, &r2, gmXyTol()))
  {
    return PT_ERROR;
  }
  /* compute distance from (xc,yc) to pt squared */
  double delta = sqrt(r2) - sqrt(sqr(pt.x - xc) + sqr(pt.y - yc));
  if (fabs(delta) > gmXyTol())
  {
    if (delta > gmXyTol())
    {
      return PT_IN;
    }
    else
    {
      return PT_OUT;
    }
  }
  return PT_ON;
} // gmPtInCircumcircle
//------------------------------------------------------------------------------
/// \brief Calculate XY distance between two 3D points.
/// \param[in] pt1: First point.
/// \param[in] pt2: Second point.
/// \return distance squared.
//------------------------------------------------------------------------------
double gmXyDistanceSquared(const Pt3d& pt1, const Pt3d& pt2)
{
  return (sqr(pt1.x - pt2.x) + sqr(pt1.y - pt2.y));
} // gmXyDistanceSqared
//------------------------------------------------------------------------------
/// \brief Computes center & radius squared for circumcircle of triangle
///        made by the three points.
/// \param pt1: first of 3 locations defining a triangle
/// \param pt2: second of 3 locations defining a triangle
/// \param pt3: third of 3 locations defining a triangle
/// \param xc: circumcenter x coord
/// \param yc: circumcenter y coord
/// \param r2: radius squared of circumcircle
/// \param tol: tolerance for geometric comparison
/// \return false on error (triangle has zero area), otherwise true
//------------------------------------------------------------------------------
bool gmCircumcircleWithTol(const Pt3d* pt1,
                           const Pt3d* pt2,
                           const Pt3d* pt3,
                           double* xc,
                           double* yc,
                           double* r2,
                           double tol)
{
  bool ok = true;
  double det11, det12, det13, det21, det22, det23;
  double determinate;
  /* compute these */
  det11 = pt1->x - pt2->x;
  det12 = pt1->y - pt2->y;
  det13 = det11 * (pt1->x + pt2->x) / 2.0 + det12 * (pt1->y + pt2->y) / 2.0;

  det21 = pt3->x - pt2->x;
  det22 = pt3->y - pt2->y;
  det23 = det21 * (pt3->x + pt2->x) / 2.0 + det22 * (pt3->y + pt2->y) / 2.0;
  /* compute determinant */
  determinate = det11 * det22 - det12 * det21;
  /* zero determinate indicates collinear pts */
  if (fabs(determinate) < tol)
  {
    ok = false;
    determinate = tol;
    /* the old subtri perturbed a vertex to
     * give a non zero determinant as follows:
i = 0;
while (determinate == 0.0) {
i++;
Nx = ((double)i/1000.0)*(vtx1->y - vtx0->y);
Ny = ((double)i/1000.0)*(vtx0->x - vtx1->x);
x1new = vtx1->x + Nx;
y1new = vtx1->y + Ny;
det11 = x1new - vtx0->x;
det12 = y1new - vtx0->y;
det13 = det11*(x1new + vtx0->x)/2.0 + det12*(y1new + vtx0->y)/2.0;
determinate = det11 * det22 - det12 * det21;
}
       * END OF COMMENTED CODE */
  }
  *xc = (det13 * det22 - det23 * det12) / determinate;
  *yc = (det11 * det23 - det21 * det13) / determinate;
  *r2 = sqr(pt1->x - *xc) + sqr(pt1->y - *yc);
  return ok;
} // gmCircumcircleWithTol
//------------------------------------------------------------------------------
/// \brief Compute Barycentric coords for point. Use gmBaryPrepare to get the
///        coefficients and direction.
/// \param[in] cart: cart?
/// \param[in] orig: orig?
/// \param[in] coef: 6 coefficients.
/// \param[in] dir: Direction.
/// \param[out] bary: point?
/// \return XM_SUCCESS (always it seems)
//------------------------------------------------------------------------------
int gmCartToBary(const Pt3d* cart, const Pt3d* orig, double coef[6], int dir, Pt3d* bary)
{
  double x, y, z;
  x = cart->x - orig->x;
  y = cart->y - orig->y;
  z = cart->z - orig->z;
  /* get magnitudes of the plane normal */
  switch (dir)
  {
  case (0):
    bary->x = coef[0] * y + coef[1] * z + coef[2];
    bary->y = coef[3] * y + coef[4] * z + coef[5];
    bary->z = 1.0 - bary->x - bary->y;
    break;
  case (1):
    bary->x = coef[0] * z + coef[1] * x + coef[2];
    bary->y = coef[3] * z + coef[4] * x + coef[5];
    bary->z = 1.0 - bary->x - bary->y;
    break;
  case (2):
    bary->x = coef[0] * x + coef[1] * y + coef[2];
    bary->y = coef[3] * x + coef[4] * y + coef[5];
    bary->z = 1.0 - bary->x - bary->y;
    break;
  }
  return (XM_SUCCESS);
} // gmCartToBary
//------------------------------------------------------------------------------
/// \brief For a tri - compute dir & coefs to compute Barycentric coords
/// \param p1 = 1st point of triangle.
/// \param p2 = 2nd point of triangle.
/// \param p3 = 3rd point of triangle.
/// \param norm = triangle normal.
/// \param orig = triangle origin (min xyz of p1, p2 and p3).
/// \param coef = ?
/// \param dir  = 0 if norm points to x, 1 if to y, 2 if to z.
/// \param flag = if true, origin will be computed from the triangle. If false,
///               just calculate the coefficients and the origin is zero.
/// \return XM_SUCCESS or XM_FAILURE.
//------------------------------------------------------------------------------
int gmBaryPrepare(const Pt3d* p1,
                  const Pt3d* p2,
                  const Pt3d* p3,
                  const Pt3d* norm,
                  Pt3d* orig,
                  double coef[6],
                  int* dir,
                  bool flag)
{
  double x1, x2, x3, y1, y2, y3, z1, z2, z3, sizeinv;
  /* compute the direction */
  x1 = fabs(norm->x);
  y1 = fabs(norm->y);
  z1 = fabs(norm->z);
  if (x1 > y1 && x1 > z1)
    *dir = 0;
  else if (y1 > z1)
    *dir = 1;
  else
    *dir = 2;
  /* get the origin */
  if (flag)
  {
    orig->x = Mmin(p1->x, Mmin(p2->x, p3->x));
    orig->y = Mmin(p1->y, Mmin(p2->y, p3->y));
    orig->z = Mmin(p1->z, Mmin(p2->z, p3->z));
  }
  /* compute the coefficients */
  x1 = p1->x - orig->x;
  y1 = p1->y - orig->y;
  z1 = p1->z - orig->z;
  x2 = p2->x - orig->x;
  y2 = p2->y - orig->y;
  z2 = p2->z - orig->z;
  x3 = p3->x - orig->x;
  y3 = p3->y - orig->y;
  z3 = p3->z - orig->z;
  switch (*dir)
  {
  case (0):
    sizeinv = 1.0 / ((y2 - y3) * (z1 - z3) - (z2 - z3) * (y1 - y3));
    coef[0] = (z3 - z2) * sizeinv;
    coef[1] = (y2 - y3) * sizeinv;
    coef[2] = (y3 * z2 - z3 * y2) * sizeinv;
    coef[3] = (z1 - z3) * sizeinv;
    coef[4] = (y3 - y1) * sizeinv;
    coef[5] = (y1 * z3 - z1 * y3) * sizeinv;
    break;
  case (1):
    sizeinv = 1.0 / ((z2 - z3) * (x1 - x3) - (x2 - x3) * (z1 - z3));
    coef[0] = (x3 - x2) * sizeinv;
    coef[1] = (z2 - z3) * sizeinv;
    coef[2] = (z3 * x2 - x3 * z2) * sizeinv;
    coef[3] = (x1 - x3) * sizeinv;
    coef[4] = (z3 - z1) * sizeinv;
    coef[5] = (z1 * x3 - x1 * z3) * sizeinv;
    break;
  case (2):
    sizeinv = 1.0 / ((x2 - x3) * (y1 - y3) - (y2 - y3) * (x1 - x3));
    coef[0] = (y3 - y2) * sizeinv;
    coef[1] = (x2 - x3) * sizeinv;
    coef[2] = (x3 * y2 - y3 * x2) * sizeinv;
    coef[3] = (y1 - y3) * sizeinv;
    coef[4] = (x3 - x1) * sizeinv;
    coef[5] = (x1 * y3 - y1 * x3) * sizeinv;
    break;
  }
  return (XM_SUCCESS);
} // gmBaryPrepare
//------------------------------------------------------------------------------
/// \brief Returns true if (the three vertices are gmColinear. Result should be
///         insensitive to the order of the vertices.
/// \param[in] p1: Point 1.
/// \param[in] p2: Point 2.
/// \param[in] p3: Point 3.
/// \param[in] tol: Tolerance
/// \return true if colinear.
//------------------------------------------------------------------------------
bool gmColinearWithTol(const Pt3d& p1, const Pt3d& p2, const Pt3d& p3, const double tol)
{
  if (gmOnLineWithTol(p1, p2, p3.x, p3.y, tol))
    return true;
  else if (gmOnLineWithTol(p2, p3, p1.x, p1.y, tol))
    return true;
  else if (gmOnLineWithTol(p3, p1, p2.x, p2.y, tol))
    return true;
  else
    return false;
} // gmColinearWithTol
//------------------------------------------------------------------------------
/// \brief Intersects the plan projection of two line segments
/// \param one1: first point on segment 1
/// \param one2: second point on segment 1
/// \param two1: first point on segment 2
/// \param two2: second point on segment 2
/// \param xi: x coord of intersection
/// \param yi: y coord of intersection
/// \param zi1: z coord of intersection on segment 1
/// \param zi2: z coord of intersection on segemnt 2
/// \param tol: tolerance for geometric comparison
///
/// segment 1 = one1,one2  = one1 + lambda(one2 - one1)
/// segment 2 = two1,two2  = two1 + mu (two2 - two1)
/// \return true if lines intersect, else false
//------------------------------------------------------------------------------
bool gmIntersectLineSegmentsWithTol(const Pt3d& one1,
                                    const Pt3d& one2,
                                    const Pt3d& two1,
                                    const Pt3d& two2,
                                    double* xi /*=NULL*/,
                                    double* yi /*=NULL*/,
                                    double* zi1 /*=NULL*/,
                                    double* zi2 /*=NULL*/,
                                    double tol /*=0.0*/)
{
  double minx1, minx2, miny1, miny2, maxx1, maxx2, maxy1, maxy2;
  double dx1, dy1, dz1, dx2, dy2, dz2, lambda, mu, cross;
  // do a trivial rejection
  // RDJ - 4/20/2004 Do tests one at a time so if we fail on one we stop
  minx1 = std::min(one1.x, one2.x);
  maxx2 = std::max(two1.x, two2.x);
  if (GT_TOL(minx1, maxx2, tol))
  {
    return false;
  }
  maxx1 = std::max(one1.x, one2.x);
  minx2 = std::min(two1.x, two2.x);
  if (LT_TOL(maxx1, minx2, tol))
  {
    return false;
  }
  miny1 = std::min(one1.y, one2.y);
  maxy2 = std::max(two1.y, two2.y);
  if (GT_TOL(miny1, maxy2, tol))
  {
    return false;
  }
  maxy1 = std::max(one1.y, one2.y);
  miny2 = std::min(two1.y, two2.y);
  if (LT_TOL(maxy1, miny2, tol))
  {
    return false;
  }
  // define the vectors
  dx1 = one2.x - one1.x;
  dy1 = one2.y - one1.y;
  dz1 = one2.z - one1.z;
  dx2 = two2.x - two1.x;
  dy2 = two2.y - two1.y;
  dz2 = two2.z - two1.z;
  /* see if lines are parallel */
  cross = (dx1 * dy2) -
          (dy1 * dx2); // dy1/dx1 = dy2/dx2 lines have same slope
                       // This checks the line slopes. If equal then the lines are parallel or
                       // the same line. We assume they are parallel and exit.
                       // But really theta should be computed using vector magnitudes
  if (EQ_TOL(cross, 0.0, tol))
    return false;
  // compute the value of lambda
  lambda = (dy2 * (two1.x - one1.x) + dx2 * (one1.y - two1.y)) / cross;

  // There is some question as to what effect the tolerance should have on this
  // function. It was decided that in the case where the segments do not
  // intersect, but the minimum distance between the two segments is within
  // tolerance, then the location of the minimum will be returned as the
  // point of intersection.
  // If the point of intersection is off the end of the first segment, then
  // set it to be at the end and set the checkDistance flag.
  bool checkDistance(false);
  if (lambda < 0.0)
  {
    lambda = 0.0;
    checkDistance = true;
  }
  else if (lambda > 1.0)
  {
    lambda = 1.0;
    checkDistance = true;
  }
  // The intersection is inside of segment 1, so we need to check segment 2
  // Compute mu from lambda and the two parametric segment equations
  //     two1 + mu (two2 - two1) = one1 + lambda ( one2 - one1)
  //                    so
  //     mu = (one1 + lambda (one2-one1) - two1)/(two2-two1)
  if (fabs(dx2) > fabs(dy2))
    mu = (one1.x + lambda * dx1 - two1.x) / dx2;
  else
    mu = (one1.y + lambda * dy1 - two1.y) / dy2;

  // If the point of intersection is off the end of the second segment, then
  // set it to be at the end and set the checkDistance flag.
  if (mu < 0.0)
  {
    mu = 0.0;
    checkDistance = true;
  }
  else if (mu > 1.0)
  {
    mu = 1.0;
    checkDistance = true;
  }
  // if checkDistance flag is true check distance between mu/lambda
  // positions (nearest points). If it is not within tolerance, return false.
  Pt3d lambdapos = one1 + Pt3d(lambda * dx1, lambda * dy1, lambda * dz1);
  Pt3d mupos = two1 + Pt3d(mu * dx2, mu * dy2, mu * dz2);
  if (checkDistance)
  {
    if (!gmEqualPointsXY(lambdapos, mupos, tol))
    {
      return false;
    }
  }

  if (gmColinearWithTol(one1, one2, lambdapos, tol / 2) &&
      gmColinearWithTol(two1, two2, lambdapos, tol / 2))
  {
    if (xi)
    {
      *xi = lambdapos.x;
    }
    if (yi)
    {
      *yi = lambdapos.y;
    }
  }
  else
  {
    if (xi)
    {
      *xi = mupos.x;
    }
    if (yi)
    {
      *yi = mupos.y;
    }
  }
  if (zi2)
    *zi2 = mupos.z;
  if (zi1)
    *zi1 = lambdapos.z;
  return true;
} // gmIntersectLineSegmentsWithTol
//------------------------------------------------------------------------------
/// \brief Returns true if the triangle is wrapped counter clockwise.
/// \param[in] vtx0: Vertex 0.
/// \param[in] vtx1: Vertex 1.
/// \param[in] vtx2: Vertex 2.
/// \return true if vertex order is counter clockwise.
//------------------------------------------------------------------------------
bool gmCounterClockwiseTri(const Pt3d& vtx0, const Pt3d& vtx1, const Pt3d& vtx2)
{
  double triarea1 = trArea(vtx0, vtx1, vtx2);
  return (triarea1 > 0.0);
} // gmCounterClockwiseTri
//------------------------------------------------------------------------------
/// \brief Returns the cross product of two 2-d vectors.
/// \param dx1: x component of vector 1
/// \param dy1: y component of vector 1
/// \param dx2: x component of vector 2
/// \param dy2: y component of vector 2
/// \return cross product of 2 vectors
///          vector 1 = dx1i + dy1j
///          vector 2 = dx2i + dy2j
//------------------------------------------------------------------------------
double gmCross2D(const double& dx1, const double& dy1, const double& dx2, const double& dy2)
{
  return (dx1 * dy2) - (dx2 * dy1);
} // gmCross2D
//------------------------------------------------------------------------------
/// \brief Returns the angle (0-2PI) in radians between the edges p and n
///        based on a ccw rotation from p to n.
/// \param[in] dxp: x of vector p.
/// \param[in] dyp: y of vector p.
/// \param[in] dxn: x of vector n.
/// \param[in] dyn: y of vector n.
/// \return The angle.
//------------------------------------------------------------------------------
double gmAngleBetween2DVectors(double dxp, double dyp, double dxn, double dyn)
{
  double magn, magp;

  magn = sqrt(sqr(dxn) + sqr(dyn));
  magp = sqrt(sqr(dxp) + sqr(dyp));
  return gmAngleBetween2DVectors(dxp, dyp, dxn, dyn, magn, magp);
} // gmAngleBetween2DVectors
//----- OVERLOAD ---------------------------------------------------------------
/// \brief Returns the angle (0-2PI) in radians between the edges p and n
///        based on a ccw rotation from p to n.
/// \param[in] dxp: x of vector p.
/// \param[in] dyp: y of vector p.
/// \param[in] dxn: x of vector n.
/// \param[in] dyn: y of vector n.
/// \param[in] a_magn: Magnitude of n.
/// \param[in] a_magp: Magnitude of p.
/// \return The angle.
//----- OVERLOAD ---------------------------------------------------------------
double gmAngleBetween2DVectors(double dxp,
                               double dyp,
                               double dxn,
                               double dyn,
                               double a_magn,
                               double a_magp)
{
  double theangle, cosign;

  if (a_magn == 0.0 || a_magp == 0.0)
    return (0.0);
  cosign = (dxn * dxp + dyn * dyp) / (a_magn * a_magp);
  if (cosign > 1.0)
    cosign = 1.0;
  if (cosign < -1.0)
    cosign = -1.0;
  theangle = acos(cosign);
  if (theangle == 0.0)
  {
    if ((dxn * dxp) + (dyn * dyp) < 0.0)
      theangle = XM_PI;
  }
  else if (gmCross2D(dxp, dyp, dxn, dyn) < 0.0)
    theangle = 2 * XM_PI - theangle;
  return theangle;
} // gmAngleBetween2DVectors
//------------------------------------------------------------------------------
/// \brief Returns the ccw angle (0-2pi) between p2-p1 and p2-p3
/// \param[in] p1: Point 1.
/// \param[in] p2: Point 2.
/// \param[in] p3: Point 3.
/// \return The angle.
//------------------------------------------------------------------------------
double gmAngleBetweenEdges(const Pt3d& p1, const Pt3d& p2, const Pt3d& p3)
{
  double dxp, dyp, dxn, dyn;
  /* compute the vectors */
  dxp = p1.x - p2.x;
  dyp = p1.y - p2.y;
  dxn = p3.x - p2.x;
  dyn = p3.y - p2.y;
  return gmAngleBetween2DVectors(dxp, dyp, dxn, dyn);
} // gmAngleBetweenEdges
//------------------------------------------------------------------------------
/// \brief Returns the ccw angle (0-2pi) between p2-p1 and p2-p3
/// \param[in] p1: Point 1.
/// \param[in] p2: Point 2.
/// \param[in] p3: Point 3.
/// \return The angle.
//------------------------------------------------------------------------------
double gmAngleBetweenEdges(const Pt2d& p1, const Pt2d& p2, const Pt2d& p3)
{
  double dxp, dyp, dxn, dyn;
  /* compute the vectors */
  dxp = p1.x - p2.x;
  dyp = p1.y - p2.y;
  dxn = p3.x - p2.x;
  dyn = p3.y - p2.y;
  return gmAngleBetween2DVectors(dxp, dyp, dxn, dyn);
} // gmAngleBetweenEdges
//------------------------------------------------------------------------------
/// \brief   Computes the deviation in direction from one seg to next
///          seg 1 is from a_p0 to a_p1
///          seg 2 is from a_p1 to a_p2
/// \param[in] a_p0: 1st point on 1st segment.
/// \param[in] a_p1: 2nd point on 1st segment and 1st point on second segment.
/// \param[in] a_p2: 2nd point on 2nd segment.
/// \return  Returns value between 0 and PI.
///          note: magnitude zero would cause problem - return PI in this case
///             returning PI would indicate that there could be discontinuity
///                 this is a streamlined version of gmAngleBetweenEdges that function
///             - computes a directed internal difference
///             - requires an extra cross product, several compares & multiplies
///             this function still has the sqrt and acos, so maybe not a big deal
//------------------------------------------------------------------------------
double gmComputeDeviationInDirection(const Pt3d& a_p0, const Pt3d& a_p1, const Pt3d& a_p2)
{
  double x1, y1, x2, y2, m, cosine_dev;
  x1 = a_p1.x - a_p0.x;
  y1 = a_p1.y - a_p0.y;
  x2 = a_p2.x - a_p1.x;
  y2 = a_p2.y - a_p1.y;
  m = sqrt(x1 * x1 + y1 * y1) * sqrt(x2 * x2 + y2 * y2);
  if (m > 0.0)
  {
    cosine_dev = Mmin((x1 * x2 + y1 * y2) / (m), 1.0);
    cosine_dev = Mmax(cosine_dev, -1.0);
    return acos(cosine_dev);
  }
  else
  {
    return XM_PI;
  }
} // gmComputeDeviationInDirection
//------------------------------------------------------------------------------
/// \brief   Determines if a point (x,y) is on the line defined by p1 and
///          p2. Assumes p1 and p2 aren't the same.
/// \param p1: first location defining a line
/// \param p2: second location defining a line
/// \param x: x coord of point to test
/// \param y: y coord of point to test
/// \param tol: tolerance for geometric comparison
/// \return  Returns true if the Point is on the line passing through p1 and p2
///          within the tolerance passed.
///          note: you should always be careful to consider the case when p1 and
///          p2 are very close to each other but not to x,y.  in that case this
///          test will almost always fail because it will be more susceptible to
///          roundoff error.  if you need to know if three points are colinear
///          you should use the gmColinear function.
//------------------------------------------------------------------------------
bool gmOnLineWithTol(const Pt3d& p1,
                     const Pt3d& p2,
                     const double x,
                     const double y,
                     const double tol)
{
  // compute vector components
  double dx = p2.x - p1.x;
  double dy = p2.y - p1.y;
  double mag = sqrt(sqr(dx) + sqr(dy));
  // check for extremely small segment
  if (mag <= tol)
    return gmEqualPointsXY(p1.x, p1.y, x, y);
  else
  {
    double a = -dy / mag;
    double b = dx / mag;
    double c = -a * p2.x - b * p2.y;
    // compute distance from line to (x,y)
    double d = a * x + b * y + c;
    return fabs(d) <= tol;
  }
} // gmOnLineWithTol
//------------------------------------------------------------------------------
/// \brief   determines if (x,y) is on the line passing through p1 & p2
///          and between p1 & p2.
/// \param a_pt1: first location defining segment
/// \param a_pt2: second location defining segment
/// \param a_x: x coord
/// \param a_y: y coord
/// \param a_tol: tolerance for comparison
/// \return true if on line segment
///
/// \note   For simple test of colinearity, call gmOnLineWithTol or gmColinear
//------------------------------------------------------------------------------
bool gmOnLineAndBetweenEndpointsWithTol(const Pt3d& a_pt1,
                                        const Pt3d& a_pt2,
                                        const double a_x,
                                        const double a_y,
                                        double a_tol)
{
  if ((Mmin(a_pt1.x, a_pt2.x) - a_tol <= a_x && Mmax(a_pt1.x, a_pt2.x) + a_tol >= a_x) &&
      (Mmin(a_pt1.y, a_pt2.y) - a_tol <= a_y && Mmax(a_pt1.y, a_pt2.y) + a_tol >= a_y))
    return gmOnLineWithTol(a_pt1, a_pt2, a_x, a_y, a_tol) == true;
  else
    return false;
} // gmOnLineAndBetweenEndpointsWithTol
//------------------------------------------------------------------------------
/// \brief   Compares \a a_pt to \a a_min and \a a_max. If \a a_pt is less than
///          \a a_min or greater than \a a_max, \a a_min and \a a_max are
///          updated.
/// \param[in] a_pt: A point.
/// \param[in,out] a_min: Minimum.
/// \param[in,out] a_max: Maximum.
//------------------------------------------------------------------------------
void gmAddToExtents(const Pt3d& a_pt, Pt3d& a_min, Pt3d& a_max)
{
  a_min.x = std::min(a_pt.x, a_min.x);
  a_min.y = std::min(a_pt.y, a_min.y);
  a_min.z = std::min(a_pt.z, a_min.z);
  a_max.x = std::max(a_pt.x, a_max.x);
  a_max.y = std::max(a_pt.y, a_max.y);
  a_max.z = std::max(a_pt.z, a_max.z);
} // gmAddToExtents
//------------------------------------------------------------------------------
/// \overload
/// \param[in] a_pt: A point.
/// \param[in,out] a_min: Minimum.
/// \param[in,out] a_max: Maximum.
//------------------------------------------------------------------------------
void gmAddToExtents(const Pt3d& a_pt, Pt2d& a_min, Pt2d& a_max)
{
  a_min.x = std::min(a_pt.x, a_min.x);
  a_min.y = std::min(a_pt.y, a_min.y);
  a_max.x = std::max(a_pt.x, a_max.x);
  a_max.y = std::max(a_pt.y, a_max.y);
} // gmAddToExtents
//------------------------------------------------------------------------------
/// \overload
/// \param[in] a_pt: A point.
/// \param[in,out] a_min: Minimum.
/// \param[in,out] a_max: Maximum.
//------------------------------------------------------------------------------
void gmAddToExtents(const Pt2d& a_pt, Pt3d& a_min, Pt3d& a_max)
{
  a_min.x = std::min(a_pt.x, a_min.x);
  a_min.y = std::min(a_pt.y, a_min.y);
  a_max.x = std::max(a_pt.x, a_max.x);
  a_max.y = std::max(a_pt.y, a_max.y);
} // gmAddToExtents
//------------------------------------------------------------------------------
/// \brief   Compute the 2d distance between 2 points.
/// \param x1: x coord of location 1
/// \param y1: y coord of location 1
/// \param x2: x coord of location 2
/// \param y2: y coord of location 2
/// \return Distance between location 1 and 2
///
/// \note See gmActualDistance if you don't want the distance in model units.
//------------------------------------------------------------------------------
double gmXyDistance(double x1, double y1, double x2, double y2)
{
  return sqrt(sqr(x1 - x2) + sqr(y1 - y2));
} // gmXyDistance
//------------------------------------------------------------------------------
/// \overload
/// \param[in] pt1: Point 1.
/// \param[in] pt2: Point 2.
/// \return Distance.
//------------------------------------------------------------------------------
double gmXyDistance(const Pt3d& pt1, const Pt3d& pt2)
{
  return sqrt(sqr(pt1.x - pt2.x) + sqr(pt1.y - pt2.y));
} // gmXyDistance
//------------------------------------------------------------------------------
/// \brief   Determine if angle a_v1, a_v2, a_v3 is a left turn, a right turn,
///          colinear 180 degrees, or colinear 0 degrees.
/// \param a_v1: first of 3 locations defining an angle
/// \param a_v2: second of 3 locations defining an angle
/// \param a_v3: third of 3 locations defining an angle
/// \param a_angtol: tolerance for comparing angle see notes
/// \return the type of turn defined by the angle
/// \note    a_angtol was added in August 2014 by AKZ (with consultation by APC).
///          This is the sine of the angle that is considered a turn at all.
///          The default is 0.0017453 (sine 0.1 and sine 179.9).
///          Therefore, this function will only classify two segments as
///          colinear if the angle between edges is less than this tol of
///          the angle between edges is greater than 180-tol.
///          In that case it will determine if the angle is considered to be
///          180 TURN_COLINEAR_180 or 0 TURN_COLINEAR_0  based on the cosine.
//------------------------------------------------------------------------------
Turn_enum gmTurn(const Pt3d& a_v1, const Pt3d& a_v2, const Pt3d& a_v3, double a_angtol)
{
  // compute sin T = (v3-v2)x(v1-v2)/(d12*d32)
  double dx32 = a_v3.x - a_v2.x;
  double dy32 = a_v3.y - a_v2.y;
  double dx12 = a_v1.x - a_v2.x;
  double dy12 = a_v1.y - a_v2.y;
  double d32 = sqrt(dx32 * dx32 + dy32 * dy32);
  double d12 = sqrt(dx12 * dx12 + dy12 * dy12);
  double mag = d12 * d32;
  double sint = (dx32 * dy12 - dx12 * dy32) / mag;

  // for 99.999 > T > 0.1 deg - sin T > 0.0017
  if (sint > a_angtol)
    return TURN_LEFT;
  else if (sint < -a_angtol)
    return TURN_RIGHT;

  // compute cos T = (v3-v2)DOT(v1-v2)/(d12*d32)
  // Near colinear case, Cosine should be near 1 or -1,
  // -1 indicates 180 deg
  double cost = (dx12 * dx32 + dy12 * dy32) / mag;
  if (cost < 0.0)
    return TURN_COLINEAR_180;
  return TURN_COLINEAR_0;
} // gmTurn
//------------------------------------------------------------------------------
/// \brief   Computes the centroid of points (but not of a polygon).
///          Shouldn't pass an empty vector (no checks are performed).
/// \param[in] a_points: Points.
/// \return Centroid.
//------------------------------------------------------------------------------
Pt3d gmComputeCentroid(const VecPt3d& a_points)
{
  Pt3d centroid;
  size_t size = a_points.size();
  for (size_t i = 0; i < size; ++i)
    centroid += a_points[i];
  return (centroid / (double)size);
} // gmComputeCentroid
//------------------------------------------------------------------------------
/// \brief Computes the plan view centroid of a non-self-intersecting polygon.
/// \param[in] pts: Points.
/// \return Centroid.
//------------------------------------------------------------------------------
Pt3d gmComputePolygonCentroid(const VecPt3d& pts)
{
  Pt3d centroid;
  if (pts.empty())
    return centroid;
  // get offset to use in calculation below to fix precision issues
  double xMax = XM_DBL_LOWEST, yMax = XM_DBL_LOWEST, xMin = XM_DBL_HIGHEST, yMin = XM_DBL_HIGHEST;
  size_t i = 0;
  for (i = 0; i < pts.size(); ++i)
  {
    double x = pts[i].x;
    double y = pts[i].y;
    xMax = (x > xMax) ? x : xMax;
    yMax = (y > yMax) ? y : yMax;
    xMin = (x < xMin) ? x : xMin;
    yMin = (y < yMin) ? y : yMin;
  }
  double xOffset = (xMax + xMin) / 2.0;
  double yOffset = (yMax + yMin) / 2.0;
  // For all vertices except last
  double signedArea = 0.0;
  for (i = 0; i < pts.size() - 1; ++i)
  {
    double x0 = pts[i].x - xOffset;
    double y0 = pts[i].y - yOffset;
    double x1 = pts[i + 1].x - xOffset;
    double y1 = pts[i + 1].y - yOffset;
    double a = x0 * y1 - x1 * y0;
    signedArea += a;
    centroid.x += (x0 + x1) * a;
    centroid.y += (y0 + y1) * a;
  }
  // Do last vertex
  double x0 = pts[i].x - xOffset;
  double y0 = pts[i].y - yOffset;
  double x1 = pts[0].x - xOffset;
  double y1 = pts[0].y - yOffset;
  double a = x0 * y1 - x1 * y0;
  signedArea += a;
  centroid.x += (x0 + x1) * a;
  centroid.y += (y0 + y1) * a;
  signedArea *= 0.5;
  centroid.x /= (6.0 * signedArea);
  centroid.y /= (6.0 * signedArea);
  centroid.x += xOffset;
  centroid.y += yOffset;
  return centroid;
} // gmComputePolygonCentroid
//------------------------------------------------------------------------------
/// \brief  intersects the plan projection of two line segments (bad func name)
///         segment 1 = one1,one2  = one1 + lambda(one2 - one1)
///         segment 2 = two1,two2  = two1 + mu (two2 - two1)
/// \param one1: first point on first segment
/// \param one2: second point on first segment
/// \param two1: first point on second segment
/// \param two2: second point on second segment
/// \return true if segments intersect, else false
//------------------------------------------------------------------------------
bool gmLinesIntersect(const Pt3d& one1, const Pt3d& one2, const Pt3d& two1, const Pt3d& two2)
{
  // do a trivial rejection
  double min = std::min(one1.x, one2.x);
  double max = std::max(two1.x, two2.x);
  const double tol(XM_ZERO_TOL);
  if (GT_TOL(min, max, tol))
    return false;
  max = std::max(one1.x, one2.x);
  min = std::min(two1.x, two2.x);
  if (LT_TOL(max, min, tol))
    return false;
  min = std::min(one1.y, one2.y);
  max = std::max(two1.y, two2.y);
  if (GT_TOL(min, max, tol))
    return false;
  max = std::max(one1.y, one2.y);
  min = std::min(two1.y, two2.y);
  if (LT_TOL(max, min, tol))
    return false;
  // define the vectors
  Pt2d d1(one2 - one1), d2(two2 - two1);
  // see if lines are parallel
  // SMS bug fix: replaced tolerance check with a non-zero check.
  // Sometimes we have very small tolerances
  double cross = (d2.x * d1.y) - (d2.y * d1.x);
  if (cross == 0.0)
    return false;
  // if the lines intersect s should be between 0.0 and 1.0
  double s = ((d1.x * (two1.y - one1.y)) + (d1.y * (one1.x - two1.x))) / cross;
  if (LT_TOL(s, 0.0, tol) || GT_TOL(s, 1.0, tol))
    return false;
  // if the lines intersect t should be between -1.0 and 0.0
  double t = ((d2.x * (one1.y - two1.y)) + (d2.y * (two1.x - one1.x))) / cross;
  if (LT_TOL(t, -1.0, tol) || GT_TOL(t, 0.0, tol))
    return false;
  return true;
} // gmLinesIntersect
//------------------------------------------------------------------------------
/// \brief   Determines whether (a_x, a_y) is inside=1, on=0, or
///          outside=-1 the polygon defined by the given vertices.
///
///          DON'T repeat the first point at the end of the polygon array
/// \see EFastPointInPoly
/// \see GmPolygon
/// \param[in] a_verts: The polygon in CW or CCW order (doesn't matter which).
/// \param[in] a_num:   Number of verts in a_verts.
/// \param[in] a_x:     X coordinate of point to test.
/// \param[in] a_y:     Y coordinate of point to test.
/// \param[in] a_tol:   Tolerance.
/// \return 1 (inside), 0 (on), or -1 (outside)
//------------------------------------------------------------------------------
template <typename T>
int gmPointInPolygon2D(const T* a_verts,
                       const size_t a_num,
                       const double a_x,
                       const double a_y,
                       const double a_tol)
{
  if (a_verts && a_num)
  {
    int nmleft = 0, nmrght = 0;
    double dy2 = fabs(a_verts[0].y - a_y);
    double diff;
    for (size_t i = 0; i < a_num; i++)
    {
      size_t i2 = 0;
      if (i != a_num - 1)
        i2 = i + 1;
      double dy1 = dy2;
      dy2 = fabs(a_verts[i2].y - a_y);
      if (dy1 <= a_tol)
      {
        if (dy2 <= a_tol)
        {
          // Case #1 - edge is on the same y value as the point
          //         - see if the point is on the edge
          if (((a_verts[i].x >= a_x) && (a_verts[i2].x <= a_x)) ||
              ((a_verts[i].x <= a_x) && (a_verts[i2].x >= a_x)))
            return (0);
        }
        else
        {
          // Case #2 - first vertex has same y value as point
          //         - see if the point actually coincides with
          //           the ith vertex
          diff = a_verts[i].x - a_x;
          if (fabs(diff) <= a_tol)
            return (0);
          else if (a_verts[i].y < a_verts[i2].y)
          {
            // edge ascends, classify as right or left
            if (diff > 0)
              ++nmrght;
            else
              ++nmleft;
          }
        }
      }
      else if (dy2 <= a_tol)
      {
        // Case #3 - 2nd vertex of edge lies on dividing plane
        //         - see if the point actually coincides with
        //           the i2-th vertex
        diff = a_verts[i2].x - a_x;
        if (fabs(diff) <= a_tol)
          return (0);
        else if (a_verts[i2].y < a_verts[i].y)
        {
          // if edge descends classify as right or left
          if (diff > 0)
            ++nmrght;
          else
            ++nmleft;
        }
      }
      else if (((a_verts[i].y < a_y) && (a_verts[i2].y > a_y)) ||
               ((a_verts[i].y > a_y) && (a_verts[i2].y < a_y)))
      {
        // Case #4 - edge cleanly intersects dividing plane
        //         - flag edge as left, right or on edge
        double val = a_verts[i].x + (a_verts[i2].x - a_verts[i].x) * (a_y - a_verts[i].y) /
                                      (a_verts[i2].y - a_verts[i].y);
        diff = val - a_x;
        if (fabs(diff) <= a_tol)
          return (0);
        else if (diff > 0)
          ++nmrght;
        else
          ++nmleft;
      }
    }
    nmleft = nmleft % 2;
    nmrght = nmrght % 2;
    if (nmleft != nmrght)
      return (-1); // this should never happen actually
    else if (nmleft == 1)
      return (1);
    else
      return (-1);
  }
  return (-1);
} // gmPointInPolygon2D
//------------------------------------------------------------------------------
/// \overload
/// \param[in] a_verts: The polygon in CW or CCW order (doesn't matter which).
/// \param[in] a_num:   Number of verts in a_verts.
/// \param[in] a_x:     X coordinate of point to test.
/// \param[in] a_y:     Y coordinate of point to test.
/// \return 1 (inside), 0 (on), or -1 (outside)
//------------------------------------------------------------------------------
int gmPointInPolygon2D(const Pt3d* a_verts, size_t a_num, double a_x, double a_y)
{
  return gmPointInPolygon2D(a_verts, a_num, a_x, a_y, gmXyTol());
} // gmPointInPolygon2D
//------------------------------------------------------------------------------
/// \overload
/// \param[in] a_verts: The polygon in CW or CCW order (doesn't matter which).
/// \param[in] a_num:   Number of verts in a_verts.
/// \param[in] a_pt:    Point to test.
/// \return 1 (inside), 0 (on), or -1 (outside)
//------------------------------------------------------------------------------
int gmPointInPolygon2D(const Pt3d* a_verts, size_t a_num, Pt3d a_pt)
{
  return gmPointInPolygon2D(a_verts, a_num, a_pt.x, a_pt.y, gmXyTol());
} // gmPointInPolygon2D
//------------------------------------------------------------------------------
/// \overload
/// \param[in] a_verts: The polygon in CW or CCW order (doesn't matter which).
/// \param[in] a_num:   Number of verts in a_verts.
/// \param[in] a_pt:    Point to test.
/// \return 1 (inside), 0 (on), or -1 (outside)
//------------------------------------------------------------------------------
int gmPointInPolygon2D(const Pt2i* a_verts, size_t a_num, Pt2d a_pt)
{
  return gmPointInPolygon2D(a_verts, a_num, a_pt.x, a_pt.y, gmXyTol());
} // gmPointInPolygon2D
//------------------------------------------------------------------------------
/// \overload
/// \param[in] a_verts: The polygon in CW or CCW order (doesn't matter which).
/// \param[in] a_num:   Number of verts in a_verts.
/// \param[in] a_pt:    Point to test.
/// \return 1 (inside), 0 (on), or -1 (outside)
//------------------------------------------------------------------------------
int gmPointInPolygon2D(const Pt2i* a_verts, size_t a_num, Pt3d a_pt)
{
  return gmPointInPolygon2D(a_verts, a_num, a_pt.x, a_pt.y, gmXyTol());
} // gmPointInPolygon2D
//------------------------------------------------------------------------------
/// \overload
/// \param[in] a_verts: The polygon in CW or CCW order (doesn't matter which).
/// \param[in] a_num:   Number of verts in a_verts.
/// \param[in] a_pt:    Point to test.
/// \return 1 (inside), 0 (on), or -1 (outside)
//------------------------------------------------------------------------------
int gmPointInPolygon2D(const Pt2i* a_verts, size_t a_num, Pt2i a_pt)
{
  return gmPointInPolygon2D(a_verts, a_num, a_pt.x, a_pt.y, gmXyTol());
} // gmPointInPolygon2D
//------------------------------------------------------------------------------
/// \overload
/// \param[in] a_verts: The polygon in CW or CCW order (doesn't matter which).
/// \param[in] a_pt:    Point to test.
/// \return 1 (inside), 0 (on), or -1 (outside)
//------------------------------------------------------------------------------
int gmPointInPolygon2D(const VecPt3d& a_verts, const Pt3d& a_pt)
{
  return gmPointInPolygon2D(&a_verts[0], a_verts.size(), a_pt.x, a_pt.y, gmXyTol());
} // gmPointInPolygon2D
//------------------------------------------------------------------------------
/// \overload
/// \param[in] a_verts: The polygon in CW or CCW order (doesn't matter which).
/// \param[in] a_num:   Number of verts in a_verts.
/// \param[in] a_x:     X coordinate of point to test.
/// \param[in] a_y:     Y coordinate of point to test.
/// \param[in] a_tol:   Tolerance.
/// \return 1 (inside), 0 (on), or -1 (outside)
//------------------------------------------------------------------------------
template int gmPointInPolygon2D<Pt3d>(const Pt3d* a_verts,
                                      const size_t a_num,
                                      const double a_x,
                                      const double a_y,
                                      const double a_tol);
//------------------------------------------------------------------------------
/// \overload
/// \param[in] a_verts: The polygon in CW or CCW order (doesn't matter which).
/// \param[in] a_num:   Number of verts in a_verts.
/// \param[in] a_x:     X coordinate of point to test.
/// \param[in] a_y:     Y coordinate of point to test.
/// \param[in] a_tol:   Tolerance.
/// \return 1 (inside), 0 (on), or -1 (outside)
//------------------------------------------------------------------------------
template int gmPointInPolygon2D<Pt2d>(const Pt2d* a_verts,
                                      const size_t a_num,
                                      const double a_x,
                                      const double a_y,
                                      const double a_tol);
//------------------------------------------------------------------------------
/// \overload
/// \param[in] a_verts: The polygon in CW or CCW order (doesn't matter which).
/// \param[in] a_num:   Number of verts in a_verts.
/// \param[in] a_x:     X coordinate of point to test.
/// \param[in] a_y:     Y coordinate of point to test.
/// \param[in] a_tol:   Tolerance.
/// \return 1 (inside), 0 (on), or -1 (outside)
//------------------------------------------------------------------------------
template int gmPointInPolygon2D<Pt2i>(const Pt2i* a_verts,
                                      const size_t a_num,
                                      const double a_x,
                                      const double a_y,
                                      const double a_tol);
//------------------------------------------------------------------------------
/// \brief  Given extents (min, max), compute a tolerance for the xy plane to
///         be used with geometric functions.
/// \param a_mn: Minimum.
/// \param a_mx: Maximum.
/// \return Tolerance.
//------------------------------------------------------------------------------
double gmComputeXyTol(const Pt3d& a_mn, const Pt3d& a_mx)
{
  double d = gmXyDistance(a_mn, a_mx);
  double const kFactor = 1e-9;
  double xytol = d * kFactor;
  if (xytol < kFactor)
  {
    xytol = kFactor;
  }
  return xytol;
} // gmComputeXyTol
//------------------------------------------------------------------------------
/// \brief Get or set (set first time) global xy tolerance for float operations.
/// \param a_set: True if setting.
/// \param a_value: The value if setting.
/// \return XY tolerance.
//------------------------------------------------------------------------------
double gmXyTol(bool a_set /*false*/, double a_value /*1e-9*/)
{
  static double xytol = a_value;
  if (a_set)
    xytol = a_value;
  return xytol;
} // gmXyTol
//------------------------------------------------------------------------------
/// \brief  Get or set (set first time) global z tolerance for float operations
/// \param a_set: True if setting.
/// \param a_value: The value if setting.
/// \return Z tolerance.
//------------------------------------------------------------------------------
double gmZTol(bool a_set, double a_value)
{
  static double ztol = a_value;
  if (a_set)
    ztol = a_value;
  return ztol;
} // gmZTol
//------------------------------------------------------------------------------
/// \brief  Returns true if the points are equal to within gmXyTol().
/// \param x1: x of point 1.
/// \param y1: y of point 1.
/// \param x2: x of point 2.
/// \param y2: y of point 2.
/// \param tolerance: tolerance.
/// \return true if equal.
//------------------------------------------------------------------------------
bool gmEqualPointsXY(double x1, double y1, double x2, double y2, double tolerance)
{
  double dx = fabs(x1 - x2);
  double dy = fabs(y1 - y2);
  if (dx > tolerance || dy > tolerance)
    return false;
  else if (sqrt(dx * dx + dy * dy) <= tolerance)
    return true;
  else
    return false;
} // gmEqualPointsXY
//------------------------------------------------------------------------------
/// \overload
/// \param x1: x of point 1.
/// \param y1: y of point 1.
/// \param x2: x of point 2.
/// \param y2: y of point 2.
/// \return true if equal.
//------------------------------------------------------------------------------
bool gmEqualPointsXY(double x1, double y1, double x2, double y2)
{
  return gmEqualPointsXY(x1, y1, x2, y2, gmXyTol());
} // gmEqualPointsXY
//------------------------------------------------------------------------------
/// \overload
/// \param a_pt1: Point 1.
/// \param a_pt2: Point 2.
/// \param tol: tolerance.
/// \return true if equal within tolerance.
//------------------------------------------------------------------------------
bool gmEqualPointsXY(const Pt2d& a_pt1, const Pt2d& a_pt2, double tol)
{
  return gmEqualPointsXY(a_pt1.x, a_pt1.y, a_pt2.x, a_pt2.y, tol);
} // gmEqualPointsXY
//------------------------------------------------------------------------------
/// \overload
/// \param a_pt1: Point 1.
/// \param a_pt2: Point 2.
/// \param tol: tolerance.
/// \return true if equal within tolerance.
//------------------------------------------------------------------------------
bool gmEqualPointsXY(const Pt3d& a_pt1, const Pt3d& a_pt2, double tol)
{
  return gmEqualPointsXY(a_pt1.x, a_pt1.y, a_pt2.x, a_pt2.y, tol);
} // gmEqualPointsXY
//------------------------------------------------------------------------------
/// \overload
/// \param point1: Point 1.
/// \param point2: Point 2.
/// \return true if equal within tolerance.
//------------------------------------------------------------------------------
bool gmEqualPointsXY(const Pt2i& point1, const Pt2i& point2)
{
  if (point1.x == point2.x && point1.y == point2.y)
    return true;
  return false;
} // gmEqualPointsXY
//------------------------------------------------------------------------------
/// \brief  Returns true if the points are equal to within tolerance.
/// \param x1: x of point 1.
/// \param y1: y of point 1.
/// \param z1: z of point 1.
/// \param x2: x of point 2.
/// \param y2: y of point 2.
/// \param z2: z of point 2.
/// \param tolerance: tolerance.
/// \return true if equal.
//------------------------------------------------------------------------------
bool gmEqualPointsXYZ(double x1,
                      double y1,
                      double z1,
                      double x2,
                      double y2,
                      double z2,
                      double tolerance)
{
  if ((fabs(x1 - x2) <= tolerance) && (fabs(y1 - y2) <= tolerance) && (fabs(z1 - z2) <= tolerance))
    return true;
  return false;
} // gmEqualPointsXYZ
//------------------------------------------------------------------------------
/// \overload
/// \param x1: x of point 1.
/// \param y1: y of point 1.
/// \param z1: z of point 1.
/// \param x2: x of point 2.
/// \param y2: y of point 2.
/// \param z2: z of point 2.
/// \return true if equal within tolerance.
//------------------------------------------------------------------------------
bool gmEqualPointsXYZ(double x1, double y1, double z1, double x2, double y2, double z2)
{
  return gmEqualPointsXYZ(x1, y1, z1, x2, y2, z2, gmXyTol());
} // gmEqualPointsXYZ
//------------------------------------------------------------------------------
/// \overload
/// \param pt1: Point 1.
/// \param pt2: Point 2.
/// \param tol: tolerance.
/// \return true if equal within tolerance.
//------------------------------------------------------------------------------
bool gmEqualPointsXYZ(const Pt3d& pt1, const Pt3d& pt2, double tol)
{
  return gmEqualPointsXYZ(pt1.x, pt1.y, pt1.z, pt2.x, pt2.y, pt2.z, tol);
} // gmEqualPointsXYZ
//------------------------------------------------------------------------------
/// \brief  Returns true if (x,y) is in the tri formed by p1, p2, p3
/// \param[in] p1: Point 1.
/// \param[in] p2: Point 2.
/// \param[in] p3: Point 3.
/// \param[in] x: x of point.
/// \param[in] y: y of point.
/// \param[in] tol: Tolerance.
/// \return true if in.
//------------------------------------------------------------------------------
bool gmPointInTriangleWithTol(const Pt3d* p1,
                              const Pt3d* p2,
                              const Pt3d* p3,
                              double x,
                              double y,
                              double tol)
{
  if (gmInsideOrOnLineWithTol(p1, p2, p3, x, y, tol))
    if (gmInsideOrOnLineWithTol(p2, p3, p1, x, y, tol))
      if (gmInsideOrOnLineWithTol(p3, p1, p2, x, y, tol))
        return true;
  return false;
} // gmPointInTriangleWithTol
//------------------------------------------------------------------------------
/// \brief Returns true if the (x,y) is on the line segment (p1,p2) or
/// on the same side of the line as "inpoint". ASSERTs in debug if "inpoint"
/// is on the line (within tol).
/// \param[in] p1: Point 1.
/// \param[in] p2: Point 2.
/// \param[in] inpoint: Point on same side of line as xy if true is returned.
/// \param[in] x: x of point.
/// \param[in] y: y of point.
/// \param[in] tol: Tolerance.
/// \param[out] dist: How far "outside" the point is. A negative distance
///             indicates point is in by that distance.
/// \return true if in.
//------------------------------------------------------------------------------
bool gmInsideOrOnLineWithTol(const Pt3d* p1,
                             const Pt3d* p2,
                             const Pt3d* inpoint,
                             const double x,
                             const double y,
                             const double tol,
                             double* dist /*= NULL */)
{
  double dx, dy, a, b, c, mag;
  double d1, d2;
  /* make sure line is not a point */
  dx = p2->x - p1->x;
  dy = p2->y - p1->y;
  mag = sqrt(sqr(dx) + sqr(dy));
  if (mag <= tol)
  {
    return gmEqualPointsXY(p1->x, p1->y, x, y);
  }
  else
  {
    a = -dy / mag;
    b = dx / mag;
    c = -a * p1->x - b * p1->y;
    /* compute distance from line to (x,y) */
    d1 = a * x + b * y + c;
    /* compute distance from line to opposite p */
    d2 = a * inpoint->x + b * inpoint->y + c;
    // if inpoint is really on, we can't make a judgment on the point
    if (Mfabs(d2) <= tol)
    {
      // AKZ - Since "inpoint" is usually the other point in a triangle
      //       this usually indicates a poorly formed triangle.
      //       You should determine how this triangle was formed.
      //      XM_ASSERT(0);
      return false;
    }
    /* return true if point on line */
    if (Mfabs(d1) <= tol)
    {
      if (dist)
      {
        if (d1 * d2 < 0.0)
        {
          *dist = Mfabs(d1);
        }
        else
        {
          *dist = -Mfabs(d1);
        }
      }
      return true;
    }
    /* return true if d1 and d2 have same sign */
    if ((d1 < 0.0) && (d2 < 0.0))
    {
      if (dist)
      {
        *dist = -Mfabs(d1);
      }
      return true;
    }
    else if ((d1 > 0.0) && (d2 > 0.0))
    {
      if (dist)
      {
        *dist = -Mfabs(d1);
      }
      return true;
    }
    else
    {
      if (dist)
      {
        *dist = Mfabs(d1);
      }
      return false;
    }
  }
} // gmInsideOrOnLineWithTol
//------------------------------------------------------------------------------
/// \brief   Compute 2d planview projection of area of polygon.
/// \param pts: locations defining the polygon
/// \param npoints: number of pts
/// \return Area of the polygon.
///
/// CCW = positive area, CW = negative. Don't repeat the last point.
//------------------------------------------------------------------------------
double gmPolygonArea(const Pt3d* pts, size_t npoints)
{
  size_t id;
  double area = 0.0;

  /* original method with precision errors
  for (id = 0; id < npoints; id++)
  {
    if (id != (npoints - 1))
    {
      area += (pts[id].x * pts[id + 1].y);
      area -= (pts[id].y * pts[id + 1].x);
    }
    else
    {
      area += (pts[id].x * pts[0].y);
      area -= (pts[id].y * pts[0].x);
    }
  }
  area /= 2.0;
  */

  // AKZ 2/15/2018
  // I changed the implementation to translate the polygon
  //   so that the first point is at the origin
  // Reduces round off error due to large coordinates
  // Reduces the number of computations because the first and last
  //   computations in the loop would be 0.0
  VecDbl x, y;
  double x0 = pts[0].x;
  double y0 = pts[0].y;
  for (id = 1; id < npoints; id++)
  {
    x.push_back((pts[id].x - x0));
    y.push_back((pts[id].y - y0));
  }
  for (id = 0; id < npoints - 2; id++)
  {
    area += (x[id] * y[id + 1]);
    area -= (y[id] * x[id + 1]);
  }
  area /= 2.0;

  return (area);
} // gmPolygonArea
//------------------------------------------------------------------------------
/// \brief Useful in testing to create a VecPt3d from a C array of xy pairs.
/// \param[in] a_array: Array of xy pairs ([x][y][x][y]...).
/// \param[in] a_size: Array size.
/// \return Vector of Pt3d.
//------------------------------------------------------------------------------
VecPt3d gmArrayToVecPt3d(double* a_array, int a_size)
{
  VecPt3d v(a_size / 2);
  for (int i = 0; i < a_size; i += 2)
  {
    v[i / 2].x = a_array[i];
    v[i / 2].y = a_array[i + 1];
  }
  return v;
} // gmArrayToVecPt3d
//------------------------------------------------------------------------------
/// \brief Calculates the envelope of a vector of points
/// \param[in]   a_pts: Array of points
/// \param[out]  a_min: Min x,y,z of the points
/// \param[out]  a_max: Max x,y,z of the points
//------------------------------------------------------------------------------
void gmEnvelopeOfPts(const VecPt3d& a_pts, Pt3d& a_min, Pt3d& a_max)
{
  a_min = a_max = Pt3d();
  XM_ENSURE_TRUE(!a_pts.empty());
  a_min = a_max = a_pts.front();
  for (size_t i = 0; i < a_pts.size(); ++i)
  {
    if (a_pts[i].x < a_min.x)
      a_min.x = a_pts[i].x;
    if (a_pts[i].y < a_min.y)
      a_min.y = a_pts[i].y;
    if (a_pts[i].z < a_min.z)
      a_min.z = a_pts[i].z;
    if (a_pts[i].x > a_max.x)
      a_max.x = a_pts[i].x;
    if (a_pts[i].y > a_max.y)
      a_max.y = a_pts[i].y;
    if (a_pts[i].z > a_max.z)
      a_max.z = a_pts[i].z;
  }
} // gmEnvelopeOfPts
//------------------------------------------------------------------------------
/// \brief Orders array of points counter clockwise. Given non-empty array of
/// points. array of point indices ordered counter clockwise based on the angle
/// from the centroid where angle starts at point at startindex
/// \param[in]   a_pts:        Array of points
/// \param[out]  a_ccwOrder:   Array of indices to points
/// \param[in]   a_startindex: Starting position in arrays
//------------------------------------------------------------------------------
void gmOrderPointsCounterclockwise(const VecPt3d& a_pts, VecInt& a_ccwOrder, int a_startindex)
{
  int numnodes = (int)a_pts.size();

  // compute centroid of points
  Pt2d center;
  double sumx = 0.0;
  double sumy = 0.0;
  for (int i = 0; i < numnodes; i++)
  {
    sumx += a_pts[i].x;
    sumy += a_pts[i].y;
  }

  center.x = sumx / numnodes;
  center.y = sumy / numnodes;

  // compute polar angle for each node about the centroid of the element
  // along with the original point index
  std::vector<std::pair<float, int> > angleIndex(numnodes);
  for (int i = 0; i < numnodes; i++)
  {
    float angle = (float)(atan2(a_pts[i].y - center.y, a_pts[i].x - center.x));
    angleIndex[i] = std::pair<float, int>(angle, i);
  }

  std::stable_sort(angleIndex.begin(), angleIndex.end());

  // find where starting index is in sorted array
  auto startIter = angleIndex.begin();
  while (startIter != angleIndex.end() && startIter->second != a_startindex)
  {
    ++startIter;
  }
  if (startIter == angleIndex.end())
    startIter = angleIndex.begin();

  // assign the result preserving the order with a_startindex the first node
  a_ccwOrder.resize(numnodes, 0);
  size_t j = 0;
  for (auto iter = startIter; iter != angleIndex.end(); ++iter)
  {
    a_ccwOrder[j] = iter->second;
    j++;
  }
  for (auto iter = angleIndex.begin(); iter != startIter; ++iter)
  {
    a_ccwOrder[j] = iter->second;
    j++;
  }
} // gmOrderPointsCounterclockwise
//------------------------------------------------------------------------------
/// \brief Overload to gmOrderPointsCounterclockwise
/// \param[out]  a_pts:        Array of points
//------------------------------------------------------------------------------
void gmOrderPointsCounterclockwise(VecPt3d& a_pts)
{
  if (a_pts.empty())
    return;

  VecInt ccwOrder;
  VecPt3d pts(a_pts);
  gmOrderPointsCounterclockwise(pts, ccwOrder);
  for (size_t i = 0; i < ccwOrder.size(); ++i)
  {
    a_pts[i] = pts[ccwOrder[i]];
  }
} // gmOrderPointsCounterclockwise
//------------------------------------------------------------------------------
/// \brief Finds the closest point to another point on a segment
/// \param[in]  a_pt1: First point of segment
/// \param[in]  a_pt2: Second point of segment
/// \param[in]  a_pt: Point used to find closest point on the segment
/// \param[out] a_newpt: The point on the segment (a_pt1, a_pt2) that is
/// closest to a_pt.
/// \param[in]  a_tol: Tolerance
/// \return parametric value along the line a_pt1, a_pt2 that is the location
/// of a_newpt.
//------------------------------------------------------------------------------
double gmFindClosestPtOnSegment(const Pt3d& a_pt1,
                                const Pt3d& a_pt2,
                                const Pt3d& a_pt,
                                Pt3d& a_newpt,
                                const double a_tol)
{
  double t = gmPtDistanceAlongSegment(a_pt1, a_pt2, a_pt, a_tol);
  if (t < 0.0)
  {
    a_newpt.x = a_pt1.x;
    a_newpt.y = a_pt1.y;
  }
  else if (t > 1.0)
  {
    a_newpt.x = a_pt2.x;
    a_newpt.y = a_pt2.y;
  }
  else
  {
    double dx = a_pt2.x - a_pt1.x;
    double dy = a_pt2.y - a_pt1.y;
    a_newpt.x = a_pt1.x + dx * t;
    a_newpt.y = a_pt1.y + dy * t;
  }
  return t;
} // gmFindClosestPtOnSegment
//------------------------------------------------------------------------------
/// \brief Finds the distance along a segment for the location closest to a_pt
/// \param[in]  a_pt1: First point of segment
/// \param[in]  a_pt2: Second point of segment
/// \param[in]  a_pt: Point used to find closest point on the segment
/// \param[in]  a_tol: Tolerance
/// \return parametric value along the line a_pt1, a_pt2 that is the closest
/// location to a_pt.
//------------------------------------------------------------------------------
double gmPtDistanceAlongSegment(const Pt3d& a_pt1,
                                const Pt3d& a_pt2,
                                const Pt3d& a_pt,
                                const double a_tol)
{
  double dx, dy, t;

  dx = a_pt2.x - a_pt1.x;
  dy = a_pt2.y - a_pt1.y;

  if ((dx == 0.0 && dy == 0.0) || (sqrt(dx * dx + dy * dy) <= a_tol))
  {
    t = -1.0;
  }
  else
  {
    t = ((a_pt.x - a_pt1.x) * dx + (a_pt.y - a_pt1.y) * dy) / (dx * dx + dy * dy);
  }

  return t;
} // gmPtDistanceAlongSegment
//------------------------------------------------------------------------------
/// \brief Returns TRUE if the Point on the same side of the line (defined by
/// vertex1 and vertex2) as oppositevertex.
/// \param[in] a_vertex1 : First point on the line segment
/// \param[in] a_vertex2 : Second point on the line segment
/// \param[in] a_oppositevertex : Point on one side of the line
/// \param[in] a_x : X location of the point being tested
/// \param[in] a_y : Y location of the point being tested
/// \param[in] a_tol : Tolerance for degenerate cases
/// \return true if the a_x, a_y are inside the line defined by a_vertex1
/// and a_vertex2. Inside is defined as the opposite side of a_oppositevertex
//------------------------------------------------------------------------------
bool gmInsideOfLineWithTol(const Pt3d& a_vertex1,
                           const Pt3d& a_vertex2,
                           const Pt3d& a_oppositevertex,
                           const double a_x,
                           const double a_y,
                           const double a_tol)
{
  double dx = a_vertex2.x - a_vertex1.x;
  double dy = a_vertex2.y - a_vertex1.y;
  double mag = sqrt(sqr(dx) + sqr(dy));
  if (mag <= a_tol)
  {
    return gmEqualPointsXY(a_vertex1.x, a_vertex1.y, a_x, a_y);
  }
  else
  {
    double a = -dy / mag;
    double b = dx / mag;
    double c = -a * a_vertex1.x - b * a_vertex1.y;
    /* compute the distance from the line to the Point */
    double d1 = a * a_x + b * a_y + c;
    /* compute the distance from the line to the opposite vertex */
    double d2 = a * a_oppositevertex.x + b * a_oppositevertex.y + c;
    if (fabs(d1) <= a_tol)
      return false;
    else if ((d1 < 0.0) && (d2 < 0.0))
      return true;
    else if ((d1 > 0.0) && (d2 > 0.0))
      return true;
    else
      return false;
  }
} // gmInsideOfLine
//------------------------------------------------------------------------------
/// \brief Get the 2D extents of a vector of points.
/// \param[in] a_points: The points.
/// \param[out] a_min: Minimum point (xy) of bounding rectangle.
/// \param[out] a_max: Maximum point (xy) of bounding rectangle.
//------------------------------------------------------------------------------
void gmExtents2D(const VecPt3d& a_points, Pt2d& a_min, Pt2d& a_max)
{
  XM_ENSURE_TRUE_VOID_NO_ASSERT(!a_points.empty());

  a_min.x = a_max.x = a_points.at(0).x;
  a_min.y = a_max.y = a_points.at(0).y;
  for (int i = 1; i < (int)a_points.size(); i++)
  {
    gmAddToExtents(a_points[i], a_min, a_max);
  }
} // gmExtents2D
//----- OVERLOAD ---------------------------------------------------------------
/// \overload
/// \param[in] a_points: Vector of points.
/// \param[in,out] a_min: Minimum value.
/// \param[in,out] a_max: Maximum value.
//----- OVERLOAD ---------------------------------------------------------------
void gmExtents2D(const VecPt3d& a_points, Pt3d& a_min, Pt3d& a_max)
{
  XM_ENSURE_TRUE_VOID_NO_ASSERT(!a_points.empty());

  a_min.x = a_max.x = a_points.at(0).x;
  a_min.y = a_max.y = a_points.at(0).y;
  a_min.z = a_max.z = 0.0;
  for (int i = 1; i < (int)a_points.size(); i++)
  {
    gmAddToExtents((Pt2d)a_points[i], a_min, a_max);
  }
} // gmExtents2D
//------------------------------------------------------------------------------
/// \brief Get the 3D extents of a vector of points.
/// \param[in] a_points: The points.
/// \param[out] a_min: Minimum point (xyz) of bounding box.
/// \param[out] a_max: Maximum point (xyz) of bounding box.
//------------------------------------------------------------------------------
void gmExtents3D(const VecPt3d& a_points, Pt3d& a_min, Pt3d& a_max)
{
  XM_ENSURE_TRUE_VOID_NO_ASSERT(!a_points.empty());

  a_min.x = a_max.x = a_points.at(0).x;
  a_min.y = a_max.y = a_points.at(0).y;
  a_min.z = a_max.z = a_points.at(0).z;
  for (int i = 1; i < (int)a_points.size(); i++)
  {
    gmAddToExtents(a_points[i], a_min, a_max);
  }
} // gmExtents3D
//------------------------------------------------------------------------------
/// \brief Returns the angle in radians perpendicular to the two points.
/// \param[in] a_pt1 The first point defining an edge
/// \param[in] a_pt2 The second point defining an edge
/// \return angle perpendicular to the edge
//------------------------------------------------------------------------------
double gmPerpendicularAngle(const Pt3d& a_pt1, const Pt3d& a_pt2)
{
  double hypot;
  double deltax, deltay, arad, theangle;

  deltax = a_pt1.x - a_pt2.x;
  deltay = a_pt1.y - a_pt2.y;
  hypot = sqrt(sqr(deltax) + sqr(deltay));
  arad = deltax / hypot;
  if (arad > .9999)
    arad = 1.0;
  else if (arad < -.9999)
    arad = -1.0;
  if (deltay >= 0.0)
    theangle = acos(arad);
  else
    theangle = 2 * XM_PI - acos(arad);
  return (theangle - (XM_PI / 2));
} // gmPerpendicularAngle
//------------------------------------------------------------------------------
/// \brief Returns the angle (0-2pi) which bisects the edges p2-p1 and p2-p3
/// based on a ccw rotation from edge 1 to edge 2.
/// \param[in] a_p1 The first point defining the edge (a_p1:a_p2)
/// \param[in] a_p2 The second point defining two edges (a_p1:a_p2, a_p2:a_p3)
/// \param[in] a_p3 The third point defining the edge (a_p2:a_p3)
/// \return angle that bisects the edges
//------------------------------------------------------------------------------
double gmBisectingAngle(const Pt3d& a_p1, const Pt3d& a_p2, const Pt3d& a_p3)
{
  double dxn, dyn, dxp, dyp, magn, magp, angletoedge1, theanglebetween;
  double cosign;

  dxp = a_p1.x - a_p2.x;
  dyp = a_p1.y - a_p2.y;
  dxn = a_p3.x - a_p2.x;
  dyn = a_p3.y - a_p2.y;
  angletoedge1 = atan2(dyp, dxp);
  magn = sqrt(sqr(dxn) + sqr(dyn));
  magp = sqrt(sqr(dxp) + sqr(dyp));
  cosign = (dxn * dxp + dyn * dyp) / (magn * magp);
  if (cosign > .99999)
    cosign = 1.0;
  if (cosign < -.99999)
    cosign = -1.0;
  theanglebetween = acos(cosign);
  if (theanglebetween == 0.0)
  {
    if ((dxn * dxp) + (dyn * dyp) < 0.0)
      theanglebetween = XM_PI;
  }
  else if (gmCross2D(dxp, dyp, dxn, dyn) < 0.0)
    theanglebetween = 2 * XM_PI - theanglebetween;
  return angletoedge1 + theanglebetween / 2.0;
} // gmBisectingAngle
//------------------------------------------------------------------------------
/// \brief converts the magnitude and angle to xy components or vice versa
/// \param[in,out] a_x vector x component either specified or calculated from
/// a_mag, a_dir
/// \param[in,out] a_y vector y component either specified or calculated from
/// a_mag, a_dir
/// \param[in,out] a_mag vector magnitude either specified or calculated from
/// a_x, a_y
/// \param[in,out] a_dir vector direction (degrees) either specified or
/// calculated from a_x, a_y
/// \param[in] a_tomagdir flag, when true a_x, a_y are used to calculate a_mag,
/// a_dir; when false a_mag, a_dir are used to calculate a_x, a_y
//------------------------------------------------------------------------------
void gmComponentMagnitudes(double* a_x, double* a_y, double* a_mag, double* a_dir, bool a_tomagdir)
{
  double rads;

  if (a_tomagdir)
  { // convert (x,y) to (mag,dir)
    if (fabs(*a_x) < XM_ZERO_TOL && fabs(*a_y) < XM_ZERO_TOL)
    {
      *a_mag = 0.0;
      *a_dir = 0.0;
    }
    else
    {
      if (*a_x == 0)
        *a_x = XM_ZERO_TOL;
      *a_mag = sqrt(sqr(*a_x) + sqr(*a_y));
      *a_dir = (atan(*a_y / *a_x)) * (180 / XM_PI);
      if (*a_x < 0)
        (*a_dir) += 180;
      if (*a_dir < 0)
        (*a_dir) += 360;
    }
  }
  else
  { // convert (mag,dir) to (x,y)
    rads = *a_dir * (XM_PI / 180);
    *a_x = cos(rads) * *a_mag;
    *a_y = sin(rads) * *a_mag;
    if (fabs(*a_x) < XM_ZERO_TOL)
      *a_x = 0;
    if (fabs(*a_y) < XM_ZERO_TOL)
      *a_y = 0;
  }
} // gmComponentMagnitudes
//------------------------------------------------------------------------------
/// \brief creates a vector from a_p1 to a_p2
/// \param[in] a_p1 The first point
/// \param[in] a_p2 The second point
/// \return a vector with x,y,z components representing the direction from a_p1
/// to a_p2
//------------------------------------------------------------------------------
Pt3d gmCreateVector(const Pt3d& a_p1, const Pt3d& a_p2)
{
  Pt3d vector;
  vector.x = a_p2.x - a_p1.x;
  vector.y = a_p2.y - a_p1.y;
  vector.z = a_p2.z - a_p1.z;
  return vector;
} // gmCreateVector
//------------------------------------------------------------------------------
/// \brief given an angle, this function will return the corresponding angle
/// that matches it in the range of 0 deg to 360 deg
/// \param[in] a_angle The angle to convert to 0 to 360
/// \param[in] a_InDegrees Flag to tell if the angle is in degrees vs radians
/// \return an angle between 0 and 360
//------------------------------------------------------------------------------
double gmConvertAngleToBetween0And360(double a_angle, bool a_InDegrees /*= true*/
)
{
  double ang = a_angle;

  if (!a_InDegrees)
  {
    ang *= (180.0 / XM_PI);
  }

#if BOOST_OS_WINDOWS
  while (LT_TOL(ang, 0.0, DBL_EPSILON) && _finite(ang))
  {
#else
  while (LT_TOL(ang, 0.0, DBL_EPSILON) && std::isfinite(ang))
  {
#endif
    ang += 360.0;
  }
#if BOOST_OS_WINDOWS
  while (GTEQ_TOL(ang, 360.0, DBL_EPSILON) && _finite(ang))
  {
#else
  while (GTEQ_TOL(ang, 360.0, DBL_EPSILON) && std::isfinite(ang))
  {
#endif
    ang -= 360.0;
  }

  return ang;
} // gmConvertAngleToBetween0And360
//------------------------------------------------------------------------------
/// \brief Perform a cross product of Pt3d's
/// \param[in] a_vec1 First vector to cross
/// \param[in] a_vec2 Second vector to cross
/// \param[out] a_vec3 Resulting cross product
//------------------------------------------------------------------------------
void gmCross3D(const Pt3d& a_vec1, const Pt3d& a_vec2, Pt3d* a_vec3)
{
  a_vec3->x = a_vec1.y * a_vec2.z - a_vec1.z * a_vec2.y;
  a_vec3->y = a_vec1.z * a_vec2.x - a_vec1.x * a_vec2.z;
  a_vec3->z = a_vec1.x * a_vec2.y - a_vec1.y * a_vec2.x;
} // gmCross3D
//------------------------------------------------------------------------------
/// \brief Perform a dot product of Pt3d's
/// \param[in] a_vec1 First vector to dot
/// \param[in] a_vec2 Second vector to dot
/// \return The dot product has the geometric interpretation as the length
/// of the projection of a_vec1 onto the unit vector a_vec2 when the
/// two vectors are placed so that their tails coincide.
//------------------------------------------------------------------------------
inline double gmDot3D(const Pt3d& a_vec1, const Pt3d& a_vec2)
{
  return (a_vec1.x * a_vec2.x + a_vec1.y * a_vec2.y + a_vec1.z * a_vec2.z);
} // gmDot3D
//------------------------------------------------------------------------------
/// \brief Determine if the line (described by a_pt1 and a_pt2) intersects the
/// triangle (a_t0, a_t1, a_t2).
/// \param[in] a_pt1 First point defining a segment
/// \param[in] a_pt2 Second point defining a segment
/// \param[in] a_t0 First point defining a triangle
/// \param[in] a_t1 Second point defining a triangle
/// \param[in] a_t2 Third point defining a triangle
/// \param[out] a_IntersectPt The point of intersection if the segment and
/// triangle intersect
/// \return -1 if triangle is degenerate (a point or a line)
///          0 if line does not intersect triangle
///          1 if line does intersect triangle
///          2 if line and triangle are in the same plane
//
// Copyright 2001, softSurfer (www.softsurfer.com)
//   This code may be freely used and modified for any purpose providing that
//   this copyright notice is included with it.  SoftSurfer makes no warranty
//   for this code, and cannot be held liable for any real or imagined damage
//   resulting from its use.  Users of this code must verify correctness for
//   their application.
// http://geometryalgorithms.com/Archive/algorithm_0105/algorithm_0105.htm
//------------------------------------------------------------------------------
int gmIntersectTriangleAndLineSegment(const Pt3d& a_pt1,
                                      const Pt3d& a_pt2,
                                      const Pt3d& a_t0,
                                      const Pt3d& a_t1,
                                      const Pt3d& a_t2,
                                      Pt3d& a_IntersectPt)
{
  double a, b, r;
  Pt3d dir, n, NullVector(0.0, 0.0, 0.0), u, v, w, w0;

  // get the triangle edge vectors and plane normal
  u = a_t1 - a_t0;
  v = a_t2 - a_t0;
  gmCross3D(u, v, &n);

  // check if the triangle is degenerate
  if (n == NullVector)
  {
    return -1;
  }

  // get the ray direction vector
  dir = a_pt2 - a_pt1;
  w0 = a_pt1 - a_t0;
  a = -gmDot3D(n, w0);
  b = gmDot3D(n, dir);

  // see if ray is parallel to the triangle
  if (fabs(b) < XM_ZERO_TOL)
  {
    // see if ray lies in triangle plane
    if (a == 0)
    {
      return 2;
    }
    // else ray is disjoint from the triangle plane
    else
    {
      return 0;
    }
  }

  // get the intersection point or ray with triangle plane
  r = a / b;

  // see if there is an intersection
  // if (r < 0.0 || r > 1.0) {
  if (r < -FLT_EPSILON || r > 1.0 + FLT_EPSILON)
  {
    return 0;
  }

  // intersect point of ray and plane
  a_IntersectPt = a_pt1 + dir * r;

  // see if the intersection is inside of the triangle
  double D, uu, uv, vv, wu, wv;

  uu = gmDot3D(u, u);
  uv = gmDot3D(u, v);
  vv = gmDot3D(v, v);
  w = a_IntersectPt - a_t0;
  wu = gmDot3D(w, u);
  wv = gmDot3D(w, v);
  D = uv * uv - uu * vv;

  // get the test parametric coords
  double s, t;

  s = (uv * wv - vv * wu) / D;
  if (s < 0.0 || s > 1.0)
  {
    // the intersect point is outside the triangle
    return 0;
  }
  t = (uv * wu - uu * wv) / D;
  if (t < 0.0 || (s + t) > 1.0)
  {
    // the intersect point is outside the triangle
    return 0;
  }

  // the intersect point is inside the triangle
  return 1;
} // gmIntersectTriangleAndLineSegment
/***********************************************************************
* FUNCTION  gm2DDistanceToLineWithTol
* PURPOSE   return the xy distance from (x,y) to line through (pt1, pt2)
* NOTES     this doesn't return fabs of the dist!!  Differs from
gm2DDistanceToLineSegmentWithTol in that the line is not
treated like a line segment and the point on the line closest
to xy may be outside the 2 points defining the line.
***********************************************************************/
//------------------------------------------------------------------------------
/// \brief return the xy distance from (a_x,a_y) to line through (a_pt1, a_pt2)
/// \param[in] a_pt1 first point of line segment
/// \param[in] a_pt2 second point of line segment
/// \param[in] a_x x location of point used to compute distance to line
/// \param[in] a_y y location of point used to compute distance to line
/// \param[in] a_tol tolerance used in geometric computations
/// \return distance from a_x,a_y to the line (a_pt1, a_pt2)
//------------------------------------------------------------------------------
double gm2DDistanceToLineWithTol(const Pt3d* a_pt1,
                                 const Pt3d* a_pt2,
                                 double a_x,
                                 double a_y,
                                 double a_tol)
{
  double a1, b1, c, mag;
  double dist;
  // see if the (x,y) is on infinite line
  a1 = a_pt2->x - a_pt1->x;
  b1 = a_pt2->y - a_pt1->y;
  mag = sqrt(a1 * a1 + b1 * b1);
  // handle case of line segment with length < tol distance to either point (pt1)
  if (mag <= a_tol)
  {
    return sqrt(sqr(a_pt1->x - a_x) + sqr(a_pt1->y - a_y));
  }

  // compute line equation
  double a, b;
  a = -b1 / mag;
  b = a1 / mag;
  c = -a * a_pt1->x - b * a_pt1->y;
  // compute distance from the line to (x,y)
  dist = a * a_x + b * a_y + c;

  return dist;
} // gm2DDistanceToLineWithTol

} // namespace xms

//----- Tests ------------------------------------------------------------------

#ifdef CXX_TEST

//#include <RunTest.h>

#include <boost/timer/timer.hpp>

#include <xmscore/testing/TestTools.h>
#include <xmsinterp/geometry/GmPolygon.t.h>
#include <xmsinterp/geometry/geoms.t.h>
#include <xmscore/misc/XmLog.h>

//----- Namespace declaration --------------------------------------------------

// namespace xms {

////////////////////////////////////////////////////////////////////////////////
/// \class GmPointInPolyUnitTests
/// \brief Used for speed tests of various point in poly functions / classes.
///
/// Uses non-virtual interface pattern. You must override TestPoint and MaxTime.
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/// \brief constructor
//------------------------------------------------------------------------------
GmPointInPolyUnitTests::GmPointInPolyUnitTests()
: m_outPoly()
, m_inPoly()
, m_timer()
, m_count(0)
, m_status(XM_NODATA)
{
} // GmPointInPolyUnitTests::GmPointInPolyUnitTests
//------------------------------------------------------------------------------
/// \brief Run the test. This is the main function to call.
//------------------------------------------------------------------------------
void GmPointInPolyUnitTests::DoTest()
{
  Setup();
  CheckPoints();
  GetResults();
} // GmPointInPolyUnitTests::DoTest
//------------------------------------------------------------------------------
/// \brief Setup the polygons
//------------------------------------------------------------------------------
void GmPointInPolyUnitTests::Setup()
{
  GmPolygonUnitTests::SetUpPolyWithHole(m_outPoly, m_inPoly);
} // GmPointInPolyUnitTests::Setup
//------------------------------------------------------------------------------
/// \brief Check a lot of points to see if they are in the polys.
//------------------------------------------------------------------------------
void GmPointInPolyUnitTests::CheckPoints()
{
  xms::Pt3d pt;
  const double xStart = -5;
  const double xEnd = 55;
  const double yStart = 35;
  const double yEnd = -5;
  const double kIncrement = 1e-1;
  m_timer.start();
  for (double y = yStart; y >= yEnd; y -= kIncrement)
  {
    for (double x = xStart; x <= xEnd; x += kIncrement)
    {
      pt.Set(x, y, 0);
      CheckPoint(pt);
      ++m_count;
    }
  }
} // GmPointInPolyUnitTests::CheckPoints
//--------------------------------------------------------------------------
/// \brief Get the timer results and do some assertions.
//--------------------------------------------------------------------------
void GmPointInPolyUnitTests::GetResults()
{
  // Using elapsed user + system time, not wall clock time, so this should be
  // similar on other computers. "For a production process, the wall clock
  // time may be what is most important. To study the efficiency of code,
  // total CPU time (user + system) is often a much better measure."
  // http://www.boost.org/doc/libs/1_59_0/libs/timer/doc/cpu_timers.html

  // Get elapsed user + system time, not wall clock time
  boost::timer::cpu_times const elapsed_times(m_timer.elapsed());
  boost::timer::nanosecond_type time = elapsed_times.system + elapsed_times.user;

  // convert nanoseconds to seconds
  const double NANO_PER_SEC = 1e9;
  double seconds = time / NANO_PER_SEC;

  // Make sure we checked a bunch of points
  int const kCountTotal = 240000;
  TS_ASSERT_EQUALS(m_count, kCountTotal);

  // For speed tests we don't check correctness of every point. We just
  // make sure m_status has changed.
  TS_ASSERT(m_status != XM_NODATA);

  // Check that the time is below a max time and write the time to the log
  // so we can check this in release.
  TS_ASSERT(seconds < MaxTime());
  XM_LOG(xmlog::debug, std::to_string((long long)seconds));
} // GmPointInPolyUnitTests::GetResults

////////////////////////////////////////////////////////////////////////////////
/// For testing point in polygon speed
class GmPointInPolyTester_gmPointInPolygon2D : public GmPointInPolyUnitTests
{
public:
  //----------------------------------------------------------------------------
  /// \brief Constructor
  //----------------------------------------------------------------------------
  GmPointInPolyTester_gmPointInPolygon2D()
  : GmPointInPolyUnitTests()
  {
  }

private:
  //----------------------------------------------------------------------------
  /// \brief Determine if a point is in or out.
  /// \param a_pt: The point to check.
  //----------------------------------------------------------------------------
  virtual void CheckPoint(const xms::Pt3d& a_pt) override
  {
    /// \brief   Determines whether (a_x, a_y) is inside=1, on=0, or
    ///          outside=-1 the polygon defined by the given vertices.
    int outer = xms::gmPointInPolygon2D(&m_outPoly[0], m_outPoly.size(), a_pt);
    int inner = xms::gmPointInPolygon2D(&m_inPoly[0], m_inPoly.size(), a_pt);
    if (outer == 1 && inner == -1)
    {
      m_status = 1; // in
    }
    else if (outer == -1 || inner == 1)
    {
      m_status = 0; // completely out or in the hole
    }
    else if (outer == 0 || inner == 0)
    {
      m_status = 2; // on the outer poly or the inner poly
    }
  } // CheckPoint
  //----------------------------------------------------------------------------
  /// \brief Maximum time test should take.
  /// \return The maximum time.
  //----------------------------------------------------------------------------
  virtual double MaxTime() override
  {
    // It takes .09 - .17 seconds on my machine in release.
    return 0.5;
    // Hopefully big enough for the tests to pass on all machines but give us
    // an error if it starts going a lot slower for some reason.
  } // MaxTime

private:
  /// prevent copy/assign compiler generated functions
  XM_DISALLOW_COPY_AND_ASSIGN(GmPointInPolyTester_gmPointInPolygon2D);
}; // GmPointInPolyTester_gmPointInPolygon2D

////////////////////////////////////////////////////////////////////////////////
/// \class GeomsXmsngUnitTests
/// \brief Tests for functions in geoms.cpp
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/// \brief
/// \return CxxTest::TestGroup reference.
//------------------------------------------------------------------------------
// virtual
// const CxxTest::TestGroup& GeomsXmsngUnitTests::group ()
//{
//  return *CxxTest::TestGroup::GetGroup(CxxTest::TG_INTERMEDIATE);
//} // GeomsXmsngUnitTests::group
//------------------------------------------------------------------------------
/// \brief
/// \verbatim
// 10 *------*
//    |      |
//    |      *
//    |      |
//  0 *------*
//    0     10
/// \endverbatim
//------------------------------------------------------------------------------
void GeomsXmsngUnitTests::test_gmComputeCentroid()
{
  using xms::Pt3d;

  // 10 *------*
  //    |      |
  //    |      *
  //    |      |
  //  0 *------*
  //    0     10

  std::vector<Pt3d> points{{0, 0, 0}, {10, 0, 0}, {10, 5, 0}, {10, 10, 0}, {0, 10, 0}};
  const double kDelta = 1e-5;
  TS_ASSERT_DELTA_PT3D(xms::gmComputeCentroid(points), Pt3d(6, 5, 0), kDelta);
  // Notice the 6 above means it's not the centroid, but the average
} // GeomsXmsngUnitTests::test_gmComputeCentroid
//------------------------------------------------------------------------------
/// \brief
/// \verbatim
// 10 *------*
//    |      |
//    |      *
//    |      |
//  0 *------*
//    0     10
/// \endverbatim
//------------------------------------------------------------------------------
void GeomsXmsngUnitTests::test_gmComputePolygonCentroid()
{
  using xms::Pt3d;

  std::vector<Pt3d> points{{0, 0, 0}, {10, 0, 0}, {10, 5, 0}, {10, 10, 0}, {0, 10, 0}};
  const double kDelta = 1e-5;
  TS_ASSERT_DELTA_PT3D(xms::gmComputePolygonCentroid(points), Pt3d(5, 5, 0), kDelta);
} // GeomsXmsngUnitTests::test_gmComputePolygonCentroid
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
void GeomsXmsngUnitTests::test_gmPointInPolygon2D_Speed()
{
#ifndef _DEBUG
  GmPointInPolyTester_gmPointInPolygon2D tester;
  tester.DoTest();
#endif
} // GeomsXmsngUnitTests::test_gmPointInPolygon2D_Speed

////////////////////////////////////////////////////////////////////////////////
/// \class GeomsXmsngIntermediateTests
/// \brief Intermediate tests for functions in geoms.cpp
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/// \brief    Defines the test group.
/// \return CxxTest::TestGroup reference.
//------------------------------------------------------------------------------
#ifndef CXXTEST4
const CxxTest::TestGroup& GeomsXmsngIntermediateTests::group()
{
  return *CxxTest::TestGroup::GetGroup(CxxTest::TG_INTERMEDIATE);
  // return CxxTest::TestSuite::group();
}
#endif
//------------------------------------------------------------------------------
/// \brief
/// \verbatim
// 15 -    0   1   2   3   4
//    |
// 10 -    5   6---7---8   9
//    |        |       |
//  5 -   10  11  12  13  14
//    |        |       |
//  0 -   15  16--17--18  19
//    |
// -15-   20  21  22  23  24
//
//         |---|---|---|---|
//        -5   0   5   10  15
/// \endverbatim
//------------------------------------------------------------------------------
void GeomsXmsngIntermediateTests::test_gmPointInPolygon2D()
{
  using xms::Pt3d;
  using xms::gmPointInPolygon2D;

  // CCW, first point not repeated. We also test it CW below.
  xms::VecPt3d poly{{0, 0, 0}, {5, 0, 0}, {10, 0, 0}, {10, 5, 0}, {10, 10, 0}, {0, 10, 0}};
  for (size_t i = 0; i < 2; ++i)
  {
    if (i == 1)
    {
      // Make it CW and recheck
      std::reverse(poly.begin(), poly.end());
    }
    TS_ASSERT_EQUALS(gmPointInPolygon2D(&poly[0], poly.size(), Pt3d(-5, 15, 0)), -1); // 0
    TS_ASSERT_EQUALS(gmPointInPolygon2D(&poly[0], poly.size(), Pt3d(0, 15, 0)), -1);  // 1
    TS_ASSERT_EQUALS(gmPointInPolygon2D(&poly[0], poly.size(), Pt3d(5, 15, 0)), -1);  // 2
    TS_ASSERT_EQUALS(gmPointInPolygon2D(&poly[0], poly.size(), Pt3d(10, 15, 0)), -1); // 3
    TS_ASSERT_EQUALS(gmPointInPolygon2D(&poly[0], poly.size(), Pt3d(15, 15, 0)), -1); // 4
    TS_ASSERT_EQUALS(gmPointInPolygon2D(&poly[0], poly.size(), Pt3d(-5, 10, 0)), -1); // 5
    TS_ASSERT_EQUALS(gmPointInPolygon2D(&poly[0], poly.size(), Pt3d(0, 10, 0)), 0);   // 6
    TS_ASSERT_EQUALS(gmPointInPolygon2D(&poly[0], poly.size(), Pt3d(5, 10, 0)), 0);   // 7
    TS_ASSERT_EQUALS(gmPointInPolygon2D(&poly[0], poly.size(), Pt3d(10, 10, 0)), 0);  // 8
    TS_ASSERT_EQUALS(gmPointInPolygon2D(&poly[0], poly.size(), Pt3d(15, 10, 0)), -1); // 9
    TS_ASSERT_EQUALS(gmPointInPolygon2D(&poly[0], poly.size(), Pt3d(-5, 5, 0)), -1);  // 10
    TS_ASSERT_EQUALS(gmPointInPolygon2D(&poly[0], poly.size(), Pt3d(0, 5, 0)), 0);    // 11
    TS_ASSERT_EQUALS(gmPointInPolygon2D(&poly[0], poly.size(), Pt3d(5, 5, 0)), 1);    // 12
    TS_ASSERT_EQUALS(gmPointInPolygon2D(&poly[0], poly.size(), Pt3d(10, 5, 0)), 0);   // 13
    TS_ASSERT_EQUALS(gmPointInPolygon2D(&poly[0], poly.size(), Pt3d(15, 5, 0)), -1);  // 14
    TS_ASSERT_EQUALS(gmPointInPolygon2D(&poly[0], poly.size(), Pt3d(-5, 0, 0)), -1);  // 15
    TS_ASSERT_EQUALS(gmPointInPolygon2D(&poly[0], poly.size(), Pt3d(0, 0, 0)), 0);    // 16
    TS_ASSERT_EQUALS(gmPointInPolygon2D(&poly[0], poly.size(), Pt3d(5, 0, 0)), 0);    // 17
    TS_ASSERT_EQUALS(gmPointInPolygon2D(&poly[0], poly.size(), Pt3d(10, 0, 0)), 0);   // 18
    TS_ASSERT_EQUALS(gmPointInPolygon2D(&poly[0], poly.size(), Pt3d(15, 0, 0)), -1);  // 19
    TS_ASSERT_EQUALS(gmPointInPolygon2D(&poly[0], poly.size(), Pt3d(-5, -5, 0)), -1); // 20
    TS_ASSERT_EQUALS(gmPointInPolygon2D(&poly[0], poly.size(), Pt3d(0, -5, 0)), -1);  // 21
    TS_ASSERT_EQUALS(gmPointInPolygon2D(&poly[0], poly.size(), Pt3d(5, -5, 0)), -1);  // 22
    TS_ASSERT_EQUALS(gmPointInPolygon2D(&poly[0], poly.size(), Pt3d(10, -5, 0)), -1); // 23
    TS_ASSERT_EQUALS(gmPointInPolygon2D(&poly[0], poly.size(), Pt3d(15, -5, 0)), -1); // 24
  }
} // GeomsXmsngIntermediateTests::test_gmPointInPolygon2D

  //} // namespace xms

#endif
