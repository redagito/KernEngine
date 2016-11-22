#pragma once

#include <istream>
#include <cstdint>

#include <string>
#include <tuple>

#include <list>
#include <vector>
#include <map>
#include <unordered_map>

/**
 * Serialization routines for basic types.
 */
bool deserialize(bool &value, std::istream &is);
bool deserialize(char &value, std::istream &is);

bool deserialize(std::int8_t &value, std::istream &is);
bool deserialize(std::int16_t &value, std::istream &is);
bool deserialize(std::int32_t &value, std::istream &is);
bool deserialize(std::int64_t &value, std::istream &is);
bool deserialize(std::uint8_t &value, std::istream &is);
bool deserialize(std::uint16_t &value, std::istream &is);
bool deserialize(std::uint32_t &value, std::istream &is);
bool deserialize(std::uint64_t &value, std::istream &is);

bool deserialize(float &value, std::istream &is);
bool deserialize(double &value, std::istream &is);

/**
 * Serialization for STL types.
 */
bool deserialize(std::string &value, std::istream &is);

template <typename TypeA, typename TypeB>
bool deserialize(std::pair<TypeA, TypeB> &value, std::istream &is);

/**
 * Serialization routines for STL container.
 */
template <typename Type>
bool deserialize(std::list<Type> &container, std::istream &is);

template <typename Type>
bool deserialize(std::vector<Type> &container, std::istream &is);

template <typename KeyType, typename ValueType>
bool deserialize(std::map<KeyType, ValueType> &container, std::istream &is);

template <typename KeyType, typename ValueType>
bool deserialize(std::unordered_map<KeyType, ValueType> &container,
                 std::istream &is);

/**
 * Implementations of template functions.
 */
template <typename TypeA, typename TypeB>
bool deserialize(std::pair<TypeA, TypeB> &value, std::istream &is)
{
  if (!deserialize(value.first, is))
    return false;
  return deserialize(value.second, is);
}

template <typename Type>
bool deserialize(std::list<Type> &container, std::istream &is)
{
  std::size_t size;
  if (!deserialize(size, is))
  {
    return false;
  }
  for (std::size_t i = 0; i < size; ++i)
  {
    Type value;
    if (!deserialize(value, is))
    {
      return false;
    }
    container.push_back(value);
  }
  return true;
}

template <typename Type>
bool deserialize(std::vector<Type> &container, std::istream &is)
{
  std::size_t size;
  if (!deserialize(size, is))
  {
    return false;
  }
  container.reserve(size);
  for (std::size_t i = 0; i < size; ++i)
  {
    Type value;
    if (!deserialize(value, is))
    {
      return false;
    }
    container.push_back(value);
  }
  return true;
}

template <typename KeyType, typename ValueType>
bool deserialize(std::map<KeyType, ValueType> &container, std::istream &is)
{
  std::size_t size;
  if (!deserialize(size, is))
  {
    return false;
  }
  for (std::size_t i = 0; i < size; ++i)
  {
    KeyType key;
    ValueType value;
    if (!deserialize(key, is))
    {
      return false;
    }
    if (!deserialize(value, is))
    {
      return false;
    }
    container[key] = value;
  }
  return true;
}

template <typename KeyType, typename ValueType>
bool deserialize(std::unordered_map<KeyType, ValueType> &container,
                 std::istream &is)
{
  std::size_t size;
  if (!deserialize(size, is))
  {
    return false;
  }
  for (std::size_t i = 0; i < size; ++i)
  {
    KeyType key;
    ValueType value;
    if (!deserialize(key, is))
    {
      return false;
    }
    if (!deserialize(value, is))
    {
      return false;
    }
    container[key] = value;
  }
  return true;
}
