#pragma once

#include <string>

#include "kern/resource/IResourceManager.h"

class IResourceLoader
{
   public:
    virtual ~IResourceLoader() = default;

    // Loads resource from file and adds it to the resource manager
    virtual bool loadFromFile(const std::string& file, IResourceManager& manager) = 0;
};