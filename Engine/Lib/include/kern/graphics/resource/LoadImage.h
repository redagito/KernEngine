#pragma once

#include <string>

#include "kern/graphics/resource/ResourceConfig.h"
#include "kern/graphics/resource/Image.h"

bool load(const std::string &file, EColorFormat format, Image &image);
bool loadPng(const std::string &file, EColorFormat format, Image &image);
bool loadJpeg(const std::string &file, EColorFormat format, Image &image);
