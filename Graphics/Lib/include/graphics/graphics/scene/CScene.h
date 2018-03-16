#pragma once

#include <memory>
#include <vector>

#include <glm/glm.hpp>

#include "graphics/graphics/IScene.h"

struct SSceneObject;
struct SScenePointLight;
struct SSceneDirectionalLight;

class IGraphicsResourceManager;

/**
 * \brief Simple scene implementation.
 */
class CScene : public IScene
{
   public:
    CScene(const IGraphicsResourceManager *resourceManager);
    ~CScene();

    SceneObjectId createObject(ResourceId model, const glm::vec3 &position,
                               const glm::quat &rotation, const glm::vec3 &scale) override;

    SceneObjectId createObject(ResourceId mesh, ResourceId material, const glm::vec3 &position,
                               const glm::quat &rotation, const glm::vec3 &scale) override;

    bool getObject(SceneObjectId id, ResourceId &mesh, ResourceId &material, glm::vec3 &position,
                   glm::quat &rotation, glm::vec3 &scale, bool &visible) const override;

    void setObject(SceneObjectId id, ResourceId mesh, ResourceId material,
                   const glm::vec3 &position, const glm::quat &rotation, const glm::vec3 &scale,
                   bool visible) override;

    SceneObjectId createPointLight(const glm::vec3 &position, float radius, const glm::vec3 &color,
                                   float intensity, bool castsShadow) override;

    bool getPointLight(SceneObjectId id, glm::vec3 &position, float &radius, glm::vec3 &color,
                       float &intensity, bool &castsShadow) const override;

    void setPointLight(SceneObjectId id, const glm::vec3 &position, float radius,
                       const glm::vec3 &color, float intensity, bool castsShadow) override;

    SceneObjectId createDirectionalLight(const glm::vec3 &direction, const glm::vec3 &color,
                                         float intensity, bool castsShadow) override;

    bool getDirectionalLight(SceneObjectId id, glm::vec3 &direction, glm::vec3 &color,
                             float &intensity, bool &castsShadow) const override;

    void setDirectionalLight(SceneObjectId id, const glm::vec3 &direction, const glm::vec3 &color,
                             float intensity, bool castsShadow) override;

    void setAmbientLight(const glm::vec3 &color, float intensity) override;

    bool getAmbientLight(glm::vec3 &color, float &intensity) const override;

    void getVisibleObjects(const ICamera &camera, ISceneQuery &query) const override;

    static bool getViewFrustumCulling();

    // Hacky way to set global culling parameter
    static void setViewFrustumCulling(bool enable);

   private:
    // Hacky
    static bool s_useViewFrustumCulling;

    glm::vec3 m_ambientColor = glm::vec3(1.f); /**< Global ambient light color. */
    float m_ambientIntensity = 0.f;            /**< Global ambient light intensity. */

    std::vector<SSceneObject> m_objects;                     /**< Drawable scene objects. */
    std::vector<SScenePointLight> m_pointLights;             /**< Point lights. */
    std::vector<SSceneDirectionalLight> m_directionalLights; /**< Directional lights. */

    const IGraphicsResourceManager *m_resourceManager = nullptr;
};
