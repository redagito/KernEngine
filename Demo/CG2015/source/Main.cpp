#include <application/GuardedMain.h>

#include "CDemoCG2015.h"

int main(int argc, const char **argv)
{
    return GuardedMain<CDemoCG2015>(argc, argv, "CG2015", "Computer Graphics Demo Game 2015",
                                    "v1.0", "KernTec");
}
