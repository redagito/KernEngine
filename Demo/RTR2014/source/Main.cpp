#include <fmtlog/fmtlog.h>

#include "RTRDemo.h"

int main(int argc, const char** argv)
{
#ifndef NDEBUG
    fmtlog::setLogLevel(fmtlog::DBG);
#endif
    fmtlog::startPollingThread();

    try
    {
        RTRDemo demo;
        if (demo.init("data/demo.ini") != 0)
            return EXIT_FAILURE;

        return demo.run();
    }
    catch (const std::exception& e)
    {
        loge("{}", e.what());
        return EXIT_FAILURE;
    }
}