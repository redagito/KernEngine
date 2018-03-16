#pragma once

#include <atomic>
#include <mutex>
#include <string>
#include <thread>

/**
 * Abstract thread class.
 */
class AThread
{
   public:
    AThread();
    virtual ~AThread();

    /**
     * Thread name access (threadsafe).
     */
    void setName(const std::string &threadName);
    std::string getName() const;

    void start();
    void stop();
    bool isRunning() const;

   protected:
    /**
     * Returns true if a stop has been requested.
     */
    bool stopRequested() const;

    /**
     * Work initialization function.
     * Called once on thread start.
     * Runs in thread context.
     */
    virtual void initializeWork() = 0;
    /**
     * Worker function, called continuously in loop.
     * Runs in thread context.
     */
    virtual void doWork() = 0;

    /**
     * Work deinitialization function.
     * Called once on thread stop.
     * Runs in thread context.
     */
    virtual void deinitializeWork() = 0;

   private:
    std::string m_threadName;
    mutable std::mutex m_threadNameMutex;
    std::atomic_bool m_isRunning{false};
    std::atomic_bool m_stopRequested{false};
    // Worker thread
    std::thread m_thread;
};
