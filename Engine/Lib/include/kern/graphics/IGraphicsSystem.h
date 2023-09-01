#pragma once

#include <string>

class IScene;
class Window;
class IResourceManager;
class ICamera;

class IGraphicsSystem
{
   public:
    virtual ~IGraphicsSystem();

    /**
     * \brief Initializes graphics system and sets resource manager listener.
     */
    virtual bool init(IResourceManager &manager) = 0;

    virtual void setActiveRenderer(const std::string &renderer) = 0;

    /**
     * \brief Create scene.
     */
    virtual IScene *createScene() = 0;

    /**
     * \brief Set active scene.
     */
    virtual void setActiveScene(IScene *scene) = 0;

    virtual ICamera *createCamera() = 0;

    /**
     * \brief Sets active camera.
     */
    virtual void setActiveCamera(const ICamera *camera) = 0;

    /**
     * \brief Toggles debug overlay on/off.
     *
     * Default is off.
     */
    virtual void toggleDebugOverlay() = 0;

    /**
     * \brief Toggles debug overlay transparency on/off.
     *
     * Default is on.
     */
    virtual void toggleDebugOverlayTransparency() = 0;

    /**
     * \brief Toggles wireframe mode on/off.
     *
     * Default is off.
     */
    virtual void toggleWireframeMode() = 0;

    virtual void toggleViewFrustumCulling() = 0;

    /**
     * \brief Draws active scene.
     */
    virtual void draw(Window &window) = 0;
};