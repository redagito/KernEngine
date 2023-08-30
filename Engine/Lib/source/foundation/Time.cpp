#include <chrono>

#include "kern/foundation/Time.h"

double getTime()
{
    auto time = std::chrono::high_resolution_clock::now().time_since_epoch();
    double microsec = (double)std::chrono::duration_cast<std::chrono::microseconds>(time).count();
    return microsec / 1000000.0;
}