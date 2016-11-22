#include "foundation/io/CBinaryWriter.h"

CBinaryWriter::CBinaryWriter(const std::shared_ptr<std::ostream> &stream)
    : m_stream(stream)
{
}
