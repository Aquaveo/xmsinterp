#pragma once
//------------------------------------------------------------------------------
/// \file
/// \brief Signals that can be called when performing idw or kriging
/// interpolation. Used by GMS with Pilot Points.
/// \ingroup interpolate
/// \copyright (C) Copyright Aquaveo 2018. Distributed under the xmsng
///  Software License, Version 1.0. (See accompanying file
///  LICENSE_1_0.txt or copy at http://www.aquaveo.com/xmsng/LICENSE_1_0.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------
#include <boost/signals2.hpp>

//----- Forward declarations ---------------------------------------------------

//----- Namespace declaration --------------------------------------------------
namespace xms
{
//----- Constants / Enumerations -----------------------------------------------

//----- Structs / Classes ------------------------------------------------------

//----- Function prototypes ----------------------------------------------------
void inSetDataIndex(int val);
void inFillWtArray(int id, double wt);

namespace interp
{
namespace connectSet
{
boost::signals2::connection SetDataIndex(
  const boost::signals2::signal<void(int)>::slot_type& a_slot);
boost::signals2::connection FillWtArray(
  const boost::signals2::signal<void(int, double)>::slot_type& a_slot);
}
}

} // namespace xms
