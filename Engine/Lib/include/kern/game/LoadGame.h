#pragma once

#include <string>

#include <nlohmann/json.hpp>

class IResourceManager;
class GameSystem;

/**
 * \brief Loads entire game.
 */
bool load(const std::string &file, IResourceManager &manager, GameSystem &system);
bool loadGameStates(const nlohmann::json &node, IResourceManager &manager, GameSystem &system);
bool loadGameState(const nlohmann::json &node, IResourceManager &manager, GameSystem &system);
bool loadGameState(const std::string &file, const std::string &name, IResourceManager &manager,
                   GameSystem &system);