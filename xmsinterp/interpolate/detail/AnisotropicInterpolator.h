#pragma once
//------------------------------------------------------------------------------
/// \file
/// \ingroup interpolate_detail
/// \copyright (C) Copyright Aquaveo 2018. Distributed under the xmsng
///  Software License, Version 1.0. (See accompanying file
///  LICENSE_1_0.txt or copy at http://www.aquaveo.com/xmsng/LICENSE_1_0.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------
#include <xmscore/misc/base_macros.h>
#include <xmscore/misc/boost_defines.h>
#include <xmscore/points/ptsfwd.h>
#include <xmscore/stl/vector.h>

//----- Forward declarations ---------------------------------------------------

//----- Namespace declaration --------------------------------------------------
namespace xms
{
//----- Constants / Enumerations -----------------------------------------------

//----- Forward declarations ---------------------------------------------------

//----- Structs / Classes ------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// \brief Hold data of transforming from xyz to station,normal space
struct SNResult
{
  /// \brief Construct a station normal result
  /// \param[in] a_index The index of the centerline segment contain the station
  /// \param[in] a_param The segment parameter of the intersection
  /// \param[in] a_cross The cross product of vector from segment start to end
  ///                  with vector from segment start to point. It will be >0
  ///                  if the point is to the right of the segment.
  /// \param[in] a_position The intersection point of a normal through the
  ///                 point with the segment
  SNResult(size_t a_index, double a_param, double a_cross, const Pt3d& a_position)
  : m_position(a_position)
  , m_param(a_param)
  , m_cross(a_cross)
  , m_index(a_index)
  {
  }

  Pt3d m_position; ///< intersection of the normal through point in the segment
  double m_param;  ///< parameter in the range [0..1] within the segment
  double m_cross;  ///< cross product (>0 if on left of segment; <0 on right)
  size_t m_index;  ///< index of the segment within the centerline polyline
};

////////////////////////////////////////////////////////////////////////////////
/// \brief 2D Line definition. If vertical m_slope is Nan.
struct LineParameters
{
  /// \brief Constructor from slope and intercept
  /// \param[in] a_slope The slope. Use kVERTICAL for vertical lines.
  /// \param[in] a_intercept The y intercept (or x intercept for vertical).
  LineParameters(double a_slope, double a_intercept);
  /// \brief Get the LineParameters for a line normal to this through a point.
  /// \param[in] a_p The point the normal is to run through.
  /// \return LineParameters for the line normal to this through a_p.
  LineParameters NormalThrough(const Pt3d& a_p) const;
  /// \brief Get the intersection point of this and another line.
  /// \param[in] a_other The other line.
  /// \param[out] a_p The intersection point.
  /// \return false if the lines are parallel so there is no intersection.
  bool Intersection(const LineParameters& a_other, Pt3d& a_p) const;

  double m_slope;     ///< Slope of the line. 0 is horizontal. Nan is vertical.
  double m_intercept; ///< x intercept if vertical; else, the y intercept.
};

/// \brief A Vector of station normal results (for a given point).
typedef std::vector<SNResult> VecSNResult;

////////////////////////////////////////////////////////////////////////////////
/// \brief Class to transform points relative to a centerline polyline into a
/// space where the centerline segments are conceptually rotated to lie on the
/// x-axis. The points used to interpolate from will be projected via a normal
/// onto each centerline segment.  The distance along that centerline where
/// that normal intersects will be the new x (or s) value. The new y (or n)
/// value will be the distance of the point above or below the centerline to
/// that intersection. The z value remains unchanged. Note that a point can
/// project onto multiple centerline segments. An option allows from using
/// only the transformation of each point with the smallest n. Points to
/// interpolate "to" will be transformed the same way.  Then an inverse distance
/// weighting (raised to a power) from the interpolation points will be used to
/// compute the interpolated z value. This is an internal implementation class
/// used by InterpAnisotropic.
class AnisotropicInterpolator
{
public:
  AnisotropicInterpolator();

  void SetPoints(const VecPt3d& a_centerlinePts,
                 const VecPt3d& a_interpolationPts,
                 bool a_pickClosest = false);
  const VecPt3d& GetInterpolationPoints() const;

  double InterpolatePoint(const Pt3d& a_target,
                          const VecPt3d& a_interpolationPoints,
                          double a_xScale,
                          double a_IDWPower = 2,
                          double a_tol = 1.0e-5);

  // Implementation details - for testing only

  VecPt3d ComputeInterpolationPoints(const VecPt3d& a_interpolationPts, bool a_pickClosest);

  double Distance(const Pt3d& a_p0, const Pt3d& a_p1) const;

  double AnisotropicDistanceSquared(const Pt3d& a_p0, const Pt3d& a_p1, double a_xScale);

  double CalculateIDW(double a_xScale,
                      const VecPt3d& a_points,
                      const Pt3d& a_target,
                      double a_power,
                      double a_tol = 1.0e-5);

  double CrossProduct(size_t a_segmentIndex, const Pt3d& a_p) const;

  LineParameters GetLineParameters(const Pt3d& a_p0, const Pt3d& a_p1) const;

  double GetParameterInSegment(size_t a_segmentIndex, const Pt3d& a_p) const;

  bool GetIntersectionOfSegmentWithPoint(size_t a_segmentIndex,
                                         const Pt3d& a_p,
                                         Pt3d& a_intersection,
                                         double& a_param) const;

  void GetNormalPointParameter(size_t a_segmentIndex,
                               const Pt3d& a_p,
                               double& a_lastParam,
                               VecSNResult& a_results) const;

  void GetAllNormalPointParameters(const Pt3d& a_p, VecSNResult& a_results);

  VecPt3d TransformPoint(const Pt3d& a_p, bool a_onlyClosest = false);

  /// \cond
private:
  XM_DISALLOW_COPY_AND_ASSIGN(AnisotropicInterpolator);
  /// \endcond

  // data members:
  VecPt3d m_centerLinePts; ///< The polyline representing the centerline.
  VecDbl m_accLengths;     ///< The accumulated distance along the centerline of each point.
  std::vector<LineParameters> m_lineParams; ///< LineParameters for each
                                            ///< centerline segment.
  VecPt3d m_SNPoints;                       ///< The interpolation points.
};

//----- Function prototypes ----------------------------------------------------

} // namespace xms
