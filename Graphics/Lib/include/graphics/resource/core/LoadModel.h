#pragma once

#include <string>

#include "graphics/resource/core/SModel.h"
#include "graphics/resource//core/CResourceManager.h"

bool load(const std::string &file, CResourceManager &manager, SModel &model);
bool loadModelFromJson(const std::string &file, std::string &meshFile,
                       std::string &materialFile);