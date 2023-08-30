#pragma once

#include <memory>

#include "kern/graphics/resource/ShaderProgram.h"
#include "kern/graphics/resource/Texture.h"

/**
 * \brief Material class.
 *
 * Stores visual properties of a surface.
 */
class Material
{
   public:
    Material(const Texture *diffuse, const Texture *normal, const Texture *specular,
              const Texture *glow, const Texture *alpha);

    bool init(const Texture *diffuse, const Texture *normal, const Texture *specular,
              const Texture *glow, const Texture *alpha);

    bool hasDiffuse() const;
    bool hasNormal() const;
    bool hasSpecular() const;
    bool hasGlow() const;
    bool hasAlpha() const;

    const Texture *getDiffuse() const;
    const Texture *getNormal() const;
    const Texture *getSpecular() const;
    const Texture *getGlow() const;
    const Texture *getAlpha() const;

   private:
    const Texture *m_diffuseTexture;  /**< Base color. */
    const Texture *m_normalTexture;   /**< Normal map. */
    const Texture *m_specularTexture; /**< Specular highlights. */
    const Texture *m_glowTexture;     /**< Glow map. */
    const Texture *m_alphaTexture;    /**< Alpha map. */
};
