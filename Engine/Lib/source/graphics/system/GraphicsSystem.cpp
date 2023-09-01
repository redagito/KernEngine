#include "kern/graphics/system/GraphicsSystem.h"

#include <fmtlog/fmtlog.h>

#include "kern/foundation/Time.h"
#include "kern/graphics/camera/Camera.h"
#include "kern/graphics/renderer/DeferredRenderer.h"
#include "kern/graphics/renderer/ForwardRenderer.h"
#include "kern/graphics/renderer/RendererCoreConfig.h"
#include "kern/graphics/resource/GraphicsResourceManager.h"
#include "kern/resource/IResourceManager.h"
#include "kern/graphics/scene/Scene.h"

GraphicsSystem::GraphicsSystem() {}

GraphicsSystem::~GraphicsSystem() {}

bool GraphicsSystem::init(IResourceManager &manager)
{
    // Create graphics resource manager and register as listener to resource
    // manager
    GraphicsResourceManager *grm = new GraphicsResourceManager;
    manager.addResourceListener(grm);
    m_resourceManager.reset(grm);

    // Create renderers
    // Deferred renderer
    m_deferredRenderer.reset(DeferredRenderer::create(manager));
    if (m_deferredRenderer == nullptr)
    {
        loge("Failed to initialize deferred renderer.");
        return false;
    }

    // Forward renderer
    m_forwardRenderer.reset(ForwardRenderer::create(manager));
    if (m_forwardRenderer == nullptr)
    {
        loge("Failed to initialize forward renderer.");
        return false;
    }

    // Set default active renderer
    m_activeRenderer = m_deferredRenderer.get();

    // Init ok
    return true;
}

void GraphicsSystem::setActiveRenderer(const std::string &renderer)
{
    // Deferred renderer does not support wireframe mode, force forward renderer
    if (renderer == "deferred" && !m_wireframeMode)
    {
        m_activeRenderer = m_deferredRenderer.get();
    }
    else if (renderer == "forward")
    {
        m_activeRenderer = m_forwardRenderer.get();
    }
    else
    {
        // Use default
        logw("Invalid or unknown renderer type {}. Setting renderer to default.", renderer.c_str());
        m_activeRenderer = m_deferredRenderer.get();
    }
}

IScene *GraphicsSystem::createScene()
{
    // TODO Refactor
    IScene *scene = new Scene(m_resourceManager.get());
    m_scenes.push_back(std::unique_ptr<IScene>(scene));
    return scene;
}

void GraphicsSystem::setActiveScene(IScene *scene) { m_activeScene = scene; }

ICamera *GraphicsSystem::createCamera()
{
    ICamera *camera = new Camera;
    m_cameras.push_back(std::unique_ptr<ICamera>(camera));
    return camera;
}

void GraphicsSystem::setActiveCamera(const ICamera *camera) { m_activeCamera = camera; }

void GraphicsSystem::toggleDebugOverlay() { m_drawDebugOverlay = !m_drawDebugOverlay; }

void GraphicsSystem::toggleDebugOverlayTransparency() { m_transparentDebugOverlay = !m_transparentDebugOverlay; }

void GraphicsSystem::toggleWireframeMode()
{
    m_wireframeMode = !m_wireframeMode;
    if (m_wireframeMode)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        // Only supported for forward renderer
        // TODO Implement rendering parameters for IRenderer interface
        // Force usage of forward renderer
        setActiveRenderer("forward");
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        // Reset renderer
        setActiveRenderer("deferred");
    }
}

void GraphicsSystem::toggleViewFrustumCulling()
{
    Scene::setViewFrustumCulling(!Scene::getViewFrustumCulling());
    if (Scene::getViewFrustumCulling())
    {
        logi("View frustum culling enabled.");
    }
    else
    {
        logi("View frustum culling disabled.");
    }
}

void GraphicsSystem::draw(Window &window)
{
    // Current calling time
    double callTime = getTime();
    double timeDiff = 0.0;

    // Check for first call to draw
    // TODO Actually necessary?
    if (m_lastCallTime == 0.0)
    {
        m_lastCallTime = callTime;
    }
    else
    {
        // Time difference between calls
        timeDiff = callTime - m_lastCallTime;
        // Set last call time to current call time
        m_lastCallTime = callTime;
    }

    // Update time accumulator
    m_timeAccum += timeDiff;
    // Check if time accumulator passed 1 sec
    if (m_timeAccum >= 1.0)
    {
        // Set last frame count for fps calculation
        m_lastFrameCount = m_currentFrameCount;
        // Reset current frame count
        m_currentFrameCount = 0;
        // Reset time accumulator
        m_timeAccum = 0.0;
    }

    // Update frame count
    ++m_currentFrameCount;

    // Scene draw
    if (m_activeScene != nullptr && m_activeCamera != nullptr)
    {
        m_activeRenderer->draw(*m_activeScene, *m_activeCamera, window, *m_resourceManager);
    }
}