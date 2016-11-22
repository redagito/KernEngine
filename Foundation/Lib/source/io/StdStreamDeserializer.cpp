#include "foundation/io/StdStreamDeserializer.h"

#include <glm/ext.hpp>

template <typename T>
bool deserializeNative(std::istream &stream, T *value, uint64_t size)
{
  if (size == 0)
  {
    return true;
  }
  stream.read(reinterpret_cast<char *>(value), sizeof(T) * size);
  return !stream.fail() && !stream.eof();
}

template <typename T> bool deserializeNative(std::istream &stream, T &value)
{
  return deserializeNative(stream, &value, 1);
}

template <typename T>
bool deserializeNativeVector(std::istream &stream, std::vector<T> &value)
{
  uint32_t size;
  if (!deserialize(stream, size))
  {
    return false;
  }
  value.resize(size);
  return deserializeNative(stream, value.data(), size);
}

template <typename T>
bool deserializeVector(std::istream &stream, std::vector<T> &value)
{
  // Read size
  uint32_t size;
  if (!deserialize(stream, size))
  {
    return false;
  }
  // Alloc
  value.resize(size);
  // Read data
  for (auto &val : value)
  {
    if (!deserialize(stream, val))
    {
      return false;
    }
  }
  return true;
}

bool deserialize(std::istream &stream, bool &value)
{
  return deserializeNative(stream, value);
}

bool deserialize(std::istream &stream, int8_t &value)
{
  return deserializeNative(stream, value);
}

bool deserialize(std::istream &stream, int16_t &value)
{
  return deserializeNative(stream, value);
}

bool deserialize(std::istream &stream, int32_t &value)
{
  return deserializeNative(stream, value);
}

bool deserialize(std::istream &stream, int64_t &value)
{
  return deserializeNative(stream, value);
}

bool deserialize(std::istream &stream, uint8_t &value)
{
  return deserializeNative(stream, value);
}

bool deserialize(std::istream &stream, uint16_t &value)
{
  return deserializeNative(stream, value);
}

bool deserialize(std::istream &stream, uint32_t &value)
{
  return deserializeNative(stream, value);
}

bool deserialize(std::istream &stream, uint64_t &value)
{
  return deserializeNative(stream, value);
}

bool deserialize(std::istream &stream, float &value)
{
  return deserializeNative(stream, value);
}

bool deserialize(std::istream &stream, double &value)
{
  return deserializeNative(stream, value);
}

bool deserialize(std::istream &stream, glm::vec2 &value)
{
  return deserializeNative(stream, &value, 2);
}

bool deserialize(std::istream &stream, glm::vec3 &value)
{
  return deserializeNative(stream, &value, 3);
}

bool deserialize(std::istream &stream, glm::vec4 &value)
{
  return deserializeNative(stream, &value, 4);
}

bool deserialize(std::istream &stream, glm::mat2 &value)
{
  return deserializeNative(stream, &value, 4);
}

bool deserialize(std::istream &stream, glm::mat3 &value)
{
  return deserializeNative(stream, &value, 9);
}

bool deserialize(std::istream &stream, glm::mat4 &value)
{
  return deserializeNative(stream, &value, 16);
}

bool deserialize(std::istream &stream, std::string &value)
{
  uint32_t size;
  if (!deserialize(stream, size))
  {
    return false;
  }
  value.resize(size);
  for (uint32_t i = 0; i < size; ++i)
  {
    if (!deserializeNative(stream, value[i]))
    {
      return false;
    }
  }
  return true;
}

bool deserialize(std::istream &stream, std::vector<uint8_t> &value)
{
  return deserializeNativeVector(stream, value);
}

bool deserialize(std::istream &stream, std::vector<uint16_t> &value)
{
  return deserializeNativeVector(stream, value);
}

bool deserialize(std::istream &stream, std::vector<uint32_t> &value)
{
  return deserializeNativeVector(stream, value);
}

bool deserialize(std::istream &stream, std::vector<uint64_t> &value)
{
  return deserializeNativeVector(stream, value);
}

bool deserialize(std::istream &stream, std::vector<float> &value)
{
  return deserializeNativeVector(stream, value);
}

bool deserialize(std::istream &stream, std::vector<glm::vec3> &value)
{
  return deserializeVector(stream, value);
}