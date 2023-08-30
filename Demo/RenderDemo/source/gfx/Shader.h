#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <string>
#include <unordered_map>

#include "gfx/Texture.h"

class Shader
{
   public:
    Shader(const std::string& vertexShaderCode, const std::string& fragmentShaderCode);
    ~Shader();

    void setActive() const;

    void set(const std::string& name, const int value, bool required = false);
    void set(const std::string& name, const unsigned int value, bool required = false);
    void set(const std::string& name, const float value, bool required = false);
    void set(const std::string& name, const glm::vec2& value, bool required = false);
    void set(const std::string& name, const glm::vec3& value, bool required = false);
    void set(const std::string& name, const glm::vec4& value, bool required = false);
    void set(const std::string& name, const glm::mat4& value, bool required = false);
    void setTexture(const std::string& name, const Texture& texture, bool required = false);

   private:
    int getUniformLocation(const std::string& name, bool required) const;

    // Shader id
    GLuint id = 0;
    // Cached uniform names with location ids
    mutable std::unordered_map<std::string, int> uniformLocations;
};

GLuint createShaderObject(const std::string& code, GLenum shaderType);
GLuint createShaderProgram(const std::string& vertexShaderCode, const std::string& fragmentShaderCode);