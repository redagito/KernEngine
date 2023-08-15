#include "graphics/resource/core/LoadImage.h"

#include <vector>

#include <stb_image.h>

#include <foundation/debug/Log.h>
#include <foundation/util/StringUtil.h>

bool load(const std::string &file, EColorFormat format, SImage &image)
{
    // Map color type
    int desiredChannels = 0;
    switch (format)
    {
    case EColorFormat::GreyScale8:
        desiredChannels = 1;
        break;
    case EColorFormat::RGB24:
        desiredChannels = 3;
        break;
    case EColorFormat::RGBA32:
        desiredChannels = 4;
        break;
    default:
        LOG_ERROR("Unknown color format encountered while loading image file %s.", file.c_str());
        return false;
        break;
    }

    // Decode image data
    int channels = 0;
    int width = 0;
    int height = 0;
    stbi_set_flip_vertically_on_load(1);
    uint8_t* data = stbi_load(file.c_str(), &width, &height, &channels, desiredChannels);
    if (data == nullptr)
    {
        LOG_ERROR("An error occured while decoding the image file %s", file.c_str());
        return false;
    }

    image.m_format = format;
    image.m_width = width;
    image.m_height = height;
    image.m_data.assign(data, data + width * height * desiredChannels);
    stbi_image_free(data);
    return true;
}