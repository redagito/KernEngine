#pragma once

#include <memory>
#include <string>
#include <vector>

#include <nlohmann/json_fwd.hpp>
#include <glm/glm.hpp>

#include "graphics/animation/Animation.h"
#include "graphics/graphics/SceneConfig.h"

class IScene;
class IResourceManager;
class CAnimationWorld;

/**
 * \brief Scene loader utility class.
 * Populates scene with static geometry from file.
 * Used for testing only, ids are not stored.
 */

class CSceneLoader
{
   public:
    /**
     * \brief Create scene loader with resource manager.
     */
    CSceneLoader(IResourceManager &resourceManager);
    bool load(const std::string &file, IScene &scene, CAnimationWorld &animationWorld);

   protected:
    bool loadSceneObjects(const nlohmann::json &node, IScene &scene, CAnimationWorld &animationWorld);
    bool loadSceneObject(const nlohmann::json &node, IScene &scene, CAnimationWorld &animationWorld);

    bool loadPointLights(const nlohmann::json &node, IScene &scene, CAnimationWorld &animationWorld);
    bool loadPointLight(const nlohmann::json &node, IScene &scene, CAnimationWorld &animationWorld);

    bool loadDirectionalLights(const nlohmann::json &node, IScene &scene,
                               CAnimationWorld &animationWorld);
    bool loadDirectionalLight(const nlohmann::json &node, IScene &scene,
                              CAnimationWorld &animationWorld);

    bool loadAmbientLight(const nlohmann::json &node, IScene &scene);

    bool loadAnimationControllers(const nlohmann::json &node, IScene &scene,
                                  CAnimationWorld &animationWorld, SceneObjectId id,
                                  AnimationObjectType type);

    bool loadAnimationController(const nlohmann::json &node, IScene &scene,
                                 CAnimationWorld &animationWorld, SceneObjectId id,
                                 AnimationObjectType type);

   private:
    IResourceManager &m_resourceManager;
};