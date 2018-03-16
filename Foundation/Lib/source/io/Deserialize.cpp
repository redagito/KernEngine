#include "foundation/io/Deserialize.h"

template <typename Type>
bool deserializePrimitiveType(Type &value, std::istream &is)
{
    is.read(reinterpret_cast<char *>(&value), sizeof(Type));
    return is.good();
}

bool deserialize(bool &value, std::istream &is) { return deserializePrimitiveType(value, is); }

bool deserialize(char &value, std::istream &is) { return deserializePrimitiveType(value, is); }

bool deserialize(std::int8_t &value, std::istream &is)
{
    return deserializePrimitiveType(value, is);
}

bool deserialize(std::int16_t &value, std::istream &is)
{
    return deserializePrimitiveType(value, is);
}

bool deserialize(std::int32_t &value, std::istream &is)
{
    return deserializePrimitiveType(value, is);
}

bool deserialize(std::int64_t &value, std::istream &is)
{
    return deserializePrimitiveType(value, is);
}

bool deserialize(std::uint8_t &value, std::istream &is)
{
    return deserializePrimitiveType(value, is);
}

bool deserialize(std::uint16_t &value, std::istream &is)
{
    return deserializePrimitiveType(value, is);
}

bool deserialize(std::uint32_t &value, std::istream &is)
{
    return deserializePrimitiveType(value, is);
}

bool deserialize(std::uint64_t &value, std::istream &is)
{
    return deserializePrimitiveType(value, is);
}

bool deserialize(float &value, std::istream &is) { return deserializePrimitiveType(value, is); }

bool deserialize(double &value, std::istream &is) { return deserializePrimitiveType(value, is); }

bool deserialize(std::string &value, std::istream &is)
{
    std::vector<char> buffer;
    if (!deserialize(buffer, is))
    {
        return false;
    }
    value.assign(buffer.data());
    return is.good();
}
