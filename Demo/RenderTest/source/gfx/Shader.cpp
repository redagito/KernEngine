#include "gfx/Shader.h"

#include <vector>

#include <glm/gtc/type_ptr.hpp>
#include <fmtlog/fmtlog.h>

Shader::Shader(const std::string& vertexShaderCode, const std::string& fragmentShaderCode)
{
    id = createShaderProgram(vertexShaderCode, fragmentShaderCode);
}

Shader::~Shader()
{
    if (id == 0)
        return;
    glDeleteProgram(id);
}

void Shader::setActive() const { glUseProgram(id); }

void Shader::set(const std::string& name, const int value, bool required)
{
    auto loc = getUniformLocation(name, required);
    if (loc < 0)
        return;
    glUniform1i(loc, value);
}

void Shader::set(const std::string& name, const unsigned int value, bool required)
{
    auto loc = getUniformLocation(name, required);
    if (loc < 0)
        return;
    glUniform1ui(loc, value);
}

void Shader::set(const std::string& name, const float value, bool required)
{
    auto loc = getUniformLocation(name, required);
    if (loc < 0)
        return;
    glUniform1f(loc, value);
}

void Shader::set(const std::string& name, const glm::vec2& value, bool required)
{
    auto loc = getUniformLocation(name, required);
    if (loc < 0)
        return;
    glUniform2f(loc, value.x, value.y);
}

void Shader::set(const std::string& name, const glm::vec3& value, bool required)
{
    auto loc = getUniformLocation(name, required);
    if (loc < 0)
        return;
    glUniform3f(loc, value.x, value.y, value.z);
}

void Shader::set(const std::string& name, const glm::vec4& value, bool required)
{
    auto loc = getUniformLocation(name, required);
    if (loc < 0)
        return;
    glUniform4f(loc, value.x, value.y, value.z, value.w);
}

void Shader::set(const std::string& name, const glm::mat4& value, bool required)
{
    auto loc = getUniformLocation(name, required);
    if (loc < 0)
        return;
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
}

int Shader::getUniformLocation(const std::string& name, bool required) const
{
    auto iter = uniformLocations.find(name);
    if (iter != uniformLocations.end())
    {
        return iter->second;
    }
    int index = glGetUniformLocation(id, name.c_str());

    // Error if not found?
    if (index < 0 && required)
        throw std::runtime_error{"Uniform does not exist: " + name};

    uniformLocations[name] = index;
    return index;
}

GLuint createShaderObject(const std::string& code, GLenum shaderType)
{
    GLuint shaderObject = glCreateShader(shaderType);

    // Compile shader
    logi("Compiling shader");
    const char* temp = code.c_str();
    glShaderSource(shaderObject, 1, &temp, NULL);
    glCompileShader(shaderObject);

    // Check shader
    GLint result = GL_FALSE;
    glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &result);

    int infoLogLength = 0;
    glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 0)
    {
        std::vector<char> infoLog(infoLogLength + 1);
        glGetShaderInfoLog(shaderObject, infoLogLength, NULL, infoLog.data());
        logi("Error while compiling shader: {}", infoLog.data());
    }
    if (result == GL_FALSE)
    {
        glDeleteShader(shaderObject);
        throw std::runtime_error{"Failed to compile shader object"};
    }

    return shaderObject;
}

GLuint createShaderProgram(const std::string& vertexShaderCode, const std::string& fragmentShaderCode)
{
    GLuint vertexShader = createShaderObject(vertexShaderCode, GL_VERTEX_SHADER);
    GLuint fragmentShader = createShaderObject(fragmentShaderCode, GL_FRAGMENT_SHADER);

    // Link the program
    logi("Linking shader program");
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check the program
    GLint result = GL_FALSE;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &result);

    int infoLogLength = 0;
    glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 0)
    {
        std::vector<char> infoLog(infoLogLength + 1);
        glGetProgramInfoLog(shaderProgram, infoLogLength, NULL, &infoLog[0]);
        logi("Error while linking shader: {}", infoLog.data());
    }

    glDetachShader(shaderProgram, vertexShader);
    glDetachShader(shaderProgram, fragmentShader);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    if (result == GL_FALSE)
    {
        glDeleteProgram(shaderProgram);
        throw std::runtime_error{"Failed to link shader program"};
    }

    return shaderProgram;
}