#include "control/CCameraController.h"

#include "application/game/CGameObject.h"

CCameraController::CCameraController(IControllableCamera *camera) : m_camera(camera) { return; }

CCameraController::~CCameraController() {}

void CCameraController::attach(CGameObject *object) { m_object = object; }

void CCameraController::detach()
{
    m_object = nullptr;
    m_camera = nullptr;
}

void CCameraController::update(float dtime)
{
    if (m_active && m_object != nullptr && m_camera != nullptr)
    {
        // Update camera movement
        glm::vec3 pos = m_object->getPosition();
        glm::vec3 forward = m_object->getForward();

        // Where the camera should be
        glm::vec3 camPos(pos.x - forward.x * 8.f, pos.y * 0.95f + 3.f, pos.z - forward.z * 8.f);
        m_camera->setPosition(camPos);

        // Init on first update
        if (m_first)
        {
            m_first = false;
            m_prevLookAt = pos + forward * 3.f;
        }
        //
        glm::vec3 targetLookAt = pos + forward * 3.f;

        float distance = glm::distance(targetLookAt, m_prevLookAt);
        glm::vec3 d = glm::normalize(targetLookAt - m_prevLookAt);

        if (distance > 5.f)
        {
            m_speed = 26.f;
        }
        if (distance < 2.f)
        {
            m_speed = 20.f;
        }
        float move = m_speed * dtime;
        if (move >= distance || distance > 8.f)
        {
            m_prevLookAt = targetLookAt;
        }
        else
        {
            m_prevLookAt += d * move;
        }

        m_camera->lookAt(m_prevLookAt, glm::vec3(0.f, 1.f, 0.f));
    }
}

void CCameraController::setActive(bool state) { m_active = state; }

void CCameraController::receiveMessage(Message msg) {}