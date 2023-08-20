#pragma once

#include <string>

#include "graphics/resource/SShader.h"

class CResourceManager;

bool load(const std::string &file, CResourceManager &manager, SShader &shader);

bool loadShaderFromIni(const std::string &file, std::string &vertex, std::string &tessControl,
                       std::string &tessEval, std::string &geometry, std::string &fragment);

bool loadShaderFromJson(const std::string &file, std::string &vertex, std::string &tessControl,
                        std::string &tessEval, std::string &geometry, std::string &fragment);