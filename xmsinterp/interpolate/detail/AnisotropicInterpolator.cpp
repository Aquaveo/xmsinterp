//------------------------------------------------------------------------------
/// \file
/// \ingroup interpolate_detail
/// \copyright (C) Copyright Aquaveo 2018. Distributed under the xmsng
///  Software License, Version 1.0. (See accompanying file
///  LICENSE_1_0.txt or copy at http://www.aquaveo.com/xmsng/LICENSE_1_0.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------

// 1. Precompiled header

// 2. My header
#include <xmsinterp/interpolate/detail/AnisotropicInterpolator.h>

// 3. Standard Library Headers
#include <cmath>
#include <functional>
#include <limits>

// 4. External Library Headers

// 5. Shared Headers
#include <xmscore/misc/XmError.h>
#include <xmscore/misc/xmstype.h>
#include <xmscore/stl/vector.h>
#include <xmsinterp/geometry/geoms.h>

// 6. Non-shared Headers

//----- Forward declarations ---------------------------------------------------

//----- External globals -------------------------------------------------------

//----- Namespace declaration --------------------------------------------------
namespace xms
{
/// \brief Slope value indicating that a line is vertical.
const double kVERTICAL = std::numeric_limits<double>::quiet_NaN();

//------------------------------------------------------------------------------
/// \brief Constructor
/// \param[in] a_slope The slope of the line. Use kVERTICAL for vertical lines.
/// \param[in] a_intercept The x intercept if vertical; else, the y intercept.
//------------------------------------------------------------------------------
LineParameters::LineParameters(double a_slope, double a_intercept)
: m_slope(a_slope)
, m_intercept(a_intercept)
{
} // LineParameters::LineParameters

//------------------------------------------------------------------------------
/// \brief Return the parameters for a line normal to this one through a point.
/// \param[in] a_p The point through which the normal line will contain.
//------------------------------------------------------------------------------
LineParameters LineParameters::NormalThrough(const Pt3d& a_p) const
// Get parameters of a line perpendicular to this through point p.
{
  if (std::isnan(m_slope))
  {
    return LineParameters(0.0, a_p.y);
  }
  if (m_slope == 0.0)
  {
    return LineParameters(kVERTICAL, a_p.x);
  }
  double slope = -1.0 / m_slope;
  return LineParameters(slope, a_p.y - slope * a_p.x);
} // LineParameters::NormalThrough

//------------------------------------------------------------------------------
/// \brief Compute the intersection point of this line with another.
/// \param[in] a_other The other line's definition.
/// \param[out] a_p The intersection point (if any)
/// \return True if there is an intersection. False if the lines are parallel.
//------------------------------------------------------------------------------
bool LineParameters::Intersection(const LineParameters& a_other, Pt3d& a_p) const
{
  if (m_slope == a_other.m_slope ||
      (std::isnan(m_slope) && std::isnan(a_other.m_slope))) // Parallel Lines
  {
    return false;
  }

  double a(m_slope), b(m_intercept);
  double c(a_other.m_slope), d(a_other.m_intercept);
  if (std::isnan(m_slope))
  {
    a_p.x = b;
    a_p.y = c * b + d;
    return true;
  }
  a_p.x = std::isnan(a_other.m_slope) ? d : ((d - b) / (a - c));
  a_p.y = a * a_p.x + b;
  return true;
} // LineParameters::Intersection

//------------------------------------------------------------------------------
/// \brief Constructor
//------------------------------------------------------------------------------
AnisotropicInterpolator::AnisotropicInterpolator()
: m_centerLinePts()
, m_accLengths()
, m_lineParams()
, m_SNPoints()
{
} // AnisotropicInterpolator::AnisotropicInterpolator
//------------------------------------------------------------------------------
/// \brief Set the centerline points to parameterize to.
/// \param[in] a_centerLinePts The centerline points.
/// \param[in] a_interpolationPts The points to be interpolated from (typically
/// cross sections).
/// \param[in] a_pickClosest True to only transform interpolation points to the
/// closest x station (or segment) on the centerline.  If false, interpolation
/// points will transform to every segment of the centerline where a normal to
/// the segment through the point is between the points defining the segment.
//------------------------------------------------------------------------------
void AnisotropicInterpolator::SetPoints(const VecPt3d& a_centerLinePts,
                                        const VecPt3d& a_interpolationPts,
                                        bool a_pickClosest)
{
  double total = 0.0;

  size_t points_sz = a_centerLinePts.size();
  m_centerLinePts.reserve(points_sz);
  m_accLengths.reserve(points_sz);
  m_lineParams.reserve(points_sz - 1);
  m_centerLinePts.push_back(a_centerLinePts[0]);
  m_accLengths.push_back(0.0);
  for (size_t i = 1; i < points_sz; ++i)
  {
    const Pt3d& p0 = a_centerLinePts[i - 1];
    const Pt3d& p1 = a_centerLinePts[i];
    if (p0.x == p1.x && p0.y == p1.y)
      continue;
    total += Distance(p0, p1);
    m_accLengths.push_back(total);
    m_lineParams.push_back(GetLineParameters(p0, p1));
    m_centerLinePts.push_back(p1);
  }

  m_SNPoints = ComputeInterpolationPoints(a_interpolationPts, a_pickClosest);
} // AnisotropicInterpolator::SetPoints
//------------------------------------------------------------------------------
/// \brief Transforms a set of points from normal (x, y, z) coordinates to
/// (s, n, z) coordinates. Note: The z coordinate is left unchanged.
/// \param[in] a_points The points to transform. (Typically cross sections.)
/// \param[in] a_pickClosest True to produce only one (s, n, value) for each
/// input point.
/// \return a vector of points in (s, n, z) space. See TransformPoint for a
/// definition of (s, n, z).
//------------------------------------------------------------------------------
VecPt3d AnisotropicInterpolator::ComputeInterpolationPoints(const VecPt3d& a_points,
                                                            bool a_pickClosest)
{
  VecPt3d results;
  for (auto& p : a_points)
  {
    VecPt3d tranformedPts = TransformPoint(p, a_pickClosest);
    results.insert(results.end(), tranformedPts.begin(), tranformedPts.end());
  }
  return results;
} // AnisotropicInterpolator::ComputeInterpolationPoints
//------------------------------------------------------------------------------
/// \brief Get the transformed interpolation points in (s, n, z) space.
/// \return a vector of points in (s, n, value) space.
//------------------------------------------------------------------------------
const VecPt3d& AnisotropicInterpolator::GetInterpolationPoints() const
{
  return m_SNPoints;
} // AnisotropicInterpolator::GetSNVInterpolationPoints
//------------------------------------------------------------------------------
/// \brief Interpolate the z coordinate of the interpolation points to point
///        a_target.
/// \param[in] a_target The (x, y, z) point whose z coordinate is to be
///                     interpolated.
/// \param[in] a_interpolationPoints The points to interpolate from
///                     (presumably in [s, n, v] space).
/// \param[in] a_xScale The scale factor to apply to the x coordinates when
///                     computing anisotropic distance.
/// \param[in] a_IDWPower The power to apply to the inverse distance weighting.
/// \param[in] a_tol If the distance squared between a_target and a point in
///                  a_points is less than or equal to this tolerance, just
///                  return the z coordinate of that interpolation point.
/// \return A weighted interpolation of the z-coordinate values of a_points
///         based on the inverse of the distance from each point in a_points.
//------------------------------------------------------------------------------
double AnisotropicInterpolator::InterpolatePoint(const Pt3d& a_target,
                                                 const VecPt3d& a_interpolationPoints,
                                                 double a_xScale,
                                                 double a_IDWPower,
                                                 double a_tol)
{
  double result = XM_NODATA;
  VecPt3d snvs = TransformPoint(a_target, true);
  if (!snvs.empty())
  {
    result = CalculateIDW(a_xScale, a_interpolationPoints, snvs[0], a_IDWPower, a_tol);
  }
  return result;
} // AnisotropicInterpolator::InterpolatePoint
//------------------------------------------------------------------------------
/// \brief Compute the distance between two points.
/// \param[in] a_p0 The first point.
/// \param[in] a_p1 The second point.
/// \return The distance between the points.
//------------------------------------------------------------------------------
double AnisotropicInterpolator::Distance(const Pt3d& a_p0, const Pt3d& a_p1) const
{
  double dx(a_p0.x - a_p1.x);
  double dy(a_p0.y - a_p1.y);
  double distanceSquared(dx * dx + dy * dy);
  return sqrt(distanceSquared);
} // AnisotropicInterpolator::Distance
//------------------------------------------------------------------------------
/// \brief Compute the distance squared between two points, after a scale factor
///        is applied to their x coordinates.
/// \param[in] a_p0 The first point.
/// \param[in] a_p1 The second point.
/// \param[in] a_xScale The scale factor to apply to the x coordinate.
/// \return The anisotropic distance between the points.
//------------------------------------------------------------------------------
double AnisotropicInterpolator::AnisotropicDistanceSquared(const Pt3d& a_p0,
                                                           const Pt3d& a_p1,
                                                           double a_xScale)
{
  double dx = a_xScale * (a_p0.x - a_p1.x);
  double dy = a_p0.y - a_p1.y;
  return (dx * dx) + (dy * dy);
} // AnisotropicInterpolator::AnisotropicDistanceSquared
//------------------------------------------------------------------------------
/// \brief Calculates the Inverse Distance Weighted interpolation of a set of
///        points at a target point.
/// \param[in] a_xScale The scale factor to apply to the x coordinates when
///                      computing anisotropic distance.
/// \param[in] a_points The points to interpolate from (presumably in [s, n, z]
/// space).
/// \param[in] a_target The point in (s,n,?) whose z coordinate is to be
/// interpolated.
/// \param[in] a_power The exponent to apply to the inverse distance weighting.
/// \param[in] a_tol If the distance squared between a_target and a point in
///                   a_points is less than or equal to this tolerance, just
///                   return the z coordinate of that point.
/// \return A weighted interpolation of the z-coordinate values of a_points
///         based on the inverse of the distance from each point in a_points.
//------------------------------------------------------------------------------
double AnisotropicInterpolator::CalculateIDW(double a_xScale,
                                             const VecPt3d& a_points,
                                             const Pt3d& a_target,
                                             double a_power,
                                             double a_tol)
{
  double numerator(0.0);
  double denominator(0.0);
  double power(double(a_power) * 0.5);
  std::function<double(double)> powerFunc;
  if (a_power == 2)
    powerFunc = [](double x) -> double { return x; };
  else
    powerFunc = [=](double x) -> double { return pow(x, power); };
  for (auto& p : a_points)
  {
    double distanceSquared(AnisotropicDistanceSquared(p, a_target, a_xScale));
    double value = p.z;
    if (distanceSquared <= a_tol)
    {
      return value;
    }
    double weight(1.0 / powerFunc(distanceSquared));
    numerator += value * weight;
    denominator += weight;
  }
  return numerator / denominator;
} // AnisotropicInterpolator::CalculateIDW
//------------------------------------------------------------------------------
/// \brief Compute the 2d cross product of the vectors from a_p to the 1st and
///        last points in a segment of m_centerLinePts. The cross product will
///        be positive if a_p is to the left of the vector from first to last
///        point in the segment, negative if to the right, and 0.0 if on the
///        line segment.
/// \param[in] a_segmentIndex The index of the segment of m_centerlinePts;
/// \param[in] a_p A point in (x, y) space (typically in a cross section).
/// \return The 2d cross product.
//------------------------------------------------------------------------------
double AnisotropicInterpolator::CrossProduct(size_t a_segmentIndex, const Pt3d& a_p) const
{
  const Pt3d& pa = a_p;
  const Pt3d& pb = m_centerLinePts[a_segmentIndex];
  const Pt3d& pc = m_centerLinePts[a_segmentIndex + 1];
  double bcx = pc.x - pb.x;
  double bcy = pc.y - pb.y;
  double bax = pa.x - pb.x;
  double bay = pa.y - pb.y;
  return gmCross2D(bcx, bcy, bax, bay);
  // return (pb.x - pa.x) * (pc.y - pa.y) - (pb.y - pa.y) * (pc.x - pa.x);
} // AnisotropicInterpolator::CrossProduct
//------------------------------------------------------------------------------
/// \brief Compute the 2d slope and intercept for a line through two points.
/// \param[in] a_p0 The first point.
/// \param[in] a_p1 The second point.
/// \return The LineParameters for the 2d line between the points.
//------------------------------------------------------------------------------
LineParameters AnisotropicInterpolator::GetLineParameters(const Pt3d& a_p0, const Pt3d& a_p1) const
{
  if (a_p0.x == a_p1.x) // Vertical Line
  {
    return LineParameters(kVERTICAL, a_p0.x);
  }
  double slope = (a_p1.y - a_p0.y) / (a_p1.x - a_p0.x);
  double intercept(a_p0.y - slope * a_p0.x);
  return LineParameters(slope, intercept);
} // AnisotropicInterpolator::GetLineParameters
//------------------------------------------------------------------------------
/// \brief Compute the parameter for a point in a segment of m_centerLinePts.
/// The parameter will be between 0 and 1 if the normal to the segment through
/// the point is between the segment end points. It will be < 0 if the point
/// projects onto the segment before the first point and > 1 if it projects
/// onto the line after the second point.
/// \param[in] a_segmentIndex The index of the segment of m_centerLinePts.
/// \param[in] a_p A point whose parameter in that segment is wanted.
/// \return The parametric position of the point within the segment.
//------------------------------------------------------------------------------
double AnisotropicInterpolator::GetParameterInSegment(size_t a_segmentIndex, const Pt3d& a_p) const
{
  const Pt3d& pi = m_centerLinePts[a_segmentIndex];
  const Pt3d& pf = m_centerLinePts[a_segmentIndex + 1];
  return gmPtDistanceAlongSegment(pi, pf, a_p, 1.0e-7);
} // AnisotropicInterpolator::GetParameterInSegment
//------------------------------------------------------------------------------
/// \brief Get the intersecting point and its parameter of a line through point
///        a_p normal to a segment of m_centerLinePts.
/// \param[in] a_segmentIndex The index of a segment of m_centerLinePts.
/// \param[in] a_p A point.
/// \param[out] a_intersection The intersection of a line normal to the segment
/// through a_p.
/// \param[out] a_param The parameter of the intersection in a range of [0..1]
/// if the intersection is within the segment.
/// \return true if there is an intersection; false otherwise.
//------------------------------------------------------------------------------
bool AnisotropicInterpolator::GetIntersectionOfSegmentWithPoint(size_t a_segmentIndex,
                                                                const Pt3d& a_p,
                                                                Pt3d& a_intersection,
                                                                double& a_param) const
{
  const LineParameters& line = m_lineParams[a_segmentIndex];
  const LineParameters normal = line.NormalThrough(a_p);
  if (line.Intersection(normal, a_intersection))
  {
    a_param = GetParameterInSegment(a_segmentIndex, a_intersection);
    return true;
  }
  return false;
} // AnisotropicInterpolator::GetIntersectionOfSegmentWithPoint
//------------------------------------------------------------------------------
/// \brief Append the transform (if any) of a point to a centerline segment
/// \param[in] a_segmentIndex The segment of m_centerLinePts.
/// \param[in] a_p The (x, y, z) point to transform.
/// \param[in] a_lastParam The parameter of a_p for the previous segment
///                         (a_segmentIndex - 1).
/// \param[out] a_results: A vector a SNResult where the transform will be
///                        collected.
//------------------------------------------------------------------------------
void AnisotropicInterpolator::GetNormalPointParameter(size_t a_segmentIndex,
                                                      const Pt3d& a_p,
                                                      double& a_lastParam,
                                                      VecSNResult& a_results) const
{
  Pt3d intersection;
  double param;
  double cross = CrossProduct(a_segmentIndex, a_p);
  if (GetIntersectionOfSegmentWithPoint(a_segmentIndex, a_p, intersection, param))
  {
    if (param >= 0.0)
    {
      if (param <= 1.0)
      {
        a_results.push_back(SNResult(a_segmentIndex, param, cross, intersection));
      }
    }
    else if (a_lastParam > 1.0)
    {
      const Pt3d& pa = m_centerLinePts[a_segmentIndex];
      a_results.push_back(SNResult(a_segmentIndex, 0.0, cross, pa));
      param = 0.0;
    }
    a_lastParam = param;
  }
} // AnisotropicInterpolator::GetNormalPointParameter
//------------------------------------------------------------------------------
/// \brief Get the SNResults for a point.
/// \param[in] a_p The (x, y, z) point whose parameters are desired.
/// \param[out] a_results: The vector of SNResult.
//------------------------------------------------------------------------------
void AnisotropicInterpolator::GetAllNormalPointParameters(const Pt3d& a_p, VecSNResult& a_results)
{
  a_results.clear();
  double lastParam = -1.0;
  for (size_t i = 0; i < m_lineParams.size(); ++i)
  {
    GetNormalPointParameter(i, a_p, lastParam, a_results);
  }
} // AnisotropicInterpolator::GetAllNormalPointParameters
//------------------------------------------------------------------------------
/// \brief Transforms a point from (x, y, z) to (s, n, z) coordinates.
/// s is for station (distance along the centerline) and n for normal
/// (perpendicular distance) to a segment to m_centerLinePts. The z coordinate
/// is left unchanged.
/// \param[in] a_p  The (x, y, z) point to transform.
/// \param[in] a_onlyClosest  True to return only one (the closest) (s, n, z)
///                           point.
/// \return A vector of (s, n, z) points corresponding to a_p.  Conceptually,
/// the centerline is straightened out to lie along the x axis, with this first
/// point at x = 0. The point will be projected via a normal through it onto
/// each centerline segment. Their s-value will be the accumulated distance
/// along the centerline to that intersection. The n-value will be the distance
/// along that normal. It will be positive if the point is to the left of the
/// segment and negative if to the right. A (s, n, z) point will be generated
/// for each segment where the normal is between the segment end points. If
/// a_onlyClosest is true, only the (s, n, z) point with the smallest
/// absolute value of n will be returned.
//------------------------------------------------------------------------------
VecPt3d AnisotropicInterpolator::TransformPoint(const Pt3d& a_p, bool a_onlyClosest)
{
  double value = a_p.z;

  VecSNResult results;
  GetAllNormalPointParameters(a_p, results);
  VecPt3d snPoints;
  for (auto& result : results)
  {
    // const Pt3d& closest = result.m_position;
    double len0 = m_accLengths[result.m_index];
    double len1 = m_accLengths[result.m_index + 1];
    double s = len0 + result.m_param * (len1 - len0);
    double n = (result.m_cross == 0.0) ? 0.0 // Collinear
                                       : result.m_cross < 0.0 ? -Distance(a_p, result.m_position)
                                                              : Distance(a_p, result.m_position);
    Pt3d snv(s, n, value);
    if (a_onlyClosest)
    {
      if (snPoints.size() == 0)
      {
        snPoints.push_back(snv);
      }
      else if (abs(snv.y) < abs(snPoints[0].y))
      {
        snPoints[0] = snv;
      }
    }
    else
    {
      snPoints.push_back(snv);
    }
  }
  return snPoints;
} // AnisotropicInterpolator::TransformPoint

} // namespace xms

#ifdef CXX_TEST
////////////////////////////////////////////////////////////////////////////////

#include <xmsinterp/interpolate/detail/AnisotropicInterpolator.t.h>
#include <xmscore/testing/TestTools.h>

// namespace xms {
using namespace xms;

////////////////////////////////////////////////////////////////////////////////
/// \class AnisotropicInterpolatorUnitTests
/// \brief Unit tests for the AnisotropicInterpolator class
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/// \brief Tests simple interpolation problem.
//------------------------------------------------------------------------------
void AnisotropicInterpolatorUnitTests::testSimple()
{
  VecPt3d centerline = {{-1, 0, 0}, {0, 0, 0}, {1, 0, 0}};
  VecPt3d crossSections = {{-0.5, -1, 0}, {-0.5, 0, -1}, {-0.5, 1, 0},
                           {0.5, -1, 0},  {0.5, 0, -1},  {0.5, 1, 0}};
  Pt3d target(0, 0, 0);

  AnisotropicInterpolator interpolator;
  bool pickClosest = true;
  interpolator.SetPoints(centerline, crossSections, pickClosest);
  Pt3d sn0 = interpolator.TransformPoint(crossSections[0], pickClosest)[0];
  Pt3d sn1 = interpolator.TransformPoint(crossSections[1], pickClosest)[0];
  Pt3d sn2 = interpolator.TransformPoint(crossSections[2], pickClosest)[0];
  Pt3d sn3 = interpolator.TransformPoint(crossSections[3], pickClosest)[0];
  Pt3d sn4 = interpolator.TransformPoint(crossSections[4], pickClosest)[0];
  Pt3d sn5 = interpolator.TransformPoint(crossSections[5], pickClosest)[0];

  VecPt3d snPoints = interpolator.GetInterpolationPoints();
  double xScale = 1.0;

  VecPt3d snPoints2 = {sn1, sn4};
  double result = interpolator.InterpolatePoint(target, snPoints2, xScale);
  double expect(-1.0);
  TS_ASSERT_EQUALS(expect, result);

  VecPt3d snPoints3 = {sn0, sn2, sn3, sn5};
  result = interpolator.InterpolatePoint(target, snPoints3, xScale);
  expect = 0.0;
  TS_ASSERT_EQUALS(expect, result); // -0.6250

  result = interpolator.InterpolatePoint(target, snPoints, xScale);
  expect = -0.714;
  TS_ASSERT_DELTA(expect, result, 0.01);
} // AnisotropicInterpolatorUnitTests::testSimple
//------------------------------------------------------------------------------
/// \brief Tests interpolation of crosssection going through a centerline point.
/// \verbatim
/// +                         +
///  \                       /
///   \                     /
///    +-------------------+
///    |\                 /|
///    | \               / |
///    |  \             /  |
///    |   +           +   |
///    +                   +
/// \endverbatim
//------------------------------------------------------------------------------
void AnisotropicInterpolatorUnitTests::testCrossSectionThroughPoint()
{
  VecPt3d centerline = {{-10, 0, 0}, {-10, 10, 0}, {10, 10, 0}, {10, 0, 0}};
  VecPt3d crossSections = {{-15, 15, 0}, {-10, 10, -1}, {-5, 5, 0},
                           {5, 5, 0},    {10, 10, -1},  {15, 15, 0}};
  Pt3d target(0, 5, 0);

  AnisotropicInterpolator interpolator;
  bool pickClosest = false;
  interpolator.SetPoints(centerline, crossSections, pickClosest);

  double cross = interpolator.CrossProduct(0, crossSections[2]);
  TS_ASSERT(cross < 0.0);

  VecPt3d sn2 = interpolator.TransformPoint(crossSections[2]);
  VecPt3d sn2Expected = {{5, -5, 0}, {15, -5, 0}, {35, -15, 0}};
  TS_ASSERT_DELTA_VECPT3D(sn2Expected, sn2, 0.001);
  VecPt3d sn3 = interpolator.TransformPoint(crossSections[3]);
  VecPt3d sn3Expected = {{5, -15, 0}, {25, -5, 0}, {35, -5, 0}};
  TS_ASSERT_DELTA_VECPT3D(sn3Expected, sn3, 0.001);

  VecPt3d snPoints = interpolator.GetInterpolationPoints();
  double xScale = 1.0;
  double result = interpolator.InterpolatePoint(target, snPoints, xScale);
  double expect = -0.2699;
  TS_ASSERT_DELTA(expect, result, 0.01);

  Pt3d target2(0, 15, 0);
  double result2 = interpolator.InterpolatePoint(target, snPoints, xScale);
  TS_ASSERT_DELTA(result, result2, 1.0e-6);

  Pt3d target3 = crossSections[1];
  target3.z = 10;
  double result3 = interpolator.InterpolatePoint(target3, snPoints, xScale);
  TS_ASSERT_DELTA(crossSections[1].z, result3, 1.0e-6);
} // AnisotropicInterpolatorUnitTests::testCrossSectionThroughPoint
//------------------------------------------------------------------------------
/// \brief Test to demonstrate ambiguity.
/// \verbatim
///  +                     +
///   \       +   +       /
///    \     /     \     /
///     \   /       \   /
///      \ /    *    \ /
///       X           X
///      / \         / \
///     /   \       /   \
///    /     \     /     \
///   +       \   /       +
///            \ /
///             +
///
///             *
/// \endverbatim
//------------------------------------------------------------------------------
void AnisotropicInterpolatorUnitTests::testAmbiguity()
{
  VecPt3d centerline = {{-10, 10, 0}, {0, 0, 0}, {10, 10, 0}};
  VecPt3d crossSections = {{-8, 2, -3}, {-2, 8, 2}, {2, 8, 2}, {8, 2, -3}};

  AnisotropicInterpolator interpolator;
  bool pickClosest = true;
  interpolator.SetPoints(centerline, crossSections, pickClosest);

  VecPt3d snPoints = interpolator.GetInterpolationPoints();
  double xScale = 0.25;
  double idwPower = 3.0;
  
  // Test interpolate to point in both stations
  Pt3d target1(0, 4.242, 0);
  double result1 = interpolator.InterpolatePoint(target1, snPoints, xScale, idwPower);
  double expect1 = 2.0;
  TS_ASSERT_DELTA(expect1, result1, 0.1);

  // test interpolate to point inbetween but outside of both stations
  Pt3d target2(0, -4.242, 0);
  double result2 = interpolator.InterpolatePoint(target2, snPoints, xScale, idwPower);
  double expect2 = -3.0;
  TS_ASSERT_DELTA(expect2, result2, 0.1);
  
  VecPt3d transform1 = interpolator.TransformPoint(target1);
  VecPt3d transform1Expected = {{11.14, 3.00, 0}, {17.14, 3.00, 0}};
  TS_ASSERT_DELTA_VECPT3D(transform1Expected, transform1, 0.01);
  
  VecPt3d transform2 = interpolator.TransformPoint(target2);
  VecPt3d transform2Expected = {{14.14, -4.242, 0}};
  TS_ASSERT_DELTA_VECPT3D(transform2Expected, transform2, 0.01);
} // AnisotropicInterpolatorUnitTests::testAmbiguity()

#endif // CXX_TEST
