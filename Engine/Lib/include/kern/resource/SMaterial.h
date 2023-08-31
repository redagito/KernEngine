#pragma once

#include "kern/resource/ResourceId.h"

/**
 * \brief Material data.
 */
struct SMaterial
{
    SMaterial() = default;
    SMaterial(ResourceId base, ResourceId normal, ResourceId specular, ResourceId glow, ResourceId alpha);

    ResourceId m_base = InvalidResource;
    ResourceId m_normal = InvalidResource;
    ResourceId m_specular = InvalidResource;
    ResourceId m_glow = InvalidResource;
    ResourceId m_alpha = InvalidResource;
};