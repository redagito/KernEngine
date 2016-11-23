#include <memory>

#include "Engine.h"

int main(int argc, const char **argv)
{
  // Create engine interface
  std::unique_ptr<IEngine> engine(createEngine());

  // TODO Parse program arguments

  // Initialize from config file
  if (!engine->init("data/startup.ini"))
  {
    return 1;
  }
  // Run engine
  engine->run();
  return 0;
}
