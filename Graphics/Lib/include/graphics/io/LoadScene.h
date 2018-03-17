#pragma once

#include <memory>
#include <string>
#include <vector>

#include <glm/glm.hpp>

#include "graphics/animation/Animation.h"
#include "graphics/graphics/SceneConfig.h"

namespace Json
{
class Value; /**< Forward declare for json node handling. */
}

class IScene;
class IResourceManager;
class CAnimationWorld;

/**
 * \brief Scene loading fnctionality.
 * Populates scene with static geometry from file.
 */
bool loadScene(const std::string &file, IScene &scene, CAnimationWorld &animationWorld, IResourceManager& resourceManager);

bool loadSceneObjects(const Json::Value &node, IScene &scene, CAnimationWorld &animationWorld, IResourceManager& resourceManager);
bool loadSceneObject(const Json::Value &node, IScene &scene, CAnimationWorld &animationWorld, IResourceManager& resourceManager);

bool loadPointLights(const Json::Value &node, IScene &scene, CAnimationWorld &animationWorld);
bool loadPointLight(const Json::Value &node, IScene &scene, CAnimationWorld &animationWorld);

bool loadDirectionalLights(const Json::Value &node, IScene &scene,
	CAnimationWorld &animationWorld);
bool loadDirectionalLight(const Json::Value &node, IScene &scene,
	CAnimationWorld &animationWorld);

bool loadAmbientLight(const Json::Value &node, IScene &scene);

bool loadAnimationControllers(const Json::Value &node, IScene &scene,
	CAnimationWorld &animationWorld, SceneObjectId id,
	AnimationObjectType type);

bool loadAnimationController(const Json::Value &node, IScene &scene,
	CAnimationWorld &animationWorld, SceneObjectId id,
	AnimationObjectType type);