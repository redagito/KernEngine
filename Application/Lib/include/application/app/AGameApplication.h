#pragma once

#include <foundation/app/AApplication.h>

class AGameApplication : public AApplication
{
public:

	virtual bool initialize() override;
	virtual void deinitialize() override;

private:

};