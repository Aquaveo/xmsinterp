//------------------------------------------------------------------------------
/// \file
/// \ingroup geometry
/// \copyright (C) Copyright Aquaveo 2018. Distributed under FreeBSD License
/// (See accompanying file LICENSE or https://aqaveo.com/bsd/license.txt)
//------------------------------------------------------------------------------
#pragma once

//----- Included files ---------------------------------------------------------
#include <xmscore/stl/vector.h>
#include <xmscore/misc/base_macros.h>
#include <xmscore/misc/boost_defines.h>

//----- Forward declarations ---------------------------------------------------

//----- Namespace declaration --------------------------------------------------
namespace xms
{
//----- Constants / Enumerations -----------------------------------------------

//----- Structs / Classes ------------------------------------------------------
/// \brief Redistributes the point locations on a polyline based on a size
/// \see GmPolyLinePtRedistributerImpl
class GmPolyLinePtRedistributer
{
public:
  static BSHP<GmPolyLinePtRedistributer> New();

  /// \cond
  virtual VecPt3d Redistribute(const VecPt3d& a_input, double a_size) = 0;

private:
  XM_DISALLOW_COPY_AND_ASSIGN(GmPolyLinePtRedistributer);
  /// \endcond

protected:
  GmPolyLinePtRedistributer();
  virtual ~GmPolyLinePtRedistributer();
};
//----- Function prototypes ----------------------------------------------------

} // namespace xms
