#include "CDemoRTR2014.h"

int main(int argc, const char** argv)
{
	CDemoRTR2014 app;

    if (app.init("config/startup.ini") != 0)
    {
        return 1;
    }

    return app.run();
}