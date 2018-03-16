#pragma once

#include <istream>
#include <memory>

#include "foundation/io/Deserialize.h"

/**
 * Provides wrapper for reading binary data from a stream.
 */
class CBinaryReader
{
   public:
    CBinaryReader(const std::shared_ptr<std::istream> &stream);

    template <typename T>
    bool read(T &t)
    {
        return deserialize(t, *m_stream);
    }

   private:
    std::shared_ptr<std::istream> m_stream;
};
