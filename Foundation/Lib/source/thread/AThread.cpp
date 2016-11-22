#include "foundation/thread/AThread.h"

#include <cassert>

#include "foundation/debug/Log.h"

AThread::AThread() {}

AThread::~AThread() { stop(); }

void AThread::setName(const std::string &threadName)
{
  std::lock_guard<std::mutex> lg(m_threadNameMutex);
  m_threadName = threadName;
}

std::string AThread::getName() const
{
  std::lock_guard<std::mutex> lg(m_threadNameMutex);
  return m_threadName;
}

void AThread::start()
{
  assert(!m_isRunning);
  if (m_isRunning)
  {
    KERN_WARNING("Thread " << getName() << " already running");
    return;
  }
  m_stopRequested = false;

  m_thread = std::thread{[this]
                         {
                           m_isRunning = true;
                           initializeWork();
                           while (!stopRequested())
                           {
                             doWork();
                           }
                           deinitializeWork();
                           m_isRunning = false;
                         }};
}

void AThread::stop()
{
  m_stopRequested = true;
  if (m_thread.joinable())
  {
    m_thread.join();
  }
  assert(!isRunning());
}

bool AThread::isRunning() const { return m_isRunning; }

bool AThread::stopRequested() const { return m_stopRequested; }
