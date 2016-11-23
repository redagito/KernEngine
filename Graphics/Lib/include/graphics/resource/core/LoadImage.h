#pragma once

#include <string>

#include "graphics/resource/ResourceConfig.h"
#include "graphics/resource/core/SImage.h"

bool load(const std::string &file, EColorFormat format, SImage &image);
bool loadPng(const std::string &file, EColorFormat format, SImage &image);
bool loadJpeg(const std::string &file, EColorFormat format, SImage &image);
