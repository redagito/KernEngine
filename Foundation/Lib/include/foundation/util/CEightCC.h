#pragma once

#include <cstdint>
#include <istream>
#include <ostream>
#include <string>

/**
 * Eight character code.
 */
class CEightCC
{
   public:
    CEightCC();
    CEightCC(std::uint64_t value);
    CEightCC(const std::string &value);

    void set(std::uint64_t value);
    void set(const std::string &value);
    std::uint64_t get() const;

    bool isValid() const;

    std::string toString() const;

    bool operator==(const CEightCC &rhs) const;
    bool operator<(const CEightCC &rhs) const;

   private:
    std::uint64_t m_code = 0;
};

/**
 * Serialization support.
 */
bool serialize(const CEightCC &code, std::ostream &os);
bool deserialize(CEightCC &code, std::istream &is);

std::ostream &operator<<(std::ostream &os, const CEightCC &rhs);

namespace std
{
// Support hashing
template <>
struct hash<CEightCC>
{
    std::size_t operator()(const CEightCC &code) const;
};
}  // namespace std
