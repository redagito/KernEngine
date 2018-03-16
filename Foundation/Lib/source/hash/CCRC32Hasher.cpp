#include "foundation/hash/CCRC32Hasher.h"
#include "foundation/hash/CRC32.h"

void CCRC32Hasher::start() { m_hash = crc32Start(); }

void CCRC32Hasher::update(const uint8_t *data, std::uint32_t size)
{
    m_hash = crc32Update(m_hash, data, size);
}

std::uint32_t CCRC32Hasher::end()
{
    auto hash = crc32End(m_hash);
    m_hash = 0;
    return hash;
}
