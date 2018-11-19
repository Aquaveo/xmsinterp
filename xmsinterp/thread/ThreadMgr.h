#pragma once
//------------------------------------------------------------------------------
/// \file
/// \ingroup thread
/// \copyright (C) Copyright Aquaveo 2018. Distributed under FreeBSD License
/// (See accompanying file LICENSE or https://aqaveo.com/bsd/license.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------
#include <vector>
#include <xmscore/misc/base_macros.h> // XM_DISALLOW_COPY_AND_ASSIGN
#include <xmscore/misc/boost_defines.h>

//----- Forward declarations ---------------------------------------------------

//----- Namespace declaration --------------------------------------------------
namespace xms
{
class ThreadLoop;
class Observer;

//----- Constants / Enumerations -----------------------------------------------

//----- Structs / Classes ------------------------------------------------------
/// \brief Manages a multi-threaded process
/// \see ThreadMgrImpl
class ThreadMgr
{
public:
  static BSHP<ThreadMgr> New();
  virtual ~ThreadMgr();

  /// class that does the work
  /// \param a_ ThreadLoop class.
  virtual void SetThreadLoopClass(BSHP<ThreadLoop> a_) = 0;
  /// Don't have to call this and will use all processors
  /// \param a_nThreads: Number of threads.
  virtual void ExplicitlySetNumThreads(int a_nThreads) = 0;
  /// Execute the code in multiple threads
  /// \param a_nIter: Total number of iterations for the threads to execute.
  virtual void RunThreads(int a_nIter) = 0;
  /// To provide gui feedback
  /// \param a_prog: Observer.
  virtual void SetObserver(BSHP<Observer> a_prog) = 0;
  /// Vector of threads managed by this class
  /// \return Vector of threads managed by this class
  virtual std::vector<BSHP<ThreadLoop>> Threads() = 0;

protected:
  ThreadMgr(); ///< constructor

private:
  XM_DISALLOW_COPY_AND_ASSIGN(ThreadMgr); ///< prevent copy/assign compiler generated functions
};

//----- Function prototypes ----------------------------------------------------

} // namespace xms
