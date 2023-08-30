#include <fmtlog/fmtlog.h>

#include "RTRDemo.h"

int main(int argc, const char** argv)
{
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