#include "foundation/message/AMessage.h"

#include <thread>

AMessage::AMessage() {}

AMessage::~AMessage() {}

bool AMessage::isId(const MessageId &id) const { return getId() == id; }

bool AMessage::serialize(CBinaryWriter &writer) const
{
  return writer.write(*this);
}

bool AMessage::deserialize(CBinaryReader &reader) { return false; }

bool AMessage::isHandled() const { return m_handled; }

void AMessage::setHandled(bool handled) { m_handled = handled; }

void AMessage::waitHandled() const
{
  // TODO Implement with condition variables?
  while (!m_handled)
  {
    std::this_thread::yield();
  }
}

bool AMessage::isDeferred() const { return m_deferred; }

void AMessage::setDeferred(bool deferred) { m_deferred = deferred; }

bool AMessage::isDeferredHandled() const { return m_deferredHandled; }

void AMessage::setDeferredHandled(bool deferredHandled)
{
  m_deferredHandled = deferredHandled;
}

bool AMessage::isDistributed() const { return m_distributed; }

void AMessage::setDistributed(bool distributed) { m_distributed = distributed; }

bool serialize(const AMessage &msg, std::ostream &os)
{
  return msg.serialize(os);
}

bool deserialize(AMessage &msg, std::istream &is)
{
  return msg.deserialize(is);
}