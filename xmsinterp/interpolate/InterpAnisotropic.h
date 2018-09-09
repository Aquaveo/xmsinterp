#pragma once
//------------------------------------------------------------------------------
/// \file
/// \ingroup interpolate
/// \copyright (C) Copyright Aquaveo 2018. Distributed under the xmsng
///  Software License, Version 1.0. (See accompanying file
///  LICENSE_1_0.txt or copy at http://www.aquaveo.com/xmsng/LICENSE_1_0.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------
#include <xmscore/misc/base_macros.h>
#include <xmscore/points/ptsfwd.h>
#include <xmscore/stl/vector.h>
#include <xmscore/misc/boost_defines.h>

//----- Forward declarations ---------------------------------------------------

//----- Namespace declaration --------------------------------------------------
namespace xms
{
class Observer;

//----- Constants / Enumerations -----------------------------------------------

//----- Structs / Classes ------------------------------------------------------
/// \brief Class that performs anisotropic interpolation. Transforms points
///        so x is a distance of a projected normal onto the centerline and y
///        is the distance from (above or below) the centerline. Interpolate
///        using inverse distance weighted interpolation after scaling the
///        transformed x-values.
/// \see InterpAnisotropicImpl and AnisotropicInterpolator.
class InterpAnisotropic
{
public:
  static BSHP<InterpAnisotropic> New();
  virtual ~InterpAnisotropic();

  /// \cond

  virtual void SetPoints(const VecPt3d& a_centerlinePts,
                         const VecPt3d& a_interpolationPts,
                         bool a_pickClosest = false) = 0;
  virtual float InterpToPt(const Pt3d& a_pt) = 0;
  virtual void InterpToPts(const VecPt3d& a_pts, VecFlt& a_result) = 0;
  virtual void GetInterpolationPts(VecPt3d& a_interpPoints) = 0;
  virtual void GetTransformedPts(const VecPt3d& a_points, bool a_pickClosest, VecPt3d& a_transformed) = 0;

  // various IDW options
  virtual void SetPower(double a_power) = 0;
  virtual void SetXScale(double a_xScale) = 0;

private:
  XM_DISALLOW_COPY_AND_ASSIGN(InterpAnisotropic);
  /// \endcond

protected:
  InterpAnisotropic();
};

//----- Function prototypes ----------------------------------------------------
} // namespace xms
