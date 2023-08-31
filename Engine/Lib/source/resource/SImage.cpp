#include "kern/resource/Image.h"

Image::Image(std::vector<unsigned char> data, unsigned int width, unsigned int height,
               ColorFormat format)
    : m_data(data), m_width(width), m_height(height), m_format(format)
{
    return;
}
