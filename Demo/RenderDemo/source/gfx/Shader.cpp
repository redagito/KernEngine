#include "gfx/Shader.h"

#include <fmtlog/fmtlog.h>

#include <glm/gtc/type_ptr.hpp>
#include <vector>

GLuint Shader::activeShaderId = 0;

Shader::Shader(const std::string& vertexShaderCode, const std::string& fragmentShaderCode)
{
    id = createShaderProgram(vertexShaderCode, fragmentShaderCode);
}

Shader::~Shader()
{
    if (id == 0)
        return;
    if (isActive())
    {
        glUseProgram(0);
        activeShaderId = 0;
    }
    glDeleteProgram(id);
}

void Shader::setActive() const
{
    if (id == 0)
        throw std::runtime_error("Shader id is null");
    if (isActive())
        return;

    glUseProgram(id);
    activeShaderId = id;
}

void Shader::set(const std::string& name, const int value, bool required)
{
    ensureActive();
    auto loc = getUniformLocation(name, required);
    if (loc < 0)
        return;
    glUniform1i(loc, value);
}

void Shader::set(const std::string& name, const unsigned int value, bool required)
{
    ensureActive();
    auto loc = getUniformLocation(name, required);
    if (loc < 0)
        return;
    glUniform1ui(loc, value);
}

void Shader::set(const std::string& name, const float value, bool required)
{
    ensureActive();
    auto loc = getUniformLocation(name, required);
    if (loc < 0)
        return;
    glUniform1f(loc, value);
}

void Shader::set(const std::string& name, const glm::vec2& value, bool required)
{
    ensureActive();
    auto loc = getUniformLocation(name, required);
    if (loc < 0)
        return;
    glUniform2f(loc, value.x, value.y);
}

void Shader::set(const std::string& name, const glm::vec3& value, bool required)
{
    ensureActive();
    auto loc = getUniformLocation(name, required);
    if (loc < 0)
        return;
    glUniform3f(loc, value.x, value.y, value.z);
}

void Shader::set(const std::string& name, const glm::vec4& value, bool required)
{
    ensureActive();
    auto loc = getUniformLocation(name, required);
    if (loc < 0)
        return;
    glUniform4f(loc, value.x, value.y, value.z, value.w);
}

void Shader::set(const std::string& name, const glm::mat4& value, bool required)
{
    ensureActive();
    auto loc = getUniformLocation(name, required);
    if (loc < 0)
        return;
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setTexture(const std::string& name, const Texture& texture, int unit, bool required)
{
    ensureActive();
    glBindTextureUnit(unit, texture.id);
    set(name, unit, required);
}

bool Shader::isActive() const { return activeShaderId == id; }

void Shader::ensureActive() const
{
    if (!isActive())
        throw std::runtime_error("Shader is not active");
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

static std::string shaderTypeToString(GLenum shaderType)
{
    switch (shaderType)
    {
    case GL_VERTEX_SHADER:
        return "Vertex";
    case GL_FRAGMENT_SHADER:
        return "Fragment";
    case GL_GEOMETRY_SHADER:
        return "Geometry";
    case GL_TESS_CONTROL_SHADER:
        return "Tess Control";
    case GL_TESS_EVALUATION_SHADER:
        return "Tess Eval";
    default:
        throw std::runtime_error("Unrecognized shader type");
    }
}

GLuint createShaderObject(const std::string& code, GLenum shaderType)
{
    GLuint shaderObject = glCreateShader(shaderType);

    // Compile shader
    logi("Compiling shader type");
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
        logi("Error while compiling {} shader: {}", shaderTypeToString(shaderType), infoLog.data());
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