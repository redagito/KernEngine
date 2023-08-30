#include "kern/graphics/io/SceneLoader.h"

#include <fmtlog/fmtlog.h>

#include <fstream>
#include <glm/glm.hpp>
#include <nlohmann/json.hpp>

#include "kern/foundation/JsonDeserialize.h"
#include "kern/foundation/JsonUtil.h"
#include "kern/graphics/IScene.h"
#include "kern/graphics/animation/AnimationWorld.h"
#include "kern/graphics/animation/MovementController.h"
#include "kern/graphics/animation/RotationController.h"
#include "kern/graphics/animation/SineTranslationController.h"
#include "kern/graphics/resource/IResourceManager.h"

SceneLoader::SceneLoader(IResourceManager &resourceManager) : m_resourceManager(resourceManager) { return; }

bool SceneLoader::load(const std::string &file, IScene &scene, AnimationWorld &animationWorld)
{
    nlohmann::json root;

    if (!::load(file, root))
    {
        loge("Failed to load scene.");
        return false;
    }

    // Load scene objects
    if (!loadSceneObjects(root["scene_objects"], scene, animationWorld))
    {
        loge("Error while loading scene objects from scene file {}.", file.c_str());
        return false;
    }

    // Load point lights
    if (!loadPointLights(root["point_lights"], scene, animationWorld))
    {
        loge("Error while loading point lights from scene file {}.", file.c_str());
        return false;
    }

    // Load directional lights
    if (!loadDirectionalLights(root["directional_lights"], scene, animationWorld))
    {
        loge("Error while loading ambient light from scene file &s.", file.c_str());
        return false;
    }

    // Load ambient light
    if (!loadAmbientLight(root["ambient_light"], scene))
    {
        loge("Error while loading ambient light from scene file &s.", file.c_str());
        return false;
    }

    return true;
}

bool SceneLoader::loadSceneObjects(const nlohmann::json &node, IScene &scene, AnimationWorld &animationWorld)
{
    // Node empty?
    if (node.empty())
    {
        logi("Missing or empty 'scene_objects' node. No scene objects are loaded.");
        return true;
    }

    // Node is array type
    if (!node.is_array())
    {
        loge("The node 'scene_objects' must be array type.");
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

bool SceneLoader::loadSceneObject(const nlohmann::json &node, IScene &scene, AnimationWorld &animationWorld)
{
    std::string mesh;
    std::string material;
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    if (!::load(node, "mesh", mesh))
    {
        return false;
    }

    if (!::load(node, "material", material))
    {
        return false;
    }

    if (!::load(node, "position", position))
    {
        return false;
    }

    if (!::load(node, "rotation", rotation))
    {
        return false;
    }

    if (!::load(node, "scale", scale))
    {
        return false;
    }

    // Load mesh file
    ResourceId meshId = m_resourceManager.loadMesh(mesh);
    if (meshId == -1)
    {
        loge("Failed to load mesh file {}.", mesh.c_str());
        return false;
    }

    // Load material file
    ResourceId materialId = m_resourceManager.loadMaterial(material);
    if (materialId == -1)
    {
        loge("Failed to load material file {}.", material.c_str());
        return false;
    }

    // Create object in scene
    SceneObjectId objectId = scene.createObject(meshId, materialId, position, glm::quat(rotation), scale);

    // Load optional animation controllers
    if (node.find("animations") != node.end())
    {
        if (!loadAnimationControllers(node.at("animations"), scene, animationWorld, objectId,
                                      AnimationObjectType::Model))
        {
            loge("Failed to scene object load animation controller.");
            return false;
        }
    }

    return true;
}

bool SceneLoader::loadPointLights(const nlohmann::json &node, IScene &scene, AnimationWorld &animationWorld)
{
    // Node empty?
    if (node.empty())
    {
        logi("Missing or empty 'point_lights' node. No point lights are loaded.");
        return true;
    }

    // Node is array type
    if (!node.is_array())
    {
        loge("The node 'point_lights' must be array type.");
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

bool SceneLoader::loadPointLight(const nlohmann::json &node, IScene &scene, AnimationWorld &animationWorld)
{
    glm::vec3 position;
    glm::vec3 color;
    float radius;
    float intensity;
    bool castsShadow;

    if (!::load(node, "position", position))
    {
        return false;
    }

    if (!::load(node, "color", color))
    {
        return false;
    }

    if (!::load(node, "radius", radius))
    {
        return false;
    }

    if (!::load(node, "intensity", intensity))
    {
        return false;
    }

    if (!::load(node, "casts_shadow", castsShadow))
    {
        return false;
    }

    // Create object in scene
    SceneObjectId objectId = scene.createPointLight(position, radius, color, intensity, castsShadow);

    // Load optional animation controllers
    if (node.find("animations") != node.end())
    {
        if (!loadAnimationControllers(node["animations"], scene, animationWorld, objectId,
                                      AnimationObjectType::PointLight))
        {
            loge("Failed to load point light animation controller.");
            return false;
        }
    }
    return true;
}

bool SceneLoader::loadDirectionalLights(const nlohmann::json &node, IScene &scene, AnimationWorld &animationWorld)
{
    // Node empty?
    if (node.empty())
    {
        logi(
            "Missing or empty 'directional_lights' node. No directional "
            "lights are loaded.");
        return true;
    }

    // Node is array type
    if (!node.is_array())
    {
        loge("The node 'directional_lights' must be array type.");
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

bool SceneLoader::loadDirectionalLight(const nlohmann::json &node, IScene &scene, AnimationWorld &animationWorld)
{
    glm::vec3 direction;
    glm::vec3 color;
    float intensity;
    bool castsShadow;

    if (!::load(node, "direction", direction))
    {
        return false;
    }

    if (!::load(node, "color", color))
    {
        return false;
    }

    if (!::load(node, "intensity", intensity))
    {
        return false;
    }

    if (!::load(node, "casts_shadow", castsShadow))
    {
        return false;
    }

    // Create object in scene
    scene.createDirectionalLight(direction, color, intensity, castsShadow);
    return true;
}

bool SceneLoader::loadAmbientLight(const nlohmann::json &node, IScene &scene)
{
    if (node.empty())
    {
        logi("Missing or empty 'ambient_light' node. No ambient light is loaded.");
        return true;
    }

    glm::vec3 color;
    float intensity;

    if (!::load(node, "color", color))
    {
        return false;
    }

    if (!::load(node, "intensity", intensity))
    {
        return false;
    }

    scene.setAmbientLight(color, intensity);
    return true;
}

bool SceneLoader::loadAnimationControllers(const nlohmann::json &node, IScene &scene, AnimationWorld &animationWorld,
                                            SceneObjectId id, AnimationObjectType type)
{
    if (node.empty())
    {
        // logi("Missing or empty 'animations' node. No animation controllers
        // where loaded.");
        return true;
    }

    // Node is array type
    if (!node.is_array())
    {
        loge("The node 'animations' must be array type.");
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

bool SceneLoader::loadAnimationController(const nlohmann::json &node, IScene &scene, AnimationWorld &animationWorld,
                                           SceneObjectId id, AnimationObjectType type)
{
    std::string controllerType;
    if (!::load(node, "type", controllerType))
    {
        return false;
    }
    //
    if (controllerType == "rotation")
    {
        glm::vec3 rotation;
        if (!::load(node, "rotation", rotation))
        {
            return false;
        }
        animationWorld.addAnimationController(std::make_shared<RotationController>(id, type, scene, rotation));
    }
    else if (controllerType == "movement")
    {
        glm::vec3 direction;
        if (!::load(node, "direction", direction))
        {
            return false;
        }
        animationWorld.addAnimationController(std::make_shared<MovementController>(id, type, scene, direction));
    }
    else if (controllerType == "sine_translation")
    {
        glm::vec3 translation;
        float timeScale;
        if (!::load(node, "translation", translation))
        {
            return false;
        }
        if (!::load(node, "timescale", timeScale))
        {
            return false;
        }
        animationWorld.addAnimationController(
            std::make_shared<SineTranslationController>(id, type, scene, translation, timeScale));
    }
    else
    {
        // Unknown controller type
        return false;
    }

    return true;
}