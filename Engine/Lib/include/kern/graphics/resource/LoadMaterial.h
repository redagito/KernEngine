#pragma once

#include <string>

#include "kern/graphics/resource/SMaterial.h"

class ResourceManager;

bool load(const std::string &file, ResourceManager &manager, SMaterial &material);

bool loadMaterialFromIni(const std::string &file, std::string &base, std::string &normal,
                         std::string &specular, std::string &glow, std::string &alpha);

bool loadMaterialFromJson(const std::string &file, std::string &base, std::string &normal,
                          std::string &specular, std::string &glow, std::string &alpha);