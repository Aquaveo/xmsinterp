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

#include <boost/shared_ptr.hpp>

// 5. Shared code headers

#include <xmscore/stl/vector.h>       // for VecInt2d
#include <xmscore/misc/base_macros.h> // for XM_DISALLOW_COPY_AND_ASSIGN

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
class TrOuterTriangleDeleter
{
public:
  static boost::shared_ptr<TrOuterTriangleDeleter> New();
  /// \cond

  virtual void Delete(const VecInt2d& a_polys, boost::shared_ptr<TrTin> a_tin) = 0;
  virtual void SetObserver(boost::shared_ptr<Observer> a) = 0;

  /// \endcond
protected:
  TrOuterTriangleDeleter();
  virtual ~TrOuterTriangleDeleter();

private:
  XM_DISALLOW_COPY_AND_ASSIGN(TrOuterTriangleDeleter)
}; // TrOuterTriangleDeleter

} // namespace xms
