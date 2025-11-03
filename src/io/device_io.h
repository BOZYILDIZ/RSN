#ifndef DEVICE_IO_H
#define DEVICE_IO_H

#include <string>
#include <vector>
#include <cstdint>
#include <memory>
#include <stdexcept>

/**
 * @file device_io.h
 * @brief Cross-platform low-level device I/O abstraction layer
 *
 * Provides unified interface for reading/writing block devices across:
 * - Linux (direct device files: /dev/sda*, /dev/nvme*)
 * - macOS (raw device access via BSD APIs)
 * - Windows (CreateFile with FILE_FLAG_NO_BUFFERING)
 *
 * Key Features:
 * - Abstracted device open/close/read operations
 * - Error handling and logging
 * - Block-aligned reads
 * - Position tracking
 * - Filesystem detection via magic numbers
 */

namespace rsn {
namespace io {

/**
 * @enum FilesystemType
 * @brief Detected filesystem types based on magic numbers
 */
enum class FilesystemType {
    UNKNOWN = 0,
    NTFS = 1,      // Magic: "NTFS    " at offset 3
    APFS = 2,      // Magic: 0x4253584E (NXSB) for container
    EXT4 = 3,      // Magic: 0xEF53 at offset 1024+56
    FAT32 = 4,     // Magic: 0x55AA at end of boot sector
    HFS_PLUS = 5,  // Magic: 0x482B (H+) or 0x4858 (HX)
};

/**
 * @struct DeviceInfo
 * @brief Information about opened device
 */
struct DeviceInfo {
    std::string device_path;        // Path to device (/dev/sda1, etc.)
    uint64_t total_bytes;           // Total device size in bytes
    uint64_t sector_size;           // Sector size (usually 512 or 4096)
    FilesystemType filesystem_type; // Detected filesystem
    bool is_open;                   // Is device currently open
    std::string filesystem_name;    // Human-readable filesystem name
};

/**
 * @class DeviceIO
 * @brief Cross-platform device I/O operations
 *
 * Handles low-level read/write operations on block devices.
 * Uses platform-specific implementations:
 * - Linux/macOS: POSIX file operations
 * - Windows: CreateFile() with appropriate flags
 *
 * All read operations are:
 * - Offset-based (no file pointer tracking required)
 * - Buffered for performance
 * - Error-checked
 */
class DeviceIO {
public:
    /**
     * @brief Constructor
     */
    DeviceIO();

    /**
     * @brief Destructor (automatically closes device if open)
     */
    ~DeviceIO();

    /**
     * @brief Open a block device for reading
     * @param device_path Path to device (/dev/sda1, /dev/nvme0n1p1, etc.)
     * @return true if successfully opened
     *
     * Platform-specific behavior:
     * - Linux: Open /dev/sdaX or /dev/nvmeXnXpX directly
     * - macOS: Open /dev/rdiskXsY (raw device)
     * - Windows: CreateFile with GENERIC_READ
     *
     * Throws std::runtime_error on failure
     */
    bool Open(const std::string &device_path);

    /**
     * @brief Close the currently opened device
     * @return true if successfully closed
     */
    bool Close();

    /**
     * @brief Read a block of data from device at given offset
     * @param offset Byte offset from start of device
     * @param size Number of bytes to read
     * @param buffer Output buffer (must be pre-allocated)
     * @return Number of bytes actually read
     *
     * Throws std::runtime_error on I/O errors
     * Throws std::invalid_argument if device not open or buffer null
     */
    uint64_t ReadBlock(uint64_t offset, uint64_t size, uint8_t *buffer);

    /**
     * @brief Read a block of data and return as vector
     * @param offset Byte offset from start of device
     * @param size Number of bytes to read
     * @return Vector containing read data
     *
     * Convenience method that handles allocation
     */
    std::vector<uint8_t> ReadBlockVector(uint64_t offset, uint64_t size);

    /**
     * @brief Detect filesystem type based on magic numbers
     * @return Detected FilesystemType
     *
     * Reads superblocks from common locations:
     * - NTFS: Boot sector (offset 0)
     * - EXT4: Superblock at offset 1024
     * - APFS: Container superblock at offset 0
     * - FAT32: Boot sector (offset 0)
     *
     * Returns FilesystemType::UNKNOWN if no known filesystem detected
     */
    FilesystemType DetectFilesystem();

    /**
     * @brief Get device information
     * @return DeviceInfo structure with device metadata
     *
     * Returns current device info or empty struct if no device open
     */
    DeviceInfo GetDeviceInfo() const;

    /**
     * @brief Check if device is currently open
     * @return true if device is open
     */
    bool IsOpen() const;

    /**
     * @brief Get total size of device in bytes
     * @return Device size or 0 if not open
     */
    uint64_t GetDeviceSize() const;

    /**
     * @brief Get sector size of device
     * @return Sector size in bytes (usually 512 or 4096)
     */
    uint64_t GetSectorSize() const;

    /**
     * @brief Convert FilesystemType enum to string
     * @param fs_type Filesystem type to convert
     * @return Human-readable filesystem name
     */
    static std::string FilesystemTypeToString(FilesystemType fs_type);

    /**
     * @brief Convert string to FilesystemType enum
     * @param fs_name Filesystem name string
     * @return FilesystemType enum value
     */
    static FilesystemType StringToFilesystemType(const std::string &fs_name);

private:
    // Platform-specific file handles
#ifdef _WIN32
    void *device_handle_;           // HANDLE on Windows
#else
    int device_fd_;                 // File descriptor on Unix
#endif

    // Device information
    std::string current_device_path_;
    uint64_t device_size_;
    uint64_t sector_size_;
    FilesystemType detected_fs_;
    bool is_open_;

    /**
     * @brief Platform-specific device open implementation
     * @param device_path Path to device
     * @return true if successfully opened
     */
    bool OpenImpl(const std::string &device_path);

    /**
     * @brief Platform-specific device close implementation
     * @return true if successfully closed
     */
    bool CloseImpl();

    /**
     * @brief Platform-specific read implementation
     * @param offset Byte offset
     * @param size Number of bytes
     * @param buffer Output buffer
     * @return Number of bytes read
     */
    uint64_t ReadImpl(uint64_t offset, uint64_t size, uint8_t *buffer);

    /**
     * @brief Get device size (platform-specific)
     * @return Device size in bytes
     */
    uint64_t GetSizeImpl();

    /**
     * @brief Detect NTFS filesystem
     * @return true if NTFS detected
     */
    bool DetectNTFS();

    /**
     * @brief Detect EXT4 filesystem
     * @return true if EXT4 detected
     */
    bool DetectEXT4();

    /**
     * @brief Detect APFS filesystem
     * @return true if APFS detected
     */
    bool DetectAPFS();

    /**
     * @brief Detect FAT32 filesystem
     * @return true if FAT32 detected
     */
    bool DetectFAT32();

    /**
     * @brief Detect HFS+ filesystem
     * @return true if HFS+ detected
     */
    bool DetectHFSPlus();
};

/**
 * @class DeviceIOException
 * @brief Exception for I/O errors
 */
class DeviceIOException : public std::runtime_error {
public:
    explicit DeviceIOException(const std::string &message)
        : std::runtime_error(message) {}
};

}  // namespace io
}  // namespace rsn

#endif  // DEVICE_IO_H
