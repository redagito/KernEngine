#pragma once

#include <string>

struct SEngineConfig
{
	std::string m_modeType = "demo";
	std::string m_sceneFile;
	std::string m_gameFile;
	std::string m_rendererType = "deferred";
	unsigned int m_windowWidth = 800;
	unsigned int m_windowHeight = 600;
	std::string m_windowTitle = "CG 2015";
};

bool load(const std::string& file, SEngineConfig& config);