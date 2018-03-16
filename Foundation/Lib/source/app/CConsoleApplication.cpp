#include "foundation/app/CConsoleApplication.h"

CConsoleApplication::~CConsoleApplication() {}

bool CConsoleApplication::initialize()
{
    if (AApplication::initialize())
    {
        // Start up core system
        m_coreSystem.setApplicationName(getApplicationName());
        m_coreSystem.setCompanyName(getCompanyName());
        m_coreSystem.start();

        // TODO Start up IO system
        return true;
    }
    return false;
}

void CConsoleApplication::deinitialize()
{
    m_coreSystem.stop();
    AApplication::deinitialize();
}
