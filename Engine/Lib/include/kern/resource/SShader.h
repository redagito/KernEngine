#pragma once

#include "kern/resource/ResourceId.h"

struct SShader
{
    SShader() = default;
    SShader(ResourceId vertex, ResourceId tessCtrl, ResourceId tessEval, ResourceId geometry,
            ResourceId fragment);
    
    ResourceId m_vertex = InvalidResource;
    ResourceId m_tessCtrl = InvalidResource;
    ResourceId m_tessEval = InvalidResource;
    ResourceId m_geometry = InvalidResource;
    ResourceId m_fragment = InvalidResource;
};
