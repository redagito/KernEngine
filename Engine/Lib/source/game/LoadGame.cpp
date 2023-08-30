#include "kern/game/LoadGame.h"

#include <fmtlog/fmtlog.h>

#include <glm/glm.hpp>
#include <nlohmann/json.hpp>
#include <string>

#include "kern/graphics/resource/IResourceManager.h"
#include "kern/foundation/JsonDeserialize.h"
#include "kern/foundation/JsonUtil.h"
#include "kern/game/GameSystem.h"
#include "kern/game/IGameState.h"
// #include "kern/graphics/game/state/CTitleState.h"
// #include "kern/graphics/game/state/CGamePlayState.h"

bool load(const std::string &file, IResourceManager &manager, GameSystem &system)
{
    nlohmann::json root;
    if (!load(file, root))
    {
        loge("Failed to load game from file {}.", file.c_str());
        return false;
    }

    // Load game string identifer (name of the game)
    std::string name;
    if (!load(root, "name", name))
    {
        loge("Failed to load game name identfier from file {}.", file.c_str());
        return false;
    }
    // TODO Name identifier unused. Use for window title.
    logi("Loading game {}.", name.c_str());

    // Load game states which contain high level game logic and application
    // control
    if (!loadGameStates(root["game_states"], manager, system))
    {
        loge("Failed to load game states from file {}.", file.c_str());
        return false;
    }

    // Load identifier of the initial state
    std::string initialState;
    if (!load(root, "start_state", initialState))
    {
        loge("Failed to load initial game state from file {}.", file.c_str());
        return false;
    }

    // Load successful
    logi("Loaded game file {}.", file.c_str());
    return true;
}

bool loadGameStates(const nlohmann::json &node, IResourceManager &manager, GameSystem &system)
{
    // Node empty?
    if (node.is_null())
    {
        logi("Missing or empty 'game_states' node. No game states were loaded.");
        return false;
    }

    // Node is array type?
    if (!node.is_array())
    {
        loge("The node 'game_states' must be array type.");
        return false;
    }

    // Load game states
    for (auto it : node)
    {
        if (!loadGameState(it, manager, system))
        {
            return false;
        }
    }
    return true;
}

bool loadGameState(const nlohmann::json &node, IResourceManager &manager, GameSystem &system)
{
    // Load name of state for identification
    std::string name;
    if (!load(node, "name", name))
    {
        return false;
    }

    // Load state file name
    std::string file;
    if (!load(node, "file", file))
    {
        return false;
    }

    // Load state data from file
    if (!loadGameState(file, name, manager, system))
    {
        return false;
    }
    return true;
}

bool loadGameState(const std::string &file, const std::string &name, IResourceManager &manager, GameSystem &system)
{
    // Load json file
    nlohmann::json root;
    if (!load(file, root))
    {
        loge("Failed to load game state from file {}.", file.c_str());
        return false;
    }

    // Get game state type
    // Specific game logic s implemented in sub classes of IGameState
    // TODO Whole game logic should be data driven with only one gamestate class
    std::string type;
    if (!load(root, "type", type))
    {
        loge("Failed to load state type.");
        return false;
    }

    // Create specific game state type
    IGameState *state = nullptr;
    if (type == "title")
    {
        // state = new CTitleState;
    }
    else if (type == "game")
    {
        // state = new CGamePlayState;
    }

    // TODO Remove
    // delete state;

    return false;
}
