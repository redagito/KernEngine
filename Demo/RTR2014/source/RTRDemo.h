#pragma once

#include <memory>
#include <string>
#include <vector>

#include <kern/foundation/IniFile.h>

// GLFW
struct GLFWwindow;

// Graphics
class GlfwWindow;
class IRenderer;
class IScene;

// Resource
class IResourceManager;

// Animation
class IInputProvider;
class IControllableCamera;
class CameraController;
class IGraphicsResourceManager;
class AnimationWorld;

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

    void updateAnimation(float timeDiff);

    IniFile m_config;

    std::shared_ptr<IResourceManager> m_resourceManager =
        nullptr; /**< Resource loader and manager. */
    std::shared_ptr<IGraphicsResourceManager> m_graphicsResourceManager =
        nullptr; /**< Resource manager for graphics resources. */

    std::shared_ptr<GlfwWindow> m_window = nullptr;
    std::shared_ptr<IInputProvider> m_inputProvider = nullptr;

    std::shared_ptr<IRenderer> m_renderer = nullptr;                 /**< Active renderer. */
    std::shared_ptr<IRenderer> m_deferredRenderer = nullptr;         /**< Deferred renderer. */
    std::shared_ptr<IRenderer> m_forwardRenderer = nullptr;          /**< Forward renderer. */
    std::shared_ptr<IScene> m_scene = nullptr;                       /**< Active scene. */
    std::shared_ptr<IControllableCamera> m_camera = nullptr;         /**< Active camera. */
    std::shared_ptr<CameraController> m_cameraController = nullptr; /**< Camera controller. */

    // Animation
    std::shared_ptr<AnimationWorld> m_animationWorld;
};
