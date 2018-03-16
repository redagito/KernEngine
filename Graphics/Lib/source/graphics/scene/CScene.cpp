#include "graphics/graphics/scene/CScene.h"

#include <foundation/debug/Log.h>

#include "graphics/collision/CFrustum.h"

#include "graphics/graphics/ICamera.h"
#include "graphics/graphics/IGraphicsResourceManager.h"
#include "graphics/graphics/resource/CMesh.h"
#include "graphics/graphics/scene/CSceneQuery.h"
#include "graphics/graphics/scene/SSceneDirectionalLight.h"
#include "graphics/graphics/scene/SSceneObject.h"
#include "graphics/graphics/scene/SScenePointLight.h"

bool CScene::getViewFrustumCulling() { return s_useViewFrustumCulling; }

void CScene::setViewFrustumCulling(bool enable) { s_useViewFrustumCulling = enable; }

bool CScene::s_useViewFrustumCulling = true;

CScene::CScene(const IGraphicsResourceManager *manager) : m_resourceManager(manager) {}

CScene::~CScene() {}

SceneObjectId CScene::createObject(ResourceId model, const glm::vec3 &position,
                                   const glm::quat &rotation, const glm::vec3 &scale)
{
    // const CMesh* meshPtr = m_resourceManager->getMesh(meshId);
    m_objects.push_back(SSceneObject(model, position, rotation, scale, true, CBoundingSphere()));
    return m_objects.size() - 1;
}

SceneObjectId CScene::createObject(ResourceId meshId, ResourceId material,
                                   const glm::vec3 &position, const glm::quat &rotation,
                                   const glm::vec3 &scale)
{
    const CMesh *meshPtr = m_resourceManager->getMesh(meshId);
    m_objects.push_back(SSceneObject(meshId, material, position, rotation, scale, true,
                                     meshPtr->getBoundingSphere()));
    return m_objects.size() - 1;
}

bool CScene::getObject(SceneObjectId id, ResourceId &mesh, ResourceId &material,
                       glm::vec3 &position, glm::quat &rotation, glm::vec3 &scale,
                       bool &visible) const
{
    // TODO Needs to be changed for better data structures
    if (id < 0 || ((unsigned int)id) >= m_objects.size())
    {
        return false;
    }

    // Write data
    mesh = m_objects.at(id).m_mesh;
    material = m_objects.at(id).m_material;
    position = m_objects.at(id).m_position;
    rotation = m_objects.at(id).m_rotation;
    scale = m_objects.at(id).m_scale;
    visible = m_objects.at(id).m_visible;
    return true;
}

void CScene::setObject(SceneObjectId id, ResourceId meshId, ResourceId material,
                       const glm::vec3 &position, const glm::quat &rotation, const glm::vec3 &scale,
                       bool visible)
{
    // TODO Needs to be changed for better data structures
    assert(id >= 0 && ((unsigned int)id) < m_objects.size() && "Invalid scene object id");
    unsigned int index = (unsigned int)id;
    // Write data
    const CMesh *meshPtr = m_resourceManager->getMesh(meshId);
    m_objects[index] = SSceneObject(meshId, material, position, rotation, scale, visible,
                                    meshPtr->getBoundingSphere());
    return;
}

SceneObjectId CScene::createPointLight(const glm::vec3 &position, float radius,
                                       const glm::vec3 &color, float intensity, bool castsShadow)
{
    m_pointLights.push_back(SScenePointLight(position, radius, color, intensity, castsShadow));
    return m_pointLights.size() - 1;
}

bool CScene::getPointLight(SceneObjectId id, glm::vec3 &position, float &radius, glm::vec3 &color,
                           float &intensity, bool &castsShadow) const
{
    // TODO Needs to be changed for better data structures
    if (id < 0 || ((unsigned int)id) >= m_pointLights.size())
    {
        return false;
    }

    // Write data
    position = m_pointLights.at(id).m_position;
    radius = m_pointLights.at(id).m_radius;
    color = m_pointLights.at(id).m_color;
    intensity = m_pointLights.at(id).m_intensity;
    castsShadow = m_pointLights.at(id).m_castsShadow;
    return true;
}

void CScene::setPointLight(SceneObjectId id, const glm::vec3 &position, float radius,
                           const glm::vec3 &color, float intensity, bool castsShadow)
{
    // TODO Needs to be changed for better data structures
    assert(id >= 0 && ((unsigned int)id) < m_pointLights.size() && "Invalid scene object id");

    // Write data
    m_pointLights[id].m_position = position;
    m_pointLights[id].m_radius = radius;
    m_pointLights[id].m_color = color;
    m_pointLights[id].m_intensity = intensity;
    m_pointLights[id].m_castsShadow = castsShadow;
    return;
}

SceneObjectId CScene::createDirectionalLight(const glm::vec3 &direction, const glm::vec3 &color,
                                             float intensity, bool castsShadow)
{
    m_directionalLights.push_back(SSceneDirectionalLight(direction, color, intensity, castsShadow));
    return m_directionalLights.size() - 1;
}

bool CScene::getDirectionalLight(SceneObjectId id, glm::vec3 &direction, glm::vec3 &color,
                                 float &intensity, bool &castsShadow) const
{
    // TODO Needs to be changed for better data structures
    if (id < 0 || ((unsigned int)id) >= m_directionalLights.size())
    {
        return false;
    }

    // Write data
    direction = m_directionalLights.at(id).m_direction;
    color = m_directionalLights.at(id).m_color;
    intensity = m_directionalLights.at(id).m_intensity;
    castsShadow = m_directionalLights.at(id).m_castsShadow;
    return true;
}

void CScene::setDirectionalLight(SceneObjectId id, const glm::vec3 &direction,
                                 const glm::vec3 &color, float intensity, bool castsShadow)
{
    // TODO Needs to be changed for better data structures
    assert(id >= 0 && ((unsigned int)id) < m_directionalLights.size() && "Invalid scene object id");

    // Write data
    m_directionalLights[id].m_direction = direction;
    m_directionalLights[id].m_color = color;
    m_directionalLights[id].m_intensity = intensity;
    m_directionalLights[id].m_castsShadow = castsShadow;
    return;
}

void CScene::setAmbientLight(const glm::vec3 &color, float intensity)
{
    m_ambientColor = color;
    m_ambientIntensity = intensity;
}

bool CScene::getAmbientLight(glm::vec3 &color, float &intensity) const
{
    color = m_ambientColor;
    intensity = m_ambientIntensity;
    return true;
}

void CScene::getVisibleObjects(const ICamera &camera, ISceneQuery &query) const
{
    // Create frustum from camera matrices
    CFrustum viewFrustum;
    viewFrustum.setFromViewProjectionClipSpaceApproach(camera.getView(), camera.getProjection());

    // Hackyyy
    int culledObjectCount = 0;

    // Do not cull if disabled
    if (s_useViewFrustumCulling)
    {
        // TODO Occlusion culling, better data structure for objects
        for (unsigned int i = 0; i < m_objects.size(); ++i)
        {
            const auto &object = m_objects.at(i);
            // Return only objects with visibility flag set
            if (object.m_visible)
            {
                // Check the objects bounding sphere against the view frustum
                if (viewFrustum.isInsideOrIntersects(object.boundingSphere))
                {
                    // Object is (at least partially) visible, add to query result
                    // Counter variable is object id
                    query.addObject(i);
                }
                else
                {
                    ++culledObjectCount;
                }
            }
        }

        // Add visible point Lights
        for (unsigned int i = 0; i < m_pointLights.size(); ++i)
        {
            // Check light volume against view frustum for point light culling
            const auto &pointLight = m_pointLights.at(i);
            if (viewFrustum.isInsideOrIntersects(
                    CBoundingSphere(pointLight.m_position, pointLight.m_radius)))
            {
                // Counter variable is light id
                query.addPointLight(i);
            }
        }
    }
    camera.getFeatureInfo().culledObjectCount = culledObjectCount;

    // TODO Directional light culling?
    // For now add all directional lights
    for (unsigned int i = 0; i < m_directionalLights.size(); ++i)
    {
        // Counter variable is light id
        query.addDirectionalLight(i);
    }
    return;
}
