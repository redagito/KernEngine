#pragma once

#include <glad/glad.h>

#include <string>
#include <memory>

class Texture
{
   public:
    ~Texture();

    enum class Type
    {
        Diffuse,
        Specular,
        Normal,
        Height
    };

    GLuint id = 0;
    // Either "diffuse_texture" or "specular_texture"
    // TODO Use enum
    std::string type;
    std::string fileName;
};

// Texture loading
GLuint textureIdFromFile(const std::string& fileName, const std::string& directory);
// caches
void clearTextureCache();
std::shared_ptr<Texture> textureFromFile(const std::string& fileName, const std::string& directory,
                                         const std::string& typeName);
