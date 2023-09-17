#pragma once

#include "kern/game/GameSystem.h"
#include "kern/game/IGameState.h"
#include "kern/graphics/ICamera.h"
#include "kern/graphics/IGraphicsSystem.h"
#include "kern/graphics/IScene.h"
#include "kern/graphics/animation/AnimationWorld.h"
#include "kern/resource/IResourceManager.h"

/**
 * \brief Load state.
 *
 * Just for showing off some effects.
 */
class LoadState : public IGameState
{
   public:
    LoadState(const std::string &sceneFile, float transitionTime);
    ~LoadState();

    bool init(IGraphicsSystem *graphicsSystem, IInputProvider *inputProvider, IResourceManager *resourceManager,
              SoundSystem *soundSystem) override;

    void onEnter();
    bool update(float dtime);
    void onExit();
    const std::string &getNextState() const;

   private:
    float m_transitionTime = 0.f;
    float m_timeLeft = 0.f;
    std::string m_sceneFile;
    std::shared_ptr<ICamera> m_camera;
    IScene *m_scene = nullptr;
    IGraphicsSystem *m_graphicsSystem = nullptr;
    IResourceManager *m_resourceManager = nullptr;
    AnimationWorld m_animationWorld;
    std::string m_nextState;            /**< Next state. */
    GameSystem *m_gameSystem = nullptr; /**< Game system. */
    std::shared_ptr<Sound> m_bgmSound;
    std::shared_ptr<SoundEmitter> m_bgmEmitter;
};