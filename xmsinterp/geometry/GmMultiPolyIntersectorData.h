#pragma once
//------------------------------------------------------------------------------
/// \file
/// \ingroup geometry
/// \copyright (C) Copyright Aquaveo 2018. Distributed under the xmsng
///  Software License, Version 1.0. (See accompanying file
///  LICENSE_1_0.txt or copy at http://www.aquaveo.com/xmsng/LICENSE_1_0.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------

// 3. Standard library headers
#include <set>

// 5. Shared code headers
#include <xmscore/points/pt.h>

//----- Namespace --------------------------------------------------------------

namespace xms
{
////////////////////////////////////////////////////////////////////////////////
/// \brief An intersection point of a line with a polygon.
class ix
{
public:
  /// constructor
  /// \param a_pt: The point.
  /// \param a_i: The polygon id (1 based)
  /// \param a_t: tvalue.
  ix(Pt3d a_pt, int a_i, double a_t)
  : m_pt(a_pt)
  , m_i(a_i)
  , m_t(a_t)
  {
  }
  /// operator==
  /// \param rhs: right hand side.
  /// \return true if ==
  bool operator==(const ix& rhs) { return (m_pt == rhs.m_pt && m_i == rhs.m_i && m_t == rhs.m_t); }

  Pt3d m_pt;  ///< Intersection location
  int m_i;    ///< The polygon id (1 based)
  double m_t; ///< t values
};            // class ix

////////////////////////////////////////////////////////////////////////////////
/// \brief Struct used by GmMultiPolyIntersector.
struct GmMultiPolyIntersectorData
{
  std::vector<Pt3d> m_points;             ///< All points used by all polygons
  std::vector<std::vector<int> > m_polys; ///< 0-based? indices into m_points to form polygons
  std::set<int> m_polys1;                 ///< polygon IDs (1-based) that 1st point is inside or on
  std::set<int> m_polys2;                 ///< polygon IDs (1-based) that 2nd point is inside on on
  std::vector<ix> m_ixs;                  ///< Intersections
};                                        // struct GmMultiPolyIntersectorData

} // namespace xms
