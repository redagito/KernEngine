#pragma once

#include <memory>
#include <string>
#include <vector>

#include "IEngine.h"
#include "SEngineConfig.h"

// Resource
class IResourceManager;

// Game
class GameSystem;

// Graphics
class IGraphicsSystem;
class GlfwWindow;

// Input
class IInputProvider;

/**
 * \brief Demo application class.
 */
class CEngine : public IEngine
{
   public:
    CEngine();
    ~CEngine();

    /**
     * \brief Initialize demo from config file.
     */
    bool init(const char *configFile);

    /**
     * \brief Run demo.
     */
    void run();

   private:
    /**
     * \brief Creates and initializes the application window.
     */
    bool initWindow(unsigned int width, unsigned int height, const std::string &title);

    /**
     * \brief Creates and initializes the game system.
     */
    bool initGameSystem(const std::string &gameFile);

    /**
     * \brief Creates and initializes game system in demo mode.
     * TODO Should be considered legacy and removed later.
     */
    bool initDemo(const std::string &sceneFile);

    // Engine configuration
    SEngineConfig m_engineConfig;

    // TODO Should use interface instead of concrete class.
    std::shared_ptr<GameSystem> m_gameSystem;           /**< Game system. */
    std::shared_ptr<IGraphicsSystem> m_graphicsSystem;   /**< Graphics system. */
    std::shared_ptr<IResourceManager> m_resourceManager; /**< Resource loader and manager. */

    // TODO Should use interface instead of concrete class
    std::shared_ptr<GlfwWindow> m_window;           /**< Application window. */
    std::shared_ptr<IInputProvider> m_inputProvider; /**< User input provider. */
};
