#include "CameraController.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <fmtlog/fmtlog.h>
#include <kern/foundation/JsonDeserialize.h>
#include <kern/foundation/JsonUtil.h>
#include <kern/graphics/camera/IControllableCamera.h>
#include <kern/graphics/input/IInputProvider.h>

#include <algorithm>
#include <fstream>
#include <nlohmann/json.hpp>

CameraController::CameraController() {}

CameraController::~CameraController()
{
    if (m_inputProvider != nullptr)
    {
        m_inputProvider->removeInputListener(this);
    }
}

void CameraController::setCamera(std::shared_ptr<IControllableCamera> camera) { m_camera = camera; }

void CameraController::setInputProvider(IInputProvider* provider)
{
    if (m_inputProvider != nullptr)
    {
        m_inputProvider->removeInputListener(this);
    }

    m_inputProvider = provider;

    if (m_inputProvider != nullptr)
    {
        m_inputProvider->addInputListener(this);
    }
}

bool CameraController::loadSequence(std::string file)
{
    m_sequenceTime = 0;
    m_sequencePoints.clear();

    nlohmann::json root;

    // Load scene file
    std::ifstream ifs(file);
    if (!ifs.is_open())
    {
        loge("Failed to open json file {}.", file.c_str());
        return false;
    }

    // Parse json data
    try
    {
        ifs >> root;
    }
    catch (const std::exception& e)
    {
        loge("Failed to load scene file {} with error {}.", file.c_str(), e.what());
        return false;
    }
    // Read done, close file
    ifs.close();

    // Node empty?
    if (root.find("positions") == root.end())
    {
        logi("Missing or empty 'positions' node. No scene objects are loaded.");
        return true;
    }

    nlohmann::json node = root.at("positions");

    // Node is array type
    if (!node.is_array())
    {
        loge("The node 'positions' must be array type.");
        return false;
    }

    bool success = true;

    int i = 0;
    for (auto it : node)
    {
        glm::vec3 position;
        glm::vec3 orientation;
        float timestamp;
        bool fxaaActive;
        bool fogActive;

        if (!load(it, "position", position))
        {
            loge("Failed loading node 'position' for element #{}.", i);
            success = false;
            break;
        }

        if (!load(it, "orientation", orientation))
        {
            loge("Failed loading node 'orientation' for element #{}.", i);
            success = false;
            break;
        }

        if (!load(it, "timestamp", timestamp))
        {
            loge("Failed loading node 'timestamp' for element #{}.", i);
            success = false;
            break;
        }

        if (!load(it, "fxaa", fxaaActive))
        {
            loge("Failed loading node 'fxaa' for element #{}.", i);
            success = false;
            break;
        }

        if (!load(it, "fog", fogActive))
        {
            loge("Failed loading node 'fog' for element #{}.", i);
            success = false;
            break;
        }

        SequencePoint sp{position, glm::normalize(orientation), timestamp, fxaaActive, fogActive};
        m_sequencePoints.push_back(sp);
        ++i;
    }

    m_isRunningSequence = success;
    return success;
}

void CameraController::animate(float dt)
{
    if (m_inputProvider != nullptr && m_camera != nullptr)
    {
        animateFeatures(dt);

        if (m_isRunningSequence)
        {
            animateSequence(dt);
        }
        else
        {
            animateManual(dt);
        }
    }
}

void CameraController::animateFeatures(float dt)
{
    // TODO
}

void CameraController::animateSequence(float dt)
{
    if (m_inputProvider->isKeyPressed(GLFW_KEY_C))
    {
        m_isRunningSequence = false;
        return;
    }

    float timeModifier = 1.0f;
    if (m_inputProvider->isKeyPressed(GLFW_KEY_B))
    {
        timeModifier = -2.0f;
    }
    if (m_inputProvider->isKeyPressed(GLFW_KEY_N))
    {
        timeModifier = 4.0f;
    }
    if (m_inputProvider->isKeyPressed(GLFW_KEY_M))
    {
        timeModifier = 8.0f;
    }

    m_sequenceTime += (dt * timeModifier);

    if (m_sequenceTime > m_sequencePoints.back().timestamp)
    {
        m_isRunningSequence = false;
        return;
    }

    auto it = std::find_if(m_sequencePoints.begin(), m_sequencePoints.end(),
                           [&](const SequencePoint& s) { return s.timestamp >= m_sequenceTime; });

    if (it == m_sequencePoints.begin())
    {
        // not started yet
        return;
    }

    SequencePoint after = *it;
    SequencePoint before = *(--it);

    float ix = (m_sequenceTime - before.timestamp) / (after.timestamp - before.timestamp);
    glm::vec3 cameraPosition = before.position * (1.0f - ix) + after.position * ix;
    glm::vec3 cameraOrientation = before.orientation * (1.0f - ix) + after.orientation * ix;

    m_camera->lookAt(cameraPosition, cameraPosition + cameraOrientation, glm::vec3(0, 1, 0));
    m_camera->getFeatureInfoForWrite().fogType = (before.fogActive ? FogType::Exp2 : FogType::None);
    m_camera->getFeatureInfoForWrite().fxaaActive = before.fxaaActive;
}

void CameraController::animateManual(float dt)
{
    float walkingModifier = 1.0f;
    if (m_inputProvider->isKeyPressed(GLFW_KEY_SPACE))
    {
        walkingModifier = 6.0f;
    }

    if (m_inputProvider->isKeyPressed(GLFW_KEY_W))
    {
        m_camera->moveForward(dt * 2.f * m_speed * walkingModifier);
    }
    if (m_inputProvider->isKeyPressed(GLFW_KEY_S))
    {
        m_camera->moveForward(dt * -2.f * m_speed * walkingModifier);
    }
    if (m_inputProvider->isKeyPressed(GLFW_KEY_A))
    {
        m_camera->moveRight(dt * 2.f * m_speed * walkingModifier);
    }
    if (m_inputProvider->isKeyPressed(GLFW_KEY_D))
    {
        m_camera->moveRight(dt * -2.f * m_speed * walkingModifier);
    }

    if (m_inputProvider->isKeyPressed(GLFW_KEY_Q))
    {
        m_camera->moveUp(dt * -2.f * m_speed * walkingModifier);
    }
    if (m_inputProvider->isKeyPressed(GLFW_KEY_E))
    {
        m_camera->moveUp(dt * 2.f * m_speed * walkingModifier);
    }

    if (m_inputProvider->isKeyPressed(GLFW_KEY_UP))
    {
        m_camera->pitch(-dt * m_speed);
    }

    if (m_inputProvider->isKeyPressed(GLFW_KEY_DOWN))
    {
        m_camera->pitch(dt * m_speed);
    }

    if (m_inputProvider->isKeyPressed(GLFW_KEY_RIGHT))
    {
        m_camera->yaw(-dt * m_speed);
    }

    if (m_inputProvider->isKeyPressed(GLFW_KEY_LEFT))
    {
        m_camera->yaw(dt * m_speed);
    }
}

void CameraController::handleKeyEvent(EKeyEventType type, int keyCode)
{
    if (type != EKeyEventType::KEY_PRESSED)
    {
        return;
    }

    SFeatureInfo& info = m_camera->getFeatureInfoForWrite();

    if (keyCode == GLFW_KEY_R)
    {
        switch (info.renderMode)
        {
        case RenderMode::Final:
            info.renderMode = RenderMode::Color;
            break;
        case RenderMode::Color:
            info.renderMode = RenderMode::Depth;
            break;
        case RenderMode::Depth:
            info.renderMode = RenderMode::Lights;
            break;
        case RenderMode::Lights:
            info.renderMode = RenderMode::Normals;
            break;
        case RenderMode::Normals:
            info.renderMode = RenderMode::GodRay;
            break;
        case RenderMode::GodRay:
            info.renderMode = RenderMode::Final;
            break;
        default:
            info.renderMode = RenderMode::Final;
            break;
        }
    }

    if (keyCode == GLFW_KEY_T)
    {
        switch (info.fogType)
        {
        case FogType::None:
            info.fogType = FogType::Linear;
            break;
        case FogType::Linear:
            info.fogType = FogType::Exp;
            break;
        case FogType::Exp:
            info.fogType = FogType::Exp2;
            break;
        case FogType::Exp2:
            info.fogType = FogType::None;
            break;
        default:
            info.fogType = FogType::None;
            break;
        }
    }

    if (keyCode == GLFW_KEY_Z)
    {
        info.shadowsActive = !info.shadowsActive;
    }

    if (keyCode == GLFW_KEY_U)
    {
        info.fxaaActive = !info.fxaaActive;
    }

    if (keyCode == GLFW_KEY_I)
    {
        info.dofActive = !info.dofActive;
    }

    if (keyCode == GLFW_KEY_O)
    {
        info.normalMappingActive = !info.normalMappingActive;
    }

    if (keyCode == GLFW_KEY_P)
    {
        info.godRayActive = !info.godRayActive;
    }
}

void CameraController::handleMouseMovementEvent(int x, int y)
{
    m_camera->pitch((y - m_lastY) * m_speed / 1000);
    m_camera->yaw(-(x - m_lastX) * m_speed / 1000);

    m_lastX = x;
    m_lastY = y;
}

void CameraController::handleMouseButtonEvent(EMouseButtonEventType type, int buttonCode)
{
    // TODO
}

void CameraController::handleResizeEvent(int width, int height)
{
    m_camera->setProjection(m_camera->getFieldOfView(), (float)width / height, m_camera->getZNear(),
                            m_camera->getZFar());
}