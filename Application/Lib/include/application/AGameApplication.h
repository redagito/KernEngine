#pragma once

#include <foundation/app/AApplication.h>
#include <memory>

class CGameSystem;
class IGraphicsSystem;
class IResourceManager;
class IInputProvider;
class CGlfwWindow;
class IGameState;

class AGameApplication : public AApplication
{
   public:
    virtual ~AGameApplication();

    bool initialize() override;

    int run() override;

   protected:
    void addGameState(const std::string& stateName, IGameState* state);

    virtual bool onInitialize() = 0;
    virtual std::string getInitialGameState() const = 0;

   private:
    /**
     * \brief Creates and initializes the application window.
     */
    bool initWindow(unsigned int width, unsigned int height, const std::string& title);

    // Stores and manages game states
    std::shared_ptr<CGameSystem> m_gameSystem;

    // Manages scenes and rendering
    std::shared_ptr<IGraphicsSystem> m_graphicsSystem;
    // Resource loading and lifetime management
    std::shared_ptr<IResourceManager> m_resourceManager;
    // Application window
    std::shared_ptr<CGlfwWindow> m_window;
    // Provides user input
    std::shared_ptr<IInputProvider> m_inputProvider;
};