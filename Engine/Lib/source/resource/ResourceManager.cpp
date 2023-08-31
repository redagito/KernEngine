#include "kern/resource/ResourceManager.h"

#include <fmtlog/fmtlog.h>

#include <assimp/Importer.hpp>
#include <fstream>
#include <sstream>
#include <stdexcept>

#include "kern/foundation/IniFile.h"
#include "kern/foundation/JsonDeserialize.h"
#include "kern/foundation/JsonUtil.h"
#include "kern/resource/IResourceListener.h"
#include "kern/resource/LoadImage.h"
#include "kern/resource/LoadMaterial.h"
#include "kern/resource/LoadMesh.h"
#include "kern/resource/LoadModel.h"
#include "kern/resource/LoadShader.h"
#include "kern/foundation/StringUtil.h"

ResourceManager::~ResourceManager()
{
    for (auto listener : m_resourceListeners)
    {
        listener->onDetach(this);
    }
}

bool ResourceManager::loadFromFile(const std::string &file) 
{
    auto ext = getFileExtension(file);
    auto iter = m_resourceLoaderCreators.find(ext);
    if (iter == m_resourceLoaderCreators.end())
    {
        logw("No resource loader registered for extension {}. Failed to load file {}", ext, file);
        return false;
    }

    auto loader = iter->second();
    return loader->loadFromFile(file, *this);
}

ResourceId ResourceManager::createMesh(const std::vector<float> &vertices, const std::vector<unsigned int> &indices,
                                       const std::vector<float> &normals, const std::vector<float> &uvs,
                                       PrimitiveType type)
{
    // Create mesh id
    ResourceId id = m_nextMeshId;
    ++m_nextMeshId;

    // Add mesh
    m_meshes[id] = SMesh(vertices, indices, normals, uvs, type);

    // Notify listener with create event
    notifyResourceListeners(ResourceType::Mesh, id, ResourceEvent::Create);
    return id;
}

ResourceId ResourceManager::loadMesh(const std::string &file)
{
    auto entry = m_meshFiles.find(file);
    if (entry != m_meshFiles.end())
    {
        return entry->second;
    }

    // Load mesh
    SMesh mesh;
    if (!load(file, mesh))
    {
        loge("Failed to load mesh from file {}.", file.c_str());
        throw std::runtime_error("Failed to load mesh");
    }

    // Create mesh resource
    ResourceId meshId = createMesh(mesh.m_vertices, mesh.m_indices, mesh.m_normals, mesh.m_uvs, mesh.m_type);
    if (meshId == InvalidResource)
    {
        loge("Failed to create mesh resource id from file {}.", file.c_str());
        throw std::runtime_error("Failed to create mesh");
    }
    m_meshFiles[file] = meshId;
    return meshId;
}

bool ResourceManager::getMesh(ResourceId id, std::vector<float> &vertices, std::vector<unsigned int> &indices,
                              std::vector<float> &normals, std::vector<float> &uvs, PrimitiveType &type) const
{
    // Retrieve from map
    auto iter = m_meshes.find(id);
    if (iter == m_meshes.end())
    {
        return false;
    }
    // Copy data
    vertices = iter->second.m_vertices;
    indices = iter->second.m_indices;
    normals = iter->second.m_normals;
    uvs = iter->second.m_uvs;
    type = iter->second.m_type;
    return true;
}

ResourceId ResourceManager::createImage(const std::vector<unsigned char> &imageData, unsigned int width,
                                        unsigned int height, ColorFormat format)
{
    // Create image
    ResourceId id = m_nextImageId;
    ++m_nextImageId;

    // TODO Sanity check if image already exists?
    // Add mesh
    m_images[id] = Image(imageData, width, height, format);

    // Notify listener with create event
    notifyResourceListeners(ResourceType::Image, id, ResourceEvent::Create);
    return id;
}

ResourceId ResourceManager::loadImage(const std::string &file, ColorFormat format)
{
    auto entry = m_imageFiles.find(file);
    if (entry != m_imageFiles.end())
    {
        return entry->second;
    }

    // Load image
    Image image;
    if (!load(file, format, image))
    {
        loge("Failed to load image from file {}.", file.c_str());
        throw std::runtime_error("Failed to load image");
    }

    // Create managed resource
    ResourceId imageId = createImage(image.m_data, image.m_width, image.m_height, image.m_format);
    if (imageId == InvalidResource)
    {
        loge("Failed to create image resource id from file {}.", file.c_str());
        throw std::runtime_error("Failed to create image");
    }
    m_imageFiles[file] = imageId;
    return imageId;
}

bool ResourceManager::getImage(ResourceId id, std::vector<unsigned char> &data, unsigned int &width,
                               unsigned int &height, ColorFormat &format) const
{
    // Retrieve from map
    auto iter = m_images.find(id);
    if (iter == m_images.end())
    {
        return false;
    }
    // Copy data
    data = iter->second.m_data;
    width = iter->second.m_width;
    height = iter->second.m_height;
    format = iter->second.m_format;
    return true;
}

ResourceId ResourceManager::createMaterial(ResourceId base, ResourceId normal, ResourceId specular, ResourceId glow,
                                           ResourceId alpha)
{
    // Create material
    ResourceId id = m_nextMaterialId;
    ++m_nextMaterialId;

    // Add material
    m_materials[id] = SMaterial(base, normal, specular, glow, alpha);

    // Notify listener with create event
    notifyResourceListeners(ResourceType::Material, id, ResourceEvent::Create);
    return id;
}

ResourceId ResourceManager::loadMaterial(const std::string &file)
{
    auto entry = m_materialFiles.find(file);
    if (entry != m_materialFiles.end())
    {
        return entry->second;
    }

    logd("Loading material from file {}.", file.c_str());
    SMaterial material;
    if (!load(file, *this, material))
    {
        loge("Failed to load material from file {}.", file.c_str());
        throw std::runtime_error("Failed to load material");
    }

    ResourceId materialId =
        createMaterial(material.m_base, material.m_normal, material.m_specular, material.m_glow, material.m_alpha);
    if (materialId == InvalidResource)
    {
        loge("Failed to create material resource id for material file {}.", file.c_str());
        throw std::runtime_error("Failed to create material");
    }
    m_materialFiles[file] = materialId;
    return materialId;
}

bool ResourceManager::getMaterial(ResourceId id, ResourceId &base, ResourceId &normal, ResourceId &specular,
                                  ResourceId &glow, ResourceId &alpha) const
{
    // Retrieve from map
    auto iter = m_materials.find(id);
    if (iter == m_materials.end())
    {
        return false;
    }
    // Copy data
    base = iter->second.m_base;
    normal = iter->second.m_normal;
    specular = iter->second.m_specular;
    glow = iter->second.m_glow;
    alpha = iter->second.m_alpha;
    return true;
}

ResourceId ResourceManager::createModel(ResourceId mesh, ResourceId material)
{
    // Create model
    ResourceId id = m_nextModelId;
    ++m_nextModelId;

    // Add material
    m_models[id] = SModel(mesh, material);

    // Notify listener with create event
    notifyResourceListeners(ResourceType::Model, id, ResourceEvent::Create);
    return id;
}

ResourceId ResourceManager::loadModel(const std::string &file)
{
    auto entry = m_modelFiles.find(file);
    if (entry != m_modelFiles.end())
    {
        return entry->second;
    }

    logd("Loading model from file {}.", file.c_str());
    SModel model;
    if (!load(file, *this, model))
    {
        loge("Failed to load model from file {}.", file.c_str());
        throw std::runtime_error("Failed to load model");
    }

    ResourceId modelId = createModel(model.m_mesh, model.m_material);
    if (modelId == InvalidResource)
    {
        loge("Failed to create model resource id for model file {}.", file.c_str());
        throw std::runtime_error("Failed to create model");
    }
    m_modelFiles[file] = modelId;
    return modelId;
}

bool ResourceManager::getModel(ResourceId id, ResourceId &mesh, ResourceId &material)
{
    // Retrieve from map
    auto iter = m_models.find(id);
    if (iter == m_models.end())
    {
        return false;
    }
    // Copy data
    mesh = iter->second.m_mesh;
    material = iter->second.m_material;
    return true;
}

ResourceId ResourceManager::createString(const std::string &text)
{
    // Create string
    ResourceId id = m_nextStringId;
    ++m_nextStringId;

    // Add string
    m_strings[id] = text;

    // Notify listener with create event
    notifyResourceListeners(ResourceType::String, id, ResourceEvent::Create);
    return id;
}

bool ResourceManager::getString(ResourceId id, std::string &text) const
{
    // Retrieve from map
    auto iter = m_strings.find(id);
    if (iter == m_strings.end())
    {
        return false;
    }
    // Copy data
    text = iter->second;
    return true;
}

ResourceId ResourceManager::createShader(ResourceId vertex, ResourceId tessCtrl, ResourceId tessEval,
                                         ResourceId geometry, ResourceId fragment)
{
    // Needs at least valid vertex and fragment shader
    if (vertex == InvalidResource || fragment == InvalidResource)
    {
        loge("Failed to create shader, resource id for vertex or fragment shader is invalid.");
        throw std::runtime_error("Failed to create shader: one or more ids invalid");
    }

    // Create shader
    ResourceId id = m_nextShaderId;
    ++m_nextShaderId;

    // Add shader
    m_shaders[id] = SShader(vertex, tessCtrl, tessEval, geometry, fragment);

    // Notify listener with create event
    notifyResourceListeners(ResourceType::Shader, id, ResourceEvent::Create);
    return id;
}

bool ResourceManager::getShader(ResourceId id, ResourceId &vertex, ResourceId &tessCtrl, ResourceId &tessEval,
                                ResourceId &geometry, ResourceId &fragment) const
{
    // Retrieve from map
    auto iter = m_shaders.find(id);
    if (iter == m_shaders.end())
    {
        return false;
    }
    // Copy data
    vertex = iter->second.m_vertex;
    tessCtrl = iter->second.m_tessCtrl;
    tessEval = iter->second.m_tessEval;
    geometry = iter->second.m_geometry;
    fragment = iter->second.m_fragment;
    return true;
}

ResourceId ResourceManager::loadShader(const std::string &file)
{
    // Check if shader exists
    auto entry = m_shaderFiles.find(file);
    if (entry != m_shaderFiles.end())
    {
        return entry->second;
    }

    logd("Loading shader from file {}.", file.c_str());
    SShader shader;
    if (!load(file, *this, shader))
    {
        loge("Failed to load shader from file {}.", file.c_str());
        throw std::runtime_error("Failed to load shader file");
    }

    ResourceId shaderId = InvalidResource;
    shaderId =
        createShader(shader.m_vertex, shader.m_tessCtrl, shader.m_tessEval, shader.m_geometry, shader.m_fragment);
    if (shaderId == InvalidResource)
    {
        loge("Failed to create reasource id for shader file {}.", file.c_str());
        throw std::runtime_error("Failed to create shader");
    }

    m_shaderFiles[file] = shaderId;
    return shaderId;
}

void ResourceManager::addResourceListener(IResourceListener *listener)
{
    m_resourceListeners.push_back(listener);
    listener->onAttach(this);
}

void ResourceManager::removeResourceListener(IResourceListener *listener)
{
    m_resourceListeners.remove(listener);
    listener->onDetach(this);
}

void ResourceManager::addResourceLoaderCreator(const std::string &extension,
                                               const std::function<std::unique_ptr<IResourceLoader>()> &creator)
{
    if (m_resourceLoaderCreators.find(extension) != m_resourceLoaderCreators.end())
    {
        throw std::runtime_error("Creation function for " + extension + " already exists");
    }
    m_resourceLoaderCreators[extension] = creator;
}

void ResourceManager::notifyResourceListeners(ResourceType type, ResourceId id, ResourceEvent event)
{
    for (const auto &listener : m_resourceListeners)
    {
        listener->notify(type, id, event, this);
    }
}

ResourceId ResourceManager::loadString(const std::string &file)
{
    auto iter = m_textFiles.find(file);
    if (iter != m_textFiles.end())
    {
        return iter->second;
    }

    logd("Loading text from file {}.", file.c_str());
    std::ifstream ifs(file);
    if (!ifs.is_open())
    {
        loge("Failed to open the text file {}.", file.c_str());
        throw std::runtime_error("Failed to load text file");
    }

    // Load file
    std::string text((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
    ifs.close();

    ResourceId stringId = -1;

    // Create new string entry
    stringId = createString(text);
    if (stringId == -1)
    {
        loge("Failed to create string id for text file {}.", file.c_str());
        throw std::runtime_error("Failed to create text from file");
    }

    m_textFiles[file] = stringId;
    return stringId;
}