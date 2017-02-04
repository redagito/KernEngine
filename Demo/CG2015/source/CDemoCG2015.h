#pragma once

#include <application/AGameApplication.h>

/**
* \brief Demo CG2015
*/
class CDemoCG2015 : public AGameApplication
{
public:
	CDemoCG2015();
  ~CDemoCG2015();

protected:
	bool onInitialize() override;
	std::string getInitialGameState() const override;
};
