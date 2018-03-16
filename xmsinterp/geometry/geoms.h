#pragma once
//------------------------------------------------------------------------------
/// \file
/// \brief Functions dealing with geometry.
/// \ingroup geometry
/// \copyright (C) Copyright Aquaveo 2018. Distributed under the xmsng
///  Software License, Version 1.0. (See accompanying file
///  LICENSE_1_0.txt or copy at http://www.aquaveo.com/xmsng/LICENSE_1_0.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------

// 3. Standard Library Headers
#include <vector>

// 4. External Library Headers
#include <xmscore/points/pt.h> // Pt3d, Pt2d, etc.
#include <xmscore/stl/vector.h>

// 5. Shared Headers

// 6. Non-shared Headers

//----- Namespace declaration --------------------------------------------------

namespace xms
{
/// direction of turn between 3 points
enum Turn_enum {
  TURN_LEFT,         ///< turn left
  TURN_RIGHT,        ///< turn right
  TURN_COLINEAR_180, ///< continue onward
  TURN_COLINEAR_0    ///< turns back on same segment
};

/// point in, out, or on
enum PtInOutOrOn_enum {
  PT_ERROR = -1, ///< error
  PT_IN,         ///< in
  PT_OUT,        ///< out
  PT_ON          ///< on
};

//----- Forward declarations ---------------------------------------------------

//----- Global functions -------------------------------------------------------

bool gmPointInOrOnBox2d(const Pt3d& a_bMin, const Pt3d& a_bMax, const Pt3d& a_pt);
bool gmBoxesOverlap2d(const Pt3d& a_b1Min,
                      const Pt3d& a_b1Max,
                      const Pt3d& a_b2Min,
                      const Pt3d& a_b2Max);
void gmCalculateNormalizedPlaneCoefficients(const Pt3d& p1,
                                            const Pt3d& p2,
                                            const Pt3d& p3,
                                            double* a,
                                            double* b,
                                            double* c,
                                            double* d);
void gmCalculateNormalizedPlaneCoefficients(const Pt3d* p1,
                                            const Pt3d* p2,
                                            const Pt3d* p3,
                                            double* a,
                                            double* b,
                                            double* c,
                                            double* d);
double gmMiddleZ(const VecPt3d& a_points);
PtInOutOrOn_enum gmPtInCircumcircle(const Pt3d& pt, Pt3d circumcirclePts[3]);
double gmXyDistanceSquared(const Pt3d& pt1, const Pt3d& pt2);
bool gmCircumcircleWithTol(const Pt3d* pt1,
                           const Pt3d* pt2,
                           const Pt3d* pt3,
                           double* xc,
                           double* yc,
                           double* r2,
                           double tol);
int gmCartToBary(const Pt3d* cart, const Pt3d* orig, double coef[6], int dir, Pt3d* bary);
int gmBaryPrepare(const Pt3d* p1,
                  const Pt3d* p2,
                  const Pt3d* p3,
                  const Pt3d* norm,
                  Pt3d* orig,
                  double coef[6],
                  int* dir,
                  bool flag);
bool gmColinearWithTol(const Pt3d& p1, const Pt3d& p2, const Pt3d& p3, const double tol);
bool gmIntersectLineSegmentsWithTol(const Pt3d& one1,
                                    const Pt3d& one2,
                                    const Pt3d& two1,
                                    const Pt3d& two2,
                                    double* xi,
                                    double* yi,
                                    double* zi1,
                                    double* zi2,
                                    double tol);
bool gmCounterClockwiseTri(const Pt3d& vtx0, const Pt3d& vtx1, const Pt3d& vtx2);
double gmCross2D(const double& dx1, const double& dy1, const double& dx2, const double& dy2);
double gmAngleBetween2DVectors(double dxp, double dyp, double dxn, double dyn);
double gmAngleBetween2DVectors(double dxp,
                               double dyp,
                               double dxn,
                               double dyn,
                               double a_magn,
                               double a_magp);
double gmAngleBetweenEdges(const Pt3d&, const Pt3d&, const Pt3d&);
double gmAngleBetweenEdges(const Pt2d& p1, const Pt2d& p2, const Pt2d& p3);
double gmComputeDeviationInDirection(const Pt3d&, const Pt3d&, const Pt3d&);
bool gmOnLineAndBetweenEndpointsWithTol(const Pt3d& a_pt1,
                                        const Pt3d& a_pt2,
                                        const double a_x,
                                        const double a_y,
                                        double a_tol);
bool gmOnLineWithTol(const Pt3d& vertex1,
                     const Pt3d& vertex2,
                     const double x,
                     const double y,
                     const double tol);
void gmAddToExtents(const Pt3d& a_pt, Pt3d& a_min, Pt3d& a_max);
void gmAddToExtents(const Pt3d& a_pt, Pt2d& a_min, Pt2d& a_max);
void gmAddToExtents(const Pt2d& a_pt, Pt3d& a_min, Pt3d& a_max);
double gmXyDistance(double x1, double y1, double x2, double y2);
double gmXyDistance(const Pt3d& pt1, const Pt3d& pt2);
Turn_enum gmTurn(const Pt3d& a_v1, const Pt3d& a_v2, const Pt3d& a_v3, double a_angtol = 0.0017453);
Pt3d gmComputeCentroid(const VecPt3d& a_points);
Pt3d gmComputePolygonCentroid(const VecPt3d& pts);
bool gmLinesIntersect(const Pt3d& one1, const Pt3d& one2, const Pt3d& two1, const Pt3d& two2);
int gmPointInPolygon2D(const Pt3d*, size_t, double, double);
int gmPointInPolygon2D(const Pt3d*, size_t, double, double);
int gmPointInPolygon2D(const Pt3d*, size_t, Pt3d);
int gmPointInPolygon2D(const Pt2i*, size_t, Pt2d);
int gmPointInPolygon2D(const Pt2i*, size_t, Pt2i);
int gmPointInPolygon2D(const Pt2i*, size_t, Pt3d);
int gmPointInPolygon2D(const VecPt3d&, const Pt3d&);
template <typename T>
int gmPointInPolygon2D(const T* theverts,
                       const size_t numverts,
                       const double xpt,
                       const double ypt,
                       const double tol);
double gmComputeXyTol(const Pt3d& a_mn, const Pt3d& a_mx);
double gmXyTol(bool a_set = false, double a_value = 1e-9);
double gmZTol(bool a_set = false, double a_value = 1e-6);
bool gmEqualPointsXY(double x1, double y1, double x2, double y2);
bool gmEqualPointsXY(double x1, double y1, double x2, double y2, double tol);
bool gmEqualPointsXY(const Pt2i&, const Pt2i&);
bool gmEqualPointsXY(const Pt2d& a_pt1, const Pt2d& a_pt2, double tol);
bool gmEqualPointsXY(const Pt3d& a_pt1, const Pt3d& a_pt2, double tol);
bool gmEqualPointsXYZ(double x1, double y1, double z1, double x2, double y2, double z2);
bool gmEqualPointsXYZ(double x1, double y1, double z1, double x2, double y2, double z2, double tol);
bool gmEqualPointsXYZ(const Pt3d& pt1, const Pt3d& pt2, double tol);
bool gmPointInTriangleWithTol(const Pt3d* p1,
                              const Pt3d* p2,
                              const Pt3d* p3,
                              double x,
                              double y,
                              double tol);
bool gmInsideOrOnLineWithTol(const Pt3d* p1,
                             const Pt3d* p2,
                             const Pt3d* inpoint,
                             double x,
                             double y,
                             double tol,
                             double* dist = NULL);
double gmPolygonArea(const Pt3d* points, size_t npoints);
VecPt3d gmArrayToVecPt3d(double* a_array, int a_size);
void gmEnvelopeOfPts(const VecPt3d& a_pts, Pt3d& a_min, Pt3d& a_max);
void gmOrderPointsCounterclockwise(const VecPt3d& pts, VecInt& ccwOrder, int startindex = 0);
void gmOrderPointsCounterclockwise(VecPt3d& pts);
double gmFindClosestPtOnSegment(const Pt3d& pt1,
                                const Pt3d& pt2,
                                const Pt3d& pt,
                                Pt3d& newpt,
                                const double tol);
double gmPtDistanceAlongSegment(const Pt3d& pt1, const Pt3d& pt2, const Pt3d& pt, const double tol);
bool gmInsideOfLineWithTol(const Pt3d& a_vertex1,
                           const Pt3d& a_vertex2,
                           const Pt3d& a_oppositevertex,
                           const double a_x,
                           const double a_y,
                           const double a_tol);
void gmExtents2D(const VecPt3d& a_pts, Pt2d& a_min, Pt2d& a_max);
void gmExtents2D(const VecPt3d& a_points, Pt3d& a_min, Pt3d& a_max);
void gmExtents3D(const VecPt3d& a_points, Pt3d& a_min, Pt3d& a_max);
double gmPerpendicularAngle(const Pt3d& point1, const Pt3d& point2);
double gmBisectingAngle(const Pt3d& p1, const Pt3d& p2, const Pt3d& p3);
void gmComponentMagnitudes(double* x, double* y, double* mag, double* dir, bool tomagdir);
Pt3d gmCreateVector(const Pt3d& p1, const Pt3d& p2);
double gmConvertAngleToBetween0And360(double a_angle, bool a_InDegrees = true);
void gmCross3D(const Pt3d& a_vec1, const Pt3d& a_vec2, Pt3d* a_vec3);
inline double gmDot3D(const Pt3d& a_vec1, const Pt3d& a_vec2);
int gmIntersectTriangleAndLineSegment(const Pt3d& a_pt1,
                                      const Pt3d& a_pt2,
                                      const Pt3d& a_t0,
                                      const Pt3d& a_t1,
                                      const Pt3d& a_t2,
                                      Pt3d& a_IntersectPt);
double gm2DDistanceToLineWithTol(const Pt3d* pt1, const Pt3d* pt2, double x, double y, double tol);

} // namespace xms
