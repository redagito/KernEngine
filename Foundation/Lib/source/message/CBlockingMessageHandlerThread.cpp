#include "foundation/message/CBlockingMessageHandlerThread.h"

#include <algorithm>

void CBlockingMessageHandlerThread::addMessage(
    const std::shared_ptr<AMessage> &message)
{
  {
    std::lock_guard<std::mutex> lg(m_messagesMutex);
    m_messages.push_front(message);
  }
  m_messageAdded.notify_one();
}

void CBlockingMessageHandlerThread::cancelMessage(
    const std::shared_ptr<AMessage> &message)
{
  std::lock_guard<std::mutex> lg(m_messagesMutex);
  // Remove-erase idiom
  m_messages.erase(std::remove(m_messages.begin(), m_messages.end(), message),
                   m_messages.end());
}

void CBlockingMessageHandlerThread::initializeWork() { openMessageHandlers(); }

void CBlockingMessageHandlerThread::doWork()
{
  std::chrono::milliseconds timeout = m_timeout;

  // Wait for next message or timeout
  std::unique_lock<std::mutex> ul(m_messagesMutex);
  if (timeout > std::chrono::milliseconds{0})
  {
    // Wait for message or until timout
    m_messageAdded.wait_for(ul, timeout, [this]
                            {
                              return !m_messages.empty();
                            });
  }
  else
  {
    // Wait for message
    m_messageAdded.wait(ul, [this]
                        {
                          return !m_messages.empty();
                        });
  }

  // Stores if at least one message has been handled
  bool messageHandled = false;

  // Check messages available
  if (!m_messages.empty())
  {
    // Process messages
    // Swap and clear
    std::deque<std::shared_ptr<AMessage>> messages;
    messages.swap(m_messages);
    ul.unlock();

    // Handle messages in queue
    handleMessages(messages);
  }
  else
  {
    // No messages, unlock immediately
    ul.unlock();
  }
  // Update state of deferred messages
}

void CBlockingMessageHandlerThread::deinitializeWork()
{
  clearDeferredMessages();
  closeMessageHandlers();
}

bool CBlockingMessageHandlerThread::handleMessages(
    const std::deque<std::shared_ptr<AMessage>> &messages)
{
  return false;
}
