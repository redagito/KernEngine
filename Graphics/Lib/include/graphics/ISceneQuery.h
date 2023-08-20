#pragma once

#include "graphics/SceneConfig.h"

/**
 * \brief Scene query interface class.
 * Provides a spatial query interface for scene objects.
 */
class ISceneQuery
{
   public:
    /**
     * \brief Virtual destructor for interface class.
     */
    virtual ~ISceneQuery();

    /**
     * \brief Adds scene object id to query.
     */
    virtual void addObject(SceneObjectId id) = 0;

    /**
     * \brief Has visible objects to return.
     */
    virtual bool hasNextObject() const = 0;

    /**
     * \brief Returns id of next visible object.
     */
    virtual SceneObjectId getNextObject() = 0;

    /**
     * \brief Adds point light id to query.
     */
    virtual void addPointLight(SceneObjectId id) = 0;

    /**
     * \brief Has visible point lights to return.
     */
    virtual bool hasNextPointLight() const = 0;

    /**
     * \brief Returns id of next visible point light.
     */
    virtual SceneObjectId getNextPointLight() = 0;

    /**
     * \brief Adds directional light to query.
     */
    virtual void addDirectionalLight(SceneObjectId id) = 0;

    /**
     * \brief Has visible directional light to return.
     */
    virtual bool hasNextDirectionalLight() const = 0;

    /**
     * \brief Returns id of next visible directional light.
     */
    virtual SceneObjectId getNextDirectionalLight() = 0;
};
