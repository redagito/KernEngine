#include "graphics/resource/Resource.h"

#include "graphics/resource/core/CResourceManager.h"

IResourceManager *createResourceManager() { return new CResourceManager; }