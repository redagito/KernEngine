#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

#include "kern/graphics/SceneConfig.h"
#include "kern/graphics/animation/AnimationObjectType.h"

class IScene;
class IResourceManager;
class AnimationWorld;

/**
 * \brief Scene loader utility class.
 * Populates scene with static geometry from file.
 * Used for testing only, ids are not stored.
 */

class SceneLoader
{
   public:
    /**
     * \brief Create scene loader with resource manager.
     */
    SceneLoader(IResourceManager &resourceManager);
    bool load(const std::string &file, IScene &scene, AnimationWorld &animationWorld);

   protected:
    bool loadSceneObjects(const nlohmann::json &node, IScene &scene, AnimationWorld &animationWorld);
    bool loadSceneObject(const nlohmann::json &node, IScene &scene, AnimationWorld &animationWorld);

    bool loadPointLights(const nlohmann::json &node, IScene &scene, AnimationWorld &animationWorld);
    bool loadPointLight(const nlohmann::json &node, IScene &scene, AnimationWorld &animationWorld);

    bool loadDirectionalLights(const nlohmann::json &node, IScene &scene, AnimationWorld &animationWorld);
    bool loadDirectionalLight(const nlohmann::json &node, IScene &scene, AnimationWorld &animationWorld);

    bool loadAmbientLight(const nlohmann::json &node, IScene &scene);

    bool loadAnimationControllers(const nlohmann::json &node, IScene &scene, AnimationWorld &animationWorld,
                                  SceneObjectId id, AnimationObjectType type);

    bool loadAnimationController(const nlohmann::json &node, IScene &scene, AnimationWorld &animationWorld,
                                 SceneObjectId id, AnimationObjectType type);

   private:
    IResourceManager &m_resourceManager;
};