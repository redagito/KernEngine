#pragma once

#include <string>

#include "SModel.h"
#include "CResourceManager.h"

bool load(const std::string &file, CResourceManager &manager, SModel &model);
bool loadModelFromJson(const std::string &file, std::string &meshFile,
                       std::string &materialFile);