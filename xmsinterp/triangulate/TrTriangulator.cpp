//------------------------------------------------------------------------------
/// \file
/// \ingroup triangulate
/// \copyright (C) Copyright Aquaveo 2018. Distributed under FreeBSD License
/// (See accompanying file LICENSE or https://aqaveo.com/bsd/license.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------

// 1. Precompiled header

// 2. My own header
#include <xmsinterp/triangulate/TrTriangulator.h>

// 3. Standard library headers

// 4. External library headers

// 5. Shared code headers
#include <xmsinterp/triangulate/detail/triangulate.h>

// 6. Non-shared code headers

//----- Namespace declaration --------------------------------------------------

namespace xms
{
////////////////////////////////////////////////////////////////////////////////
/// \class TrTriangulator
/// \brief Base class used to derive a class to triangulate points.
/// \see TrTriangulatorPoints
/// This class can be used to pass triangle based data to be consumed
/// however the derived class prefers. Sometimes the point information will
/// be provided and will need to be handled by derived classes. Other times
/// (like in triangulation) it is assumed that the point data has already
/// been stored for the target.
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/// \brief Constructor
//------------------------------------------------------------------------------
TrTriangulator::TrTriangulator()
{
}
//------------------------------------------------------------------------------
/// \brief Destructor
//------------------------------------------------------------------------------
TrTriangulator::~TrTriangulator()
{
}
//------------------------------------------------------------------------------
/// \brief Triangulate the points into a tin.
/// \return true on success.
//------------------------------------------------------------------------------
bool TrTriangulator::Triangulate()
{
  return trTriangulateIt(*this);
} // TrTriangulator::Triangulate
} // namespace xms
