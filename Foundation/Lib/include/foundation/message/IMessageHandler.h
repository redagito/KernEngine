#pragma once

#include "foundation/message/AMessage.h"

class IMessageHandler
{
   public:
    virtual ~IMessageHandler();

    /**
     * Called on attach to message handler thread.
     */
    virtual void onAttach() = 0;

    /**
     * Called on detach from message handler thread.
     */
    virtual void onDetach() = 0;

    /**
     * Handle message.
     * \return True on success, false otherwise.
     */
    virtual bool handle(const std::shared_ptr<AMessage> &msg) = 0;

    /**
     * Opens the message handler.
     * Called from thread context.
     */
    virtual void open() = 0;

    /**
     * Optional update method.
     * Called once per frame from thread context for continuous handlers
     */
    virtual void update() = 0;

    /**
     * Closes the message handler.
     * Called from thread context.
     */
    virtual void close() = 0;
};
