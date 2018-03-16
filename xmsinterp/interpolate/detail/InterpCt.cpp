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
#include <xmsinterp/interpolate/detail/InterpCt.h>

// 3. Standard Library Headers
#include <sstream>

// 4. External Library Headers

// 5. Shared Headers
#include <xmscore/misc/boost_defines.h>
#include <xmscore/points/pt.h>
#include <xmscore/stl/utility.h>
#include <xmsinterp/interpolate/detail/NodalFunc.h>

// 6. Non-shared Headers

//----- Forward declarations ---------------------------------------------------

//----- External globals -------------------------------------------------------

//----- Namespace declaration --------------------------------------------------
namespace xms
{
/// \brief square root of 3 precalculated
#define SQRT3 1.73205080756888772935274463415059

//----- Constants / Enumerations -----------------------------------------------

//----- Classes / Structs ------------------------------------------------------

//----- Internal functions -----------------------------------------------------

//----- Class / Function definitions -------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// \class InterpCt
/// \brief Computes the Clough Tocher interpolation to a location.
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/// \brief Constructor
/// \param[in] a_pts The point locations that are interpolated from
/// \param[in] a_n Nodal function used in the setup of ct interpolation
//------------------------------------------------------------------------------
InterpCt::InterpCt(const std::vector<Pt3d>& a_pts, BSHP<NodalFunc> a_n)
: m_pts(a_pts)
, m_nodalFunc(a_n)
{
} // InterpCt::InterpCt
//------------------------------------------------------------------------------
/// \brief Finds the region of the triangle
/// \param[in] e factor determined by the x,y location of interpolation point
/// \param[in] n factor determined by the x,y location of interpolation point
/// \return The triangle region where the point is located
//------------------------------------------------------------------------------
InterpCt::RegionEnum InterpCt::DetermineRegion(double e, double n)
{
  if (e > 0)
  {
    if (n > (-SQRT3 / 3 * e))
      return T2;
    else
      return T1;
  }
  else if (n > (SQRT3 / 3 * e))
    return T3;
  return T1;
} // InterpCt::DetermineRegion
//------------------------------------------------------------------------------
/// \brief Recalculated the nodal function. This happens when the scalars change
//------------------------------------------------------------------------------
void InterpCt::RecalcNodalFunc()
{
  if (m_nodalFunc)
    m_nodalFunc->ComputeNodalFuncs();
} // InterpCt::RecalcNodalFunc
//------------------------------------------------------------------------------
/// \brief serialize the class to a string.
/// \return The string.
//------------------------------------------------------------------------------
std::string InterpCt::ToString() const
{
  std::stringstream ss;
  VecToStream(ss, m_pts, "pts");
  ss << x1 << "=x1 " << p1 << "=p1 " << f1 << "=f1 " << x2 << "=x2 " << y2 << "=y2 " << f2 << "=f2 "
     << x3 << "=x3 " << y3 << "=y3 " << f3 << "=f3 " << d11 << "=d11 " << d12 << "=d12 " << d21
     << "=d21 " << d22 << "=d22 " << fx1 << "=fx1 " << fy1 << "=fy1 " << fx2 << "=fx2 " << fy2
     << "=fy2 " << fx3 << "=fx3 " << fy3 << "=fy3 "

     << ue1 << "=ue1 " << un1 << "=un1 " << ue2 << "=ue2 " << un2 << "=un2 " << ue3 << "=ue3 "
     << un3 << "=un3 " << ue4 << "=ue4 " << un4 << "=un4 " << ue5 << "=ue5 " << un5 << "=un5 "
     << ue6 << "=ue6 " << un6 << "=un6 "

     << dudz1 << "=dudz1 " << dudz2 << "=dudz2 " << dudz3 << "=dudz3 " << dudw1 << "=dudw1 "
     << dudw2 << "=dudw2 " << dudw3 << "=dudw3 " << dudn4 << "=dudn4 " << dudn5 << "=dudn5 "
     << dudn6 << "=dudn6 "

     << delta << "=delta " << a11 << "=a11 " << a12 << "=a12 " << a21 << "=a21 " << a22 << "=a22 "
     << d1 << "=d1 " << d2 << "=d2 "
     << "\n";
  return ss.str();
} // InterpCt::ToString
//------------------------------------------------------------------------------
/// \brief Performs Clough Tocher interpolation to a point
/// \param[in] a_pt The location being interpolated to.
/// \return The interpolated value.
//------------------------------------------------------------------------------
double InterpCt::InterpToPt(const Pt3d& a_pt)
{
  double e, n, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12;

  e = a11 * a_pt.x + a12 * a_pt.y + d1;
  n = a21 * a_pt.x + a22 * a_pt.y + d2;
  RegionEnum region = DetermineRegion(e, n);

  // compute cardinal functions
  b1 = (1.0 / 3.0) + (SQRT3 / 2.0) * n;
  b2 = (1.0 / 3.0) - 0.75 * e - (SQRT3 / 4.0) * n;
  b3 = (1.0 / 3.0) + 0.75 * e - (SQRT3 / 4.0) * n;
  b4 = 0.25 * e + (SQRT3 / 2.0) * e * n;
  b5 = -0.125 * e + (SQRT3 / 8.0) * n + 0.375 * e * e - (SQRT3 / 4.0) * e * n - 0.375 * n * n;
  b6 = -0.125 * e - (SQRT3 / 8.0) * n - 0.375 * e * e - (SQRT3 / 4.0) * e * n + 0.375 * n * n;
  b7 = -(7.0 * SQRT3 / 81.0) - (13.0 / 36.0) * n + (SQRT3 / 18.0) * n * n;
  b8 = -(7.0 * SQRT3 / 81.0) + (13.0 * SQRT3 / 72.0) * e + (13.0 / 72.0) * n +
       (SQRT3 / 24.0) * e * e + (1.0 / 12.0) * e * n + (SQRT3 / 72.0) * n * n;
  b9 = -(7.0 * SQRT3 / 81.0) - (13.0 * SQRT3 / 72.0) * e + (13.0 / 72.0) * n +
       (SQRT3 / 24.0) * e * e - (1.0 / 12.0) * e * n + (SQRT3 / 72.0) * n * n;
  b10 = -(4.0 * SQRT3 / 81.0) - (SQRT3 / 9.0) * e - (1.0 / 9.0) * n - 0.66666667 * e * n +
        (2.0 * SQRT3 / 9.0) * n * n;
  b11 = -(4.0 * SQRT3 / 81.0) + (SQRT3 / 9.0) * e - (1.0 / 9.0) * n + 0.66666667 * e * n +
        (2.0 * SQRT3 / 9.0) * n * n;
  b12 = -(4.0 * SQRT3 / 81.0) + (2.0 / 9.0) * n + (SQRT3 / 3.0) * e * e - (SQRT3 / 9.0) * n * n;
  switch (region)
  {
  case T1:
    b1 = b1 - (SQRT3 / 2.0) * n * n * n;
    b2 = b2 + 0.25 * e * e * e + (SQRT3 / 4.0) * n * n * n;
    b3 = b3 - 0.25 * e * e * e + (SQRT3 / 4.0) * n * n * n;
    b4 = b4 + 0.75 * e * n * n;
    b5 = b5 - 0.125 * e * e * e + (SQRT3 / 4.0) * e * e * n - (3.0 * SQRT3 / 8.0) * n * n * n;
    b6 = b6 - 0.125 * e * e * e - (SQRT3 / 4.0) * e * e * n + (3.0 * SQRT3 / 8.0) * n * n * n;
    b7 = b7 + (17.0 / 36.0) * n * n * n;
    b8 = b8 - (SQRT3 / 8.0) * e * e * e - 0.25 * e * e * n - (SQRT3 / 12.0) * e * n * n -
         (11.0 / 72.0) * n * n * n;
    b9 = b9 + (SQRT3 / 8.0) * e * e * e - 0.25 * e * e * n + (SQRT3 / 12.0) * e * n * n -
         (11.0 / 72.0) * n * n * n;
    b10 = b10 + (5.0 / 9.0) * n * n * n - (SQRT3 / 3.0) * e * n * n;
    b11 = b11 + (5.0 / 9.0) * n * n * n + (SQRT3 / 3.0) * e * n * n;
    b12 = b12 + e * e * n - (13.0 / 9.0) * n * n * n;
    break;
  case T2:
    b1 = b1 - 0.25 * e * e * e - (3.0 * SQRT3 / 8.0) * e * e * n - (SQRT3 / 8.0) * n * n * n;
    b2 = b2 + (9.0 / 16.0) * e * e * e + (9.0 * SQRT3 / 16.0) * e * e * n +
         (9.0 / 16.0) * e * n * n + (SQRT3 / 16.0) * n * n * n;
    b3 = b3 - (5.0 / 16.0) * e * e * e - (3.0 * SQRT3 / 16.0) * e * e * n -
         (9.0 / 16.0) * e * n * n + (SQRT3 / 16.0) * n * n * n;
    b4 = b4 - (5.0 / 16.0) * e * e * e - (5.0 * SQRT3 / 8.0) * e * e * n - (3.0 / 16.0) * e * n * n;
    b5 = b5 - (9.0 / 32.0) * e * e * e + (3.0 * SQRT3 / 32.0) * e * e * n +
         (15.0 / 32.0) * e * n * n + (3.0 * SQRT3 / 32.0) * n * n * n;
    b6 = b6 + (11.0 / 32.0) * e * e * e + (17.0 * SQRT3 / 32.0) * e * e * n +
         (15.0 / 32.0) * e * n * n - (3.0 * SQRT3 / 32.0) * n * n * n;
    b7 = b7 + (41.0 / 144.0) * n * n * n + (1.0 / (8.0 * SQRT3)) * e * e * e +
         (3.0 / 16.0) * e * e * n;
    b8 = b8 - (17.0 * SQRT3 / 96.0) * e * e * e - (17.0 / 32.0) * e * e * n -
         (17.0 * SQRT3 / 96.0) * e * n * n - (17.0 / 288.0) * n * n * n;
    b9 = b9 + (13.0 * SQRT3 / 96.0) * e * e * e - (5.0 / 32.0) * e * e * n +
         (17.0 * SQRT3 / 96.0) * e * n * n - (17.0 / 288.0) * n * n * n;
    b10 = b10 - (7.0 / 36.0) * n * n * n + (5.0 * SQRT3 / 12.0) * e * e * e + 2.25 * e * e * n +
          (5.0 * SQRT3 / 12.0) * e * n * n;
    b11 = b11 - (7.0 / 36.0) * n * n * n - (SQRT3 / 12.0) * e * e * e - 0.75 * e * e * n -
          (5.0 * SQRT3 / 12.0) * e * n * n;
    b12 = b12 - (SQRT3 / 3.0) * e * e * e - 0.5 * e * e * n + (1.0 / 18.0) * n * n * n;
    break;
  case T3:
    b1 = b1 + 0.25 * e * e * e - (3.0 * SQRT3 / 8.0) * e * e * n - (SQRT3 / 8.0) * n * n * n;
    b2 = b2 + (5.0 / 16.0) * e * e * e - (3.0 * SQRT3 / 16.0) * e * e * n +
         (9.0 / 16.0) * e * n * n + (SQRT3 / 16.0) * n * n * n;
    b3 = b3 - (9.0 / 16.0) * e * e * e + (9.0 * SQRT3 / 16.0) * e * e * n -
         (9.0 / 16.0) * e * n * n + (SQRT3 / 16.0) * n * n * n;
    b4 = b4 - (5.0 / 16.0) * e * e * e + (5.0 * SQRT3 / 8.0) * e * e * n - (3.0 / 16.0) * e * n * n;
    b5 = b5 + (11.0 / 32.0) * e * e * e - (17.0 * SQRT3 / 32.0) * e * e * n +
         (15.0 / 32.0) * e * n * n + (3.0 * SQRT3 / 32.0) * n * n * n;
    b6 = b6 - (9.0 / 32.0) * e * e * e - (3.0 * SQRT3 / 32.0) * e * e * n +
         (15.0 / 32.0) * e * n * n - (3.0 * SQRT3 / 32.0) * n * n * n;
    b7 = b7 + (41.0 / 144.0) * n * n * n - (1.0 / (8.0 * SQRT3)) * e * e * e +
         (3.0 / 16.0) * e * e * n;
    b8 = b8 - (13.0 * SQRT3 / 96.0) * e * e * e - (5.0 / 32.0) * e * e * n -
         (17.0 * SQRT3 / 96.0) * e * n * n - (17.0 / 288.0) * n * n * n;
    b9 = b9 + (17.0 * SQRT3 / 96.0) * e * e * e - (17.0 / 32.0) * e * e * n +
         (17.0 * SQRT3 / 96.0) * e * n * n - (17.0 / 288.0) * n * n * n;
    b10 = b10 - (7.0 / 36.0) * n * n * n + (SQRT3 / 12.0) * e * e * e - 0.75 * e * e * n +
          (5.0 * SQRT3 / 12.0) * e * n * n;
    b11 = b11 - (7.0 / 36.0) * n * n * n - (5.0 * SQRT3 / 12.0) * e * e * e + 2.25 * e * e * n -
          (5.0 * SQRT3 / 12.0) * e * n * n;
    b12 = b12 + (SQRT3 / 3.0) * e * e * e - 0.5 * e * e * n + (1.0 / 18.0) * n * n * n;
    break;
  }

  return (b1 * f1 + b2 * f2 + b3 * f3 + b4 * dudz1 + b5 * dudz2 + b6 * dudz3 + b7 * dudw1 +
          b8 * dudw2 + b9 * dudw3 + b10 * dudn4 + b11 * dudn5 + b12 * dudn6);
} // CloughTocher::InterpToPt
//------------------------------------------------------------------------------
/// \brief Calculates the coefficients used by ct interpolation
/// \param[in] a_ptIdxs Point indices for a triangle
/// \return true if successful, false if errors were encountered
//------------------------------------------------------------------------------
bool InterpCt::ComputeCtCoeff(int* a_ptIdxs)
{
  int ptIdx[3];
  for (int i = 0; i < 3; ++i)
    ptIdx[i] = a_ptIdxs[i];
  if (ptIdx[0] < 0 || ptIdx[1] < 0 || ptIdx[2] < 0)
    return false;
  x1 = m_pts[ptIdx[0]].x;
  p1 = m_pts[ptIdx[0]].y;
  f1 = m_pts[ptIdx[0]].z;

  x2 = m_pts[ptIdx[2]].x;
  y2 = m_pts[ptIdx[2]].y;
  f2 = m_pts[ptIdx[2]].z;

  x3 = m_pts[ptIdx[1]].x;
  y3 = m_pts[ptIdx[1]].y;
  f3 = m_pts[ptIdx[1]].z;

  delta = x3 * p1 - x1 * y3 + x1 * y2 - x2 * p1 + x2 * y3 - x3 * y2;
  a11 = (2.0 * p1 - y3 - y2) / delta;
  a12 = (-2.0 * x1 + x2 + x3) / delta;
  d1 = (x1 * (y3 + y2) - p1 * (x3 + x2)) / delta;
  a21 = (-SQRT3 * (y3 - y2)) / delta;
  a22 = (SQRT3 * (x3 - x2)) / delta;
  d2 = 2 / SQRT3 + (SQRT3 * (x1 * (y3 - y2) - p1 * (x3 - x2))) / delta;

  d11 = 0.5 * (x3 - x2);
  d12 = (1.0 / (2.0 * SQRT3)) * (2.0 * x1 - x2 - x3);
  d21 = 0.5 * (y3 - y2);
  d22 = (1.0 / (2.0 * SQRT3)) * (2.0 * p1 - y2 - y3);

  fx1 = fy1 = fx2 = fy2 = fx3 = fy3 = 0;
  if (m_nodalFunc)
  {
    Pt3d grad;
    m_nodalFunc->GradientFromPtIdx(ptIdx[0], grad);
    fx1 = grad.x;
    fy1 = grad.y;
    m_nodalFunc->GradientFromPtIdx(ptIdx[2], grad);
    fx2 = grad.x;
    fy2 = grad.y;
    m_nodalFunc->GradientFromPtIdx(ptIdx[1], grad);
    fx3 = grad.x;
    fy3 = grad.y;
  }

  ue1 = d11 * fx1 + d21 * fy1;
  un1 = d12 * fx1 + d22 * fy1;
  ue2 = d11 * fx2 + d21 * fy2;
  un2 = d12 * fx2 + d22 * fy2;
  ue3 = d11 * fx3 + d21 * fy3;
  un3 = d12 * fx3 + d22 * fy3;
  ue4 = 0.5 * (ue1 + ue3);
  un4 = 0.5 * (un1 + un3);
  ue5 = 0.5 * (ue1 + ue2);
  un5 = 0.5 * (un1 + un2);
  ue6 = 0.5 * (ue2 + ue3);
  un6 = 0.5 * (un2 + un3);

  dudz1 = ue1;
  dudz2 = -0.5 * ue2 + (SQRT3 / 2) * un2;
  dudz3 = -0.5 * ue3 - (SQRT3 / 2) * un3;
  dudw1 = un1;
  dudw2 = -(SQRT3 / 2) * ue2 - 0.5 * un2;
  dudw3 = (SQRT3 / 2) * ue3 - 0.5 * un3;
  dudn4 = (SQRT3 / 2) * ue4 + 0.5 * un4;
  dudn5 = -(SQRT3 / 2) * ue5 + 0.5 * un5;
  dudn6 = -un6;
  return true;
} // CloughTocher::ComputeCtCoeff

} // namespace xms
