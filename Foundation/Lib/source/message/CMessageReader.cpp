#include "foundation/message/CMessageReader.h"

#include "foundation/debug/Log.h"

CMessageReader::MessageFactory CMessageReader::s_messageFactory;

CMessageReader::CMessageReader(const std::shared_ptr<std::istream> &stream) : m_reader(stream) {}

AMessage *CMessageReader::read()
{
    CEightCC key;
    if (!m_reader.read(key))
    {
        return nullptr;
    }
    KERN_DEBUG("Creating message with class code " << key);
    AMessage *msg = s_messageFactory.create(key);
    if (msg == nullptr)
    {
        KERN_ERROR("Failed to create message");
        return nullptr;
    }
    if (!msg->deserialize(m_reader))
    {
        KERN_ERROR("Failed to deserialize message");
        delete msg;
        return nullptr;
    }
    return msg;
}

bool CMessageReader::registerMessage(const CEightCC &code, const MessageFactory::Creator &creator)
{
    KERN_DEBUG("Registered message with class code " << code);
    if (s_messageFactory.hasCreator(code))
    {
        return false;
    }
    s_messageFactory.addCreator(code, creator);
    return true;
}
