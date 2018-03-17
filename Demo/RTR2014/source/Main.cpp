#include <application/GuardedMain.h>

#include "CDemoRTR2014.h"

int main(int argc, const char** argv)
{
	return GuardedMain<CDemoRTR2014>(argc, argv, "RTR2014", "Realtime Rendering Demo 2014", "v1.0", "KernTec");
}