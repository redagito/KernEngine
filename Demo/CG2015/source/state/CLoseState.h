#pragma once

#include "graphics/animation/CAnimationWorld.h"

#include "graphics/ICamera.h"
#include "graphics/IGraphicsSystem.h"
#include "graphics/IScene.h"

#include "graphics/input/IInputProvider.h"
#include "graphics/resource/IResourceManager.h"

#include "application/game/CGameSystem.h"
#include "application/game/IGameState.h"

/**
 * \brief Lose state, similar to demo state with static camera.
 */
class CLoseState : public IGameState
{
   public:
    CLoseState(const std::string &sceneFile);
    ~CLoseState();

    bool init(IGraphicsSystem *graphicsSystem, IInputProvider *inputProvider,
              IResourceManager *resourceManager);

    void onEnter();
    bool update(float dtime);
    void onExit();
    const std::string &getNextState() const;

   private:
    float m_fadeInTime = 0.0;
    std::string m_sceneFile;
    IInputProvider *m_inputProvider = nullptr;
    std::shared_ptr<ICamera> m_camera = nullptr;
    IScene *m_scene = nullptr;
    IGraphicsSystem *m_graphicsSystem = nullptr;
    IResourceManager *m_resourceManager = nullptr;
    CAnimationWorld m_animationWorld;
    std::string m_nextState;             /**< Next state. */
    CGameSystem *m_gameSystem = nullptr; /**< Game system. */
};
