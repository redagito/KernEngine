#pragma once

#include <memory>

inline std::ostream& operator<<(std::ostream& os, const std::nullptr_t& n)
{
	os << "nullptr";
	return os;
}