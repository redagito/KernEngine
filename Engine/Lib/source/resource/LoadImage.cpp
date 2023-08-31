#include "kern/resource/LoadImage.h"

#include <fmtlog/fmtlog.h>
#include <stb_image.h>

#include <vector>

static ColorFormat channelsToFormat(int channels)
{
    switch (channels)
    {
    case 1:
        return ColorFormat::GreyScale8;
    case 3:
        return ColorFormat::RGB24;
    case 4:
        return ColorFormat::RGBA32;
    default:
        return ColorFormat::Invalid;
    }
}

static bool loadInternal(const std::string& file, int stbiDesiredChannels, Image& image)
{
    stbi_set_flip_vertically_on_load(true);

    // Decode image data
    int channels = 0;
    int width = 0;
    int height = 0;
    uint8_t* data = stbi_load(file.c_str(), &width, &height, &channels, stbiDesiredChannels);
    
    // Set channels to actual channels when converting
    if (stbiDesiredChannels != STBI_default)
    {
        channels = stbiDesiredChannels;
    }

    if (data == nullptr)
    {
        loge("An error occured while decoding the image file {}", file);
        return false;
    }

    image.m_format = channelsToFormat(channels);
    image.m_width = width;
    image.m_height = height;
    image.m_data.assign(data, data + channels * width * height);
    stbi_image_free(data);

    return true;
}

bool load(const std::string& file, Image& image)
{ 
    return loadInternal(file, STBI_default, image);
}

bool load(const std::string &file, ColorFormat format, Image &image)
{
    // Map color type
    int desiredChannels = 0;
    switch (format)
    {
    case ColorFormat::GreyScale8:
        desiredChannels = STBI_grey;
        break;
    case ColorFormat::RGB24:
        desiredChannels = STBI_rgb;
        break;
    case ColorFormat::RGBA32:
        desiredChannels = STBI_rgb_alpha;
        break;
    default:
        loge("Unknown color format encountered while loading image file {}.", file);
        return false;
        break;
    }

    return loadInternal(file, desiredChannels, image);
}