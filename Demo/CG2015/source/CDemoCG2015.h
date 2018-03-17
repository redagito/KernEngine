#pragma once

#include <application/AGameApplication.h>

/**
* \brief Demo CG2015
*/
class CDemoCG2015 : public AGameApplication
{
protected:
	bool onInitialize() override;
	std::string getInitialGameState() const override;
};
