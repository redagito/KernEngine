#include "foundation/debug/ELogLevel.h"

std::string toString(ELogLevel level)
{
  switch (level)
  {
  case ELogLevel::Info:
    return "Info";

  case ELogLevel::Debug:
    return "Debug";

  case ELogLevel::Warning:
    return "Warning";

  case ELogLevel::Error:
    return "Error";

  default:
    return "Unknown";
  }
}
