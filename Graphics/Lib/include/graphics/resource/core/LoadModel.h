#pragma once

#include <string>

#include "graphics/resource//core/CResourceManager.h"
#include "graphics/resource/core/SModel.h"

bool load(const std::string &file, CResourceManager &manager, SModel &model);
bool loadModelFromJson(const std::string &file, std::string &meshFile, std::string &materialFile);