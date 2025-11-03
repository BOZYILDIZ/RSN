#ifndef DEVICE_IO_ADAPTER_H
#define DEVICE_IO_ADAPTER_H

#include "device_io.h"
#include "../parsers/ntfs_parser.h"
#include "../parsers/apfs_parser.h"
#include "../parsers/ext4_parser.h"
#include <memory>
#include <vector>

/**
 * @file device_io_adapter.h
 * @brief Adapter layer to integrate DeviceIO with filesystem parsers
 *
 * This module provides integration between the cross-platform DeviceIO layer
 * and the filesystem parsers (NTFS, APFS, ext4).
 *
 * Key responsibilities:
 * - Route detected filesystem to correct parser
 * - Handle parser invocation with DeviceIO backend
 * - Manage device lifecycle
 * - Provide error handling and reporting
 */

namespace rsn {
namespace io {

/**
 * @class DeviceIOAdapter
 * @brief Adapter for integrating DeviceIO with parsers
 *
 * Bridges DeviceIO (low-level I/O) with filesystem parsers
 * (high-level parsing logic).
 *
 * Usage:
 * 1. Create adapter
 * 2. Open device
 * 3. Detect filesystem
 * 4. Route to appropriate parser
 * 5. Get results
 */
class DeviceIOAdapter {
public:
    /**
     * @brief Constructor
     */
    DeviceIOAdapter();

    /**
     * @brief Destructor
     */
    ~DeviceIOAdapter();

    /**
     * @brief Open device and prepare for parsing
     * @param device_path Path to device
     * @return true if successfully opened
     */
    bool OpenDevice(const std::string &device_path);

    /**
     * @brief Close currently opened device
     * @return true if successfully closed
     */
    bool CloseDevice();

    /**
     * @brief Detect filesystem type on opened device
     * @return Detected FilesystemType
     */
    FilesystemType DetectFilesystem();

    /**
     * @brief Parse device with appropriate filesystem parser
     * @param entries Output vector for file entries
     * @return true if parsing successful
     *
     * Automatically routes to correct parser based on
     * detected filesystem type.
     */
    bool ParseDevice(std::vector<FileEntry> &entries);

    /**
     * @brief Get recovery statistics from last parse
     * @return Pair of (total_recoverable, total_deleted)
     */
    std::pair<int, int> GetRecoveryStats() const;

    /**
     * @brief Get underlying DeviceIO object
     * @return Reference to DeviceIO instance
     */
    DeviceIO &GetDeviceIO();

    /**
     * @brief Get device information
     * @return DeviceInfo structure
     */
    DeviceInfo GetDeviceInfo() const;

    /**
     * @brief Check if device is open
     * @return true if device open
     */
    bool IsDeviceOpen() const;

private:
    // Device I/O layer
    std::unique_ptr<DeviceIO> device_io_;

    // Parser instances (created as needed)
    std::unique_ptr<NTFSParser> ntfs_parser_;
    std::unique_ptr<APFSParser> apfs_parser_;
    std::unique_ptr<EXT4Parser> ext4_parser_;

    // Last parse statistics
    int last_total_files_;
    int last_deleted_files_;

    /**
     * @brief Initialize parser for detected filesystem
     * @param fs_type Detected filesystem type
     * @return true if parser initialized successfully
     */
    bool InitializeParser(FilesystemType fs_type);

    /**
     * @brief Parse with NTFS parser
     * @param entries Output vector
     * @return true if successful
     */
    bool ParseNTFS(std::vector<FileEntry> &entries);

    /**
     * @brief Parse with APFS parser
     * @param entries Output vector
     * @return true if successful
     */
    bool ParseAPFS(std::vector<FileEntry> &entries);

    /**
     * @brief Parse with ext4 parser
     * @param entries Output vector
     * @return true if successful
     */
    bool ParseEXT4(std::vector<FileEntry> &entries);
};

}  // namespace io
}  // namespace rsn

#endif  // DEVICE_IO_ADAPTER_H
