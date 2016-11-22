#include "foundation/util/StringUtil.h"
#include "foundation/hash/CRC32Util.h"

#include <sstream>
#include <ios>

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

uint32_t crc32Hash(const std::string &text) { return crc32Calculate(text); }

std::string hexdigest(const std::uint8_t *data, std::uint32_t size)
{
  std::stringstream ss;
  ss << std::hex;
  for (std::uint32_t i = 0; i < size; ++i)
  {
    ss << data[i];
  }
  return ss.str();
}
