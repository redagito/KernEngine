#include "kern/resource/LoadModel.h"

#include <fmtlog/fmtlog.h>

#include "kern/foundation/JsonDeserialize.h"
#include "kern/foundation/JsonUtil.h"
#include "kern/foundation/StringUtil.h"

bool load(const std::string &file, ResourceManager &manager, SModel &model)
{
    std::string extension = getFileExtension(file);
    std::string mesh;
    std::string material;
    if (extension == "json")
    {
        if (!loadModelFromJson(file, mesh, material))
        {
            return false;
        }
    }
    else
    {
        loge("Invalid or unknown model file format.");
        return false;
    }

    // Load mesh
    ResourceId meshId = manager.loadMesh(mesh);
    if (meshId == InvalidResource)
    {
        loge("Failed to load mesh.");
        return false;
    }

    // Load material
    ResourceId materialId = manager.loadMaterial(material);
    if (meshId == InvalidResource)
    {
        loge("Failed to load material.");
        return false;
    }

    // Set ids
    model.m_mesh = meshId;
    model.m_material = materialId;
    return true;
}

bool loadModelFromJson(const std::string &file, std::string &meshFile, std::string &materialFile)
{
    nlohmann::json root;
    if (!load(file, root))
    {
        return false;
    }

    if (!load(root, "mesh", meshFile))
    {
        loge("Failed to load mesh node.");
        return false;
    }
    if (!load(root, "material", materialFile))
    {
        loge("Failed to load material node.");
        return false;
    }
    return true;
}