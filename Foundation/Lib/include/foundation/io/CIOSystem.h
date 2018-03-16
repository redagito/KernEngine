#pragma once

#include <chrono>
#include <cstdint>
#include <memory>

#include "foundation/io/CUniformResourceIdentifier.h"

class CIOSystem
{
   public:
    CIOSystem();
    ~CIOSystem();

    /**
     * Mount a file archive.
     */
    bool mountArchive(const CUniformResourceIdentifier &uri);

    /**
     * Unmounts a previously mounted archive.
     */
    void unmountArchive(const CUniformResourceIdentifier &uri);

    /**
     * \return True if the specified archive is currently mounted
     *         and false otherwise.
     */
    bool isArchiveMounted(const CUniformResourceIdentifier &uri);

    /**
     * Enable or disable transparent archive file system layering.
     */
    bool setArchiveFileSystemEnabled(bool enabled);

    /**
     * \return True if transparent archive file system layering is enabled.
     */
    bool isArciveFileSystemEnabled() const;

    /**
     * Mounts the standard archives home:export.zip and
     * home:export_$(platform).zip
     * \return True on success and false on failure.
     */
    bool mountStandardArchives();

    /**
     * Unmounts the standard archives.
     */
    void unmountStandardArchives();

    /**
     * Creates a stream object for the given URI.
     */
    std::shared_ptr<std::ostream> createOutputStream(const CUniformResourceIdentifier &uri) const;
    std::shared_ptr<std::istream> createInputStream(const CUniformResourceIdentifier &uri) const;
};
