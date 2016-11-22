#include "foundation/util/CEightCC.h"

#include "foundation/io/Serialize.h"
#include "foundation/io/Deserialize.h"

#include <cstring>
#include <cassert>
#include <algorithm>

CEightCC::CEightCC() {}

CEightCC::CEightCC(std::uint64_t value) { set(value); }

CEightCC::CEightCC(const std::string &value) { set(value); }

void CEightCC::set(std::uint64_t value) { m_code = value; }

void CEightCC::set(const std::string &value)
{
  assert(value.size() == 8);
  std::memcpy(&m_code, value.data(), std::min((size_t)8, value.size()));
}

std::uint64_t CEightCC::get() const { return m_code; }

bool CEightCC::isValid() const { return m_code != 0; }

std::string CEightCC::toString() const
{
  return std::string{(const char *)&m_code, 8};
}

bool CEightCC::operator==(const CEightCC &rhs) const
{
  return get() == rhs.get();
}

bool CEightCC::operator<(const CEightCC &rhs) const
{
  return get() < rhs.get();
}

bool serialize(const CEightCC &code, std::ostream &os)
{
  return serialize(code.get(), os);
}

bool deserialize(CEightCC &code, std::istream &is)
{
  std::uint64_t value;
  if (!deserialize(value, is))
  {
    return false;
  }
  code.set(value);
  return true;
}

std::ostream &operator<<(std::ostream &os, const CEightCC &rhs)
{
  os << rhs.toString();
  return os;
}

std::size_t std::hash<CEightCC>::operator()(const CEightCC &code) const
{
  std::hash<std::uint64_t> h;
  return h(code.get());
}
