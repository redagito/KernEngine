#pragma once

#include "kern/graphics/resource/Material.h"
#include "kern/graphics/resource/Mesh.h"

/**
 * \brief Model resource stores mesh and material.
 */
class Model
{
   public:
    Model(const Mesh *mesh, const Material *material);

    /**
     * \brief Initialize form mesh and material.
     */
    bool init(const Mesh *mesh, const Material *material);

    bool hasMesh() const;
    bool hasMaterial() const;

    const Mesh *getMesh() const;
    const Material *getMaterial() const;

   private:
    const Mesh *m_mesh = nullptr;         /**< Mesh resource. */
    const Material *m_material = nullptr; /**< Material resource. */
};