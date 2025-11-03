#include "device_io.h"
#include <cstring>
#include <algorithm>
#include <iostream>

#ifdef _WIN32
    #include <windows.h>
    #include <winioctl.h>
#else
    #include <unistd.h>
    #include <fcntl.h>
    #include <sys/ioctl.h>
    #include <sys/stat.h>
    #ifdef __APPLE__
        #include <sys/disk.h>
    #else  // Linux
        #include <linux/fs.h>
    #endif
#endif

/**
 * @file device_io.cpp
 * @brief Cross-platform device I/O implementation
 *
 * Implements platform-specific device operations:
 * - Windows: CreateFile() + ReadFile()
 * - Linux: open() + read() + ioctl()
 * - macOS: open() + read() + ioctl()
 *
 * All platform-specific code is properly isolated and error-handled.
 */

namespace rsn {
namespace io {

DeviceIO::DeviceIO()
    :
#ifdef _WIN32
      device_handle_(INVALID_HANDLE_VALUE),
#else
      device_fd_(-1),
#endif
      device_size_(0),
      sector_size_(512),
      detected_fs_(FilesystemType::UNKNOWN),
      is_open_(false) {
}

DeviceIO::~DeviceIO() {
    if (is_open_) {
        Close();
    }
}

bool DeviceIO::Open(const std::string &device_path) {
    if (is_open_) {
        Close();  // Close any previously opened device
    }

    if (!OpenImpl(device_path)) {
        throw DeviceIOException("Failed to open device: " + device_path);
    }

    current_device_path_ = device_path;
    device_size_ = GetSizeImpl();
    is_open_ = true;

    // Detect filesystem type
    detected_fs_ = DetectFilesystem();

    return true;
}

bool DeviceIO::Close() {
    if (!is_open_) {
        return true;
    }

    bool result = CloseImpl();
    is_open_ = false;
    return result;
}

uint64_t DeviceIO::ReadBlock(uint64_t offset, uint64_t size, uint8_t *buffer) {
    if (!is_open_) {
        throw DeviceIOException("Device not open");
    }

    if (buffer == nullptr) {
        throw DeviceIOException("Buffer pointer is null");
    }

    if (offset + size > device_size_) {
        throw DeviceIOException("Read offset beyond device size");
    }

    return ReadImpl(offset, size, buffer);
}

std::vector<uint8_t> DeviceIO::ReadBlockVector(uint64_t offset, uint64_t size) {
    std::vector<uint8_t> buffer(size);
    uint64_t bytes_read = ReadBlock(offset, size, buffer.data());
    buffer.resize(bytes_read);
    return buffer;
}

FilesystemType DeviceIO::DetectFilesystem() {
    /**
     * Try detecting each known filesystem in order
     * Detection based on magic numbers at known offsets
     */

    if (DetectNTFS()) {
        return FilesystemType::NTFS;
    }

    if (DetectEXT4()) {
        return FilesystemType::EXT4;
    }

    if (DetectAPFS()) {
        return FilesystemType::APFS;
    }

    if (DetectFAT32()) {
        return FilesystemType::FAT32;
    }

    if (DetectHFSPlus()) {
        return FilesystemType::HFS_PLUS;
    }

    return FilesystemType::UNKNOWN;
}

DeviceInfo DeviceIO::GetDeviceInfo() const {
    DeviceInfo info;
    info.device_path = current_device_path_;
    info.total_bytes = device_size_;
    info.sector_size = sector_size_;
    info.filesystem_type = detected_fs_;
    info.is_open = is_open_;
    info.filesystem_name = FilesystemTypeToString(detected_fs_);
    return info;
}

bool DeviceIO::IsOpen() const {
    return is_open_;
}

uint64_t DeviceIO::GetDeviceSize() const {
    return device_size_;
}

uint64_t DeviceIO::GetSectorSize() const {
    return sector_size_;
}

std::string DeviceIO::FilesystemTypeToString(FilesystemType fs_type) {
    switch (fs_type) {
        case FilesystemType::NTFS:
            return "NTFS";
        case FilesystemType::APFS:
            return "APFS";
        case FilesystemType::EXT4:
            return "ext4";
        case FilesystemType::FAT32:
            return "FAT32";
        case FilesystemType::HFS_PLUS:
            return "HFS+";
        default:
            return "Unknown";
    }
}

FilesystemType DeviceIO::StringToFilesystemType(const std::string &fs_name) {
    if (fs_name == "NTFS") return FilesystemType::NTFS;
    if (fs_name == "APFS") return FilesystemType::APFS;
    if (fs_name == "ext4") return FilesystemType::EXT4;
    if (fs_name == "FAT32") return FilesystemType::FAT32;
    if (fs_name == "HFS+") return FilesystemType::HFS_PLUS;
    return FilesystemType::UNKNOWN;
}

// ============================================================================
// PLATFORM-SPECIFIC IMPLEMENTATIONS
// ============================================================================

#ifdef _WIN32

bool DeviceIO::OpenImpl(const std::string &device_path) {
    /**
     * Windows implementation using CreateFile()
     * Device path example: "\\\\.\\PhysicalDrive0"
     */

    device_handle_ = CreateFileA(
        device_path.c_str(),
        GENERIC_READ,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        nullptr,
        OPEN_EXISTING,
        FILE_FLAG_NO_BUFFERING | FILE_FLAG_SEQUENTIAL_SCAN,
        nullptr
    );

    return device_handle_ != INVALID_HANDLE_VALUE;
}

bool DeviceIO::CloseImpl() {
    if (device_handle_ != INVALID_HANDLE_VALUE) {
        return CloseHandle(device_handle_) != 0;
    }
    return true;
}

uint64_t DeviceIO::ReadImpl(uint64_t offset, uint64_t size, uint8_t *buffer) {
    if (device_handle_ == INVALID_HANDLE_VALUE) {
        throw DeviceIOException("Device handle invalid");
    }

    // Seek to offset
    LARGE_INTEGER liOffset;
    liOffset.QuadPart = offset;
    if (!SetFilePointerEx(device_handle_, liOffset, nullptr, FILE_BEGIN)) {
        throw DeviceIOException("Failed to seek to offset");
    }

    // Read data
    DWORD bytes_read = 0;
    if (!ReadFile(device_handle_, buffer, (DWORD)size, &bytes_read, nullptr)) {
        throw DeviceIOException("Failed to read from device");
    }

    return bytes_read;
}

uint64_t DeviceIO::GetSizeImpl() {
    if (device_handle_ == INVALID_HANDLE_VALUE) {
        return 0;
    }

    LARGE_INTEGER size;
    if (GetFileSizeEx(device_handle_, &size)) {
        return size.QuadPart;
    }

    return 0;
}

#else  // Unix-like (Linux, macOS)

bool DeviceIO::OpenImpl(const std::string &device_path) {
    /**
     * Unix implementation using open()
     * Device paths: /dev/sda1, /dev/nvme0n1p1 (Linux)
     *               /dev/disk0s1 (macOS)
     */

    device_fd_ = open(device_path.c_str(), O_RDONLY);
    return device_fd_ >= 0;
}

bool DeviceIO::CloseImpl() {
    if (device_fd_ >= 0) {
        return close(device_fd_) == 0;
    }
    return true;
}

uint64_t DeviceIO::ReadImpl(uint64_t offset, uint64_t size, uint8_t *buffer) {
    if (device_fd_ < 0) {
        throw DeviceIOException("Device file descriptor invalid");
    }

    // Seek to offset
    if (lseek(device_fd_, offset, SEEK_SET) < 0) {
        throw DeviceIOException("Failed to seek to offset");
    }

    // Read data
    ssize_t bytes_read = read(device_fd_, buffer, size);
    if (bytes_read < 0) {
        throw DeviceIOException("Failed to read from device");
    }

    return bytes_read;
}

uint64_t DeviceIO::GetSizeImpl() {
    if (device_fd_ < 0) {
        return 0;
    }

    uint64_t size = 0;

#ifdef __APPLE__
    // macOS: Use DKIOCGETBLOCKCOUNT
    uint32_t block_size = 0;
    uint64_t block_count = 0;

    if (ioctl(device_fd_, DKIOCGETBLOCKSIZE, &block_size) >= 0 &&
        ioctl(device_fd_, DKIOCGETBLOCKCOUNT, &block_count) >= 0) {
        size = block_count * block_size;
    }
#else
    // Linux: Use BLKGETSIZE64
    if (ioctl(device_fd_, BLKGETSIZE64, &size) < 0) {
        // Fallback: use lseek
        size = lseek(device_fd_, 0, SEEK_END);
        lseek(device_fd_, 0, SEEK_SET);
    }
#endif

    return size;
}

#endif  // Platform-specific implementations

// ============================================================================
// FILESYSTEM DETECTION (Cross-platform)
// ============================================================================

bool DeviceIO::DetectNTFS() {
    /**
     * NTFS signature at offset 3-10: "NTFS    " (8 bytes)
     * Also check magic 0x55AA at offset 510-511
     */

    try {
        std::vector<uint8_t> boot_sector = ReadBlockVector(0, 512);
        if (boot_sector.size() < 512) {
            return false;
        }

        // Check NTFS signature at offset 3
        const char *ntfs_sig = "NTFS    ";
        return std::memcmp(&boot_sector[3], ntfs_sig, 8) == 0;
    } catch (...) {
        return false;
    }
}

bool DeviceIO::DetectEXT4() {
    /**
     * EXT4 magic: 0xEF53 at offset 1024+56
     * Also check feature flags to distinguish from ext3/ext2
     */

    try {
        std::vector<uint8_t> superblock = ReadBlockVector(1024, 256);
        if (superblock.size() < 120) {
            return false;
        }

        // Check magic at offset 56
        uint16_t magic = *(uint16_t *)(&superblock[56]);
        return magic == 0xEF53;
    } catch (...) {
        return false;
    }
}

bool DeviceIO::DetectAPFS() {
    /**
     * APFS container magic: "NXSB" (0x4253584E) at offset 0
     * or "APSB" (0x42535041) for volume superblock
     */

    try {
        std::vector<uint8_t> header = ReadBlockVector(0, 64);
        if (header.size() < 4) {
            return false;
        }

        // Check for NXSB (container) or APSB (volume)
        uint32_t magic = *(uint32_t *)header.data();
        return magic == 0x4253584E || magic == 0x42535041;
    } catch (...) {
        return false;
    }
}

bool DeviceIO::DetectFAT32() {
    /**
     * FAT32 signature: 0x55AA at offset 510-511
     * Also check jump instruction at offset 0 (0xEB or 0xE9)
     */

    try {
        std::vector<uint8_t> boot_sector = ReadBlockVector(0, 512);
        if (boot_sector.size() < 512) {
            return false;
        }

        // Check boot sector signature
        uint16_t signature = *(uint16_t *)(&boot_sector[510]);
        if (signature != 0xAA55) {
            return false;
        }

        // Check jump instruction
        uint8_t first_byte = boot_sector[0];
        return first_byte == 0xEB || first_byte == 0xE9;
    } catch (...) {
        return false;
    }
}

bool DeviceIO::DetectHFSPlus() {
    /**
     * HFS+ signature: 0x482B (H+) at offset 1024
     * or 0x4858 (HX) for HFSX (case-sensitive variant)
     */

    try {
        std::vector<uint8_t> header = ReadBlockVector(1024, 512);
        if (header.size() < 2) {
            return false;
        }

        // Check magic (offset 0 in this read, which is 1024 on device)
        uint16_t magic = *(uint16_t *)header.data();
        return magic == 0x482B || magic == 0x4858;
    } catch (...) {
        return false;
    }
}

}  // namespace io
}  // namespace rsn
