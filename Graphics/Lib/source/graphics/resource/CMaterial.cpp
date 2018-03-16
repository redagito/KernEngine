#include "graphics/graphics/resource/CMaterial.h"

#include <foundation/debug/Log.h>

CMaterial::CMaterial(const CTexture *diffuse, const CTexture *normal, const CTexture *specular,
                     const CTexture *glow, const CTexture *alpha)
    : m_diffuseTexture(nullptr),
      m_normalTexture(nullptr),
      m_specularTexture(nullptr),
      m_glowTexture(nullptr),
      m_alphaTexture(nullptr)
{
    init(diffuse, normal, specular, glow, alpha);
}

bool CMaterial::init(const CTexture *diffuse, const CTexture *normal, const CTexture *specular,
                     const CTexture *glow, const CTexture *alpha)
{
    // Textures must either be nullptr (unused) or valid
    if (diffuse != nullptr && !diffuse->isValid())
    {
        LOG_ERROR("Diffuse texture is not valid.");
        return false;
    }
    if (normal != nullptr && !normal->isValid())
    {
        LOG_ERROR("Normal texture is not valid.");
        return false;
    }
    if (specular != nullptr && !specular->isValid())
    {
        LOG_ERROR("Specular texture is not valid.");
        return false;
    }
    if (glow != nullptr && !glow->isValid())
    {
        LOG_ERROR("Glow texture is not valid.");
        return false;
    }
    if (alpha != nullptr && !alpha->isValid())
    {
        LOG_ERROR("Alpha texture is not valid.");
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

bool CMaterial::hasDiffuse() const { return m_diffuseTexture != nullptr; }

bool CMaterial::hasNormal() const { return m_normalTexture != nullptr; }

bool CMaterial::hasSpecular() const { return m_specularTexture != nullptr; }

bool CMaterial::hasGlow() const { return m_glowTexture != nullptr; }

bool CMaterial::hasAlpha() const { return m_alphaTexture != nullptr; }

const CTexture *CMaterial::getDiffuse() const { return m_diffuseTexture; }

const CTexture *CMaterial::getNormal() const { return m_normalTexture; }

const CTexture *CMaterial::getSpecular() const { return m_specularTexture; }

const CTexture *CMaterial::getGlow() const { return m_glowTexture; }

const CTexture *CMaterial::getAlpha() const { return m_alphaTexture; }