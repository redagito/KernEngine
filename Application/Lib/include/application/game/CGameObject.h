#pragma once

#include <list>
#include <memory>

#include <glm/glm.hpp>

#include "graphics/proxy/CSceneObjectProxy.h"

#include "application/game/Message.h"

class IController;
class CCollidable;

/**
 * \brief Game object storing relevant data.
 */
class CGameObject
{
   public:
    /**
     * \brief Default constructor.
     */
    CGameObject();

    /**
     * \brief Resource cleanup.
     */
    ~CGameObject();

    /**
     * \brief Sets rotation.
     */
    void setRotation(const glm::vec3 &rotation);

    /**
     * \brief Sets position.
     */
    void setPosition(const glm::vec3 &translation);

    /**
     * \brief Sets scale.
     */
    void setScale(const glm::vec3 &scale);

    /**
     * \brief Adds controller to object.
     */
    void addController(const std::shared_ptr<IController> &controller);

    /**
     * \brief Set scene object.
     */
    void setSceneObject(CSceneObjectProxy *proxy);

    /**
     * \brief Returns rotation.
     */
    const glm::vec3 &getRotation() const;

    /**
     * \brief Returns position.
     */
    const glm::vec3 &getPosition() const;

    /**
     * \brief Returns forward direction.
     */
    const glm::vec3 &getForward() const;

    /**
     * \brief Returns scale.
     */
    const glm::vec3 &getScale() const;

    /**
     * \brief Updates object and returns state
     *
     * Updates the object with time in seconds.
     */
    void update(float dtime);

    /**
     * \brief Marks this object for deletion.
     */
    void markDeleted();

    /**
     * \brief Returns delete request flag.
     * True if object should be deleted.
     */
    bool isDeleteRequested() const;

    /**
     * \brief Send state change messages to all controllers in Object
     */
    void sendMessage(Message msg);

    /**
     * \brief Sets the collision entity for the object.
     */
    void setCollidable(CCollidable *entity);

    /**
     * \brief Returns collidable entity.
     */
    CCollidable *getCollidable() const;

    /**
     * \brief returns true if a collidable has been set for the game object and
     * false otherwise.
     */
    bool hasCollidable() const;

    /**
     * \brief Is the object considered to be dead.
     */
    bool isDead() const;

    /**
     * \brief Sets the object into death state.
     */
    void setDead();

   protected:
    std::unique_ptr<CSceneObjectProxy> m_sceneObject; /**< Scene object. */
    glm::vec3 m_forward = glm::vec3(0.f, 0.f, 1.f);   /**< Forward direction per default +z. */
    glm::vec3 m_rotation = glm::vec3(0.f);            /**< Rotation. */
    glm::vec3 m_position = glm::vec3(0.f);            /**< Position. */
    glm::vec3 m_scale = glm::vec3(1.f);               /**< Scale. */
    bool m_transformationChanged = false;             /**< Transformation dirty flag. */
    bool m_deleteRequested = false;                   /**< Deletion of this object is requested. */
    std::list<std::shared_ptr<IController>>
        m_controllers;                   /**< Controllers attached to the object. */
    CCollidable *m_collidable = nullptr; /**< Collidable object. */
    bool m_dead = false;                 /**< Death flag. */
};
