#pragma once

#include "kern/game/IGameState.h"

#include "kern/graphics/animation/AnimationWorld.h"

#include "kern/graphics/IGraphicsSystem.h"
#include "kern/graphics/IScene.h"
#include "kern/graphics/camera/IControllableCamera.h"

#include "FreeFlightCameraController.h"

#include "kern/resource/IResourceManager.h"

/**
 * \brief Represents the demo state of the engine.
 *
 * The demo state is used for graphical demos and has no game logic.
 * A scene is loaded form a file and rendered with a freely movable camera
 * controlled by the user.
 */
class DemoState : public IGameState
{
   public:
    DemoState(const std::string &sceneFile);
    ~DemoState();

    bool init(IGraphicsSystem *graphicsSystem, IInputProvider *inputProvider, IResourceManager *resourceManager,
              SoundSystem *soundSystem);

    void onEnter();
    bool update(float dtime);
    void onExit();
    const std::string &getNextState() const;

   private:
    std::string m_sceneFile;

    std::shared_ptr<FreeFlightCameraController> m_cameraController = nullptr;
    std::shared_ptr<IControllableCamera> m_camera = nullptr;
    IScene *m_scene = nullptr;
    IGraphicsSystem *m_graphicsSystem = nullptr;
    AnimationWorld m_animationWorld;
};