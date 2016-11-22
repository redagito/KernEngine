#include "foundation/io/Serialize.h"

template <typename Type>
bool serializePrimitiveType(Type value, std::ostream &os)
{
  os.write(reinterpret_cast<char *>(&value), sizeof(Type));
  return os.good();
}

bool serialize(bool value, std::ostream &os)
{
  return serializePrimitiveType(value, os);
}

bool serialize(std::int8_t value, std::ostream &os)
{
  return serializePrimitiveType(value, os);
}

bool serialize(std::int16_t value, std::ostream &os)
{
  return serializePrimitiveType(value, os);
}

bool serialize(std::int32_t value, std::ostream &os)
{
  return serializePrimitiveType(value, os);
}

bool serialize(std::int64_t value, std::ostream &os)
{
  return serializePrimitiveType(value, os);
}

bool serialize(std::uint8_t value, std::ostream &os)
{
  return serializePrimitiveType(value, os);
}

bool serialize(std::uint16_t value, std::ostream &os)
{
  return serializePrimitiveType(value, os);
}

bool serialize(std::uint32_t value, std::ostream &os)
{
  return serializePrimitiveType(value, os);
}

bool serialize(std::uint64_t value, std::ostream &os)
{
  return serializePrimitiveType(value, os);
}

bool serialize(float value, std::ostream &os)
{
  return serializePrimitiveType(value, os);
}

bool serialize(double value, std::ostream &os)
{
  return serializePrimitiveType(value, os);
}

bool serialize(const std::string &value, std::ostream &os)
{
  if (!serialize(value.size(), os))
    return false;
  os.write(value.data(), value.size());
  return os.good();
}
