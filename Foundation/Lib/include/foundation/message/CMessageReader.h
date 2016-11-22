#pragma once

#include "foundation/io/CBinaryReader.h"
#include "foundation/core/TFactory.h"
#include "foundation/util/CEightCC.h"

#include "foundation/message/AMessage.h"

/**
 * Implements deserialization for decoding messages.
 */
class CMessageReader
{
public:
  using MessageFactory = TFactory<AMessage, CEightCC>;

  CMessageReader(const std::shared_ptr<std::istream> &stream);

  /**
   * Read message from stream.
   */
  AMessage *read();

  /**
   * Register creator by class code.
   */
  static bool registerMessage(const CEightCC &code,
                              const MessageFactory::Creator &creator);

private:
  CBinaryReader m_reader;
  static MessageFactory s_messageFactory;
};
