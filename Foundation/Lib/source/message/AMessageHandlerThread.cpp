#include "foundation/message/AMessageHandlerThread.h"

#include <cassert>
#include <algorithm>
#include <thread>

#include "foundation/message/IMessageHandler.h"

AMessageHandlerThread::~AMessageHandlerThread() {}

void AMessageHandlerThread::attachHandler(
    const std::shared_ptr<IMessageHandler> &handler)
{
  assert(handler != nullptr);
  if (handler == nullptr)
  {
    return;
  }
  std::lock_guard<std::mutex> lg(m_messageHandlersMutex);

  // Check if handler already exists
  auto iter =
      std::find(m_messageHandlers.begin(), m_messageHandlers.end(), handler);
  // Fail on debug
  assert(iter == m_messageHandlers.end());
  // Ignore on release
  if (iter != m_messageHandlers.end())
  {
    return;
  }
  handler->onAttach();
  m_messageHandlers.push_back(handler);
}

void AMessageHandlerThread::detachHandler(
    const std::shared_ptr<IMessageHandler> &handler)
{
  assert(handler != nullptr);
  if (handler == nullptr)
  {
    return;
  }
  std::lock_guard<std::mutex> lg(m_messageHandlersMutex);

  // Check if handler exists
  auto iter =
      std::find(m_messageHandlers.begin(), m_messageHandlers.end(), handler);
  // Fail on debug
  assert(iter != m_messageHandlers.end());
  // Ignore on release
  if (iter == m_messageHandlers.end())
  {
    return;
  }
  // Remove handler
  handler->onDetach();
  m_messageHandlers.erase(iter);
  return;
}

void AMessageHandlerThread::detachAllHandlers()
{
  std::lock_guard<std::mutex> lg(m_messageHandlersMutex);
  for (const auto &handler : m_messageHandlers)
  {
    handler->onDetach();
  }
  m_messageHandlers.clear();
}

void AMessageHandlerThread::waitForMessage(
    const std::shared_ptr<AMessage> &message)
{
  // TODO Implement with condition variables?
  while (!message->isHandled())
  {
    std::this_thread::yield();
  }
}

void AMessageHandlerThread::openMessageHandlers()
{
  std::lock_guard<std::mutex> lg(m_messageHandlersMutex);
  for (const auto &handler : m_messageHandlers)
  {
    handler->open();
  }
}

void AMessageHandlerThread::closeMessageHandlers()
{
  std::lock_guard<std::mutex> lg(m_messageHandlersMutex);
  for (const auto &handler : m_messageHandlers)
  {
    handler->close();
  }
}

void AMessageHandlerThread::clearDeferredMessages()
{
  // TODO Implement
}
