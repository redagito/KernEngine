#include <fmtlog/fmtlog.h>

#include "Engine.h"

int main(int argc, const char** argv)
{
#ifndef NDEBUG
    fmtlog::setLogLevel(fmtlog::DBG);
#endif
    fmtlog::startPollingThread();

    try
    {
        // Create engine interface
        Engine engine;

        // Initialize from config file
        if (!engine.init("data/startup.ini"))
        {
            return EXIT_FAILURE;
        }
        // Run engine
        engine.run();

        return EXIT_SUCCESS;
    }
    catch (const std::exception& e)
    {
        loge("{}", e.what());
        return EXIT_FAILURE;
    }
}
