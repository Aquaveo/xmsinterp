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
#include <vector>

// 4. External library headers

// 5. Shared code headers
#include <xmscore/misc/boost_defines.h> // BSHP
#include <xmscore/stl/vector.h>
#include <xmscore/misc/base_macros.h> // for XM_DISALLOW_COPY_AND_ASSIGN
#include <xmscore/points/ptsfwd.h>    // for Pt3d

//----- Namespace --------------------------------------------------------------

namespace xms
{
//----- Forward declarations ---------------------------------------------------

//----- Classes ----------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// \brief Interface to a boost::geometry::polygon class
/// \see GmPolygonImpl
class GmPolygon
{
public:
  static BSHP<GmPolygon> New();
  GmPolygon();
  virtual ~GmPolygon();

  /// \cond
  virtual void Setup(const VecPt3d& a_poly, const VecPt3d2d& a_inPolys) = 0;
  virtual void GetPoints(VecPt3d& a_poly, VecPt3d2d& a_inPolys) const = 0;

  virtual bool CoveredBy(Pt3d a_point) const = 0;
  virtual bool Within(Pt3d a_point) const = 0;
  virtual double MinDistanceToBoundary(Pt3d a_pt) const = 0;
  virtual void Intersection(const GmPolygon& a_, std::vector<BSHP<GmPolygon>>& a_output) const = 0;
  virtual void Union(const GmPolygon& a_, std::vector<BSHP<GmPolygon>>& a_output) const = 0;

private:
  XM_DISALLOW_COPY_AND_ASSIGN(GmPolygon);
  /// \endcond
}; // class GmPolygon

} // namespace xms
