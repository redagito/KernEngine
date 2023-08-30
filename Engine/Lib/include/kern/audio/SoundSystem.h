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

    // Delta time in seconds
    void update(float dtime);
   private:
    std::unordered_set<std::shared_ptr<SoundEmitter>> emitters;
    SoundListener listener;
    ALCdevice* device = nullptr;
    ALCcontext* context = nullptr;
    std::shared_ptr<SoundManager> manager;
};