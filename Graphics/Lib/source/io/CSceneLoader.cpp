#include "graphics/io/CSceneLoader.h"

#include <fstream>

#include <glm/glm.hpp>
#include <nlohmann/json.hpp>

#include <foundation/debug/Log.h>
#include <foundation/io/JsonDeserialize.h>
#include <foundation/io/JsonUtil.h>

#include "graphics/graphics/IScene.h"
#include "graphics/resource/IResourceManager.h"

#include "graphics/animation/CAnimationWorld.h"
#include "graphics/animation/CMovementController.h"
#include "graphics/animation/CRotationController.h"
#include "graphics/animation/CSineTranslationController.h"

CSceneLoader::CSceneLoader(IResourceManager &resourceManager) : m_resourceManager(resourceManager) { return; }

bool CSceneLoader::load(const std::string &file, IScene &scene, CAnimationWorld &animationWorld)
{
    nlohmann::json root;

    if (!::load(file, root))
    {
        LOG_ERROR("Failed to load scene.");
        return false;
    }

    // Load scene objects
    if (!loadSceneObjects(root["scene_objects"], scene, animationWorld))
    {
        LOG_ERROR("Error while loading scene objects from scene file %s.", file.c_str());
        return false;
    }

    // Load point lights
    if (!loadPointLights(root["point_lights"], scene, animationWorld))
    {
        LOG_ERROR("Error while loading point lights from scene file %s.", file.c_str());
        return false;
    }

    // Load directional lights
    if (!loadDirectionalLights(root["directional_lights"], scene, animationWorld))
    {
        LOG_ERROR("Error while loading ambient light from scene file &s.", file.c_str());
        return false;
    }

    // Load ambient light
    if (!loadAmbientLight(root["ambient_light"], scene))
    {
        LOG_ERROR("Error while loading ambient light from scene file &s.", file.c_str());
        return false;
    }

    return true;
}

bool CSceneLoader::loadSceneObjects(const nlohmann::json &node, IScene &scene, CAnimationWorld &animationWorld)
{
    // Node empty?
    if (node.empty())
    {
        LOG_INFO("Missing or empty 'scene_objects' node. No scene objects are loaded.");
        return true;
    }

    // Node is array type
    if (!node.is_array())
    {
        LOG_ERROR("The node 'scene_objects' must be array type.");
        return false;
    }

    // Load scene objects
    for (unsigned int i = 0; i < node.size(); ++i)
    {
        if (!loadSceneObject(node[i], scene, animationWorld))
        {
            return false;
        }
    }
    return true;
}

bool CSceneLoader::loadSceneObject(const nlohmann::json &node, IScene &scene, CAnimationWorld &animationWorld)
{
    std::string mesh;
    std::string material;
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    if (!load(node, "mesh", mesh))
    {
        return false;
    }

    if (!load(node, "material", material))
    {
        return false;
    }

    if (!load(node, "position", position))
    {
        return false;
    }

    if (!load(node, "rotation", rotation))
    {
        return false;
    }

    if (!load(node, "scale", scale))
    {
        return false;
    }

    // Load mesh file
    ResourceId meshId = m_resourceManager.loadMesh(mesh);
    if (meshId == -1)
    {
        LOG_ERROR("Failed to load mesh file %s.", mesh.c_str());
        return false;
    }

    // Load material file
    ResourceId materialId = m_resourceManager.loadMaterial(material);
    if (materialId == -1)
    {
        LOG_ERROR("Failed to load material file %s.", material.c_str());
        return false;
    }

    // Create object in scene
    SceneObjectId objectId = scene.createObject(meshId, materialId, position, glm::quat(rotation), scale);

    // Load optional animation controllers
    if (node.find("animations") != node.end() &&
        !loadAnimationControllers(node.at("animations"), scene, animationWorld, objectId, AnimationObjectType::Model))
    {
        LOG_ERROR("Failed to load animation controller.");
        return false;
    }

    return true;
}

bool CSceneLoader::loadPointLights(const nlohmann::json &node, IScene &scene, CAnimationWorld &animationWorld)
{
    // Node empty?
    if (node.empty())
    {
        LOG_INFO("Missing or empty 'point_lights' node. No point lights are loaded.");
        return true;
    }

    // Node is array type
    if (!node.is_array())
    {
        LOG_ERROR("The node 'point_lights' must be array type.");
        return false;
    }

    // Load scene point lights
    for (unsigned int i = 0; i < node.size(); ++i)
    {
        if (!loadPointLight(node[i], scene, animationWorld))
        {
            return false;
        }
    }
    return true;
}

bool CSceneLoader::loadPointLight(const nlohmann::json &node, IScene &scene, CAnimationWorld &animationWorld)
{
    glm::vec3 position;
    glm::vec3 color;
    float radius;
    float intensity;
    bool castsShadow;

    if (!load(node, "position", position))
    {
        return false;
    }

    if (!load(node, "color", color))
    {
        return false;
    }

    if (!load(node, "radius", radius))
    {
        return false;
    }

    if (!load(node, "intensity", intensity))
    {
        return false;
    }

    if (!load(node, "casts_shadow", castsShadow))
    {
        return false;
    }

    // Create object in scene
    SceneObjectId objectId = scene.createPointLight(position, radius, color, intensity, castsShadow);

    // Load optional animation controllers
    if (node.find("animations") == node.end() || !loadAnimationControllers(node["animations"], scene, animationWorld, objectId, AnimationObjectType::PointLight))
    {
        LOG_ERROR("Failed to load animation controller.");
        return false;
    }
    return true;
}

bool CSceneLoader::loadDirectionalLights(const nlohmann::json &node, IScene &scene, CAnimationWorld &animationWorld)
{
    // Node empty?
    if (node.empty())
    {
        LOG_INFO(
            "Missing or empty 'directional_lights' node. No directional "
            "lights are loaded.");
        return true;
    }

    // Node is array type
    if (!node.is_array())
    {
        LOG_ERROR("The node 'directional_lights' must be array type.");
        return false;
    }

    // Load scene directional lights
    for (unsigned int i = 0; i < node.size(); ++i)
    {
        if (!loadDirectionalLight(node[i], scene, animationWorld))
        {
            return false;
        }
    }
    return true;
}

bool CSceneLoader::loadDirectionalLight(const nlohmann::json &node, IScene &scene, CAnimationWorld &animationWorld)
{
    glm::vec3 direction;
    glm::vec3 color;
    float intensity;
    bool castsShadow;

    if (!load(node, "direction", direction))
    {
        return false;
    }

    if (!load(node, "color", color))
    {
        return false;
    }

    if (!load(node, "intensity", intensity))
    {
        return false;
    }

    if (!load(node, "casts_shadow", castsShadow))
    {
        return false;
    }

    // Create object in scene
    scene.createDirectionalLight(direction, color, intensity, castsShadow);
    return true;
}

bool CSceneLoader::loadAmbientLight(const nlohmann::json &node, IScene &scene)
{
    if (node.empty())
    {
        LOG_INFO("Missing or empty 'ambient_light' node. No ambient light is loaded.");
        return true;
    }

    glm::vec3 color;
    float intensity;

    if (!load(node, "color", color))
    {
        return false;
    }

    if (!load(node, "intensity", intensity))
    {
        return false;
    }

    scene.setAmbientLight(color, intensity);
    return true;
}

bool CSceneLoader::loadAnimationControllers(const nlohmann::json &node, IScene &scene, CAnimationWorld &animationWorld,
                                            SceneObjectId id, AnimationObjectType type)
{
    if (node.empty())
    {
        // LOG_INFO("Missing or empty 'animations' node. No animation controllers
        // where loaded.");
        return true;
    }

    // Node is array type
    if (!node.is_array())
    {
        LOG_ERROR("The node 'animations' must be array type.");
        return false;
    }

    // Load animation controllers for object
    for (unsigned int i = 0; i < node.size(); ++i)
    {
        if (!loadAnimationController(node[i], scene, animationWorld, id, type))
        {
            return false;
        }
    }
    return true;
}

bool CSceneLoader::loadAnimationController(const nlohmann::json &node, IScene &scene, CAnimationWorld &animationWorld,
                                           SceneObjectId id, AnimationObjectType type)
{
    std::string controllerType;
    if (!load(node, "type", controllerType))
    {
        return false;
    }
    //
    if (controllerType == "rotation")
    {
        glm::vec3 rotation;
        if (!load(node, "rotation", rotation))
        {
            return false;
        }
        animationWorld.addAnimationController(std::make_shared<CRotationController>(id, type, scene, rotation));
    }
    else if (controllerType == "movement")
    {
        glm::vec3 direction;
        if (!load(node, "direction", direction))
        {
            return false;
        }
        animationWorld.addAnimationController(std::make_shared<CMovementController>(id, type, scene, direction));
    }
    else if (controllerType == "sine_translation")
    {
        glm::vec3 translation;
        float timeScale;
        if (!load(node, "translation", translation))
        {
            return false;
        }
        if (!load(node, "timescale", timeScale))
        {
            return false;
        }
        animationWorld.addAnimationController(
            std::make_shared<CSineTranslationController>(id, type, scene, translation, timeScale));
    }
    else
    {
        // Unknown controller type
        return false;
    }

    return true;
}