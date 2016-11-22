#include "foundation/hash/CRC32Util.h"

#include "foundation/hash/CRC32.h"

std::uint32_t crc32Calculate(const std::uint8_t *data, std::uint32_t size)
{
  std::uint32_t hash = crc32Start();
  hash = crc32Update(hash, data, size);
  return crc32End(hash);
}

std::uint32_t crc32Calculate(const std::string &data)
{
  return crc32Calculate(reinterpret_cast<const std::uint8_t *>(data.data()),
                        data.size());
}
