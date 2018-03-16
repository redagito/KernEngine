#pragma once

#include <memory>

#include "foundation/message/AAsyncPort.h"

/**
 * Abstract interface class.
 * An interface acts as a message synchronization and -passing
 * layer to a system running in its own thread.
 * An interface is visible to and can receive messages
 * from all active threads.
 * The interface collects and dispatches the messages to
 * specialized message handlers running in the thread context.
 */
class AInterface : public AAsyncPort
{
   public:
    virtual ~AInterface();
    virtual void open() = 0;
};
