#pragma once

#include <fstream>

#include "foundation/debug/ILogListener.h"

class CLogToFileListener : public ILogListener
{
public:
  CLogToFileListener(const std::string &fileName);

  void notify(ELogLevel level, const std::string &file, unsigned int line,
              const std::string &function, const std::string &text);

private:
  std::ofstream m_stream;
};
