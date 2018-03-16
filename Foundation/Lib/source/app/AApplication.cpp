#include "foundation/app/AApplication.h"

#include <fstream>

#include "foundation/app/ApplicationSettings.h"
#include "foundation/util/TimeStamp.h"

#include "foundation/debug/Log.h"
#include "foundation/debug/Profile.h"

AApplication::~AApplication()
{
    deinitialize();
#ifdef KERN_PROFILE
    std::string profileFile = "profile_" + m_initializationTimestamp + ".log";
    std::ofstream profile(profileFile);
    if (!profile.is_open())
    {
        KERN_WARNING("Unable to open the profiling data file " << profileFile);
        return;
    }

    if (!CProfiler::write(profile))
    {
        KERN_WARNING("Failed to write profiling data");
    }
#endif
}

void AApplication::setCompanyName(const std::string &name) { m_companyName = name; }

const std::string &AApplication::getCompanyName() const { return m_companyName; }

void AApplication::setApplicationName(const std::string &name) { m_applicationName = name; }

const std::string &AApplication::getApplicationName() const { return m_applicationName; }

void AApplication::setApplicationId(const std::string &id) { m_applicationId = id; }

const std::string &AApplication::getApplicationId() const { return m_applicationId; }

void AApplication::setApplicationVersion(const std::string &version)
{
    m_applicationVersion = version;
}

const std::string &AApplication::getApplicationVersion() const { return m_applicationVersion; }

void AApplication::setCommandLineArguments(const Configuration &arguments)
{
    m_configuration = arguments;
}

const Configuration &AApplication::getCommandLineArguments() const { return m_configuration; }

bool AApplication::isInitialized() const { return m_initialized; }

bool AApplication::initialize()
{
    if (m_initialized)
    {
        // Already initialized
        return false;
    }

    // Set timestamp
    m_initializationTimestamp = createTimeStamp();
    // Initialize log file
    std::string logFile = "log_" + m_initializationTimestamp + ".log";
    if (!CLogger::initLogFile(logFile))
    {
        KERN_WARNING("Failed to initialize logging to file " << logFile);
    }

    m_initialized = true;
    return m_initialized;
}

void AApplication::deinitialize()
{
    if (!m_initialized)
    {
        // Not initialized
        return;
    }
    // Nothing to do here
    return;
}
