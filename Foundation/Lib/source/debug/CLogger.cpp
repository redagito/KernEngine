#include "foundation/debug/CLogger.h"

#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "foundation/debug/ILogListener.h"
#include "foundation/debug/Log.h"

std::ofstream CLogger::s_stream;
std::list<ILogListener *> CLogger::s_listeners;
std::mutex CLogger::s_listenersMutex;

void CLogger::log(ELogLevel level, const char *file, int line, const char *function,
                  const std::string &text)
{
    std::lock_guard<std::mutex> lg(s_listenersMutex);
    // Print to standard output
    std::cout << toString(level) << ": " << text << std::endl;

    // Print to file
    if (s_stream.is_open())
    {
        // Write detailed error message to log file
        s_stream << toString(level) << " [" << file << ":" << line << ", " << function
                 << "]: " << text << std::endl
                 << std::flush;
    }

    // Passthrough to listeners
    for (ILogListener *l : s_listeners)
    {
        l->notify(level, file, line, function, text);
    }
}

void CLogger::log(ELogLevel level, const char *file, int line, const char *function,
                  const char *format, ...)
{
    // Text buffer
    char buffer[5000];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    std::string text(buffer);
    CLogger::log(level, file, line, function, text);
}

bool CLogger::initLogFile(const std::string &logFile)
{
    if (!s_stream.is_open())
    {
        s_stream.open(logFile);
        if (!s_stream.is_open())
        {
            LOG_WARNING("Failed to open log file %s.", logFile.c_str());
            return false;
        }
        return true;
    }
    LOG_WARNING("Multiple logger initialization calls are invalid and will be ignored.");
    return false;
}

void CLogger::addListener(ILogListener *l)
{
    std::lock_guard<std::mutex> lg(s_listenersMutex);
    s_listeners.push_back(l);
}

void CLogger::removeListener(ILogListener *l)
{
    std::lock_guard<std::mutex> lg(s_listenersMutex);
    s_listeners.remove(l);
}
