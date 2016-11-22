#pragma once

#include <string>

#include <json/json.h>
#include <glm/glm.hpp>

bool load(const std::string &file, Json::Value &root);
bool load(const Json::Value &node, const std::string &name, int &i);
bool load(const Json::Value &node, const std::string &name, unsigned int &i);
bool load(const Json::Value &node, const std::string &name, float &f);
bool load(const Json::Value &node, const std::string &name, bool &b);
bool load(const Json::Value &node, const std::string &name, glm::vec3 &vec);
bool load(const Json::Value &node, const std::string &name, std::string &str);