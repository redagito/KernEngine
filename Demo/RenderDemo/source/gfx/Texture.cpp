#include "gfx/Texture.h"

#include <unordered_map>
#include <stdexcept>
#include <stdexcept>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <fmtlog/fmtlog.h>

static std::unordered_map<std::string, std::shared_ptr<Texture>>& getTexturesCache()
{
    static std::unordered_map<std::string, std::shared_ptr<Texture>> cache;
    return cache;
}

void clearTextureCache() { getTexturesCache().clear(); }

std::shared_ptr<Texture> textureFromFile(const std::string& fileName, const std::string& directory,
                                         const std::string& typeName)
{
    auto filePath = directory + "/" + fileName;
    auto key = filePath + "." + typeName;
    // In cache
    if (getTexturesCache().find(key) != getTexturesCache().end())
    {
        return getTexturesCache().at(key);
    }

    GLuint id = textureIdFromFile(fileName, directory);
    auto tex = std::make_shared<Texture>();
    tex->id = id;
    tex->fileName = fileName;
    tex->type = typeName;

    getTexturesCache()[key] = tex;
    return tex;
}

GLuint textureIdFromFile(const std::string& fileName, const std::string& directory)
{
    // Load with stb_image
    auto filePath = directory + "/" + fileName;
    logi("Load texture {}", filePath);

    int x = 0;
    int y = 0;
    int channels = 0;
    // Load as RGB
    stbi_set_flip_vertically_on_load(true);
    const uint8_t* image = stbi_load(filePath.c_str(), &x, &y, &channels, 3);
    if (image == nullptr)
    {
        loge(stbi_failure_reason());
        throw std::runtime_error("Failed to load the texture " + fileName);
    }

    GLuint id = 0;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

    glGenerateMipmap(GL_TEXTURE_2D);

    // Wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Unbind
    glBindTexture(GL_TEXTURE_2D, 0);
    return id;
}

Texture::~Texture()
{
    if (id == 0)
        return;

    glDeleteTextures(1, &id);
    id = 0;
}