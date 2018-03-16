//------------------------------------------------------------------------------
/// \file
/// \ingroup geometry
/// \copyright (C) Copyright Aquaveo 2018. Distributed under the xmsng
///  Software License, Version 1.0. (See accompanying file
///  LICENSE_1_0.txt or copy at http://www.aquaveo.com/xmsng/LICENSE_1_0.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------

// 1. Precompiled header

// 2. My header
#include <xmsinterp/geometry/GmExtents.h>

// 3. Standard Library Headers

// 4. External Library Headers

// 5. Shared Headers
#include <boost/serialization/export.hpp>
#include <boost/serialization/vector.hpp>
#include <xmscore/math/math.h>
#include <xmscore/misc/XmConst.h>

// 6. Non-shared Headers

using namespace xms;

//----- Namespace declaration --------------------------------------------------

namespace xms
{
////////////////////////////////////////////////////////////////////////////////
/// \class GmExtents2d
/// \brief 2D geometric extents (min/max).
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/// \brief constructor
//------------------------------------------------------------------------------
GmExtents2d::GmExtents2d()
{
  Clear();
} // GmExtents2d::GmExtents2d
//------------------------------------------------------------------------------
/// \brief constructor
/// \param[in] a_extents: GmExtents3d
//------------------------------------------------------------------------------
GmExtents2d::GmExtents2d(const GmExtents3d& a_extents)
{
  m_min = static_cast<Pt2d>(a_extents.GetMin());
  m_max = static_cast<Pt2d>(a_extents.GetMax());
}
//------------------------------------------------------------------------------
/// \brief constructor
/// \param[in] a_min: Minimum extents.
/// \param[in] a_max: Maximum extents.
//------------------------------------------------------------------------------
GmExtents2d::GmExtents2d(const Pt2d& a_min, const Pt2d& a_max)
: m_min(a_min)
, m_max(a_max)
{
} // GmExtents2d::GmExtents2d
//------------------------------------------------------------------------------
/// \brief constructor
/// \param[in] a_min: Minimum extents.
/// \param[in] a_max: Maximum extents.
//------------------------------------------------------------------------------
GmExtents2d::GmExtents2d(const Pt3d& a_min, const Pt3d& a_max)
: m_min(a_min)
, m_max(a_max)
{
} // GmExtents2d::GmExtents2d
//------------------------------------------------------------------------------
/// \brief destructor
//------------------------------------------------------------------------------
GmExtents2d::~GmExtents2d()
{
} // GmExtents2d::~GmExtents2d
//------------------------------------------------------------------------------
/// \brief operator +=
/// \param[in] a_rhs: GmExtents2d
//------------------------------------------------------------------------------
void GmExtents2d::operator+=(const GmExtents2d& a_rhs)
{
  m_min.x = std::min(m_min.y, a_rhs.m_min.x);
  m_max.x = std::max(m_max.x, a_rhs.m_max.x);
  m_min.y = std::min(m_min.y, a_rhs.m_min.y);
  m_max.y = std::max(m_max.y, a_rhs.m_max.y);
} // GmExtents2d::operator +=
//------------------------------------------------------------------------------
/// \brief Add a point to the extents
/// \param[in] pt: A point
//------------------------------------------------------------------------------
void GmExtents2d::AddToExtents(const Pt3d& pt)
{
  Pt3d thePt(pt);

  m_min.x = std::min(m_min.x, thePt.x);
  m_max.x = std::max(m_max.x, thePt.x);
  m_min.y = std::min(m_min.y, thePt.y);
  m_max.y = std::max(m_max.y, thePt.y);
} // GmExtents2d::AddToExtents.
//------------------------------------------------------------------------------
/// \brief If we don't have any data we return false
/// \return true if we have data.
//------------------------------------------------------------------------------
bool GmExtents2d::IsValid()
{
  return (m_min.x <= m_max.x);
} // GmExtents2d::IsValid
//------------------------------------------------------------------------------
/// \brief Sets the tolerance to use.
/// \param[in] a_Tol: The tolerance.
//------------------------------------------------------------------------------
void GmExtents2d::SetTolerance(double a_Tol)
{
  m_tolerance = a_Tol;
} // GmExtents2d::SetTolerance
//------------------------------------------------------------------------------
/// \brief Gets the tolerance in use.
/// \returns tolerance.
//------------------------------------------------------------------------------
double GmExtents2d::GetTolerance()
{
  return m_tolerance;
} // GmExtents2d::GetTolerance
//------------------------------------------------------------------------------
/// \brief Test to see if two extents overlap.
/// \param[in] a_b: The other extents.
/// \return true if they overlap.
//------------------------------------------------------------------------------
bool GmExtents2d::Overlap(const GmExtents2d& a_b) const
{
  // We add (or subtract) the tolerance to our mins and maxs to get
  // modified boundaries
  Pt2d modMin = m_min - m_tolerance;
  Pt2d modMax = m_max + m_tolerance;

  if (a_b.m_min.x > modMax.x)
  {
    return false;
  }
  if (a_b.m_max.x < modMin.x)
  {
    return false;
  }
  if (a_b.m_min.y > modMax.y)
  {
    return false;
  }
  if (a_b.m_max.y < modMin.y)
  {
    return false;
  }
  return true;
} // GmExtents2d::Overlap
//------------------------------------------------------------------------------
/// \brief Resets the extents to defaults. IsValid will return false.
//------------------------------------------------------------------------------
void GmExtents2d::Clear()
{
  m_min = XM_DBL_HIGHEST;
  m_max = XM_DBL_LOWEST;
} // GmExtents2d::Clear
//------------------------------------------------------------------------------
/// \brief Gets the extents via the parameters.
/// \param[out] a_MinX: Minimum x.
/// \param[out] a_MaxX: Maximum x.
/// \param[out] a_MinY: Minimum y.
/// \param[out] a_MaxY: Maximum y.
//------------------------------------------------------------------------------
void GmExtents2d::GetExtents(double* a_MinX, double* a_MaxX, double* a_MinY, double* a_MaxY)
{
  if (a_MinX)
  {
    *a_MinX = m_min.x;
  }
  if (a_MaxX)
  {
    *a_MaxX = m_max.x;
  }
  if (a_MinY)
  {
    *a_MinY = m_min.y;
  }
  if (a_MaxY)
  {
    *a_MaxY = m_max.y;
  }
} // GmExtents2d::GetExtents
//------------------------------------------------------------------------------
/// \brief Gets the extents via the parameters.
/// \param[out] a_dMin: Minimum.
/// \param[out] a_dMax: Maximum.
//------------------------------------------------------------------------------
void GmExtents2d::GetExtents(Pt3d& a_dMin, Pt3d& a_dMax)
{
  a_dMin = m_min;
  a_dMax = m_max;
} // GmExtents2d::GetExtents
//------------------------------------------------------------------------------
/// \brief Gets the minimum x.
/// \return Minimum x.
//------------------------------------------------------------------------------
double GmExtents2d::GetMinX() const
{
  return m_min.x;
} // GmExtents2d::GetMinX
//------------------------------------------------------------------------------
/// \brief Gets the maximum x.
/// \return Maximum x.
//------------------------------------------------------------------------------
double GmExtents2d::GetMaxX() const
{
  return m_max.x;
} // GmExtents2d::GetMaxX
//------------------------------------------------------------------------------
/// \brief Gets the minimum y.
/// \return Minimum y.
//------------------------------------------------------------------------------
double GmExtents2d::GetMinY() const
{
  return m_min.y;
} // GmExtents2d::GetMinY
//------------------------------------------------------------------------------
/// \brief Gets the maximum y.
/// \return Maximum y.
//------------------------------------------------------------------------------
double GmExtents2d::GetMaxY() const
{
  return m_max.y;
} // GmExtents2d::GetMaxY
//------------------------------------------------------------------------------
/// \brief Gets the minimum.
/// \return Minimum.
//------------------------------------------------------------------------------
Pt2d GmExtents2d::GetMin() const
{
  return m_min;
} // GmExtents2d::GetMin
//------------------------------------------------------------------------------
/// \brief Gets the maximum.
/// \return Maximum.
//------------------------------------------------------------------------------
Pt2d GmExtents2d::GetMax() const
{
  return m_max;
} // GmExtents2d::GetMax
//------------------------------------------------------------------------------
/// \brief Sets the min/max.
/// \param[in] a_min: The minimum extents.
/// \param[in] a_max: The maximum extents.
//------------------------------------------------------------------------------
void GmExtents2d::Set(const Pt2d& a_min, const Pt2d& a_max)
{
  m_min = a_min;
  m_max = a_max;
} // GmExtents2d::Set

double GmExtents2d::m_tolerance(0.0); ///< Tolerance. Static ?

////////////////////////////////////////////////////////////////////////////////
/// \class GmExtents3d
/// \brief 3D geometric extents (min/max).
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/// \brief constructor
//------------------------------------------------------------------------------
GmExtents3d::GmExtents3d()
{
  Clear();
} // GmExtents3d::GmExtents3d
//------------------------------------------------------------------------------
/// \brief constructor
/// \param a_min: Minimum.
/// \param a_max: Maximum.
//------------------------------------------------------------------------------
GmExtents3d::GmExtents3d(const Pt3d& a_min, const Pt3d& a_max)
: m_min(a_min)
, m_max(a_max)
{
} // GmExtents3d::GmExtents3d
//------------------------------------------------------------------------------
/// \brief constructor
/// \param a_min: Minimum.
/// \param a_max: Maximum.
//------------------------------------------------------------------------------
GmExtents3d::GmExtents3d(const Pt2d& a_min, const Pt2d& a_max)
: m_min(a_min)
, m_max(a_max)
{
} // GmExtents3d::GmExtents3d
//------------------------------------------------------------------------------
/// \brief constructor
/// \param a_extents: GmExtents2d.
//------------------------------------------------------------------------------
GmExtents3d::GmExtents3d(const GmExtents2d& a_extents)
{
  m_min = static_cast<Pt3d>(a_extents.GetMin());
  m_max = static_cast<Pt3d>(a_extents.GetMax());
}
//------------------------------------------------------------------------------
/// \brief operator +=
/// \param[in] a_rhs: GmExtents2d
//------------------------------------------------------------------------------
void GmExtents3d::operator+=(const GmExtents3d& a_rhs)
{
  m_min.x = std::min(m_min.x, a_rhs.m_min.x);
  m_max.x = std::max(m_max.x, a_rhs.m_max.x);
  m_min.y = std::min(m_min.y, a_rhs.m_min.y);
  m_max.y = std::max(m_max.y, a_rhs.m_max.y);
  m_min.z = std::min(m_min.z, a_rhs.m_min.z);
  m_max.z = std::max(m_max.z, a_rhs.m_max.z);
} // GmExtents3d::operator +=
//------------------------------------------------------------------------------
/// \brief Add a point to the extents
/// \param[in] pt: A point
//------------------------------------------------------------------------------
void GmExtents3d::AddToExtents(const Pt3d& pt)
{
  m_min.x = std::min(m_min.x, pt.x);
  m_max.x = std::max(m_max.x, pt.x);
  m_min.y = std::min(m_min.y, pt.y);
  m_max.y = std::max(m_max.y, pt.y);
  m_min.z = std::min(m_min.z, pt.z);
  m_max.z = std::max(m_max.z, pt.z);
} // GmExtents3d::AddToExtents
//------------------------------------------------------------------------------
/// \brief If we don't have any data we return false
/// \return true if we have data.
//------------------------------------------------------------------------------
bool GmExtents3d::IsValid() const
{
  return (m_min.x <= m_max.x);
} // GmExtents3d::IsValid
//------------------------------------------------------------------------------
/// \brief Sets the tolerance to use.
/// \param[in] a_Tol: The tolerance.
//------------------------------------------------------------------------------
void GmExtents3d::SetTolerance(double a_Tol)
{
  m_tolerance = a_Tol;
} // GmExtents3d::SetTolerance
//------------------------------------------------------------------------------
/// \brief Gets the tolerance in use.
/// \returns tolerance.
//------------------------------------------------------------------------------
double GmExtents3d::GetTolerance()
{
  return m_tolerance;
} // GmExtents3d::GetTolerance
//------------------------------------------------------------------------------
/// \brief Test to see if two extents overlap.
/// \param[in] a_b: The other extents.
/// \return true if they overlap.
//------------------------------------------------------------------------------
bool GmExtents3d::Overlap(GmExtents3d& a_b) const
{
  // We add (or subtract) the tolerance to our mins and maxs to get
  // modified boundaries
  Pt3d modMin = m_min - m_tolerance;
  Pt3d modMax = m_max + m_tolerance;

  if (a_b.m_min.x > modMax.x)
  {
    return false;
  }
  if (a_b.m_max.x < modMin.x)
  {
    return false;
  }
  if (a_b.m_min.y > modMax.y)
  {
    return false;
  }
  if (a_b.m_max.y < modMin.y)
  {
    return false;
  }
  if (a_b.m_min.z > modMax.z)
  {
    return false;
  }
  if (a_b.m_max.z < modMin.z)
  {
    return false;
  }
  return true;
} // GmExtents3d::Overlap
//------------------------------------------------------------------------------
/// \brief Resets the extents to defaults. IsValid will return false.
//------------------------------------------------------------------------------
void GmExtents3d::Clear()
{
  m_min = XM_DBL_HIGHEST;
  m_max = XM_DBL_LOWEST;
} // GmExtents3d::Clear
//------------------------------------------------------------------------------
/// \brief Tests if the point is inside the extents.
/// \param[out] pt: A point
/// \return true if point is inside or on.
//------------------------------------------------------------------------------
bool GmExtents3d::IsPointWithin(const Pt3d& pt) const
{
  return (m_min.x <= pt.x && m_max.x >= pt.x && m_min.y <= pt.y && m_max.y >= pt.y &&
          m_min.z <= pt.z && m_max.z >= pt.z);
} // GmExtents3d::IsPointWithin
//------------------------------------------------------------------------------
/// \brief Gets the extents via the parameters.
/// \param[out] a_MinX: Minimum x.
/// \param[out] a_MaxX: Maximum x.
/// \param[out] a_MinY: Minimum y.
/// \param[out] a_MaxY: Maximum y.
/// \param[out] a_MinZ: Minimum z.
/// \param[out] a_MaxZ: Maximum z.
//------------------------------------------------------------------------------
void GmExtents3d::GetExtents(double* a_MinX,
                             double* a_MaxX,
                             double* a_MinY,
                             double* a_MaxY,
                             double* a_MinZ,
                             double* a_MaxZ) const
{
  if (a_MinX)
  {
    *a_MinX = m_min.x;
  }
  if (a_MaxX)
  {
    *a_MaxX = m_max.x;
  }
  if (a_MinY)
  {
    *a_MinY = m_min.y;
  }
  if (a_MaxY)
  {
    *a_MaxY = m_max.y;
  }
  if (a_MinZ)
  {
    *a_MinZ = m_min.z;
  }
  if (a_MaxZ)
  {
    *a_MaxZ = m_max.z;
  }
} // GmExtents3d::GetExtents
//------------------------------------------------------------------------------
/// \brief Gets the extents via the parameters.
/// \param[out] a_dMin: Minimum.
/// \param[out] a_dMax: Maximum.
//------------------------------------------------------------------------------
void GmExtents3d::GetExtents(Pt3d& a_dMin, Pt3d& a_dMax) const
{
  a_dMin = m_min;
  a_dMax = m_max;
} // GmExtents3d::GetExtents
//------------------------------------------------------------------------------
/// \brief Gets the minimum x.
/// \return Minimum x.
//------------------------------------------------------------------------------
double GmExtents3d::GetMinX() const
{
  return m_min.x;
} // GmExtents3d::GetMinX
//------------------------------------------------------------------------------
/// \brief Gets the maximum x.
/// \return Maximum x.
//------------------------------------------------------------------------------
double GmExtents3d::GetMaxX() const
{
  return m_max.x;
} // GmExtents3d::GetMaxX
//------------------------------------------------------------------------------
/// \brief Gets the minimum y.
/// \return Minimum y.
//------------------------------------------------------------------------------
double GmExtents3d::GetMinY() const
{
  return m_min.y;
} // GmExtents3d::GetMinY
//------------------------------------------------------------------------------
/// \brief Gets the maximum y.
/// \return Maximum y.
//------------------------------------------------------------------------------
double GmExtents3d::GetMaxY() const
{
  return m_max.y;
} // GmExtents3d::GetMaxY
//------------------------------------------------------------------------------
/// \brief Gets the minimum z.
/// \return Minimum z.
//------------------------------------------------------------------------------
double GmExtents3d::GetMinZ() const
{
  return m_min.z;
} // GmExtents3d::GetMinZ
//------------------------------------------------------------------------------
/// \brief Gets the maximum z.
/// \return Maximum z.
//------------------------------------------------------------------------------
double GmExtents3d::GetMaxZ() const
{
  return m_max.z;
} // GmExtents3d::GetMaxZ
//------------------------------------------------------------------------------
/// \brief Gets the minimum.
/// \return Minimum.
//------------------------------------------------------------------------------
Pt3d GmExtents3d::GetMin() const
{
  return m_min;
} // GmExtents3d::GetMin
//------------------------------------------------------------------------------
/// \brief Gets the maximum.
/// \return Maximum.
//------------------------------------------------------------------------------
Pt3d GmExtents3d::GetMax() const
{
  return m_max;
} // GmExtents3d::GetMax
//------------------------------------------------------------------------------
/// \brief Sets the min/max.
/// \param[in] a_min: The minimum extents.
/// \param[in] a_max: The maximum extents.
//------------------------------------------------------------------------------
void GmExtents3d::Set(const Pt3d& a_min, const Pt3d& a_max)
{
  m_min = a_min;
  m_max = a_max;
} // GmExtents3d::Set

double GmExtents3d::m_tolerance(1.0e-6); ///< Tolerance. Static ?

} // namespace xms

////////////////////////////////////////////////////////////////////////////////
#ifdef CXX_TEST

#include <xmsinterp/geometry/GmExtents.t.h>

//----- Namespace declaration --------------------------------------------------

// namespace xms {
using namespace xms;

////////////////////////////////////////////////////////////////////////////////
/// \class GmExtents3dUnitTests
/// \brief Tests GmExtents3d
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/// \brief Tests a single point.
//------------------------------------------------------------------------------
void GmExtents3dUnitTests::testSinglePt()
{
  GmExtents3d e;

  Pt3d pt(50.0, 60.0, 80.0);

  e.AddToExtents(pt);

  TS_ASSERT(e.IsValid());
  const double kDelta = 0.000001;
  TS_ASSERT_DELTA(50.0, e.GetMinX(), kDelta);
  TS_ASSERT_DELTA(50.0, e.GetMaxX(), kDelta);
  TS_ASSERT_DELTA(60.0, e.GetMinY(), kDelta);
  TS_ASSERT_DELTA(60.0, e.GetMaxY(), kDelta);
  TS_ASSERT_DELTA(80.0, e.GetMinZ(), kDelta);
  TS_ASSERT_DELTA(80.0, e.GetMaxZ(), kDelta);
} // GmExtents3dUnitTests::testSinglePt
//------------------------------------------------------------------------------
/// \brief Tests multiple points.
//------------------------------------------------------------------------------
void GmExtents3dUnitTests::testMultiplePts()
{
  GmExtents3d e;

  Pt3d pt(50.0, 60.0, 80.0);

  e.AddToExtents(pt);

  pt.Set(100.0, 120.0, 160.0);
  e.AddToExtents(pt);
  pt.Set(200.0, -100.0, -200.0);
  e.AddToExtents(pt);

  TS_ASSERT(e.IsValid());
  const double kDelta = 0.000001;
  TS_ASSERT_DELTA(50.0, e.GetMinX(), kDelta);
  TS_ASSERT_DELTA(200.0, e.GetMaxX(), kDelta);
  TS_ASSERT_DELTA(-100.0, e.GetMinY(), kDelta);
  TS_ASSERT_DELTA(120.0, e.GetMaxY(), kDelta);
  TS_ASSERT_DELTA(-200.0, e.GetMinZ(), kDelta);
  TS_ASSERT_DELTA(160.0, e.GetMaxZ(), kDelta);
} // GmExtents3dUnitTests::testMultiplePts
//------------------------------------------------------------------------------
/// \brief Tests no points.
//------------------------------------------------------------------------------
void GmExtents3dUnitTests::testNoPts()
{
  GmExtents3d e;

  TS_ASSERT(!e.IsValid());

} // GmExtents3dUnitTests::testNoPts
//------------------------------------------------------------------------------
/// \brief Tests overlap.
//------------------------------------------------------------------------------
void GmExtents3dUnitTests::testOverlapping()
{
  {
    GmExtents3d e, f;
    // x -100 to -50, y 50 to 100, z -25 to 25
    Pt3d pt(-100.0, 50.0, -25.0);
    e.AddToExtents(pt);
    pt.Set(-50.0, 100.0, -25.0);
    e.AddToExtents(pt);
    pt.Set(-50.0, 100.0, 25.0);
    e.AddToExtents(pt);

    // x -150 to -40, y 25 to 75, z -15 to 55
    pt.Set(-150.0, 30.0, 55.0);
    f.AddToExtents(pt);
    pt.Set(-50.0, 25.0, -5.0);
    f.AddToExtents(pt);
    pt.Set(-40.0, 75.0, -15.0);
    f.AddToExtents(pt);

    TS_ASSERT(e.IsValid());
    TS_ASSERT(f.IsValid());
    TS_ASSERT_EQUALS(e.Overlap(f), true);

    // x -550 to -101, y 101 to 251, z -125 to -26
    f.Clear();
    pt.Set(-101.0, 101.0, -125.0);
    f.AddToExtents(pt);
    pt.Set(-550.0, 182.0, -98.0);
    f.AddToExtents(pt);
    pt.Set(-150.0, 251.0, -26.0);
    f.AddToExtents(pt);

    TS_ASSERT_EQUALS(e.Overlap(f), false);

    // x -150 to -100+, y 50 to 100, z -25 to 25
    f.SetTolerance(0.01);
    f.Clear();
    pt.Set(-100.0 - 2 * f.GetTolerance(), 50.0, -25.0);
    f.AddToExtents(pt);
    pt.Set(-150.0, 100.0, -25.0);
    f.AddToExtents(pt);
    pt.Set(-150.0, 100.0, 25.0);
    f.AddToExtents(pt);

    TS_ASSERT_EQUALS(e.Overlap(f), false);
  }
  //--------------------------------------------------
  {
    GmExtents2d e, f;

    Pt3d pt(-100.0, 50.0, 0);
    e.AddToExtents(pt);
    pt.Set(-50.0, 100.0, 0);
    e.AddToExtents(pt);

    pt.Set(-150.0, 25.0, 0);
    f.AddToExtents(pt);
    pt.Set(-101.0, 75.0, 0);
    f.AddToExtents(pt);

    // x -100 to -50,  y 50 to 100
    // x -150 to -101, y 25 to 75
    //      X                    Y
    //          |-------|     |--------|
    // |-------|          |--------|
    TS_ASSERT(e.IsValid());
    TS_ASSERT(f.IsValid());
    TS_ASSERT_EQUALS(e.Overlap(f), false);

    f.Clear();
    pt.Set(-550.0, 251.0, 0);
    f.AddToExtents(pt);
    pt.Set(-101.0, 101.0, 0);
    f.AddToExtents(pt);

    // x -100 to -50,  y 50 to 100
    // x -550 to -101, y 101 to 251
    //      X                    Y
    //     |-------|    |--------|
    // |-------|                  |--------|
    TS_ASSERT_EQUALS(e.Overlap(f), false);

    // x -100 to -50,  y 50 to 100
    // x -150 to -100+, y -50 to 75
    //      X                    Y
    //          |-------|       |--------|
    // |-------|           |--------|
    f.Clear();
    pt.Set(-100.0 - 2 * f.GetTolerance(), -50.0, 0);
    f.AddToExtents(pt);
    pt.Set(-150.0, 75.0, 0);
    f.AddToExtents(pt);

    TS_ASSERT_EQUALS(e.Overlap(f), true);

    // x -100 to -50,  y  50 to 100
    // x -75 to -60,   y -50 to 75
    //      X                Y
    // |-----------|     |--------|
    //   |-------|     |------------|
    f.Clear();
    pt.Set(-75.0 - f.GetTolerance(), 25.0, 0);
    f.AddToExtents(pt);
    pt.Set(-60.0, 125.0, 0);
    f.AddToExtents(pt);

    TS_ASSERT_EQUALS(e.Overlap(f), true);
  }
} // GmExtents3dUnitTests::testOverlapping

  //} // namespace xms

#endif