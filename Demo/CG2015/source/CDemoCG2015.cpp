#include "CDemoCG2015.h"

#include "state/CLoadState.h"
#include "state/CTitleState.h"
#include "state/CGamePlayState.h"
#include "state/CLoseState.h"
#include "state/CWinState.h"

bool CDemoCG2015::onInitialize()
{
	addGameState("load", new CLoadState("data/world/load_1.json", 10.f));
	addGameState("title", new CTitleState("data/world/intro_1.json"));
	addGameState("game", new CGamePlayState());
	addGameState("lose", new CLoseState("data/world/lose.json"));
	addGameState("win", new CWinState("data/world/win.json"));
	return true;
}

std::string CDemoCG2015::getInitialGameState() const
{
	return "load";
}