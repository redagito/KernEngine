#include "graphics/engine/Engine.h"

#include "graphics/engine/CEngine.h"

IEngine *createEngine() { return new CEngine; }