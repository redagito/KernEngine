#include "kern/resource/LoadMaterial.h"

#include <fmtlog/fmtlog.h>

#include "kern/foundation/IniFile.h"
#include "kern/foundation/JsonDeserialize.h"
#include "kern/foundation/JsonUtil.h"
#include "kern/resource/ResourceManager.h"

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

    ResourceId baseId = InvalidResource;
    if (!base.empty())
    {
        // Diffuse texture is RGB format, ignore alpha
        baseId = manager.loadImage(base, ColorFormat::RGB24);
        if (baseId == InvalidResource)
        {
            loge("Failed to load base.");
            return false;
        }
    }

    ResourceId normalId = InvalidResource;
    if (!normal.empty())
    {
        // Normal texture is RGB format
        normalId = manager.loadImage(normal, ColorFormat::RGB24);
        if (normalId == InvalidResource)
        {
            loge("Failed to load normal texture.");
            return false;
        }
    }

    ResourceId specularId = InvalidResource;
    if (!specular.empty())
    {
        // Specular texture is grey-scale format
        specularId = manager.loadImage(specular, ColorFormat::GreyScale8);
        if (specularId == InvalidResource)
        {
            loge("Failed to load specular texture.");
            return false;
        }
    }

    ResourceId glowId = InvalidResource;
    if (!glow.empty())
    {
        // Glow texture is grey-scale format
        glowId = manager.loadImage(glow, ColorFormat::GreyScale8);
        if (glowId == InvalidResource)
        {
            loge("Failed to load glow texture.");
            return false;
        }
    }

    ResourceId alphaId = InvalidResource;
    if (!alpha.empty())
    {
        // Alpha texture is grey-scale format
        alphaId = manager.loadImage(alpha, ColorFormat::GreyScale8);
        if (alphaId == InvalidResource)
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