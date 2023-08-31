#pragma once

#include <kern/foundation/IniFile.h>

#include <memory>
#include <string>
#include <vector>

class GlfwWindow;
class IRenderer;
class IScene;
class IResourceManager;
class IInputProvider;
class IControllableCamera;
class CameraController;
class IGraphicsResourceManager;
class AnimationWorld;
class SoundSystem;
class SoundEmitter;

/**
 * \brief Demo application class.
 */
class RTRDemo
{
   public:
    RTRDemo();
    ~RTRDemo();

    /**
     * \brief nitialize demo with config file.
     */
    int init(const std::string& configFile);

    /**
     * \brief Run demo.
     */
    int run();

   private:
    bool initWindow();
    bool initRenderer();
    bool initScene();

    IniFile m_config;

    std::shared_ptr<IResourceManager> m_resourceManager = nullptr; /**< Resource loader and manager. */
    std::shared_ptr<IGraphicsResourceManager> m_graphicsResourceManager =
        nullptr; /**< Resource manager for graphics resources. */

    std::shared_ptr<GlfwWindow> m_window = nullptr;
    std::shared_ptr<IInputProvider> m_inputProvider = nullptr;

    std::shared_ptr<IRenderer> m_renderer = nullptr;                /**< Active renderer. */
    std::shared_ptr<IRenderer> m_deferredRenderer = nullptr;        /**< Deferred renderer. */
    std::shared_ptr<IRenderer> m_forwardRenderer = nullptr;         /**< Forward renderer. */
    std::shared_ptr<IScene> m_scene = nullptr;                      /**< Active scene. */
    std::shared_ptr<IControllableCamera> m_camera = nullptr;        /**< Active camera. */
    std::shared_ptr<CameraController> m_cameraController = nullptr; /**< Camera controller. */

    // Animation
    std::shared_ptr<AnimationWorld> m_animationWorld;
    // Sound
    std::unique_ptr<SoundSystem> m_soundSystem;
    std::shared_ptr<SoundEmitter> m_bgMusic;
    std::shared_ptr<SoundEmitter> m_bgSfx;
};
