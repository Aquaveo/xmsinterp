//------------------------------------------------------------------------------
/// \file
/// \ingroup thread
/// \copyright (C) Copyright Aquaveo 2018. Distributed under FreeBSD License
/// (See accompanying file LICENSE or https://aqaveo.com/bsd/license.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------

// 1. Precompiled header

// 2. My own header
#include <xmsinterp/thread/ThreadLoop.h>

// 3. Standard library headers

// 4. External library headers

// 5. Shared code headers

// 6. Non-shared code headers

//----- Forward declarations ---------------------------------------------------

//----- External globals -------------------------------------------------------

//----- Namespace declaration --------------------------------------------------
namespace xms
{
//----- Constants / Enumerations -----------------------------------------------

//----- Classes / Structs ------------------------------------------------------
/// \brief Implementation of ThreadLoop
class ThreadLoop::impl
{
public:
  impl()
  : m_start(-1)
  , m_nIter(-1)
  , m_curIdx(-1)
  , m_cntPtr(nullptr)
  {
  }

  int m_start;   ///< starting index for the thread
  int m_nIter;   ///< number of interations for the thread to perform
  int m_curIdx;  ///< current index the thread is executing
  int* m_cntPtr; ///< pointer to a counter for this thread, updated on each iteration
};

//----- Internal functions -----------------------------------------------------

//----- Class / Function definitions -------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// \class ThreadLoop
/// \brief Class representing a boost thread.
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/// \brief
//------------------------------------------------------------------------------
ThreadLoop::ThreadLoop()
: m_p(new ThreadLoop::impl)
{
} // ThreadLoop::ThreadLoop
//------------------------------------------------------------------------------
/// \brief
//------------------------------------------------------------------------------
ThreadLoop::~ThreadLoop()
{
} // ThreadLoop::~ThreadLoop
//------------------------------------------------------------------------------
/// \brief Sets up the thread worker
/// \param[in] a_startIdx The starting index for this thread
/// \param[in] a_numIter The number of iterations for the thread to execute
/// \param[out] a_cntPtr Pointer to a counter that is updated by each iteration
/// of this thread.
//------------------------------------------------------------------------------
void ThreadLoop::SetStartNumIterCnt(int a_startIdx, int a_numIter, int* a_cntPtr)
{
  m_p->m_start = a_startIdx;
  m_p->m_nIter = a_numIter;
  m_p->m_cntPtr = a_cntPtr;
} // ThreadLoop::SetStartEnd
//------------------------------------------------------------------------------
/// \brief Returns the current index of the thread
/// \return Current index.
//------------------------------------------------------------------------------
int ThreadLoop::CurrIdx()
{
  return m_p->m_curIdx;
} // ThreadLoop::CurrIdx
//------------------------------------------------------------------------------
/// \brief Executes the work of the thread
//------------------------------------------------------------------------------
void ThreadLoop::DoWork()
{
  m_p->m_curIdx = m_p->m_start;
  for (int i = 0; i < m_p->m_nIter; ++i, ++m_p->m_curIdx, ++(*m_p->m_cntPtr))
  {
    Worker();
  }
} // ThreadLoop::operator()

} // namespace xms
