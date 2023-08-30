#pragma once

#include <string>

#include "kern/graphics/resource/SMesh.h"

bool load(const std::string &file, SMesh &mesh);
bool loadMeshFromObj(const std::string &file, SMesh &mesh);