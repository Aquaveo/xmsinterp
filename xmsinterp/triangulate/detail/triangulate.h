#pragma once
//------------------------------------------------------------------------------
/// \file
/// \brief Code that creates a Delauney triangulation from points.
/// \ingroup triangulate_detail
/// \copyright (C) Copyright Aquaveo 2018. Distributed under FreeBSD License
/// (See accompanying file LICENSE or https://aqaveo.com/bsd/license.txt)
//------------------------------------------------------------------------------

//----- Include Files ----------------------------------------------------------

// 3. Standard Library Headers

// 4. External Library Headers

// 5. Shared Headers

// 6. Non-shared Headers

//----- Namespace declaration --------------------------------------------------

namespace xms
{
//----- Constants / Enumerations -----------------------------------------------

//----- Forward Declarations ---------------------------------------------------

class TrTriangulator;

//----- Typdefs / Classes ------------------------------------------------------

//----- Function Prototypes ----------------------------------------------------

bool trTriangulateIt(TrTriangulator& a_Client);

} // namespace xms
