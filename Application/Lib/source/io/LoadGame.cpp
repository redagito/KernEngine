#include "application/io/LoadGame.h"

#include <string>

#include <json/json.h>
#include <glm/glm.hpp>

#include <foundation/debug/Log.h>
#include <foundation/io/JsonDeserialize.h>
#include <foundation/io/JsonUtil.h>

#include "graphics/resource/IResourceManager.h"

#include "application/game/CGameSystem.h"
#include "application/game/IGameState.h"
//#include "graphics/game/state/CTitleState.h"
//#include "graphics/game/state/CGamePlayState.h"

bool load(const std::string &file, IResourceManager &manager, CGameSystem &system)
{
    Json::Value root;
    if (!load(file, root))
    {
        LOG_ERROR("Failed to load game from file %s.", file.c_str());
        return false;
    }

    // Load game string identifer (name of the game)
    std::string name;
    if (!load(root, "name", name))
    {
        LOG_ERROR("Failed to load game name identfier from file %s.", file.c_str());
        return false;
    }
    // TODO Name identifier unused. Use for window title.
    LOG_INFO("Loading game %s.", name.c_str());

    // Load game states which contain high level game logic and application
    // control
    if (!loadGameStates(root["game_states"], manager, system))
    {
        LOG_ERROR("Failed to load game states from file %s.", file.c_str());
        return false;
    }

    // Load identifier of the initial state
    std::string initialState;
    if (!load(root, "start_state", initialState))
    {
        LOG_ERROR("Failed to load initial game state from file %s.", file.c_str());
        return false;
    }

    // Load successful
    LOG_INFO("Loaded game file %s.", file.c_str());
    return true;
}

bool loadGameStates(const Json::Value &node, IResourceManager &manager, CGameSystem &system)
{
    // Node empty?
    if (node.empty())
    {
        LOG_INFO("Missing or empty 'game_states' node. No game states were loaded.");
        return false;
    }

    // Node is array type?
    if (!node.isArray())
    {
        LOG_ERROR("The node 'game_states' must be array type.");
        return false;
    }

    // Load game states
    for (unsigned int i = 0; i < node.size(); ++i)
    {
        if (!loadGameState(node[i], manager, system))
        {
            return false;
        }
    }
    return true;
}

bool loadGameState(const Json::Value &node, IResourceManager &manager, CGameSystem &system)
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

bool loadGameState(const std::string &file, const std::string &name, IResourceManager &manager,
                   CGameSystem &system)
{
    // Load json file
    Json::Value root;
    if (!load(file, root))
    {
        LOG_ERROR("Failed to load game state from file %s.", file.c_str());
        return false;
    }

    // Get game state type
    // Specific game logic s implemented in sub classes of IGameState
    // TODO Whole game logic should be data driven with only one gamestate class
    std::string type;
    if (!load(root, "type", type))
    {
        LOG_ERROR("Failed to load state type.");
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
