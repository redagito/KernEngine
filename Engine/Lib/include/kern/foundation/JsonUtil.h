#pragma once

#include <string>

#include <nlohmann/json.hpp>
#include <glm/glm.hpp>

bool load(const std::string &file, nlohmann::json &root);

bool load(const nlohmann::json &node, const std::string &name, int &i);
bool load(const nlohmann::json &node, const std::string &name, unsigned int &i);
bool load(const nlohmann::json &node, const std::string &name, float &f);
bool load(const nlohmann::json &node, const std::string &name, bool &b);
bool load(const nlohmann::json &node, const std::string &name, glm::vec3 &vec);
bool load(const nlohmann::json &node, const std::string &name, std::string &str);