//------------------------------------------------------------------------------
/// \file
/// \ingroup thread
/// \copyright (C) Copyright Aquaveo 2018. Distributed under the xmsng
///  Software License, Version 1.0. (See accompanying file
///  LICENSE_1_0.txt or copy at http://www.aquaveo.com/xmsng/LICENSE_1_0.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------

// 1. Precompiled header

// 2. My own header
#include <xmsinterp/thread/ThreadMgr.h>

// 3. Standard library headers

// 4. External library headers
#include <boost/thread.hpp>

// 5. Shared code headers
#include <xmscore/stl/vector.h>
#include <xmscore/misc/Observer.h>
#include <xmsinterp/thread/ThreadLoop.h>

// 6. Non-shared code headers

//----- Forward declarations ---------------------------------------------------

//----- External globals -------------------------------------------------------

//----- Namespace declaration --------------------------------------------------
namespace xms
{
//----- Constants / Enumerations -----------------------------------------------

//----- Classes / Structs ------------------------------------------------------
/// \brief Implmentation of ThreadMgr
class ThreadMgrImpl : public ThreadMgr
{
public:
  /// \brief convenience struct for threads
  struct myThread
  {
    /// \brief Constructor
    /// \param[in] a_ Thread worker class
    myThread(BSHP<ThreadLoop> a_)
    : m_(a_)
    {
    }

    void operator()();   ///< worker code that is executed
    BSHP<ThreadLoop> m_; ///< Worker thread
  };

  /// \brief Constructor
  ThreadMgrImpl()
  : m_prog()
  , m_nThread(-1)
  , m_nIter(-1)
  {
  }

  //------------------------------------------------------------------------------
  /// \brief
  //------------------------------------------------------------------------------
  void SetThreadLoopClass(BSHP<ThreadLoop> a_) override { m_threader = a_; }
  //------------------------------------------------------------------------------
  /// \param a_nThreads: number of threads.
  /// \brief
  //------------------------------------------------------------------------------
  void ExplicitlySetNumThreads(int a_nThreads) override { m_nThread = a_nThreads; }
  void RunThreads(int a_nIter) override;
  //------------------------------------------------------------------------------
  /// \param a_prog: An observer
  /// \brief
  //------------------------------------------------------------------------------
  void SetObserver(BSHP<Observer> a_prog) override { m_prog = a_prog; }
  std::vector<BSHP<ThreadLoop>> Threads();

  void SetupThreads();
  void MonitorThreads();
  void FinishThreads();

  BSHP<ThreadLoop> m_threader; ///< the worker class that is duplicated to run in multiple threads
  BSHP<Observer> m_prog;       ///< observes the threads and gives feedback of % complete
  int m_nThread;               ///< number of threads running
  int m_nIter;                 ///< total number of iterations for all threads combined
  VecInt m_beg;                ///< starting index for each thread
  VecInt m_thrdNumIter;        ///< number of iterations for each thread
  VecInt m_counts;             ///< number of completed iterations for each thread
  std::vector<myThread> m_vMyThread;     ///< vector of created threads
  std::vector<boost::thread> m_vThreads; ///< actual boost threads that are run
};
//----- Internal functions -----------------------------------------------------

//----- Class / Function definitions -------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// \class ThreadMgrImpl
/// \brief Class to manage multiple threads using boost.
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/// \brief
//------------------------------------------------------------------------------
void ThreadMgrImpl::myThread::operator()()
{
  m_->DoWork();
} // InterpIdwImpl::myThread::operator
//------------------------------------------------------------------------------
/// \brief Creates a ThreadMgr
/// \return shared pointer to ThreadMgr
//------------------------------------------------------------------------------
BSHP<ThreadMgr> ThreadMgr::New()
{
  BSHP<ThreadMgr> ptr(new ThreadMgrImpl());
  return ptr;
} // ThreadMgr::New
//------------------------------------------------------------------------------
/// \brief
//------------------------------------------------------------------------------
ThreadMgr::ThreadMgr()
{
} // ThreadMgr::ThreadMgr
//------------------------------------------------------------------------------
/// \brief
//------------------------------------------------------------------------------
ThreadMgr::~ThreadMgr()
{
} // ThreadMgr::~ThreadMgr
  //------------------------------------------------------------------------------
  /// \brief Gets the threads managed by this class
  /// \return A Vector of threads managed by this class
  //------------------------------------------------------------------------------
std::vector<BSHP<ThreadLoop>> ThreadMgrImpl::Threads()
{
  std::vector<BSHP<ThreadLoop>> threads;
  for (size_t i = 0; i < m_vMyThread.size(); ++i)
  {
    threads.push_back(m_vMyThread[i].m_);
  }
  return threads;
} // ThreadMgrImpl::Threads
//------------------------------------------------------------------------------
/// \brief Executes the threads
/// \param[in] a_nIter: Total number of iterations for the threads to execute.
//------------------------------------------------------------------------------
void ThreadMgrImpl::RunThreads(int a_nIter)
{
  m_nIter = a_nIter;
  if (m_nThread < 1)
    m_nThread = boost::thread::hardware_concurrency() - 1;
  if (m_nThread < 1)
    m_nThread = 1;
  SetupThreads();
  MonitorThreads();
  FinishThreads();
} // ThreadMgrImpl::RunThreads
//------------------------------------------------------------------------------
/// \brief Sets up the threads
//------------------------------------------------------------------------------
void ThreadMgrImpl::SetupThreads()
{
  m_beg.assign(m_nThread, 0);
  m_counts.assign(m_nThread, 0);

  int cnt = (m_nIter / m_nThread) + 1;
  m_thrdNumIter.assign(m_nThread, cnt);
  for (int i = 1; i < m_nThread; ++i)
    m_beg[i] = m_beg[i - 1] + cnt;
  m_thrdNumIter.back() = m_nIter - m_beg.back();

  for (int i = 0; i < m_nThread; ++i)
  {
    BSHP<ThreadLoop> ptr = m_threader->CreateForNewThread();
    ptr->SetStartNumIterCnt(m_beg[i], m_thrdNumIter[i], &m_counts[i]);
    m_vMyThread.push_back(myThread(ptr));
  }
  // create the threads
  for (int i = 0; i < m_nThread; ++i)
  {
    m_vThreads.push_back(boost::thread(m_vMyThread[i]));
  }
} // ThreadMgrImpl::SetupThreads
//------------------------------------------------------------------------------
/// \brief Monitors the progress of all the threads
//------------------------------------------------------------------------------
void ThreadMgrImpl::MonitorThreads()
{
  double p(0.01);
  if (m_prog)
    m_prog->ProgressStatus(p);

  bool done(false);
  int sum, cnt, cntBase;
  cntBase = cnt = m_nIter / 100;
  while (!done)
  {
    sum = 0;
    for (size_t i = 0; i < m_counts.size(); ++i)
      sum += m_counts[i];
    while (sum >= cnt && p < 1)
    {
      p += .01;
      if (m_prog)
        m_prog->ProgressStatus(p);
      cnt += cntBase;
    }
    boost::this_thread::sleep(boost::posix_time::millisec(100));
    if (p >= 1)
      done = true;
  }
} // ThreadMgrImpl::MonitorThreads
//------------------------------------------------------------------------------
/// \brief When all iterations are complete this terminates the threads
//------------------------------------------------------------------------------
void ThreadMgrImpl::FinishThreads()
{
  for (size_t i = 0; i < m_vThreads.size(); ++i)
    m_vThreads[i].join();
} // ThreadMgrImpl::FinishThreads

} // namespace xms