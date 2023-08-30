#include "RTRDemo.h"

#include <fmtlog/fmtlog.h>

int main(int argc, const char** argv)
{
    fmtlog::startPollingThread();

    RTRDemo demo;

    int result = demo.init("data/demo.ini");
    if (result != 0)
    {
        return result;
    }

    return demo.run();
}