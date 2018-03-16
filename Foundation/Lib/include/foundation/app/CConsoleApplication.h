#pragma once

#include "foundation/app/AApplication.h"

#include "foundation/core/CCoreSystem.h"
#include "foundation/io/CIOSystem.h"

class CConsoleApplication : public AApplication
{
   public:
    virtual ~CConsoleApplication();

    virtual bool initialize() override;
    virtual void deinitialize() override;

   private:
    CCoreSystem m_coreSystem;
    CIOSystem m_ioSystem;
};
