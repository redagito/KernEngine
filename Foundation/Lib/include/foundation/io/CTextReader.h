#pragma once

#include <istream>
#include <memory>
#include <string>
#include <vector>

/**
 * Provides wrapper for reading text from a stream.
 */
class CTextReader
{
   public:
    CTextReader(const std::shared_ptr<std::istream> &stream);

    bool readChar(char &c);
    bool readLine(std::string &line);
    bool readLines(std::vector<std::string> &lines);
    bool readAll(std::string &text);

   private:
    std::shared_ptr<std::istream> m_stream;
};
