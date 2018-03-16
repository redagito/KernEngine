#include "foundation/io/CGameContentSystem.h"

CGameContentSystem::CGameContentSystem() {}

CGameContentSystem::~CGameContentSystem() { stop(); }

void CGameContentSystem::setTitle(const std::string &title) { m_title = title; }

const std::string &CGameContentSystem::getTitle() const { return m_title; }

void CGameContentSystem::setTitleId(const std::string &id) { m_titleId = id; }

const std::string &CGameContentSystem::getTitleId() const { return m_titleId; }

void CGameContentSystem::setVersion(const std::string &version) { m_version = version; }

const std::string &CGameContentSystem::getVersion() const { return m_version; }

void CGameContentSystem::start() { m_running = true; }

void CGameContentSystem::stop() { m_running = false; }
