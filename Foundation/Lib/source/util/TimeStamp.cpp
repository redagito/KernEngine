#include <cstdio>
#include <cstring>
#include <ctime>
#include <string>

#include "foundation/util/TimeStamp.h"

std::string createTimeStamp()
{
    // Create timestamp
    time_t rawtime;
    struct tm *timeinfo;
    char buffer[40];
    memset(buffer, 0, sizeof(buffer));

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(buffer, sizeof(buffer), "%Y%m%d_%H%M%S", timeinfo);

    // Create log file name
    return std::string(buffer);
}
