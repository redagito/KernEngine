#pragma once

#include <string>

#include "kern/graphics/resource/ResourceManager.h"
#include "kern/graphics/resource/SModel.h"

bool load(const std::string &file, ResourceManager &manager, SModel &model);
bool loadModelFromJson(const std::string &file, std::string &meshFile, std::string &materialFile);