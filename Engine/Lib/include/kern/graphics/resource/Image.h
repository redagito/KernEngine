#pragma once

#include <vector>

#include "kern/graphics/resource/ResourceConfig.h"

/**
 * \brief Image data.
 */
struct Image
{
    Image();
    Image(std::vector<unsigned char> data, unsigned int width, unsigned int height,
           EColorFormat format);
    std::vector<unsigned char> m_data;
    unsigned int m_width;
    unsigned int m_height;
    EColorFormat m_format;
};
