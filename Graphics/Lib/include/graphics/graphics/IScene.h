#pragma once

#include <memory>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "graphics/graphics/SceneConfig.h"
#include "graphics/resource/ResourceConfig.h"

class ICamera;
class ISceneQuery;

/**
 * \brief Scene interface class.
 * The scene should only return internal ids. This provides the foundation for
 * future threading.
 */
class IScene
{
public:
  virtual ~IScene();

  /**
  * \brief Creates scene object from model and returns scene object id..
  */
  virtual SceneObjectId createObject(ResourceId model,
                                     const glm::vec3 &position,
                                     const glm::quat &rotation,
                                     const glm::vec3 &scale) = 0;

  /**
   * \brief Creates object in scene and returns id.
   *
   * Object visibility is per default set to visible.
   */
  virtual SceneObjectId createObject(ResourceId mesh, ResourceId material,
                                     const glm::vec3 &position,
                                     const glm::quat &rotation,
                                     const glm::vec3 &scale) = 0;

  /**
   * \brief Returns scene object data.
   */
  virtual bool getObject(SceneObjectId id, ResourceId &mesh,
                         ResourceId &material, glm::vec3 &position,
                         glm::quat &rotation, glm::vec3 &scale,
                         bool &visible) const = 0;

  /**
  * \brief Set scene object parameters.
  */
  virtual void setObject(SceneObjectId id, ResourceId mesh, ResourceId material,
                         const glm::vec3 &position, const glm::quat &rotation,
                         const glm::vec3 &scale, bool visible) = 0;

  /**
   * \brief Creates point light in scene and returns id.
   */
  virtual SceneObjectId createPointLight(const glm::vec3 &position,
                                         float radius, const glm::vec3 &color,
                                         float intensity, bool castsShadow) = 0;

  /**
   * \brief Returns point light data.
   */
  virtual bool getPointLight(SceneObjectId id, glm::vec3 &position,
                             float &radius, glm::vec3 &color, float &intensity,
                             bool &castsShadow) const = 0;

  /**
  * \brief Sets point light parameters.
  */
  virtual void setPointLight(SceneObjectId id, const glm::vec3 &position,
                             float radius, const glm::vec3 &color,
                             float intensity, bool castsShadow) = 0;

  /**
  * \brief Creates directional light.
  */
  virtual SceneObjectId createDirectionalLight(const glm::vec3 &direction,
                                               const glm::vec3 &color,
                                               float intensity,
                                               bool castsShadow) = 0;

  /**
  * \brief Returns directional light data.
  */
  virtual bool getDirectionalLight(SceneObjectId id, glm::vec3 &direction,
                                   glm::vec3 &color, float &intensity,
                                   bool &castsShadow) const = 0;

  /**
  * \brief Sets directional light parameters.
  */
  virtual void setDirectionalLight(SceneObjectId id, const glm::vec3 &direction,
                                   const glm::vec3 &color, float intensity,
                                   bool castsShadow) = 0;

  /**
  * \brief Sets the ambient light for the scene.
  */
  virtual void setAmbientLight(const glm::vec3 &color, float intensity) = 0;

  /**
  * \brief Returns the ambient light of the scene.
  */
  virtual bool getAmbientLight(glm::vec3 &color, float &intensity) const = 0;

  /**
   * \brief Queries scene for objects and lights visible by the camera.
   */
  virtual void getVisibleObjects(const ICamera &camera,
                                 ISceneQuery &query) const = 0;
};
