#pragma once

#include <cstdint>
#include <string>

#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <nlohmann/json.hpp>

#include <fmtlog/fmtlog.h>

namespace glm
{
void from_json(const nlohmann::json &j, glm::vec2 &val);
void from_json(const nlohmann::json &j, glm::vec3 &val);
void from_json(const nlohmann::json &j, glm::vec4 &val);
void from_json(const nlohmann::json &j, glm::quat &val);
}