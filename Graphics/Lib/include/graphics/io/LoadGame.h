#pragma once

#include <string>

namespace Json
{
class Value;
}

class IResourceManager;
class CGameSystem;

/**
* \brief Loads entire game.
*/
bool load(const std::string &file, IResourceManager &manager,
          CGameSystem &system);
bool loadGameStates(const Json::Value &node, IResourceManager &manager,
                    CGameSystem &system);
bool loadGameState(const Json::Value &node, IResourceManager &manager,
                   CGameSystem &system);
bool loadGameState(const std::string &file, const std::string &name,
                   IResourceManager &manager, CGameSystem &system);