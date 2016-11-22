#pragma once

#include "foundation/message/AMessage.h"
#include "foundation/message/IMessageHandler.h"
#include "foundation/message/AMessageHandlerThread.h"

/**
 * The async port class runs message handlers in a separate
 * thread for asynchronous message processing.
 */
class AAsyncPort
{
public:
  virtual ~AAsyncPort();

  /**
  * Sets handler thread object for internal message processing.
  */
  void
  setHandlerThread(const std::shared_ptr<AMessageHandlerThread> &handlerThread);

  /**
  * Returns currently set handler thread.
  */
  const std::shared_ptr<AMessageHandlerThread> &getHandlerThread() const;

  /**
  * Attaches a message handler to the port.
  */
  void attachHandler(const std::shared_ptr<IMessageHandler> &handler);

  /**
  * Detaches a previously attached message handler.
  */
  void detachHandler(const std::shared_ptr<IMessageHandler> &handler);

  /**
  * Opens the port to receive messages.
  */
  virtual void open();

  /**
  * Closes the port.
  */
  virtual void close();

  /**
  * Returns open state of the port.
  */
  bool isOpen() const;

  /**
  * Send a message for handling.
  */
  void send(const std::shared_ptr<AMessage> &message);

  /**
  * Cancel a previously sent message.
  */
  void cancel(const std::shared_ptr<AMessage> &message);

private:
  std::shared_ptr<AMessageHandlerThread> m_handlerThread;
  bool m_open = false;
};
