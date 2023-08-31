#pragma once

#include "kern/resource/IResourceLoader.h"

class ImageLoader : public IResourceLoader
{
   public:
    ~ImageLoader() = default;

    bool loadFromFile(const std::string& file, IResourceManager& manager) override;
};