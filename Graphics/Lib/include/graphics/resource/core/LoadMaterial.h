#pragma once

#include <string>

#include "graphics/resource/core/SMaterial.h"

class CResourceManager;

bool load(const std::string &file, CResourceManager &manager, SMaterial &material);

bool loadMaterialFromIni(const std::string &file, std::string &base, std::string &normal,
                         std::string &specular, std::string &glow, std::string &alpha);

bool loadMaterialFromJson(const std::string &file, std::string &base, std::string &normal,
                          std::string &specular, std::string &glow, std::string &alpha);