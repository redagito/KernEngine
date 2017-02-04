#pragma once

#include <cstdint>
#include <string>

/**
 * \brief Utility function for CRC32 calculation.
 *
 * Calculates CRC32 hash for a block of data.
 */
std::uint32_t crc32Calculate(const std::uint8_t *data, std::size_t size);

/**
 * Calculates CRC32 hash for a string.
 */
std::uint32_t crc32Calculate(const std::string &data);
