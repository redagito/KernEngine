#include "kern/foundation/JsonUtil.h"

#include <fmtlog/fmtlog.h>

#include <fstream>

#include "kern/foundation/JsonDeserialize.h"

template <typename T>
bool deserialize(const nlohmann::json &node, const std::string &key, T &value)
{
    auto iter = node.find(key);
    if (iter == node.end())
    {
        logw("Failed to read json value, key does not exist: {}", key.c_str());
        return false;
    }

    try
    {
        value = node.at(key).get<T>();
    }
    catch (const std::exception &e)
    {
        loge("Failed to read json value: {}", e.what());
        return false;
    }
    return true;
}

bool load(const std::string &file, nlohmann::json &value)
{
    std::ifstream ifs(file);
    if (!ifs.is_open())
    {
        loge("failed to open json file {}.", file.c_str());
        return false;
    }

    try
    {
        value = nlohmann::json::parse(ifs);
    }
    catch (const nlohmann::json::parse_error &e)
    {
        loge("Failed to parse json file {}: {}", file.c_str(), e.what());
        return false;
    }
    return true;
}

bool load(const nlohmann::json &node, const std::string &name, int &i)
{
    if (node.find(name) == node.end())
    {
        loge("Failed to load '{}' parameter.", name.c_str());
        return false;
    }

    if (!deserialize(node, name, i))
    {
        loge("Failed to load '{}' parameter.", name.c_str());
        return false;
    }
    return true;
}

bool load(const nlohmann::json &node, const std::string &name, unsigned int &u)
{
    if (node.find(name) == node.end())
    {
        loge("Failed to load '{}' parameter.", name.c_str());
        return false;
    }

    if (!deserialize(node, name, u))
    {
        loge("Failed to load '{}' parameter.", name.c_str());
        return false;
    }
    return true;
}

bool load(const nlohmann::json &node, const std::string &name, float &f)
{
    if (node.find(name) == node.end())
    {
        loge("Failed to load '{}' parameter.", name.c_str());
        return false;
    }

    if (!deserialize(node, name, f))
    {
        loge("Failed to load '{}' parameter.", name.c_str());
        return false;
    }
    return true;
}

bool load(const nlohmann::json &node, const std::string &name, bool &b)
{
    if (node.find(name) == node.end())
    {
        loge("Failed to load '{}' parameter.", name.c_str());
        return false;
    }

    if (!deserialize(node, name, b))
    {
        loge("Failed to load '{}' parameter.", name.c_str());
        return false;
    }
    return true;
}

bool load(const nlohmann::json &node, const std::string &name, glm::vec3 &vec)
{
    if (node.find(name) == node.end())
    {
        loge("Failed to load '{}' parameter.", name.c_str());
        return false;
    }

    if (!deserialize(node, name, vec))
    {
        loge("Failed to load '{}' parameter.", name.c_str());
        return false;
    }
    return true;
}

bool load(const nlohmann::json &node, const std::string &name, std::string &str)
{
    if (node.find(name) == node.end())
    {
        loge("Failed to load '{}' parameter.", name.c_str());
        return false;
    }

    if (!deserialize(node, name, str))
    {
        loge("Failed to load '{}' parameter.", name.c_str());
        return false;
    }
    return true;
}