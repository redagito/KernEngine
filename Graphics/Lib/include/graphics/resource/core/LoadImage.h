#pragma once

#include <string>

#include "resource/ResourceConfig.h"
#include "SImage.h"

bool load(const std::string &file, EColorFormat format, SImage &image);
bool loadPng(const std::string &file, EColorFormat format, SImage &image);
bool loadJpeg(const std::string &file, EColorFormat format, SImage &image);
