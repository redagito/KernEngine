#include <memory>

#include "CDemoCG2015.h"

int main(int argc, const char **argv)
{
	// Command line parameters
	Configuration cmdConfig;
	if (!parseCommandLine(argc, argv, cmdConfig))
	{
		// Unable to parse command line
		return 1;
	}

	CDemoCG2015 app;

	app.setCommandLineArguments(cmdConfig);
	app.setApplicationId("CG2015");
	app.setApplicationName("Computer Graphics Demo Game 2015");
	app.setApplicationVersion("v1.0");
	app.setCompanyName("KernTec");

    // Initialize from config file
    if (!app.initialize())
    {
        return 1;
    }
    // Run engine
    app.run();

    return 0;
}
