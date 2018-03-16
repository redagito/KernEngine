#include "foundation/io/CIOInterfaceHandler.h"

void CIOInterfaceHandler::onAttach() {}

void CIOInterfaceHandler::onDetach() {}

bool CIOInterfaceHandler::handle(const std::shared_ptr<AMessage> &msg) { return false; }

void CIOInterfaceHandler::open() {}

void CIOInterfaceHandler::update() {}

void CIOInterfaceHandler::close() {}
