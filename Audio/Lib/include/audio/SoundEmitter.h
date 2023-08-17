#pragma once

#include <memory>

#include <AL/al.h>
#include <glm/glm.hpp>

#include "audio/Sound.h"
#include "audio/SoundPriority.h"

// Represents a sound source in 3d space
class SoundEmitter
{
   public:
    SoundEmitter();
    ~SoundEmitter();

    void play(const std::shared_ptr<Sound>& sound);
    void reset();

    void setPosition(const glm::vec3& position);
    const glm::vec3& getPosition() const;

    void setVelocity(const glm::vec3& velocity);
    const glm::vec3& getVelocity() const;

    SoundPriority getPriority() const;
    void setPriority(SoundPriority prio);

    float getVolume() const;
    void setVolume(float volume);

    bool getLooping() const;
    void setLooping(bool loop);

    float getRadius() const;
    void setRadius(float radius);

   private:
    ALuint source = 0;
    glm::vec3 position = glm::vec3(0.f);
    glm::vec3 velocity = glm::vec3(0.f);
    std::shared_ptr<Sound> sound;
    SoundPriority priority = SoundPriority::Low;
    float volume = 0.f;
    bool looping = false;
    float radius = 0.f;
};