#include "foundation/job/CJobDataDescriptor.h"

#include <cassert>

CJobDataDescriptor::CJobDataDescriptor() {}

void CJobDataDescriptor::update(unsigned int index, void *data, unsigned int dataSize,
                                unsigned int sliceSize)
{
}

unsigned int CJobDataDescriptor::getBufferCount() const { return 0; }

void *CJobDataDescriptor::getBufferData(unsigned int index) const { return nullptr; }

unsigned int CJobDataDescriptor::getBufferSize(unsigned int index) const { return 0; }

unsigned int CJobDataDescriptor::getBufferSliceSize(unsigned int index) const { return 0; }
