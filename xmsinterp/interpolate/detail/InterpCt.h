#pragma once
//------------------------------------------------------------------------------
/// \file
/// \ingroup interpolate_detail
/// \copyright (C) Copyright Aquaveo 2018. Distributed under the xmsng
///  Software License, Version 1.0. (See accompanying file
///  LICENSE_1_0.txt or copy at http://www.aquaveo.com/xmsng/LICENSE_1_0.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------
#include <vector>
#include <xmscore/points/ptsfwd.h>
#include <xmscore/misc/base_macros.h>
#include <xmscore/misc/boost_defines.h>

//----- Forward declarations ---------------------------------------------------

//----- Namespace declaration --------------------------------------------------
namespace xms
{
class NodalFunc;

//----- Constants / Enumerations -----------------------------------------------

//----- Structs / Classes ------------------------------------------------------
class InterpCt
{
public:
  /// Triangle region enumeration
  enum RegionEnum { T1, T2, T3 };

  InterpCt(const std::vector<Pt3d>& a_pts, BSHP<NodalFunc> a_n);

  bool ComputeCtCoeff(int* a_ptIdxs);
  double InterpToPt(const Pt3d& a_pt);
  RegionEnum DetermineRegion(double e, double n);
  void RecalcNodalFunc();
  std::string ToString() const;

  /// input points interpolated from
  const std::vector<Pt3d>& m_pts;
  /// nodal function used in ct calculations
  BSHP<NodalFunc> m_nodalFunc;
  /// \cond
  // variables used in ct interpolation refactored out of GMS into this class
  double x1, p1, f1, x2, y2, f2, x3, y3, f3, d11, d12, d21, d22, fx1, fy1, fx2, fy2, fx3, fy3;
  double ue1, un1, ue2, un2, ue3, un3, ue4, un4, ue5, un5, ue6, un6;
  double dudz1, dudz2, dudz3, dudw1, dudw2, dudw3, dudn4, dudn5, dudn6;
  double delta, a11, a12, a21, a22, d1, d2;

private:
  XM_DISALLOW_COPY_AND_ASSIGN(InterpCt);
  /// \endcond
};
//----- Function prototypes ----------------------------------------------------
}