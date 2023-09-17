#pragma once

#include "kern/game/GameSystem.h"
#include "kern/game/IGameState.h"

#include "kern/graphics/animation/AnimationWorld.h"

#include "kern/graphics/ICamera.h"
#include "kern/graphics/IGraphicsSystem.h"
#include "kern/graphics/IScene.h"

#include "kern/graphics/input/IInputProvider.h"
#include "kern/resource/IResourceManager.h"

/**
 * \brief Title state, similar to demo state with static camera.
 */
class TitleState : public IGameState
{
   public:
    TitleState(const std::string &sceneFile);
    ~TitleState();

    bool init(IGraphicsSystem *graphicsSystem, IInputProvider *inputProvider, IResourceManager *resourceManager,
              SoundSystem *soundSystem);

    void onEnter();
    bool update(float dtime);
    void onExit();
    const std::string &getNextState() const;

   private:
    float m_fadeInTime;
    std::string m_sceneFile;
    IInputProvider *m_inputProvider = nullptr;
    std::shared_ptr<ICamera> m_camera = nullptr;
    IScene *m_scene = nullptr;
    IGraphicsSystem *m_graphicsSystem = nullptr;
    IResourceManager *m_resourceManager = nullptr;
    AnimationWorld m_animationWorld;
    std::string m_nextState; /**< Next state. */
};