#include "foundation/thread/CSemaphore.h"

void CSemaphore::notify()
{
  std::lock_guard<std::mutex> lg(m_countMutex);
  ++m_count;
  // TODO Ok to notify after releasing lock?
  m_conditionVariable.notify_one();
}

void CSemaphore::wait()
{
  std::unique_lock<std::mutex> ul(m_countMutex);
  m_conditionVariable.wait(ul, [&]
                           {
                             return m_count > 0;
                           });
  --m_count;
}

bool CSemaphore::peek()
{
  std::lock_guard<std::mutex> lg(m_countMutex);
  if (m_count > 0)
  {
    --m_count;
    return true;
  }
  return false;
}