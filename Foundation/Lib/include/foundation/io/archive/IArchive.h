#pragma once

#include <vector>
#include <string>

#include "foundation/io/CUniformResourceIdentifier.h"

class IArchive
{
public:
  virtual ~IArchive();

  /**
   * Open archive from URI.
   */
  virtual bool open(const CUniformResourceIdentifier &uri) = 0;

  /**
   * Close archive.
   */
  virtual void close() = 0;

  /**
   * \return True if the archive is open and false otherwise.
   */
  virtual bool isOpen() const = 0;

  /**
   * \return URI of the archive.
   */
  virtual const CUniformResourceIdentifier &
  getUniformResourceIdentifier() const = 0;

  /**
   * \return Array of file names in a directory inside the archive.
   */
  virtual std::vector<std::string>
  getFileNames(const std::string &pathInArchive) const = 0;

  /**
   * \return Array of directory names in a directory inside the archive.
   */
  virtual std::vector<std::string>
  getDirectoryNames(const std::string &pathInArchive) const = 0;
};
