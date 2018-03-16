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

// 1. Precompiled header

// 2. My header
#include <xmsinterp/interpolate/InterpSignals.h>

// 3. Standard Library Headers

// 4. External Library Headers

// 5. Shared Headers

// 6. Non-shared Headers

//----- Forward declarations ---------------------------------------------------

//----- External globals -------------------------------------------------------

//----- Namespace declaration --------------------------------------------------
namespace xms
{
//----- Constants / Enumerations -----------------------------------------------

//----- Classes / Structs ------------------------------------------------------

//----- Internal functions -----------------------------------------------------
namespace interp
{
namespace connectGet
{
boost::signals2::signal<void(int)>& SetDataIndex();
boost::signals2::signal<void(int, double)>& FillWtArray();
}
}

//----- Class / Function definitions -------------------------------------------

//------------------------------------------------------------------------------
/// \brief static file variable
//------------------------------------------------------------------------------
boost::signals2::signal<void(int)>& interp::connectGet::SetDataIndex()
{
  static boost::signals2::signal<void(int)> con;
  return con;
} // interp::connectGet::SetDataIndex
//------------------------------------------------------------------------------
/// \brief static file variable
//------------------------------------------------------------------------------
boost::signals2::signal<void(int, double)>& interp::connectGet::FillWtArray()
{
  static boost::signals2::signal<void(int, double)> con;
  return con;
} // interp::connectGet::SetDataIndex
//------------------------------------------------------------------------------
/// \brief Connects to this signal
/// \param[in] a_slot: A slot.
/// \return connection.
//------------------------------------------------------------------------------
boost::signals2::connection interp::connectSet::SetDataIndex(
  const boost::signals2::signal<void(int)>::slot_type& a_slot)
{
  return (interp::connectGet::SetDataIndex().connect(a_slot));
} // interp::connectSet::SetDataIndex
//------------------------------------------------------------------------------
/// \brief Connects to this signal
/// \param[in] a_slot: A slot.
/// \return connection.
//------------------------------------------------------------------------------
boost::signals2::connection interp::connectSet::FillWtArray(
  const boost::signals2::signal<void(int, double)>::slot_type& a_slot)
{
  return (interp::connectGet::FillWtArray().connect(a_slot));
} // interp::connectSet::FillWtArray
//------------------------------------------------------------------------------
/// \brief Calls the SetDataIndex signal
/// \param[in] val: Value.
//------------------------------------------------------------------------------
void inSetDataIndex(int val)
{
  interp::connectGet::SetDataIndex()(val);
} // inSetDataIndex
//------------------------------------------------------------------------------
/// \brief  calls the FillWtArray signal
/// \param id: ID.
/// \param wt: Weight.
//------------------------------------------------------------------------------
void inFillWtArray(int id, double wt)
{
  interp::connectGet::FillWtArray()(id, wt);
} // inFillWtArray

} // namespace xms
