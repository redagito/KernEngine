#pragma once

#include <array>

/**
 * Data descriptor for input and output data of a single job.
 * The data is split into elements and slices. A job function
 * may be called with any number of elements up to the maximum
 * slice size.
 * Within the current slice, the job may perform random access
 * on the elements.
 * Slices may not depend on each other (the job system may split
 * a job into slices which are processed in parallel).
 */
class CJobDataDescriptor
{
public:
  CJobDataDescriptor();

  /**
   * Update a buffer.
   */
  void update(unsigned int index, void *data, unsigned int dataSize,
              unsigned int sliceSize);

  /**
   * Returns number of active buffers.
   */
  unsigned int getBufferCount() const;

  /**
   * \return Pointer to buffer data.
   */
  void *getBufferData(unsigned int index) const;

  /**
   * \return Size of a buffer.
   */
  unsigned int getBufferSize(unsigned int index) const;

  /**
   * \return Size of a single slice within the buffer.
   */
  unsigned int getBufferSliceSize(unsigned int index) const;

private:
  static const int s_maxNumBuffers = 8;

  unsigned int m_bufferCount = 0;

  struct SBuffer
  {
    void *m_data = nullptr;
    unsigned int m_dataSize = 0;
    unsigned int m_dataSliceSize = 0;
  };
  std::array<SBuffer, 8> m_buffers;
};
