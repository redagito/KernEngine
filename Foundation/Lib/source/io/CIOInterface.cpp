#include "foundation/io/CIOInterface.h"

#include <memory>

#include "foundation/message/CBlockingMessageHandlerThread.h"
#include "foundation/io/CIOInterfaceHandler.h"

void CIOInterface::open()
{
  // Create and setup handler thread
  auto handlerThread = std::make_shared<CBlockingMessageHandlerThread>();
  handlerThread->setName("IO interface thread");
  // Add IO handler to handler thread
  handlerThread->attachHandler(std::make_shared<CIOInterfaceHandler>());
  // Set handler thread for interface
  setHandlerThread(handlerThread);

  // Call base
  AInterface::open();
}