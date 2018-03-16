#pragma once
//------------------------------------------------------------------------------
/// \file
/// \brief boost::geometry types
/// \ingroup geometry
/// \copyright (C) Copyright Aquaveo 2018. Distributed under the xmsng
///  Software License, Version 1.0. (See accompanying file
///  LICENSE_1_0.txt or copy at http://www.aquaveo.com/xmsng/LICENSE_1_0.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------

#include <xmscore/points/ptsfwd.h>
#pragma warning(push)
#pragma warning(disable : 4512) // Boost code: no assignment operator
#include <boost/geometry/core/cs.hpp>
#include <boost/geometry/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/register/point.hpp>
#pragma warning(pop)

//----- Constants / Enumerations -----------------------------------------------

BOOST_GEOMETRY_REGISTER_POINT_2D(xms::Pt3d, double, cs::cartesian, x, y)
// It causes problems if you sprinkle this in different locations in the code.
// Especially if you try to use the 3D version of this macro with Pt3d then
// it is not clear which version will be used. So we only want this code in
// one place.

//----- Namespace declaration --------------------------------------------------

namespace xms
{
/// Boost geometry 2d point
typedef boost::geometry::model::d2::point_xy<double> GmBstPt2d;
/// Boost polygon 2d
typedef boost::geometry::model::polygon<GmBstPt2d> GmBstPoly2d;
/// Boost polygon 3d
typedef boost::geometry::model::polygon<Pt3d> GmBstPoly3d;
/// Boost box 3d
typedef boost::geometry::model::box<Pt3d> GmBstBox3d;
/// Boost line 3d
typedef boost::geometry::model::linestring<Pt3d> GmBstLine3d;
/// Boost ring 3d
typedef boost::geometry::ring_type<GmBstPoly3d>::type GmBstRing3d;

} // namespace xms
