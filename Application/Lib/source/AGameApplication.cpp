#include "application/AGameApplication.h"

#include <GLFW/glfw3.h>

#include <graphics/resource/core/CResourceManager.h>
#include <graphics/graphics/system/CGraphicsSystem.h>
#include <graphics/graphics/window/CGlfwWindow.h>
#include <graphics/input/provider/CGlfwInputProvider.h>

#include <foundation/debug/Log.h>
#include <foundation/debug/Profile.h>
#include <foundation/util/TimeStamp.h>
#include <foundation/util/StringUtil.h>
#include <foundation/io/CIniFile.h>
#include <foundation/io/JsonUtil.h>

#include "application/game/CGameSystem.h"

struct SConfig
{
	std::string m_rendererType = "deferred";
	unsigned int m_windowWidth = 800;
	unsigned int m_windowHeight = 600;
};

static bool load(const std::string& configFile, SConfig& config)
{
	// Config data with default values
	// Renderer type to use
	config.m_rendererType = "deferred";          

	// Window parameters
	config.m_windowWidth = 800;
	config.m_windowHeight = 600;

	// Load config file based on extension
	bool loadSuccess = false;
	if (getFileExtension(configFile) == "ini")
	{
		CIniFile configIni;
		if (configIni.load(configFile))
		{
			// Load values
			config.m_rendererType = configIni.getValue("renderer", "type", "forward");
			config.m_windowWidth = configIni.getValue("window", "width", 800);
			config.m_windowHeight = configIni.getValue("window", "height", 600);
			loadSuccess = true;
		}
	}
	else if (getFileExtension(configFile) == "json")
	{
		Json::Value root;
		if (load(configFile, root))
		{
			// Sub nodes
			Json::Value renderer = root["renderer"];
			Json::Value window = root["window"];

			// Load values
			load(renderer, "type", config.m_rendererType);
			load(window, "width", config.m_windowWidth);
			load(window, "height", config.m_windowHeight);
			loadSuccess = true;
		}
	}
	else
	{
		KERN_WARNING("The config file " << configFile << " has an unknown file extension.");
	}
	return loadSuccess;
}

AGameApplication::~AGameApplication() 
{
	m_gameSystem = nullptr;
}

bool AGameApplication::initialize()
{
	// Initializes logging
	if (!AApplication::initialize())
	{
		return false;
	}

	// Load configuration file
	std::string configFile = "data/config.ini";
	SConfig config;
	if (!load(configFile, config))
	{
		KERN_WARNING(
			"Failed to load config file " << configFile << ". Starting with default settings.");
		// TODO Return if no config exists?
	}

	// Create window
	m_window = std::make_shared<CGlfwWindow>();
	if (!m_window->init(config.m_windowWidth, config.m_windowHeight, getApplicationName()))
	{
		LOG_ERROR("Failed to initialize GLFW window wrapper.");
		return false;
	}

	// TODO GLFW handle not properly wrapped away, GFLW should not be used
	// directly
	m_inputProvider = std::make_shared<CGlfwInputProvider>(m_window->getGlfwHandle());

	// Create central resource manager
	m_resourceManager = std::make_shared<CResourceManager>();
	if (m_resourceManager == nullptr)
	{
		LOG_ERROR("Failed to initialize resource manager.");
		return false;
	}

	// Create and initialize graphics system
	m_graphicsSystem = std::make_shared<CGraphicsSystem>();
	if (!m_graphicsSystem->init(*m_resourceManager))
	{
		LOG_ERROR("Failed to initialize graphics system.");
		return false;
	}

	// Create and initialize game system
	m_gameSystem = std::make_shared<CGameSystem>();
	
	// Callback for subclass
	// This should register all game states
	if (!onInitialize()) 
	{
		KERN_ERROR("Initialization callback failed");
		return false;
	}

	// Finalize game system initialization
	m_gameSystem->init(getInitialGameState(), m_graphicsSystem.get(), m_inputProvider.get(), m_resourceManager.get());

	return true;
}

int AGameApplication::run()
{
	// Function key cooldown values
	// For debugging
	double f1Cooldown = 0.0;
	double f2Cooldown = 0.0;
	double f3Cooldown = 0.0;
	double f4Cooldown = 0.0;
	double f5Cooldown = 0.0;
	double f6Cooldown = 0.0;
	double f7Cooldown = 0.0;
	double f8Cooldown = 0.0;
	double f9Cooldown = 0.0;

	// Numeric key cooldown values	
	double k1Cooldown = 0.0;
	double k2Cooldown = 0.0;
	double k3Cooldown = 0.0;
	double k4Cooldown = 0.0;
	double k5Cooldown = 0.0;

	double timeDiff = 0.0;
	bool running = true;

	// Activate mouse capture as default
	m_window->toggleMouseCapture();

	do
	{
		double startTime = glfwGetTime();

		// Cooldown calculations
		f1Cooldown -= timeDiff;
		f2Cooldown -= timeDiff;
		f3Cooldown -= timeDiff;
		f4Cooldown -= timeDiff;
		f5Cooldown -= timeDiff;
		f6Cooldown -= timeDiff;
		f7Cooldown -= timeDiff;
		f8Cooldown -= timeDiff;
		f9Cooldown -= timeDiff;

		k1Cooldown -= timeDiff;
		k2Cooldown -= timeDiff;
		k3Cooldown -= timeDiff;
		k4Cooldown -= timeDiff;
		k5Cooldown -= timeDiff;

		// Global key mappings
		// Key F1
		if (glfwGetKey(m_window->getGlfwHandle(), GLFW_KEY_F1) == GLFW_PRESS &&
			f1Cooldown <= 0.f)
		{
			f1Cooldown = 0.3f;
			// Not implemented
		}

		// Key F2
		if (glfwGetKey(m_window->getGlfwHandle(), GLFW_KEY_F2) == GLFW_PRESS &&
			f2Cooldown <= 0.f)
		{
			f2Cooldown = 0.3f;
			// Turns debug overlay on / off
			m_graphicsSystem->toggleDebugOverlay();
		}

		// Key F3
		if (glfwGetKey(m_window->getGlfwHandle(), GLFW_KEY_F3) == GLFW_PRESS &&
			f3Cooldown <= 0.f)
		{
			f3Cooldown = 0.3f;
			// Turns wireframe mode on / off
			m_graphicsSystem->toggleWireframeMode();
		}

		// Key F4
		if (glfwGetKey(m_window->getGlfwHandle(), GLFW_KEY_F4) == GLFW_PRESS &&
			f4Cooldown <= 0.f)
		{
			f4Cooldown = 0.3f;
			// Toggles global texture filtering between nearest neighbor and bilinear
			// TODO Implement
		}

		// Key F5
		if (glfwGetKey(m_window->getGlfwHandle(), GLFW_KEY_F5) == GLFW_PRESS &&
			f5Cooldown <= 0.f)
		{
			f5Cooldown = 0.3f;
			// Toggles global texture mip map filtering between off, nearest neighbor
			// and bilinear
			// TODO Implement
		}

		// Key F6
		if (glfwGetKey(m_window->getGlfwHandle(), GLFW_KEY_F6) == GLFW_PRESS &&
			f6Cooldown <= 0.f)
		{
			f6Cooldown = 0.3f;
			// Unused
		}

		// Key F7
		if (glfwGetKey(m_window->getGlfwHandle(), GLFW_KEY_F7) == GLFW_PRESS &&
			f7Cooldown <= 0.f)
		{
			f7Cooldown = 0.3f;
			// Unused
		}

		// Key F8
		if (glfwGetKey(m_window->getGlfwHandle(), GLFW_KEY_F8) == GLFW_PRESS &&
			f8Cooldown <= 0.f)
		{
			f8Cooldown = 0.3f;
			// Toggles view frustum culling on/off
			m_graphicsSystem->toggleViewFrustumCulling();
		}

		// Key F9
		if (glfwGetKey(m_window->getGlfwHandle(), GLFW_KEY_F9) == GLFW_PRESS &&
			f9Cooldown <= 0.f)
		{
			f9Cooldown = 0.3f;
			m_graphicsSystem->toggleDebugOverlayTransparency();
		}

		// Key 1
		if (glfwGetKey(m_window->getGlfwHandle(), GLFW_KEY_1) == GLFW_PRESS &&
			k1Cooldown <= 0.f)
		{
			k1Cooldown = 0.3f;
			// Turns mouse capture on/off
			m_window->toggleMouseCapture();
		}

		// Key 2
		if (glfwGetKey(m_window->getGlfwHandle(), GLFW_KEY_2) == GLFW_PRESS &&
			k2Cooldown <= 0.f)
		{
			k2Cooldown = 0.3f;
			// Sets active rendering device to deferred renderer
			m_graphicsSystem->setActiveRenderer("deferred");
		}

		// Key 3
		if (glfwGetKey(m_window->getGlfwHandle(), GLFW_KEY_3) == GLFW_PRESS &&
			k3Cooldown <= 0.f)
		{
			k3Cooldown = 0.3f;
			// Sets active rendering device to forward renderer
			m_graphicsSystem->setActiveRenderer("forward");
		}

		// Game system update
		if (!m_gameSystem->update((float)timeDiff))
		{
			// On return false quit application
			running = false;
		}

		// Draw active scene from active camera with active rendering device
		m_graphicsSystem->draw(*m_window);

		// Swap buffers after draw
		m_window->swapBuffer();

		// Update input
		glfwPollEvents();

		// Frame time
		timeDiff = glfwGetTime() - startTime;

	} while (glfwGetKey(m_window->getGlfwHandle(), GLFW_KEY_ESCAPE) !=
		GLFW_PRESS &&
		glfwWindowShouldClose(m_window->getGlfwHandle()) == 0 && running);

	return 0;
}

void AGameApplication::addGameState(const std::string& stateName, IGameState* state)
{
	assert(m_gameSystem != nullptr);
	assert(!m_gameSystem->hasState(stateName));
	m_gameSystem->addState(stateName, state);
}

bool AGameApplication::initWindow(unsigned int width, unsigned int height, const std::string &title)
{
	// Check if already initialized
	if (m_window != nullptr)
	{
		return true;
	}

	LOG_INFO("Initializing application window.");
	LOG_INFO("Window width: %u.", width);
	LOG_INFO("Window height: %u.", height);
	LOG_INFO("Window title: %s.", title.c_str());

	// Create window
	CGlfwWindow *window = new CGlfwWindow;
	if (!window->init(width, height, title))
	{
		LOG_ERROR("Failed to initialize GLFW window wrapper.");
		delete window;
		return false;
	}

	// Set pointer
	m_window.reset(window);
	return true;
}