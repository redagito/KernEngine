#pragma once

#include <string>

#include <nlohmann/json.hpp>

class IResourceManager;
class CGameSystem;

/**
 * \brief Loads entire game.
 */
bool load(const std::string &file, IResourceManager &manager, CGameSystem &system);
bool loadGameStates(const nlohmann::json &node, IResourceManager &manager, CGameSystem &system);
bool loadGameState(const nlohmann::json &node, IResourceManager &manager, CGameSystem &system);
bool loadGameState(const std::string &file, const std::string &name, IResourceManager &manager,
                   CGameSystem &system);