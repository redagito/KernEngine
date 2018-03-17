#include "CDemoRTR2014.h"

#include "state/CLoadState.h"
#include "state/CDemoState.h"

bool CDemoRTR2014::onInitialize()
{
	addGameState("load", new CLoadState("data/scene/load_1.json", 10.f));
	addGameState("demo", new CDemoState("data/scene/all.json"));
	return true;
}

std::string CDemoRTR2014::getInitialGameState() const
{
	return "load";
}