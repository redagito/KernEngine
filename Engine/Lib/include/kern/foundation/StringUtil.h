#pragma once

#include <cstdint>
#include <string>

// Extension of the file without the dot
// Empty if theres no extension
// Basically returns everything after the last dot
std::string getFileExtension(const std::string &file);
