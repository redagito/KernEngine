#pragma once

#include <string>

#include "SMesh.h"

bool load(const std::string &file, SMesh &mesh);
bool loadMeshFromObj(const std::string &file, SMesh &mesh);