#pragma once

#include <glm/glm.hpp>

#include "kern/game/IGameObjectController.h"
#include "kern/game/Message.h"

#include "kern/graphics/camera/IControllableCamera.h"

/**
 * \brief Controls camera movement based on the attached game object.
 */
class CameraController : public IGameObjectController
{
   public:
    /**
     * \brief Sets camera.
     */
    CameraController(IControllableCamera *camera);

    /**
     * \brief Destructor for cleanup.
     */
    ~CameraController();

    /**
     * \brief On attach to object.
     */
    void attach(GameObject *object);

    /**
     * \brief On detaching from game object.
     */
    void detach();

    /**
     * \brief Updates controller.
     */
    void update(float dtime);

    /**
     * \brief Sets active state.
     */
    void setActive(bool state);

    /**
     * \brief Racive state from other objects
     */
    void receiveMessage(Message msg);

   private:
    IControllableCamera *m_camera = nullptr; /**< Controlled camera. */
    GameObject *m_object = nullptr;         /**< Controlled game object. */
    bool m_active = true;                    /**< Active state flag. */
    glm::vec3 m_prevLookAt;
    bool m_first = true;
    float m_speed = 5.f;
};