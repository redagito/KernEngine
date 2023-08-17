#pragma once

#include "audio/Sound.h"

#include <string>
#include <memory>
#include <unordered_map>

class SoundManager
{
   public:
    SoundManager(const std::string& soundDirectory);
    // Registers a sound name to filename mapping
    void registerSound(const std::string& name, const std::string& fileName);
    // Retrieves sound by previously registered name
    std::shared_ptr<Sound> getSound(const std::string& name);

    private:
    std::shared_ptr<Sound> loadFromFile(const std::string& fileName);
    std::shared_ptr<Sound> loadFromWav(const std::string& fileName);

    std::string directory;
    std::unordered_map<std::string, std::string> namesToFiles;
    // File name to sound
    std::unordered_map<std::string, std::shared_ptr<Sound>> filesToSounds;
};