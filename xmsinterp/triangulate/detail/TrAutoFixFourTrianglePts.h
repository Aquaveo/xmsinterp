#pragma once
//------------------------------------------------------------------------------
/// \file
/// \ingroup triangulate_detail
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
#include <xmscore/stl/vector.h>         // VecInt
#include <xmscore/misc/base_macros.h>   // for XM_DISALLOW_COPY_AND_ASSIGN

//----- Forward declarations ---------------------------------------------------

//----- Namespace declaration --------------------------------------------------

namespace xms
{
class TrTin;
class Observer;

//----- Constants / Enumerations -----------------------------------------------

//----- Structs / Classes ------------------------------------------------------

//----- Function prototypes ----------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
class TrAutoFixFourTrianglePts
{
public:
  static BSHP<TrAutoFixFourTrianglePts> New();
  /// \cond

  virtual void Fix(BSHP<TrTin> a_tin) = 0;
  virtual void SetObserver(BSHP<Observer> a) = 0;
  virtual void SetUndeleteablePtIdxs(VecInt& a_ptIdx) = 0;

  /// \endcond
protected:
  TrAutoFixFourTrianglePts();
  virtual ~TrAutoFixFourTrianglePts();

private:
  XM_DISALLOW_COPY_AND_ASSIGN(TrAutoFixFourTrianglePts)
}; // TrAutoFixFourTrianglePts

} // namespace xms
