#include "kern/audio/SoundSystem.h"

#include <stdexcept>

SoundSystem::SoundSystem(const std::string& directory) : manager(std::make_shared<SoundManager>(directory))
{
    // Default device
    device = alcOpenDevice(nullptr);
    if (device == nullptr)
    {
        throw std::runtime_error("Device is null");
    }

    context = alcCreateContext(device, nullptr);
    if (context == nullptr)
    {
        throw std::runtime_error("Context is null");
    }

    alcMakeContextCurrent(context);
}

SoundSystem::~SoundSystem()
{
    // Cleanup in order
    emitters.clear();
    manager = nullptr;
    alcMakeContextCurrent(nullptr);

    alcDestroyContext(context);
    context = nullptr;

    alcCloseDevice(device);
    device = nullptr;
}

SoundListener& SoundSystem::getListener() { return listener; }

std::shared_ptr<SoundManager>& SoundSystem::getManager() { return manager; }

void SoundSystem::update(float dtime) { return; }

std::shared_ptr<SoundEmitter> SoundSystem::createEmitter()
{
    auto emitter = std::make_shared<SoundEmitter>();
    emitters.insert(emitter);
    return emitter;
}