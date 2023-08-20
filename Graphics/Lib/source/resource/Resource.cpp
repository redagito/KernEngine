#include "graphics/resource/Resource.h"

#include "graphics/resource/CResourceManager.h"

IResourceManager *createResourceManager() { return new CResourceManager; }