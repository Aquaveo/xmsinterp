#pragma once
//------------------------------------------------------------------------------
/// \file
/// \ingroup thread
/// \copyright (C) Copyright Aquaveo 2018. Distributed under the xmsng
///  Software License, Version 1.0. (See accompanying file
///  LICENSE_1_0.txt or copy at http://www.aquaveo.com/xmsng/LICENSE_1_0.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------
#include <xmscore/misc/base_macros.h> // XM_DISALLOW_COPY_AND_ASSIGN
#include <xmscore/misc/boost_defines.h>

//----- Forward declarations ---------------------------------------------------

//----- Namespace declaration --------------------------------------------------
namespace xms
{
//----- Constants / Enumerations -----------------------------------------------

//----- Structs / Classes ------------------------------------------------------
/// \brief Thread worker class
/// \see ThreadLoop::impl
class ThreadLoop
{
public:
  ThreadLoop();
  virtual ~ThreadLoop();

  void SetStartNumIterCnt(int a_startIdx, int a_numIter, int* a_cntPtr);
  void DoWork();

  /// \brief Overridden by derived class
  /// \return shared pointer to ThreadLoop
  virtual BSHP<ThreadLoop> CreateForNewThread() = 0;

protected:
  int CurrIdx();

private:
  virtual void Worker() = 0; ///< Overridden by derived class

  class impl;
  BSHP<impl> m_p; ///< implementation class

  XM_DISALLOW_COPY_AND_ASSIGN(ThreadLoop); ///< prevent compiler generated copy/assign
};
//----- Function prototypes ----------------------------------------------------

} // namespace xms
