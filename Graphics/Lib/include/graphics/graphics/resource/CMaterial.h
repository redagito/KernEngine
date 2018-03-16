#pragma once

#include <memory>

#include "graphics/graphics/resource/CShaderProgram.h"
#include "graphics/graphics/resource/CTexture.h"

/**
 * \brief Material class.
 *
 * Stores visual properties of a surface.
 */
class CMaterial
{
   public:
    CMaterial(const CTexture *diffuse, const CTexture *normal, const CTexture *specular,
              const CTexture *glow, const CTexture *alpha);

    bool init(const CTexture *diffuse, const CTexture *normal, const CTexture *specular,
              const CTexture *glow, const CTexture *alpha);

    bool hasDiffuse() const;
    bool hasNormal() const;
    bool hasSpecular() const;
    bool hasGlow() const;
    bool hasAlpha() const;

    const CTexture *getDiffuse() const;
    const CTexture *getNormal() const;
    const CTexture *getSpecular() const;
    const CTexture *getGlow() const;
    const CTexture *getAlpha() const;

   private:
    const CTexture *m_diffuseTexture;  /**< Base color. */
    const CTexture *m_normalTexture;   /**< Normal map. */
    const CTexture *m_specularTexture; /**< Specular highlights. */
    const CTexture *m_glowTexture;     /**< Glow map. */
    const CTexture *m_alphaTexture;    /**< Alpha map. */
};
