#pragma once

#include <unordered_set>

#include <AL/al.h>
#include <AL/alc.h>

#include <glm/glm.hpp>

#include "kern/audio/SoundManager.h"
#include "kern/audio/SoundEmitter.h"
#include "kern/audio/SoundListener.h"

class SoundSystem
{
   public:
    SoundSystem(const std::string& directory);
    ~SoundSystem();

    SoundListener& getListener();
    std::shared_ptr<SoundManager>& getManager();
    std::shared_ptr<SoundEmitter> createEmitter();
    std::shared_ptr<SoundEmitter> createEmitter(const std::shared_ptr<Sound> sound);

    // Global background sound emitter
    std::shared_ptr<SoundEmitter>& getGlobalSoundEmitter();

    // Delta time in seconds
    void update(float dtime);
   private:
    // Global background sound emitter
    std::shared_ptr<SoundEmitter> bgmEmitter;
    std::unordered_set<std::shared_ptr<SoundEmitter>> emitters;

    // The current listener
    SoundListener listener;
    ALCdevice* device = nullptr;
    ALCcontext* context = nullptr;

    // Sound manager
    std::shared_ptr<SoundManager> manager;
};