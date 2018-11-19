#pragma once
//------------------------------------------------------------------------------
/// \file
/// \ingroup geometry
/// \copyright (C) Copyright Aquaveo 2018. Distributed under FreeBSD License
/// (See accompanying file LICENSE or https://aqaveo.com/bsd/license.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------

// 3. Standard Library Headers
#include <vector>

// 5. Shared code headers
#include <xmscore/points/ptsfwd.h>

//----- Namespace --------------------------------------------------------------

namespace xms
{
//----- Forward declarations ---------------------------------------------------

struct GmMultiPolyIntersectorData;

//----- Classes ----------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// \class GmMultiPolyIntersectionSorter
/// \brief Base class for sorting intersections from GmMultiPolyIntersector
/// \see GmMultiPolyIntersectionSorterTerse
class GmMultiPolyIntersectionSorter
{
public:
  virtual ~GmMultiPolyIntersectionSorter() {}
  /// \cond
  virtual void Sort(GmMultiPolyIntersectorData& a_data,
                    std::vector<int>& polyids,
                    std::vector<double>& tvalues,
                    std::vector<Pt3d>& a_pts,
                    double a_tol) = 0;
  /// \endcond
}; // class GmMultiPolyIntersectionSorter

} // namespace xms
