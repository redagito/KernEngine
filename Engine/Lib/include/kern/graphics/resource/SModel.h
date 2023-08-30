#pragma once

#include "kern/graphics/resource/ResourceConfig.h"

/**
 * \brief Stores model data.
 */
struct SModel
{
    SModel();
    SModel(ResourceId mesh, ResourceId material);

    ResourceId m_mesh = invalidResource;
    ResourceId m_material = invalidResource;
};