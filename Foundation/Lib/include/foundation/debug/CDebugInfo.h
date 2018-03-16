#pragma once

#include <list>
#include <map>
#include <string>

#include "foundation/debug/ILogListener.h"

/**
 * \brief Stores debug data.
 * Stores log events in a ring buffer and key/value debug data pairs.
 */
class CDebugInfo : public ILogListener
{
   public:
    void notify(ELogLevel level, const std::string &file, unsigned int line,
                const std::string &function, const std::string &text) override;

    /**
     * Returns stored logs.
     */
    const std::list<std::string> &getLog() const;

    /**
     * Sets size for the internal log buffer.
     */
    void setLogBufferSize(size_t size);
    size_t getLogBufferSize() const;

    /**
     * Sets a value by key.
     */
    void setValue(const std::string &key, const std::string &value);
    const std::map<std::string, std::string> &getValues() const;

   private:
    size_t m_logBufferSize = 10;                 /**< Maximum number of log events stored. */
    std::list<std::string> m_logBuffer;          /**< Log buffer with latest log events. */
    std::map<std::string, std::string> m_values; /**< Stores debug key/value pairs. */
};
