#pragma once

#include <cstdint>
#include <string>

std::string getFileExtension(const std::string &file);
uint32_t crc32Hash(const std::string &text);
std::string hexdigest(const std::uint8_t *data, std::uint32_t size);
