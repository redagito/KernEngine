#pragma once

#include <atomic>
#include <istream>
#include <ostream>

#include "foundation/io/CBinaryReader.h"
#include "foundation/io/CBinaryWriter.h"

#include "foundation/message/MessageId.h"

/**
 * Abstract message class.
 * Base class for all message classes.
 */
class AMessage
{
   public:
    AMessage();
    virtual ~AMessage();

    bool isId(const MessageId &id) const;

    bool serialize(CBinaryWriter &writer) const;
    bool deserialize(CBinaryReader &reader);

    bool isHandled() const;
    void setHandled(bool handled);

    /**
     * Bloks until message has been handled.
     */
    void waitHandled() const;

    bool isDeferred() const;
    void setDeferred(bool deferred);

    bool isDeferredHandled() const;
    void setDeferredHandled(bool deferredHandled);

    /**
     * Message should be distributed over the network.
     */
    bool isDistributed() const;
    void setDistributed(bool distributed);

    /**
     * Implement in specialized class.
     */
    virtual bool serialize(std::ostream &os) const = 0;
    virtual bool deserialize(std::istream &is) = 0;
    virtual const MessageId &getId() const = 0;

   private:
    std::atomic_bool m_handled{false};
    bool m_deferred = false;
    bool m_deferredHandled = false;
    bool m_distributed = false;
};

bool serialize(const AMessage &msg, std::ostream &os);
bool deserialize(AMessage &msg, std::istream &is);
