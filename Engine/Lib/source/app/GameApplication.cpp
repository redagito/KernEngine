#include "kern/app/GameApplication.h"

GameApplication::GameApplication()
{
    //
    m_gameSystem = std::make_unique<GameSystem>();
}

GameApplication::~GameApplication()
{
    m_gameSystem = nullptr;
    //
}

bool GameApplication::init() { return true; }
void GameApplication::run() {}