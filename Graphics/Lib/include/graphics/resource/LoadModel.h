#pragma once

#include <string>

#include "graphics/resource/CResourceManager.h"
#include "graphics/resource/SModel.h"

bool load(const std::string &file, CResourceManager &manager, SModel &model);
bool loadModelFromJson(const std::string &file, std::string &meshFile, std::string &materialFile);