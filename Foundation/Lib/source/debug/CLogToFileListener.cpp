#include "foundation/debug/CLogToFileListener.h"

#include <cassert>

#include "foundation/debug/Log.h"

CLogToFileListener::CLogToFileListener(const std::string &fileName)
{
  m_stream.open(fileName);
  assert(m_stream.is_open());
  if (!m_stream.is_open())
  {
    KERN_WARNING("Failed to open log file " << fileName << " for writing.");
  }
}

void CLogToFileListener::notify(ELogLevel level, const std::string &file,
                                unsigned int line, const std::string &function,
                                const std::string &text)
{
  if (!m_stream)
  {
    return;
  }

  // Write formatted message
  m_stream << toString(level) << " [" << file << ":" << line << ", " << function
           << "]: " << text << std::endl;

  // Flush on warning or error
  if (level == ELogLevel::Warning || level == ELogLevel::Error)
  {
    m_stream << std::flush;
  }
}
