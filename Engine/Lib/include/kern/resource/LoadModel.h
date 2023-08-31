#pragma once

#include <string>

#include "kern/resource/ResourceManager.h"
#include "kern/resource/SModel.h"

bool load(const std::string &file, ResourceManager &manager, SModel &model);
bool loadModelFromJson(const std::string &file, std::string &meshFile, std::string &materialFile);