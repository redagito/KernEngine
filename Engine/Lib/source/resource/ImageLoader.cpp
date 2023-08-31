#include "kern/resource/ImageLoader.h"

#include "kern/resource/LoadImage.h"

bool ImageLoader::loadFromFile(const std::string& file, IResourceManager& manager) { 
	Image img;
    if (!load(file, img))
        return false;

    manager.createImage(img.m_data, img.m_width, img.m_height, img.m_format);
    return true;
}