#include "EngineConfig.h"

#include <fmtlog/fmtlog.h>
#include <kern/foundation/IniFile.h>
#include <kern/foundation/JsonUtil.h>
#include <kern/foundation/StringUtil.h>

bool load(const std::string& configFile, EngineConfig& config)
{
    // Config data with default values
    config.m_modeType = "demo";                                      // Startup mode for the application
    config.m_sceneFile = "data/world/test_1.json";                   // Scene file to load and render if mode is demo
    config.m_gameFile = "data/game/defenders_of_cthedra/game.json";  // Game file to load if mode
                                                                     // is game
    config.m_rendererType = "deferred";                              // Renderer type to use

    // Window parameters
    config.m_windowWidth = 800;
    config.m_windowHeight = 600;
    config.m_windowTitle = "CG 2015";

    // Load config file based on extension
    bool loadSuccess = false;
    if (getFileExtension(configFile) == "ini")
    {
        IniFile configIni;
        if (configIni.load(configFile))
        {
            // Load values
            config.m_modeType = configIni.getValue("mode", "type", "demo");
            config.m_sceneFile = configIni.getValue("scene", "file", "data/world/test_1.json");
            config.m_gameFile = configIni.getValue("game", "file", "data/game/defenders_of_cthedra/game.json");
            config.m_rendererType = configIni.getValue("renderer", "type", "forward");
            config.m_windowWidth = configIni.getValue("window", "width", 800);
            config.m_windowHeight = configIni.getValue("window", "height", 600);
            config.m_windowTitle = configIni.getValue("window", "type", "CG 2015");
            loadSuccess = true;
        }
    }
    else if (getFileExtension(configFile) == "json")
    {
        nlohmann::json root;
        if (load(configFile, root))
        {
            // Sub nodes
            nlohmann::json game = root["game"];
            nlohmann::json renderer = root["renderer"];
            nlohmann::json window = root["window"];

            // Load values
            config.m_modeType = "game";  // Json only supports game mode
            config.m_sceneFile = "";     // Scene file not supported/legacy
            load(game, "file", config.m_gameFile);
            load(renderer, "type", config.m_rendererType);
            load(window, "width", config.m_windowWidth);
            load(window, "height", config.m_windowHeight);
            load(window, "title", config.m_windowTitle);
            loadSuccess = true;
        }
    }
    else
    {
        logw("The config file {} has an unknown file extension.", configFile);
    }
    return loadSuccess;
}