#include "kern/foundation/StringUtil.h"

#include <ios>
#include <sstream>

std::string getFileExtension(const std::string &file)
{
    // Get last dot
    std::size_t dashPos = file.find_last_of('/');
    std::size_t pos;

    if (dashPos != std::string::npos)
    {
        pos = file.find_last_of('.');
        if (pos < dashPos)
        {
            pos = std::string::npos;
        }
    }
    else
    {
        pos = file.find_last_of('.');
    }

    if (pos == std::string::npos)
    {
        return "";
    }

    // Retrieve file extension
    std::string extension = file.substr(pos + 1);
    return extension;
}