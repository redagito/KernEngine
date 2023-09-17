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

    void set(const std::string& name, const int value, bool required = true);
    void set(const std::string& name, const unsigned int value, bool required = true);
    void set(const std::string& name, const float value, bool required = true);
    void set(const std::string& name, const glm::vec2& value, bool required = true);
    void set(const std::string& name, const glm::vec3& value, bool required = true);
    void set(const std::string& name, const glm::vec4& value, bool required = true);
    void set(const std::string& name, const glm::mat4& value, bool required = true);
    void setTexture(const std::string& name, const Texture& texture, int unit, bool required = true);

   private:
    bool isActive() const;
    void ensureActive() const;
    int getUniformLocation(const std::string& name, bool required) const;

    // Shader id
    GLuint id = 0;
    // Cached uniform names with location ids
    mutable std::unordered_map<std::string, int> uniformLocations;
    static GLuint activeShaderId;
};

GLuint createShaderObject(const std::string& code, GLenum shaderType);
GLuint createShaderProgram(const std::string& vertexShaderCode, const std::string& fragmentShaderCode);