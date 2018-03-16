#pragma once

#include <cstring>
#include <fstream>
#include <list>
#include <mutex>
#include <sstream>
#include <string>

#include "foundation/debug/ELogLevel.h"

class ILogListener;

/**
 * \brief Logger, prints log statements to console and, if set, to a valid log
 * file.
 */
class CLogger
{
   public:
    /**
     * Sends log message to all listeners.
     */
    static void log(ELogLevel level, const char *file, int line, const char *function,
                    const std::string &text);

    /**
     * Temporary logger helper class.
     * Collects log message and sends it to central logger on destruction.
     */
    class CTempLogger
    {
       public:
        CTempLogger(ELogLevel level, const char *file, int line, const char *function)
            : m_level(level), m_line(line), m_function(function)
        {
            // Stripped file name
            m_file = strrchr(file, '\\') ? strrchr(file, '\\') + 1 : file;
        }

        ~CTempLogger() { CLogger::log(m_level, m_file, m_line, m_function, m_ss.str()); }

        std::ostream &getStream() { return m_ss; }

       private:
        ELogLevel m_level = ELogLevel::Info;
        const char *m_file;
        int m_line = 0;
        const char *m_function;
        std::stringstream m_ss;
    };

    /**
     * \brief Logs text.
     * Deprecated.
     */
    static void log(ELogLevel level, const char *file, int line, const char *function,
                    const char *format, ...);

    /**
     * \brief Sets logging output to specified log file.
     */
    static bool initLogFile(const std::string &logFile);

    static void addListener(ILogListener *listener);
    static void removeListener(ILogListener *listener);

   private:
    static std::ofstream s_stream; /**< Log file stream. */

    // TODO make access to s_listeners thread safe?
    static std::list<ILogListener *> s_listeners;
    static std::mutex s_listenersMutex;
};
