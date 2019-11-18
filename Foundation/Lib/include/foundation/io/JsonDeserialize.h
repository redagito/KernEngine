#pragma once

#include <cstdint>
#include <string>

#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <nlohmann/json.hpp>

#include "foundation/debug/Log.h"

/**
 * Load json from file
 */
bool load(const std::string &file, nlohmann::json &value);

namespace nlohmann
{
namespace detail
{

template <typename BasicJsonType>
void from_json(const BasicJsonType &j, glm::vec2 &val)
{
    auto arr = j.get<std::array<2, float>>();
    val = decltype(val){arr.at(0), arr.at(1)};
}

template <typename BasicJsonType>
void from_json(const BasicJsonType &j, glm::vec3 &val)
{
    auto arr = j.get<std::array<3, float>>();
    val = decltype(val){arr.at(0), arr.at(1), arr.at(2)};
}

template <typename BasicJsonType>
void from_json(const BasicJsonType &j, glm::vec4 &val)
{
    auto arr = j.get<std::array<4, float>>();
    val = decltype(val){arr.at(0), arr.at(1), arr.at(2), arr.at(3)};
}

template <typename BasicJsonType>
void from_json(const BasicJsonType &j, glm::quat &val)
{
    auto arr = j.get<std::array<4, float>>();
    val = decltype(val){arr.at(0), arr.at(1), arr.at(2), arr.at(3)};
}

}  // namespace detail
}  // namespace nlohmann

template <typename T>
bool deserialize(const nlohmann::json &node, const std::string &key, T &value)
{
    auto iter = node.find(key);
    if (iter == node.end())
    {
        LOG_WARNING("Failed to read json value, key does not exist: %s", key.c_str());
        return false;
    }

    try
    {
        value = node.at(key).get<T>();
    }
    catch (const std::exception &e)
    {
        LOG_ERROR("Failed to read json value: %s", e.what());
        return false;
    }
    return true;
}