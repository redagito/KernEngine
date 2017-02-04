#pragma once

#include <cstddef>
#include <cstdint>

/**
 * CRC32 hasher class.
 */
class CCRC32Hasher
{
   public:
    /**
     * Start new hashing operation.
     */
    void start();

    /**
     * Update current hash.
     */
    void update(const uint8_t *data, std::size_t size);

    /**
     * Finalize and return current hash.
     * Sets internal hash to 0.
     */
    std::uint32_t end();

   private:
    std::uint32_t m_hash = 0;
};
