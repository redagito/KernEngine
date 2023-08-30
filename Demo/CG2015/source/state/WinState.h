#pragma once

#include "kern/game/GameSystem.h"
#include "kern/game/IGameState.h"

#include "kern/graphics/animation/AnimationWorld.h"
#include "kern/graphics/ICamera.h"
#include "kern/graphics/IGraphicsSystem.h"
#include "kern/graphics/IScene.h"
#include "kern/graphics/input/IInputProvider.h"
#include "kern/graphics/resource/IResourceManager.h"

/**
 * \brief Lose state, similar to demo state with static camera.
 */
class WinState : public IGameState
{
   public:
    WinState(const std::string &sceneFile);
    ~WinState();

    bool init(IGraphicsSystem *graphicsSystem, IInputProvider *inputProvider,
              IResourceManager *resourceManager);

    void onEnter();
    bool update(float dtime);
    void onExit();
    const std::string &getNextState() const;

   private:
    float m_fadeInTime = 1.f;
    std::string m_sceneFile;
    IInputProvider *m_inputProvider = nullptr;
    std::shared_ptr<ICamera> m_camera = nullptr;
    IScene *m_scene = nullptr;
    IGraphicsSystem *m_graphicsSystem = nullptr;
    IResourceManager *m_resourceManager = nullptr;
    AnimationWorld m_animationWorld;
    std::string m_nextState; /**< Next state. */
};
