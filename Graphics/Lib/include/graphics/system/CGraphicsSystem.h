#pragma once

#include <list>
#include <memory>

#include <foundation/debug/CDebugInfo.h>

#include "graphics/CDebugInfoDisplay.h"
#include "graphics/IGraphicsResourceManager.h"
#include "graphics/IGraphicsSystem.h"
#include "graphics/IRenderer.h"
#include "graphics/IScene.h"
#include "graphics/system/SGraphicsSettings.h"

class CGraphicsSystem : public IGraphicsSystem
{
   public:
    CGraphicsSystem();
    ~CGraphicsSystem();

    bool init(IResourceManager &manager);
    void setActiveRenderer(const std::string &renderer);

    IScene *createScene();
    void setActiveScene(IScene *scene);

    ICamera *createCamera();
    void setActiveCamera(const ICamera *camera);

    void toggleDebugOverlay();
    void toggleDebugOverlayTransparency();
    void toggleWireframeMode();
    void toggleViewFrustumCulling();

    void draw(IWindow &window);

   private:
    bool m_wireframeMode = false; /**< Wireframe mode flag. */

    bool m_drawDebugOverlay = false;
    bool m_transparentDebugOverlay = true;
    CDebugInfo m_debugInfo; /**< Debug info data. */
    std::unique_ptr<CDebugInfoDisplay> m_debugInfoDisplay =
        nullptr; /**< Debug info overlay renderer. */

    // Debug info
    double m_lastCallTime = 0.f;
    double m_timeAccum = 0.f; /**< Time accumulator. */
    unsigned int m_currentFrameCount = 0;
    unsigned int m_lastFrameCount = 0;

    std::list<std::unique_ptr<IScene>> m_scenes;
    IScene *m_activeScene = nullptr;

    std::list<std::unique_ptr<ICamera>> m_cameras;
    const ICamera *m_activeCamera = nullptr;

    std::unique_ptr<IRenderer> m_deferredRenderer = nullptr;
    std::unique_ptr<IRenderer> m_forwardRenderer = nullptr;
    IRenderer *m_activeRenderer = nullptr;

    std::unique_ptr<IGraphicsResourceManager> m_resourceManager = nullptr;
};
