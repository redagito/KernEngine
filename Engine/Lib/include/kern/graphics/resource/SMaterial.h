#pragma once

#include "kern/graphics/resource/ResourceConfig.h"

/**
 * \brief Material data.
 */
struct SMaterial
{
    SMaterial();
    SMaterial(ResourceId base, ResourceId normal, ResourceId specular, ResourceId glow,
              ResourceId alpha);

    ResourceId m_base = invalidResource;
    ResourceId m_normal = invalidResource;
    ResourceId m_specular = invalidResource;
    ResourceId m_glow = invalidResource;
    ResourceId m_alpha = invalidResource;
};