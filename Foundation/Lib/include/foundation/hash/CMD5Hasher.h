#pragma once

#include <array>
#include <cstdint>

class CMD5Hasher
{
   public:
    /**
     * Start new hashing operation.
     */
    void start();

    /**
     * Update current hash.
     */
    void update(const uint8_t *data, std::uint32_t size);

    /**
     * Finalize and return current hash.
     * Sets internal hash to 0.
     */
    std::array<std::uint8_t, 16> end();

   private:
    static const int blocksize = 64;

    void transform(const std::uint8_t block[blocksize]);
    static void decode(std::uint32_t output[], const std::uint8_t input[], std::uint32_t len);
    static void encode(std::uint8_t output[], const std::uint32_t input[], std::uint32_t len);

    bool finalized = false;
    std::uint8_t buffer[blocksize];       // bytes that didn't fit in last 64 byte chunk
    std::uint32_t count[2];               // 64bit counter for number of bits (lo, hi)
    std::uint32_t state[4];               // digest so far
    std::array<std::uint8_t, 16> digest;  // the result

    // low level logic operations
    static inline std::uint32_t F(std::uint32_t x, std::uint32_t y, std::uint32_t z);
    static inline std::uint32_t G(std::uint32_t x, std::uint32_t y, std::uint32_t z);
    static inline std::uint32_t H(std::uint32_t x, std::uint32_t y, std::uint32_t z);
    static inline std::uint32_t I(std::uint32_t x, std::uint32_t y, std::uint32_t z);
    static inline std::uint32_t rotate_left(std::uint32_t x, int n);
    static inline void FF(std::uint32_t &a, std::uint32_t b, std::uint32_t c, std::uint32_t d,
                          std::uint32_t x, std::uint32_t s, std::uint32_t ac);
    static inline void GG(std::uint32_t &a, std::uint32_t b, std::uint32_t c, std::uint32_t d,
                          std::uint32_t x, std::uint32_t s, std::uint32_t ac);
    static inline void HH(std::uint32_t &a, std::uint32_t b, std::uint32_t c, std::uint32_t d,
                          std::uint32_t x, std::uint32_t s, std::uint32_t ac);
    static inline void II(std::uint32_t &a, std::uint32_t b, std::uint32_t c, std::uint32_t d,
                          std::uint32_t x, std::uint32_t s, std::uint32_t ac);
};
