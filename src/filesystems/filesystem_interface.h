#pragma once

#include <string>
#include <vector>
#include <memory>

/**
 * @struct FileEntry
 * @brief Represents a recoverable file found on disk
 */
struct FileEntry
{
  std::string path;           ///< File path
  std::string name;           ///< File name
  unsigned long size;         ///< File size in bytes
  bool is_deleted;            ///< Whether file is deleted
  double recovery_confidence; ///< Recovery success confidence (0.0-1.0)
};

/**
 * @brief Abstract base class for file system parsers
 *
 * Each file system (NTFS, APFS, ext4, etc.) implements this interface
 * to provide file recovery capabilities.
 */
class FileSystemInterface
{
public:
  /**
   * @brief Virtual destructor
   */
  virtual ~FileSystemInterface() = default;

  /**
   * @brief Get the file system type name
   *
   * @return Name of file system (e.g., "NTFS", "APFS", "ext4")
   */
  virtual std::string GetFileSystemType() const = 0;

  /**
   * @brief Check if this parser can handle the given device
   *
   * @param device_path Path to device
   * @return True if this parser can parse the device
   */
  virtual bool CanHandle(const std::string& device_path) const = 0;

  /**
   * @brief Parse the file system and discover recoverable files
   *
   * @param device_path Path to device to scan
   * @return Vector of recoverable files found
   */
  virtual std::vector<FileEntry> Parse(const std::string& device_path) = 0;

  /**
   * @brief Get recovery statistics
   *
   * @return Pair of (total_files, successfully_recovered)
   */
  virtual std::pair<int, int> GetRecoveryStats() const = 0;

protected:
  /**
   * @brief Protected constructor (for derived classes only)
   */
  FileSystemInterface() = default;
};

// Forward declarations for concrete implementations
class NTFSParser;      ///< NTFS file system parser (Windows)
class APFSParser;      ///< APFS file system parser (macOS)
class Ext4Parser;      ///< ext4 file system parser (Linux)
class BtrfsParser;     ///< Btrfs file system parser (Linux)
