#pragma once

#include "foundation/debug/CProfiler.h"

// Default enabled
#define KERN_PROFILE

/**
 * Profiling macro
 */
#ifdef KERN_PROFILE
#define PROFILE_START(x) CProfiler x_profiler(#x)
#else
#define PROFILE_START(x)
#endif
