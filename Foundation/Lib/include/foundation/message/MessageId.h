#pragma once

/**
 * Message identifier.
 */
class MessageId
{
public:
  MessageId();
  /**
   * Compares by actual memory address to ensure uniqueness.
   */
  bool operator==(const MessageId &rhs) const;
};
