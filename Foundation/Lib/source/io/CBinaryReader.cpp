#include "foundation/io/CBinaryReader.h"

CBinaryReader::CBinaryReader(const std::shared_ptr<std::istream> &stream)
    : m_stream(stream)
{
}
