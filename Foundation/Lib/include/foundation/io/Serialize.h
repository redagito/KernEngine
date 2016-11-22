#pragma once

#include <ostream>
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
bool serialize(bool value, std::ostream &os);

bool serialize(std::int8_t value, std::ostream &os);
bool serialize(std::int16_t value, std::ostream &os);
bool serialize(std::int32_t value, std::ostream &os);
bool serialize(std::int64_t value, std::ostream &os);
bool serialize(std::uint8_t value, std::ostream &os);
bool serialize(std::uint16_t value, std::ostream &os);
bool serialize(std::uint32_t value, std::ostream &os);
bool serialize(std::uint64_t value, std::ostream &os);

bool serialize(float value, std::ostream &os);
bool serialize(double value, std::ostream &os);

/**
 * Serialization for STL types.
 */
bool serialize(const std::string &value, std::ostream &os);

template <typename TypeA, typename TypeB>
bool serialize(const std::pair<TypeA, TypeB> &value, std::ostream &os);

/**
 * Serialization routines for STL container.
 */
template <typename Type>
bool serialize(const std::list<Type> &container, std::ostream &os);

template <typename Type>
bool serialize(const std::vector<Type> &container, std::ostream &os);

template <typename KeyType, typename ValueType>
bool serialize(const std::map<KeyType, ValueType> &container, std::ostream &os);

template <typename KeyType, typename ValueType>
bool serialize(const std::unordered_map<KeyType, ValueType> &container,
               std::ostream &os);

/**
 * Implementations of template functions.
 */
template <typename TypeA, typename TypeB>
bool serialize(const std::pair<TypeA, TypeB> &value, std::ostream &os)
{
  if (!serialize(value.first, os))
    return false;
  return serialize(value.second, os);
}

template <typename Type>
bool serialize(const std::list<Type> &container, std::ostream &os)
{
  if (!serialize(container.size(), os))
    return false;
  for (const auto &value : container)
  {
    if (!serialize(value, os))
      return false;
  }
  return true;
}

template <typename Type>
bool serialize(const std::vector<Type> &container, std::ostream &os)
{
  if (!serialize(container.size(), os))
    return false;
  for (const auto &value : container)
  {
    if (!serialize(value, os))
      return false;
  }
  return true;
}

template <typename KeyType, typename ValueType>
bool serialize(const std::map<KeyType, ValueType> &container, std::ostream &os)
{
  if (!serialize(container.size(), os))
    return false;
  for (const auto &value : container)
  {
    if (!serialize(value, os))
      return false;
  }
  return true;
}

template <typename KeyType, typename ValueType>
bool serialize(const std::unordered_map<KeyType, ValueType> &container,
               std::ostream &os)
{
  if (!serialize(container.size(), os))
    return false;
  for (const auto &value : container)
  {
    if (!serialize(value, os))
      return false;
  }
  return true;
}
