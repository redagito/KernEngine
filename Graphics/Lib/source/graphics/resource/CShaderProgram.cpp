#include "graphics/graphics/resource/CShaderProgram.h"

#include <cassert>

#include <glm/ext.hpp>

#include <foundation/debug/Log.h>

#include "graphics/graphics/resource/CTexture.h"
#include "graphics/graphics/renderer/debug/RendererDebug.h"

GLuint CShaderProgram::s_activeShaderProgram = 0;

CShaderProgram::CShaderProgram(
    TShaderObject<GL_VERTEX_SHADER> *vertex,
    TShaderObject<GL_TESS_CONTROL_SHADER> *tessControl,
    TShaderObject<GL_TESS_EVALUATION_SHADER> *tessEval,
    TShaderObject<GL_GEOMETRY_SHADER> *geometry,
    TShaderObject<GL_FRAGMENT_SHADER> *fragment)
    : m_programId(0), m_valid(false)
{
  init(vertex, tessControl, tessEval, geometry, fragment);
}

CShaderProgram::~CShaderProgram()
{
  if (m_valid)
  {
    glDeleteProgram(m_programId);
  }
}

bool CShaderProgram::init(TShaderObject<GL_VERTEX_SHADER> *vertex,
                          TShaderObject<GL_TESS_CONTROL_SHADER> *tessControl,
                          TShaderObject<GL_TESS_EVALUATION_SHADER> *tessEval,
                          TShaderObject<GL_GEOMETRY_SHADER> *geometry,
                          TShaderObject<GL_FRAGMENT_SHADER> *fragment)
{
  // Needs vertex shader
  if (vertex == nullptr || !vertex->isValid())
  {
    return false;
  }
  // Need fragment shader
  if (fragment == nullptr || !fragment->isValid())
  {
    return false;
  }

  // Check if any of the other shaders are invalid
  // nullptr signals unused stage
  if (geometry != nullptr && !geometry->isValid())
  {
    return false;
  }
  else if (tessControl != nullptr && !tessControl->isValid())
  {
    return false;
  }
  else if (tessEval != nullptr && !tessEval->isValid())
  {
    return false;
  }

  // Create program id
  GLuint programId = glCreateProgram();
  // Attach shader objects after checking validity

  // Needed stages
  glAttachShader(programId, vertex->getId());
  glAttachShader(programId, fragment->getId());
  // Optional stages
  if (geometry != nullptr)
  {
    glAttachShader(programId, geometry->getId());
  }
  if (tessControl != nullptr)
  {
    glAttachShader(programId, tessControl->getId());
  }
  if (tessEval != nullptr)
  {
    glAttachShader(programId, tessEval->getId());
  }

  // Link program
  glLinkProgram(programId);
  // Check result
  GLint result;
  glGetProgramiv(programId, GL_LINK_STATUS, &result);
  if (result == GL_FALSE)
  {
    // Set info log size
    GLint size;
    glGetShaderiv(programId, GL_INFO_LOG_LENGTH, &size);
    if (size > 0)
    {
      // Create buffer
      std::vector<GLchar> log(size + 1);
      // Get log
      glGetProgramInfoLog(programId, size, NULL, log.data());
      m_infoLog.assign(log.data(), size);
    }
    // Clean up temp id
    glDeleteProgram(programId);
    return false;
  }
  // New shader program linked successfully
  // Delete old shader object
  if (m_valid)
  {
    glDeleteShader(m_programId);
  }
  // Set new id
  m_programId = programId;
  // Set validity flag
  m_valid = true;
  // Clear uniform location cache
  m_uniformLocations.clear();

  // Error check
  std::string error;
  if (hasGLError(error))
  {
    LOG_ERROR("GL Error: %s", error.c_str());
  }

  return true;
}

void CShaderProgram::setActive()
{
  assert(isValid());
  if (s_activeShaderProgram != m_programId)
  {
    glUseProgram(m_programId);
    s_activeShaderProgram = m_programId;
  }
}

void CShaderProgram::setInactive()
{
  assert(isValid());
  if (s_activeShaderProgram == m_programId)
  {
    glUseProgram(0);
    s_activeShaderProgram = 0;
  }
}

const std::string &CShaderProgram::getErrorString() const { return m_infoLog; }

bool CShaderProgram::isValid() const { return m_valid; }

GLint CShaderProgram::getUniformLocation(const std::string &uniformName) const
{
  // Search for cached location
  auto iter = m_uniformLocations.find(uniformName);
  // Not found
  if (iter == m_uniformLocations.end())
  {
    // Cache location
    GLint location = glGetUniformLocation(m_programId, uniformName.data());
    // Invalid location, uniform name does not exist in shader
    if (location == -1)
    {
      LOG_ERROR("Failed to retrieve uniform name %s from shader program.",
                uniformName.c_str());
    }
    m_uniformLocations[uniformName] = location;
    return location;
  }
  // Found
  return iter->second;
}

GLint CShaderProgram::getAttributeLocation(
    const std::string &attributeName) const
{
  return glGetAttribLocation(m_programId, attributeName.data());
}

bool CShaderProgram::setUniform(GLint location, int i)
{
  if (location == -1)
  {
    return false;
  }
  setActive();
  glUniform1i(location, i);
  return true;
}

bool CShaderProgram::setUniform(const std::string &name, int i)
{
  return setUniform(getUniformLocation(name), i);
}

bool CShaderProgram::setUniform(GLint location, float f)
{
  if (location == -1)
  {
    return false;
  }
  setActive();
  glUniform1f(location, f);
  return true;
}

bool CShaderProgram::setUniform(const std::string &name, float f)
{
  return setUniform(getUniformLocation(name), f);
}

bool CShaderProgram::setUniform(GLint location, const glm::vec2 &v)
{
  if (location == -1)
  {
    return false;
  }
  setActive();
  glUniform2f(location, v.x, v.y);
  return true;
}

bool CShaderProgram::setUniform(const std::string &name, const glm::vec2 &v)
{
  return setUniform(getUniformLocation(name), v);
}

bool CShaderProgram::setUniform(GLint location, const glm::vec3 &v)
{
  if (location == -1)
  {
    return false;
  }
  setActive();
  glUniform3f(location, v.x, v.y, v.z);
  return true;
}

bool CShaderProgram::setUniform(const std::string &name, const glm::vec3 &v)
{
  return setUniform(getUniformLocation(name), v);
}

bool CShaderProgram::setUniform(GLint location, const glm::vec4 &v)
{
  if (location == -1)
  {
    return false;
  }
  setActive();
  glUniform4f(location, v.x, v.y, v.z, v.w);
  return true;
}

bool CShaderProgram::setUniform(const std::string &name, const glm::vec4 &v)
{
  return setUniform(getUniformLocation(name), v);
}

bool CShaderProgram::setUniform(GLint location, const glm::mat2 &m)
{
  if (location == -1)
  {
    return false;
  }
  setActive();
  glUniformMatrix2fv(location, 1, GL_FALSE, glm::value_ptr(m));
  return true;
}

bool CShaderProgram::setUniform(const std::string &name, const glm::mat2 &m)
{
  return setUniform(getUniformLocation(name), m);
}

bool CShaderProgram::setUniform(GLint location, const glm::mat3 &m)
{
  if (location == -1)
  {
    return false;
  }
  setActive();
  glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(m));
  return true;
}

bool CShaderProgram::setUniform(const std::string &name, const glm::mat3 &m)
{
  return setUniform(getUniformLocation(name), m);
}

bool CShaderProgram::setUniform(GLint location, const glm::mat4 &m)
{
  if (location == -1)
  {
    return false;
  }
  setActive();
  glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(m));
  return true;
}

bool CShaderProgram::setUniform(const std::string &name, const glm::mat4 &m)
{
  return setUniform(getUniformLocation(name), m);
}

bool CShaderProgram::setUniform(CTexture &texture,
                                const std::string &textureName,
                                GLint textureUnit)
{
  texture.setActive(textureUnit);
  return setUniform(textureName, textureUnit);
}