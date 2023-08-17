#include "audio/SoundManager.h"

#include <fstream>
#include <stdexcept>

struct WavHeader
{
    int16_t format = 0;
    int16_t channels = 0;
    // Samples per second
    uint32_t srate = 0;
    // Average bytes per second
    uint32_t bps = 0;
    // Block alignment
    int16_t balign = 0;
    // Bits per sample?
    int16_t amp = 0;
};

SoundManager::SoundManager(const std::string& soundDirectory) : directory(soundDirectory) {}

void SoundManager::registerSound(const std::string& name, const std::string& fileName)
{
    if (namesToFiles.find(name) != namesToFiles.end())
        throw std::runtime_error("Name already exists in mappings");

    namesToFiles[name] = fileName;
}

std::shared_ptr<Sound> SoundManager::getSound(const std::string& name)
{
    auto fileIter = namesToFiles.find(name);
    if (fileIter == namesToFiles.end())
        throw std::runtime_error("Name is not registered");

    auto fileName = fileIter->second;
    auto soundIter = filesToSounds.find(fileName);
    if (soundIter != filesToSounds.end())
    {
        return soundIter->second;
    }

    auto sound = loadFromFile(fileName);
    if (sound == nullptr)
        throw std::runtime_error("Failed to load sound");

    filesToSounds[fileName] = sound;
    return sound;
}

std::shared_ptr<Sound> SoundManager::loadFromFile(const std::string& fileName)
{
    auto extension = fileName.substr(fileName.length() - 4);
    auto fullPath = directory + "/" + fileName;
    if (extension == ".wav")
    {
        return loadFromWav(fullPath);
    }
    else if (extension == ".ogg")
    {
        // TODO
        return nullptr;
    }
    return nullptr;
}

static std::int32_t convert_to_int(char* buffer, std::size_t len)
{
    std::int32_t a = 0;
    std::memcpy(&a, buffer, len);
    return a;
}

bool loadWavFile(std::ifstream& file, std::uint8_t& channels, std::int32_t& sampleRate, std::uint8_t& bitsPerSample,
                 ALsizei& size)
{
    char buffer[4];
    if (!file.is_open())
        return false;

    // the RIFF
    if (!file.read(buffer, 4))
    {
        throw std::runtime_error("ERROR: could not read RIFF");
    }
    if (std::strncmp(buffer, "RIFF", 4) != 0)
    {
        throw std::runtime_error("ERROR: file is not a valid WAVE file (header doesn't begin with RIFF)");
    }

    // the size of the file
    if (!file.read(buffer, 4))
    {
        throw std::runtime_error("ERROR: could not read size of file");
    }

    // the WAVE
    if (!file.read(buffer, 4))
    {
        throw std::runtime_error("ERROR: could not read WAVE");
    }
    if (std::strncmp(buffer, "WAVE", 4) != 0)
    {
        throw std::runtime_error("ERROR: file is not a valid WAVE file (header doesn't contain WAVE)");
    }

    // "fmt/0"
    if (!file.read(buffer, 4))
    {
        throw std::runtime_error("ERROR: could not read fmt/0");
    }

    // this is always 16, the size of the fmt data chunk
    if (!file.read(buffer, 4))
    {
        throw std::runtime_error("ERROR: could not read the 16");
    }

    // PCM should be 1?
    if (!file.read(buffer, 2))
    {
        throw std::runtime_error("ERROR: could not read PCM");
    }

    // the number of channels
    if (!file.read(buffer, 2))
    {
        throw std::runtime_error("ERROR: could not read number of channels");
    }
    channels = convert_to_int(buffer, 2);

    // sample rate
    if (!file.read(buffer, 4))
    {
        throw std::runtime_error("ERROR: could not read sample rate");
    }
    sampleRate = convert_to_int(buffer, 4);

    // (sampleRate * bitsPerSample * channels) / 8
    if (!file.read(buffer, 4))
    {
        throw std::runtime_error("ERROR: could not read (sampleRate * bitsPerSample * channels) / 8");
    }

    // ?? dafaq
    if (!file.read(buffer, 2))
    {
        throw std::runtime_error("ERROR: could not read dafaq");
    }

    // bitsPerSample
    if (!file.read(buffer, 2))
    {
        throw std::runtime_error("ERROR: could not read bits per sample");
    }
    bitsPerSample = convert_to_int(buffer, 2);

    // data chunk header "data"
    if (!file.read(buffer, 4))
    {
        throw std::runtime_error("ERROR: could not read data chunk header");
    }

    if (std::strncmp(buffer, "LIST", 4) == 0)
    {
        // LIST chunk
        if (!file.read(buffer, 4))
        {
            throw std::runtime_error("ERROR: could not read data chunk header");
        }
        size = convert_to_int(buffer, 4);
        char buf[1000];
        // skip
        if (!file.read(buf, size))
        {
            throw std::runtime_error("ERROR: could not read data chunk header");
        }

        // data chunk header "data"
        if (!file.read(buffer, 4))
        {
            throw std::runtime_error("ERROR: could not read data chunk header");
        }
    }

    if (std::strncmp(buffer, "data", 4) != 0)
    {
        throw std::runtime_error("ERROR: file is not a valid WAVE file (doesn't have 'data' tag)");
    }

    // size of data
    if (!file.read(buffer, 4))
    {
        throw std::runtime_error("ERROR: could not read data size");
    }
    size = convert_to_int(buffer, 4);

    /* cannot be at the end of file */
    if (file.eof())
    {
        throw std::runtime_error("ERROR: reached EOF on the file");
    }
    if (file.fail())
    {
        throw std::runtime_error("ERROR: fail state set on the file");
    }

    return true;
}

std::shared_ptr<Sound> SoundManager::loadFromWav(const std::string& fileName)
{
    std::ifstream ifs(fileName, std::ios::binary);
    if (!ifs.is_open())
        throw std::runtime_error("Failed to open file");

    std::uint8_t channels = 0;
    std::int32_t sampleRate = 0;
    std::uint8_t bitsPerSample = 0;
    ALsizei size = 0;
    loadWavFile(ifs, channels, sampleRate, bitsPerSample, size);

    std::vector<char> data(size);
    data.resize(size);
    ifs.read(data.data(), size);

    return std::make_shared<Sound>(data.data(), size, channels, bitsPerSample, sampleRate);
}