#pragma once

#include <vector>

#include "graphics/graphics/ISceneQuery.h"

/**
 * \brief Scene query implementation.
 */
class CSceneQuery : public ISceneQuery
{
   public:
    /**
     * \brief Sets initial storage.
     */
    CSceneQuery(unsigned int objectStorage = 200, unsigned int lightStorage = 100);

    void addObject(SceneObjectId id);

    bool hasNextObject() const;

    SceneObjectId getNextObject();

    void addPointLight(SceneObjectId id);

    bool hasNextPointLight() const;

    SceneObjectId getNextPointLight();

    void addDirectionalLight(SceneObjectId id);

    bool hasNextDirectionalLight() const;

    SceneObjectId getNextDirectionalLight();

   private:
    unsigned int m_nextObjectIndex = 0;
    unsigned int m_nextPointLightIndex = 0;
    unsigned int m_nextDirectionalLightIndex = 0;
    std::vector<SceneObjectId> m_visibleObjects;           /**< Visible objects. */
    std::vector<SceneObjectId> m_visiblePointLights;       /**< Visible lights. */
    std::vector<SceneObjectId> m_visibleDirectionalLights; /**< Visible directional lights. */
};
