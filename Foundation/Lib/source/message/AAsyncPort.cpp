#include "foundation/message/AAsyncPort.h"

#include <cassert>

AAsyncPort::~AAsyncPort() {}

void AAsyncPort::setHandlerThread(
    const std::shared_ptr<AMessageHandlerThread> &handlerThread)
{
  m_handlerThread = handlerThread;
}

const std::shared_ptr<AMessageHandlerThread> &
AAsyncPort::getHandlerThread() const
{
  return m_handlerThread;
}

void AAsyncPort::attachHandler(const std::shared_ptr<IMessageHandler> &handler)
{
  m_handlerThread->attachHandler(handler);
}

void AAsyncPort::detachHandler(const std::shared_ptr<IMessageHandler> &handler)
{
  m_handlerThread->detachHandler(handler);
}

void AAsyncPort::open()
{
  assert(!m_open);
  assert(m_handlerThread != nullptr);
  // m_handlerThread->
}
