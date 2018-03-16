#pragma once

#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <mutex>

/**
 * C++11 Semaphore implementation.
 */
class CSemaphore
{
   public:
    /**
     * Disallow copying.
     */
    CSemaphore(const CSemaphore &sem) = delete;
    CSemaphore &operator=(const CSemaphore &sem) = delete;

    /**
     * Notify the semaphore, increases couunt by one.
     */
    void notify();

    /**
     * Blocks and waits for notification.
     */
    void wait();

    /**
     * Checks if notifcation is available.
     */
    bool peek();

    /**
     * Blocks and waits for notification or until timeout.
     */
    template <class Rep, class Period>
    bool waitFor(const std::chrono::duration<Rep, Period> &timeout);

   private:
    uint64_t m_count = 0;
    std::mutex m_countMutex;
    std::condition_variable m_conditionVariable;
};

template <class Rep, class Period>
bool CSemaphore::waitFor(const std::chrono::duration<Rep, Period> &timeout)
{
    std::unique_lock<std::mutex> ul(m_countMutex);
    bool done = m_conditionVariable.wait_for(ul, timeout, [&] { return m_count > 0; });

    if (done)
    {
        // Wait finished, consumes notification
        --m_count;
    }
    return done;
}