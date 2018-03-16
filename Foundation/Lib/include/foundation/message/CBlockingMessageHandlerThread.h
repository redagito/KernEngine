#pragma once

#include <chrono>
#include <condition_variable>
#include <deque>
#include <mutex>

#include "foundation/message/AMessageHandlerThread.h"

/**
 * Message handler class which blocks until messages arrive
 * or an optional timeout occurs.
 */
class CBlockingMessageHandlerThread : public AMessageHandlerThread
{
   public:
    /**
     * Sets wait timeout (set to 0 for infinite timeout).
     */
    void setTimeout(std::chrono::milliseconds timeout);
    std::chrono::milliseconds getTimeout() const;

    void addMessage(const std::shared_ptr<AMessage> &message) override;
    void cancelMessage(const std::shared_ptr<AMessage> &message) override;

   protected:
    void initializeWork() override;
    void doWork() override;
    void deinitializeWork() override;
    bool handleMessages(const std::deque<std::shared_ptr<AMessage>> &messages);

   private:
    std::atomic<std::chrono::milliseconds> m_timeout{std::chrono::milliseconds{0}};
    std::deque<std::shared_ptr<AMessage>> m_messages;
    std::mutex m_messagesMutex;
    std::condition_variable m_messageAdded;
};
