#include <memory>

#include <fmtlog/fmtlog.h>

#include "Engine.h"

int main(int argc, const char **argv)
{
    fmtlog::startPollingThread();

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
