#pragma once

#include "foundation/interface/AInterface.h"

/**
 * Implements an asynchronous interface for the
 * IO system. Messages are guaranteed to be handled
 * sequentially in FIFO order.
 */
class CIOInterface : public AInterface
{
   public:
    void open() override;
};