#pragma once

#include <string>

#include "foundation/debug/ELogLevel.h"

/**
 * \brief Log listener interface.
 */
class ILogListener
{
public:
  /**
   * \biref Virtual destructor for interface class.
   */
  virtual ~ILogListener();

  /**
   * \brief Notfies listener of log event.
   */
  virtual void notify(ELogLevel level, const std::string &file,
                      unsigned int line, const std::string &function,
                      const std::string &text) = 0;
};
