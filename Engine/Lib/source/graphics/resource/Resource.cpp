#include "kern/graphics/resource/Resource.h"

#include "kern/graphics/resource/ResourceManager.h"

IResourceManager *createResourceManager() { return new ResourceManager; }