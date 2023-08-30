#include "kern/graphics/resource/LoadMaterial.h"

#include <fmtlog/fmtlog.h>

#include "kern/foundation/IniFile.h"
#include "kern/foundation/JsonDeserialize.h"
#include "kern/foundation/JsonUtil.h"
#include "kern/graphics/resource/ResourceManager.h"

bool load(const std::string &file, ResourceManager &manager, SMaterial &material)
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
            loge("Failed to load base.");
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
            loge("Failed to load normal texture.");
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
            loge("Failed to load specular texture.");
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
            loge("Failed to load glow texture.");
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
            loge("Failed to load alpha texture.");
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

bool loadMaterialFromIni(const std::string &file, std::string &base, std::string &normal, std::string &specular,
                         std::string &glow, std::string &alpha)
{
    logd("Loading material from ini file {}.", file.c_str());
    IniFile ini;
    if (!ini.load(file))
    {
        loge("Failed to load material file {} as ini file.", file.c_str());
        return false;
    }

    // Load from ini
    base = ini.getValue("base", "file", "");
    if (base.empty())
    {
        base = ini.getValue("diffuse", "file", "");
        if (!base.empty())
        {
            logw("The material file {} uses deprecated 'diffuse' instead of 'base'", file);
        }
    }
    normal = ini.getValue("normal", "file", "");
    specular = ini.getValue("specular", "file", "");
    glow = ini.getValue("glow", "file", "");
    alpha = ini.getValue("alpha", "file", "");
    return true;
}

bool loadMaterialFromJson(const std::string &file, std::string &base, std::string &normal, std::string &specular,
                          std::string &glow, std::string &alpha)
{
    nlohmann::json root;
    if (!load(file, root))
    {
        loge("Failed to load material file {} as json file.", file.c_str());
        return false;
    }

    // Load from json
    load(root, "base", base);
    load(root, "normal", normal);
    load(root, "specular", specular);
    load(root, "glow", glow);
    load(root, "alpha", alpha);
    return true;
}