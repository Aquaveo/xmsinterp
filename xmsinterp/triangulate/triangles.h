#pragma once
//------------------------------------------------------------------------------
/// \file
/// \brief Functions dealing with triangles.
/// \ingroup triangulate
/// \copyright (C) Copyright Aquaveo 2018. Distributed under the xmsng
///  Software License, Version 1.0. (See accompanying file
///  LICENSE_1_0.txt or copy at http://www.aquaveo.com/xmsng/LICENSE_1_0.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------

// 3. Standard Library Headers
#include <vector>

// 4. External Library Headers
#include <xmscore/points/pt.h>

// 5. Shared Headers
#include <xmscore/stl/vector.h>

// 6. Non-shared Headers

//----- Namespace declaration --------------------------------------------------

namespace xms
{
//----- Constants / Enumerations -----------------------------------------------

//----- Forward declarations ---------------------------------------------------

//----- Structs / Classes -----------------------------------------------------

//----- Function prototypes ----------------------------------------------------

double trArea(const Pt3d& a_pt1, const Pt3d& a_pt2, const Pt3d& a_pt3);
void trBuildGridPolys(int rows, int cols, VecPt3d& pts, VecInt2d& polys);
void trBuildGridTrianglePolys(int rows, int cols, VecPt3d& a_points, VecInt2d& a_polys);

//------------------------------------------------------------------------------
/// \brief Faster than a % operation and we do this a lot
/// \param i: The index.
/// \return The incremented index.
//------------------------------------------------------------------------------
inline int trIncrementIndex(int i)
{
  static const int plus1mod3[3] = {1, 2, 0};
  return plus1mod3[i];
}
//------------------------------------------------------------------------------
/// \brief Faster than a % operation and we do this a lot
/// \param i: The index.
/// \return The decremented index.
//------------------------------------------------------------------------------
inline int trDecrementIndex(int i)
{
  static const int minus1mod3[3] = {2, 0, 1};
  return minus1mod3[i];
}

} // namespace xms
