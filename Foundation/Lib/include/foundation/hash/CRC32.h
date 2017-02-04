#pragma once

#include <cstddef>
#include <cstdint>

/**
 * \brief CRC32 implementation.
 */
std::uint32_t crc32Start();
std::uint32_t crc32Update(std::uint32_t currentHash, std::uint8_t data);
std::uint32_t crc32Update(std::uint32_t currentHash, const std::uint8_t *data, std::size_t size);
std::uint32_t crc32End(std::uint32_t currentHash);
