#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "kern/audio/Sound.h"

class SoundManager
{
   public:
    SoundManager(const std::string& soundDirectory);
    // Registers a sound name to filename mapping
    void registerSound(const std::string& name, const std::string& fileName);
    // Retrieves sound by previously registered name
    std::shared_ptr<Sound> getSound(const std::string& name);
    bool hasSound(const std::string& name) const;

   private:
    std::shared_ptr<Sound> loadFromFile(const std::string& fileName);
    std::shared_ptr<Sound> loadFromWav(const std::string& fileName);
    std::shared_ptr<Sound> loadFromMp3(const std::string& fileName);

    std::string directory;
    std::unordered_map<std::string, std::string> namesToFiles;
    // File name to sound
    std::unordered_map<std::string, std::shared_ptr<Sound>> filesToSounds;
};