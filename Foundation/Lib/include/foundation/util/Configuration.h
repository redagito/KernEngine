#pragma once

#include <unordered_map>

#include "foundation/util/CValue.h"

/**
 * Stores generic values with string key.
 */
using Configuration = std::unordered_map<std::string, CValue>;

/**
 * Parse configuration from command line arguments.
 * Does not clear the map but overwrites existing entries.
 */
bool parseCommandLine(int argc, const char **argv, Configuration &config);