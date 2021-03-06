#include "foundation/util/CValue.h"

#include <cassert>
#include <cstring>
#include <sstream>

CValue::CValue()
{
    // Empty
}

CValue::CValue(const CValue &value) { (*this) = value; }

CValue::CValue(CValue &&value) { (*this) = value; }

CValue::CValue(bool value)
{
    m_type = EType::Boolean;
    m_boolean = value;
}

CValue::CValue(int32_t value)
{
    m_type = EType::Integer;
    m_integer = value;
}

CValue::CValue(uint32_t value)
{
    m_type = EType::UnsignedInteger;
    m_unsignedInteger = value;
}

CValue::CValue(float value)
{
    m_type = EType::Float;
    m_float = value;
}

CValue::CValue(const std::string &value)
{
    m_type = EType::String;
    m_string = new char[value.length() + 1];
    memcpy(m_string, value.data(), value.length());
    m_string[value.length()] = '\0';
}

CValue::CValue(const char *value)
{
    m_type = EType::String;
    auto length = std::strlen(value);
    m_string = new char[length + 1];
    memcpy(m_string, value, length);
    m_string[length] = '\0';
}

CValue::~CValue()
{
    if (m_type == EType::String && m_string != nullptr)
    {
        delete[] m_string;
    }
}

bool CValue::convert(bool &value) const
{
    switch (m_type)
    {
    case EType::Boolean:
        value = m_boolean;
        return true;

    case EType::Integer:
        value = m_integer != 0;
        return true;

    case EType::UnsignedInteger:
        value = m_unsignedInteger != 0;
        return true;

    case EType::Float:
        value = m_float != 0.f;
        return true;

    case EType::String:
        if (std::strcmp(m_string, "true") == 0)
        {
            value = true;
            return true;
        }
        else if (std::strcmp(m_string, "false") == 0)
        {
            value = false;
            return true;
        }
        return false;

    default:
        // Unknown or invalid conversion
        return false;
    }
}

bool CValue::convert(int32_t &value) const
{
    switch (m_type)
    {
    case EType::Boolean:
        if (m_boolean)
        {
            value = 1;
        }
        else
        {
            value = 0;
        }
        return true;

    case EType::Integer:
        value = m_integer;
        return true;

    case EType::UnsignedInteger:
        value = (int32_t)m_unsignedInteger;
        return true;

    case EType::Float:
        value = (int32_t)m_float;
        return true;

    case EType::String:
        try
        {
            value = std::stoi(m_string);
        }
        catch (const std::exception &)
        {
            return false;
        }
        return true;

    default:
        // Unknown or invalid conversion
        return false;
    }
}

bool CValue::convert(uint32_t &value) const
{
    switch (m_type)
    {
    case EType::Boolean:
        if (m_boolean)
        {
            value = 1;
        }
        else
        {
            value = 0;
        }
        return true;

    case EType::Integer:
        value = (uint32_t)m_integer;
        return true;

    case EType::UnsignedInteger:
        value = m_unsignedInteger;
        return true;

    case EType::Float:
        value = (uint32_t)m_float;
        return true;

    case EType::String:
        try
        {
            value = std::stoul(m_string);
        }
        catch (const std::exception &)
        {
            return false;
        }
        return true;

    default:
        // Unknown or invalid conversion
        return false;
    }
}

bool CValue::convert(float &value) const
{
    switch (m_type)
    {
    case EType::Boolean:
        if (m_boolean)
        {
            value = 1.0f;
        }
        else
        {
            value = 0.0f;
        }
        return true;

    case EType::Integer:
        value = (float)m_integer;
        return true;

    case EType::UnsignedInteger:
        value = (float)m_unsignedInteger;
        return true;

    case EType::Float:
        value = m_float;
        return true;

    case EType::String:
        try
        {
            value = std::stof(m_string);
        }
        catch (const std::exception &)
        {
            return false;
        }
        return true;

    default:
        // Unknown or invalid conversion
        return false;
    }
}

bool CValue::convert(std::string &value) const
{
    // TODO Slow!!
    std::stringstream ss;
    switch (m_type)
    {
    case EType::Boolean:
        if (m_boolean)
        {
            value = "true";
        }
        else
        {
            value = "false";
        }
        return true;

    case EType::Integer:
        ss << m_integer;
        value = ss.str();
        return true;

    case EType::UnsignedInteger:
        ss << m_unsignedInteger;
        value = ss.str();
        return true;

    case EType::Float:
        ss << m_float;
        value = ss.str();
        return true;

    case EType::String:
        value = std::string(m_string);
        return true;

    default:
        // Unknown or invalid conversion
        return false;
    }
}

CValue::EType CValue::getType() const { return m_type; }

std::string CValue::toString() const
{
    std::stringstream ss;
    switch (m_type)
    {
    case CValue::EType::Boolean:
        ss << "Bool";
        break;

    case CValue::EType::Float:
        ss << "Float";
        break;

    case CValue::EType::Integer:
        ss << "Integer";
        break;

    case CValue::EType::Invalid:
        return "Invalid";
        break;

    case CValue::EType::String:
        ss << "String";
        break;

    case CValue::EType::UnsignedInteger:
        ss << "UnsignedInteger";
        break;
    }
    std::string val;
    convert(val);
    ss << " : " << val;
    return ss.str();
}

CValue &CValue::operator=(const CValue &rhs)
{
    // Same object?
    if (this == &rhs)
    {
        return *this;
    }

    // Clean up internal string memory
    if (m_type == EType::String)
    {
        delete[] m_string;
        m_string = nullptr;
    }

    m_type = rhs.m_type;
    switch (m_type)
    {
    case EType::Boolean:
        m_boolean = rhs.m_boolean;
        break;

    case EType::Integer:
        m_integer = rhs.m_integer;
        break;

    case EType::UnsignedInteger:
        m_unsignedInteger = rhs.m_unsignedInteger;
        break;

    case EType::Float:
        m_float = rhs.m_float;
        break;

    case EType::String:
    {
        size_t length = strlen(rhs.m_string) + 1;
        m_string = new char[length];
        memcpy(m_string, rhs.m_string, length);
    }
    break;
    default:
        // Invalid
        break;
    }
    return *this;
}

CValue &CValue::operator=(CValue &&rhs)
{
    if (this == &rhs)
    {
        // Same object, return
        return *this;
    }

    if (m_type == EType::String)
    {
        delete[] m_string;
        m_string = nullptr;
    }

    m_type = rhs.m_type;
    switch (m_type)
    {
    case EType::Boolean:
        m_boolean = rhs.m_boolean;
        rhs.m_boolean = false;
        break;

    case EType::Integer:
        m_integer = rhs.m_integer;
        rhs.m_integer = 0;
        break;

    case EType::UnsignedInteger:
        m_unsignedInteger = rhs.m_unsignedInteger;
        rhs.m_unsignedInteger = 0;
        break;

    case EType::Float:
        m_float = rhs.m_float;
        rhs.m_float = 0.f;
        break;

    case EType::String:
    {
        m_string = rhs.m_string;
        rhs.m_string = nullptr;
    }
    break;

    default:
        // Invalid
        break;
    }
    rhs.m_type = EType::Invalid;

    return *this;
}

bool CValue::operator==(const CValue &rhs) const
{
    // Compare
    switch (m_type)
    {
    case EType::Integer:
        switch (rhs.m_type)
        {
        case EType::Integer:
            // Integer with integer
            return m_integer == rhs.m_integer;
        default:
            // Not implemented
            assert(false);
        }
    case EType::String:
        switch (rhs.m_type)
        {
        case EType::String:
            // String with string
            return std::strcmp(this->m_string, rhs.m_string) == 0;
        case EType::Integer:
        {
            int32_t i;
            if (!this->convert(i))
                return false;
            return i == rhs.m_integer;
        }
        case EType::Boolean:
            bool b;
            if (!this->convert(b))
                return false;
            return b == rhs.m_boolean;
        default:
            // Not implemented
            assert(false);
        }
    }
    return false;
}

bool CValue::operator!=(const CValue &rhs) const { return !((*this) == rhs); }

bool CValue::operator<=(const CValue &rhs) const { return ((*this) == rhs) || ((*this) < rhs); }

bool CValue::operator<(const CValue &rhs) const
{
    // Arithmetic subtract
    switch (m_type)
    {
    case EType::Integer:
        switch (rhs.m_type)
        {
        case EType::Integer:
            // Integer with integer
            return m_integer < rhs.m_integer;
        default:
            // Not implemented
            assert(false);
        }
    }
    return false;
}
