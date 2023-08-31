#pragma once

#include "kern/resource/ResourceId.h"

/**
 * \brief Stores model data.
 */
struct SModel
{
    SModel() = default;
    SModel(ResourceId mesh, ResourceId material);

    ResourceId m_mesh = InvalidResource;
    ResourceId m_material = InvalidResource;
};