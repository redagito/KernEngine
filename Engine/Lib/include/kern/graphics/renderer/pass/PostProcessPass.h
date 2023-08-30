#pragma once

#include <string>

#include "kern/graphics/renderer/pass/IRenderPass.h"

class CPostProcessPass
{
   public:
    CPostProcessPass(const std::string &shaderFile);

    bool init(IResourceManager *manager);

    void run();
};