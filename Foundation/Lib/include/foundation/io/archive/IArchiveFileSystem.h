#pragma once

#include <memory>

#include "foundation/io/CUniformResourceIdentifier.h"
#include "foundation/io/archive/IArchive.h"

class IArchiveFileSystem
{
   public:
    virtual ~IArchiveFileSystem();

    virtual bool open() = 0;
    virtual void close() = 0;
    virtual bool isOpen() const = 0;

    virtual std::shared_ptr<IArchive> mount(const CUniformResourceIdentifier &uri) = 0;
    virtual void unmount(const CUniformResourceIdentifier &uri) = 0;
    virtual void unmount(const std::shared_ptr<IArchive> &archive) = 0;
    virtual bool isMounted(const CUniformResourceIdentifier &uri) const = 0;

    virtual std::vector<std::shared_ptr<IArchive>> getMountedArchives() const = 0;
    virtual std::shared_ptr<IArchive> findArchive(const CUniformResourceIdentifier &uri) const = 0;
};
