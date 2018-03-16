#pragma once

#include <memory>
#include <mutex>
#include <vector>

#include "foundation/message/AMessage.h"
#include "foundation/thread/AThread.h"

class IMessageHandler;

/**
 * Abstract message handler base class for the async port class.
 */
class AMessageHandlerThread : public AThread
{
   public:
    virtual ~AMessageHandlerThread();

    /**
     * Attach a message handler to the handler thread.
     * This method is thread safe.
     */
    void attachHandler(const std::shared_ptr<IMessageHandler> &handler);

    /**
     * Remove a message handler and calls its onDetach method.
     * This method is thread safe.
     */
    void detachHandler(const std::shared_ptr<IMessageHandler> &handler);

    /**
     * Removes all message handlers and calls their onDetach methods.
     * This method is thread safe.
     */
    void detachAllHandlers();

    /**
     * Blocks until the message has been set to handled state.
     */
    virtual void waitForMessage(const std::shared_ptr<AMessage> &message);

    /**
     * Add a message to be handled.
     * Override in sub class.
     */
    virtual void addMessage(const std::shared_ptr<AMessage> &message) = 0;

    /**
     * Cancels a pending message.
     * Override in sub class.
     */
    virtual void cancelMessage(const std::shared_ptr<AMessage> &message) = 0;

   protected:
    /**
     * Opens active message handlers.
     */
    void openMessageHandlers();

    /**
     * Closes active message handlers.
     */
    void closeMessageHandlers();

    /**
     * Clears remaining deferred messages.
     */
    void clearDeferredMessages();

   private:
    std::vector<std::shared_ptr<IMessageHandler>> m_messageHandlers;
    std::mutex m_messageHandlersMutex;
};
