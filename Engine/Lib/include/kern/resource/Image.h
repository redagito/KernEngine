#pragma once

#include <vector>

#include "kern/resource/ResourceId.h"
#include "kern/resource/ColorFormat.h"

/**
 * \brief Image data.
 */
struct Image
{
    Image() = default;
    Image(std::vector<unsigned char> data, unsigned int width, unsigned int height,
           ColorFormat format);

    std::vector<unsigned char> m_data;
    unsigned int m_width = 0;
    unsigned int m_height = 0;
    ColorFormat m_format = ColorFormat::Invalid;
};
