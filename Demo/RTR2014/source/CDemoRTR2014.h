#pragma once

#include <application/AGameApplication.h>

/**
 * \brief Demo application class.
 */
class CDemoRTR2014 : public AGameApplication
{
private:
	bool onInitialize() override;
	std::string getInitialGameState() const override;
};
