#pragma once
//------------------------------------------------------------------------------
/// \file
/// \ingroup triangulate
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
#include <xmscore/stl/vector.h>
#include <xmscore/misc/base_macros.h> // for XM_DISALLOW_COPY_AND_ASSIGN

namespace xms
{
//----- Forward declarations ---------------------------------------------------

class Observer;
class TrTin;

////////////////////////////////////////////////////////////////////////////////
class TrBreaklineAdder
{
public:
  static boost::shared_ptr<TrBreaklineAdder> New();
  /// \cond

  virtual void SetObserver(boost::shared_ptr<Observer> a) = 0;
  virtual void SetTin(boost::shared_ptr<TrTin> a_tin, double a_tol = -1) = 0;
  virtual void AddBreakline(const VecInt& a_line) = 0;
  virtual void AddBreaklines(const VecInt2d& a_line) = 0;
  virtual std::string ErrorMessage(int) const = 0;

  /// \endcond
protected:
  TrBreaklineAdder();
  virtual ~TrBreaklineAdder();

private:
  XM_DISALLOW_COPY_AND_ASSIGN(TrBreaklineAdder)

}; // class TrBreaklineAdder

} // namespace xms
