#pragma once

#include <string>

#include "kern/resource/ResourceId.h"
#include "kern/resource/ColorFormat.h"
#include "kern/resource/Image.h"

bool load(const std::string &file, Image &image);
bool load(const std::string &file, ColorFormat format, Image &image);
