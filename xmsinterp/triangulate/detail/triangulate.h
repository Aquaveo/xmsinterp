#pragma once
//------------------------------------------------------------------------------
/// \file
/// \brief Code that creates a Delauney triangulation from points.
/// \ingroup triangulate_detail
/// \copyright (C) Copyright Aquaveo 2018. Distributed under the xmsng
///  Software License, Version 1.0. (See accompanying file
///  LICENSE_1_0.txt or copy at http://www.aquaveo.com/xmsng/LICENSE_1_0.txt)
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
