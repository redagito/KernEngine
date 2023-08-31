#pragma once

#include "kern/resource/ResourceId.h"
#include "kern/resource/ResourceType.h"
#include "kern/resource/ResourceEvent.h"

class IResourceManager;

/**
 * \brief Resource listener interface class.
 * Receives resource events after being registered to a resource manager.
 */
class IResourceListener
{
   public:
    /**
     * \brief Virtual destructor for interface class.
     */
    virtual ~IResourceListener() = default;

    /**
     * \brief Called on attaching listener.
     */
    virtual void onAttach(IResourceManager *resourceManager) = 0;

    /**
     * \brief Called on detaching listener.
     */
    virtual void onDetach(IResourceManager *resourceManager) = 0;

    /**
     * \brief Called on resource event.
     */
    virtual void notify(ResourceType type, ResourceId id, ResourceEvent event,
                        IResourceManager *resourceManager) = 0;
};
