#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <string>
#include <unordered_map>

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

   private:
    int getUniformLocation(const std::string& name, bool required) const;

    GLuint id = 0;
    mutable std::unordered_map<std::string, int> uniformLocations;
};

GLuint createShaderObject(const std::string& code, GLenum shaderType);
GLuint createShaderProgram(const std::string& vertexShaderCode, const std::string& fragmentShaderCode);