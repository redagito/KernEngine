#pragma once

#include <string>

/**
 * Supported log levels.
 */
enum class ELogLevel
{
  Info,
  Debug,
  Warning,
  Error
};

std::string toString(ELogLevel level);
