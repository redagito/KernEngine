#pragma once

#include <string>

#include "kern/resource/SShader.h"
#include "kern/resource/ResourceManager.h"

class ResourceManager;

bool load(const std::string &file, ResourceManager &manager, SShader &shader);

bool loadShaderFromIni(const std::string &file, std::string &vertex, std::string &tessControl,
                       std::string &tessEval, std::string &geometry, std::string &fragment);

bool loadShaderFromJson(const std::string &file, std::string &vertex, std::string &tessControl,
                        std::string &tessEval, std::string &geometry, std::string &fragment);