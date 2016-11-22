#pragma once

#include <ostream>
#include <memory>

#include "foundation/io/Serialize.h"

/**
 * Binary writer class.
 * Provides wrapper for writing binary data to a stream.
 */
class CBinaryWriter
{
public:
  CBinaryWriter(const std::shared_ptr<std::ostream> &stream);

  template <typename T> bool write(const T &t)
  {
    return serialize(t, *m_stream);
  }

private:
  std::shared_ptr<std::ostream> m_stream;
};
