#include "graphics/resource/core/LoadMaterial.h"

#include <foundation/debug/Log.h>
#include <foundation/io/CIniFile.h>
#include <foundation/io/JsonDeserialize.h>
#include <foundation/io/JsonUtil.h>

#include "graphics/resource/core/CResourceManager.h"

bool load(const std::string &file, CResourceManager &manager, SMaterial &material)
{
    std::string base;
    std::string normal;
    std::string specular;
    std::string glow;
    std::string alpha;

    if (file.find(".ini") != std::string::npos)
    {
        if (!loadMaterialFromIni(file, base, normal, specular, glow, alpha))
        {
            return false;
        }
    }
    else if (file.find(".json") != std::string::npos)
    {
        if (!loadMaterialFromJson(file, base, normal, specular, glow, alpha))
        {
            return false;
        }
    }
    else
    {
        return false;
    }

    ResourceId baseId = invalidResource;
    if (!base.empty())
    {
        // Diffuse texture is RGB format, ignore alpha
        baseId = manager.loadImage(base, EColorFormat::RGB24);
        if (baseId == invalidResource)
        {
            LOG_ERROR("Failed to load base.");
            return false;
        }
    }

    ResourceId normalId = invalidResource;
    if (!normal.empty())
    {
        // Normal texture is RGB format
        normalId = manager.loadImage(normal, EColorFormat::RGB24);
        if (normalId == invalidResource)
        {
            LOG_ERROR("Failed to load normal texture.");
            return false;
        }
    }

    ResourceId specularId = invalidResource;
    if (!specular.empty())
    {
        // Specular texture is grey-scale format
        specularId = manager.loadImage(specular, EColorFormat::GreyScale8);
        if (specularId == invalidResource)
        {
            LOG_ERROR("Failed to load specular texture.");
            return false;
        }
    }

    ResourceId glowId = invalidResource;
    if (!glow.empty())
    {
        // Glow texture is grey-scale format
        glowId = manager.loadImage(glow, EColorFormat::GreyScale8);
        if (glowId == invalidResource)
        {
            LOG_ERROR("Failed to load glow texture.");
            return false;
        }
    }

    ResourceId alphaId = invalidResource;
    if (!alpha.empty())
    {
        // Alpha texture is grey-scale format
        alphaId = manager.loadImage(alpha, EColorFormat::GreyScale8);
        if (alphaId == invalidResource)
        {
            LOG_ERROR("Failed to load alpha texture.");
            return false;
        }
    }

    // Set ids
    material.m_base = baseId;
    material.m_normal = normalId;
    material.m_specular = specularId;
    material.m_glow = glowId;
    material.m_alpha = alphaId;
    return true;
}

bool loadMaterialFromIni(const std::string &file, std::string &base, std::string &normal,
                         std::string &specular, std::string &glow, std::string &alpha)
{
    LOG_DEBUG("Loading material from ini file %s.", file.c_str());
    CIniFile ini;
    if (!ini.load(file))
    {
        LOG_ERROR("Failed to load material file %s as ini file.", file.c_str());
        return false;
    }

    // Load from ini
    base = ini.getValue("base", "file", "");
    if (base.empty())
    {
        base = ini.getValue("diffuse", "file", "");
        if (!base.empty())
        {
            KERN_WARNING("The material file " << file
                                              << " uses deprecated 'diffuse' instead of 'base'");
        }
    }
    normal = ini.getValue("normal", "file", "");
    specular = ini.getValue("specular", "file", "");
    glow = ini.getValue("glow", "file", "");
    alpha = ini.getValue("alpha", "file", "");
    return true;
}

bool loadMaterialFromJson(const std::string &file, std::string &base, std::string &normal,
                          std::string &specular, std::string &glow, std::string &alpha)
{
    nlohmann::json root;
    if (!load(file, root))
    {
        LOG_ERROR("Failed to load material file %s as json file.", file.c_str());
        return false;
    }

    // Load from json
    deserialize(root["base"], base);
    deserialize(root["normal"], normal);
    deserialize(root["specular"], specular);
    deserialize(root["glow"], glow);
    deserialize(root["alpha"], alpha);
    return true;
}