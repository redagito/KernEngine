#pragma once

#include <cstdint>
#include <string>

#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <nlohmann/json.hpp>

#include "foundation/debug/Log.h"

namespace glm
{
void from_json(const nlohmann::json &j, glm::vec2 &val);
void from_json(const nlohmann::json &j, glm::vec3 &val);
void from_json(const nlohmann::json &j, glm::vec4 &val);
void from_json(const nlohmann::json &j, glm::quat &val);
}

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