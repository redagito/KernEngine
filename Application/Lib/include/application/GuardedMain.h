#pragma once

#include <exception>
#include <string>

#include <foundation/debug/Log.h>
#include <foundation/util/Configuration.h>

/**
 * \brief Guarded main function
 * For safely creating, initializing and running game applications.
 * This should be used as the main entry point for applications using
 * the common KernEngine application infrastructure for single apps.
 */
template <typename AppType>
int GuardedMain(int argc, const char** argv, const std::string& appId, const std::string& appName,
                const std::string& appVersion, const std::string& companyName)
{
    try
    {
        // Command line parameters
        Configuration cmdConfig;
        if (!parseCommandLine(argc, argv, cmdConfig))
        {
            // Unable to parse command line
            KERN_ERROR("Failed to parse the command line arguments");
            return EXIT_FAILURE;
        }

        AppType app;

        app.setCommandLineArguments(cmdConfig);
        app.setApplicationId(appId);
        app.setApplicationName(appName);
        app.setApplicationVersion(appVersion);
        app.setCompanyName(companyName);

        // Initialize the app
        if (!app.initialize())
        {
            KERN_ERROR("Failed to initialize the application");
            return EXIT_FAILURE;
        }

        // Run application
        auto returnCode = app.run();

        // Deinitialize specifically
        app.deinitialize();

        return returnCode;
    }
    catch (const std::exception& e)
    {
        KERN_ERROR("Exception caught in Main: " << e.what());
        return EXIT_FAILURE;
    }
}
