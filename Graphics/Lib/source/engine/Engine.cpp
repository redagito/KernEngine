#include "Engine.h"

#include "CEngine.h"

IEngine *createEngine() { return new CEngine; }