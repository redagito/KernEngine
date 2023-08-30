#include "kern/graphics/resource/Material.h"

#include <fmtlog/fmtlog.h>

Material::Material(const Texture *diffuse, const Texture *normal, const Texture *specular,
                     const Texture *glow, const Texture *alpha)
    : m_diffuseTexture(nullptr),
      m_normalTexture(nullptr),
      m_specularTexture(nullptr),
      m_glowTexture(nullptr),
      m_alphaTexture(nullptr)
{
    init(diffuse, normal, specular, glow, alpha);
}

bool Material::init(const Texture *diffuse, const Texture *normal, const Texture *specular,
                     const Texture *glow, const Texture *alpha)
{
    // Textures must either be nullptr (unused) or valid
    if (diffuse != nullptr && !diffuse->isValid())
    {
        loge("Diffuse texture is not valid.");
        return false;
    }
    if (normal != nullptr && !normal->isValid())
    {
        loge("Normal texture is not valid.");
        return false;
    }
    if (specular != nullptr && !specular->isValid())
    {
        loge("Specular texture is not valid.");
        return false;
    }
    if (glow != nullptr && !glow->isValid())
    {
        loge("Glow texture is not valid.");
        return false;
    }
    if (alpha != nullptr && !alpha->isValid())
    {
        loge("Alpha texture is not valid.");
        return false;
    }

    // All checks ok, set values
    m_diffuseTexture = diffuse;
    m_normalTexture = normal;
    m_specularTexture = specular;
    m_glowTexture = glow;
    m_alphaTexture = alpha;
    return true;
}

bool Material::hasDiffuse() const { return m_diffuseTexture != nullptr; }

bool Material::hasNormal() const { return m_normalTexture != nullptr; }

bool Material::hasSpecular() const { return m_specularTexture != nullptr; }

bool Material::hasGlow() const { return m_glowTexture != nullptr; }

bool Material::hasAlpha() const { return m_alphaTexture != nullptr; }

const Texture *Material::getDiffuse() const { return m_diffuseTexture; }

const Texture *Material::getNormal() const { return m_normalTexture; }

const Texture *Material::getSpecular() const { return m_specularTexture; }

const Texture *Material::getGlow() const { return m_glowTexture; }

const Texture *Material::getAlpha() const { return m_alphaTexture; }