#pragma once

#include "graphics/resource/CMaterial.h"
#include "graphics/resource/CMesh.h"

/**
 * \brief Model resource stores mesh and material.
 */
class CModel
{
   public:
    CModel(const CMesh *mesh, const CMaterial *material);

    /**
     * \brief Initialize form mesh and material.
     */
    bool init(const CMesh *mesh, const CMaterial *material);

    bool hasMesh() const;
    bool hasMaterial() const;

    const CMesh *getMesh() const;
    const CMaterial *getMaterial() const;

   private:
    const CMesh *m_mesh = nullptr;         /**< Mesh resource. */
    const CMaterial *m_material = nullptr; /**< Material resource. */
};