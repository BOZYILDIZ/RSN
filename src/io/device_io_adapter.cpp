#include "device_io_adapter.h"
#include <iostream>

/**
 * @file device_io_adapter.cpp
 * @brief Implementation of DeviceIO adapter for parsers
 *
 * Routes filesystem parsing through appropriate parser backend.
 * Handles device detection and parser initialization.
 */

namespace rsn {
namespace io {

DeviceIOAdapter::DeviceIOAdapter()
    : device_io_(std::make_unique<DeviceIO>()),
      ntfs_parser_(nullptr),
      apfs_parser_(nullptr),
      ext4_parser_(nullptr),
      last_total_files_(0),
      last_deleted_files_(0) {
}

DeviceIOAdapter::~DeviceIOAdapter() {
    if (device_io_ && device_io_->IsOpen()) {
        device_io_->Close();
    }
}

bool DeviceIOAdapter::OpenDevice(const std::string &device_path) {
    /**
     * Open device via DeviceIO layer
     * This provides cross-platform access to block devices
     */

    try {
        return device_io_->Open(device_path);
    } catch (const DeviceIOException &e) {
        std::cerr << "Failed to open device: " << e.what() << std::endl;
        return false;
    }
}

bool DeviceIOAdapter::CloseDevice() {
    /**
     * Close device and clean up parsers
     */

    if (device_io_) {
        return device_io_->Close();
    }

    return true;
}

FilesystemType DeviceIOAdapter::DetectFilesystem() {
    /**
     * Detect filesystem using magic numbers
     * Returns detected type or UNKNOWN
     */

    if (!device_io_ || !device_io_->IsOpen()) {
        return FilesystemType::UNKNOWN;
    }

    return device_io_->DetectFilesystem();
}

bool DeviceIOAdapter::ParseDevice(std::vector<FileEntry> &entries) {
    /**
     * Parse device with appropriate filesystem parser
     *
     * Flow:
     * 1. Check device is open
     * 2. Detect filesystem
     * 3. Initialize correct parser
     * 4. Route to parser-specific parse method
     * 5. Collect statistics
     */

    if (!device_io_ || !device_io_->IsOpen()) {
        std::cerr << "Device not open" << std::endl;
        return false;
    }

    // Detect filesystem
    FilesystemType fs_type = DetectFilesystem();

    if (fs_type == FilesystemType::UNKNOWN) {
        std::cerr << "Unknown filesystem type" << std::endl;
        return false;
    }

    std::cout << "Detected filesystem: "
              << DeviceIO::FilesystemTypeToString(fs_type) << std::endl;

    // Initialize appropriate parser
    if (!InitializeParser(fs_type)) {
        std::cerr << "Failed to initialize parser" << std::endl;
        return false;
    }

    // Route to appropriate parse method
    bool result = false;
    switch (fs_type) {
        case FilesystemType::NTFS:
            result = ParseNTFS(entries);
            break;

        case FilesystemType::APFS:
            result = ParseAPFS(entries);
            break;

        case FilesystemType::EXT4:
            result = ParseEXT4(entries);
            break;

        default:
            std::cerr << "Parser not implemented for filesystem" << std::endl;
            return false;
    }

    // Collect statistics
    if (result) {
        last_total_files_ = entries.size();
        last_deleted_files_ = 0;

        for (const auto &entry : entries) {
            if (entry.is_deleted) {
                last_deleted_files_++;
            }
        }

        std::cout << "Parse successful: " << entries.size()
                  << " files, " << last_deleted_files_
                  << " deleted" << std::endl;
    }

    return result;
}

std::pair<int, int> DeviceIOAdapter::GetRecoveryStats() const {
    /**
     * Return (total_files, deleted_files) from last parse
     */

    return std::make_pair(last_total_files_, last_deleted_files_);
}

DeviceIO &DeviceIOAdapter::GetDeviceIO() {
    /**
     * Provide access to underlying DeviceIO for advanced use
     */

    return *device_io_;
}

DeviceInfo DeviceIOAdapter::GetDeviceInfo() const {
    /**
     * Get device information
     */

    if (device_io_) {
        return device_io_->GetDeviceInfo();
    }

    return DeviceInfo();
}

bool DeviceIOAdapter::IsDeviceOpen() const {
    /**
     * Check if device is open
     */

    if (device_io_) {
        return device_io_->IsOpen();
    }

    return false;
}

bool DeviceIOAdapter::InitializeParser(FilesystemType fs_type) {
    /**
     * Create and initialize appropriate parser instance
     */

    switch (fs_type) {
        case FilesystemType::NTFS:
            if (!ntfs_parser_) {
                ntfs_parser_ = std::make_unique<NTFSParser>();
            }
            return true;

        case FilesystemType::APFS:
            if (!apfs_parser_) {
                apfs_parser_ = std::make_unique<APFSParser>();
            }
            return true;

        case FilesystemType::EXT4:
            if (!ext4_parser_) {
                ext4_parser_ = std::make_unique<EXT4Parser>();
            }
            return true;

        default:
            return false;
    }
}

bool DeviceIOAdapter::ParseNTFS(std::vector<FileEntry> &entries) {
    /**
     * Parse NTFS filesystem using NTFSParser
     *
     * TODO: Integrate actual DeviceIO reads into NTFS parser
     * Current: Uses mock implementation
     * Future: Replace NTFSParser stubs with actual I/O via device_io_
     */

    if (!ntfs_parser_) {
        return false;
    }

    // For now, use the mock parser implementation
    // In Phase 5E+, integrate real device_io_ reads:
    //   - ReadSuperblock() via device_io_->ReadBlock()
    //   - ParseMFT() via device_io_->ReadBlock()
    //   - ParseFileRecord() via device_io_->ReadBlock()

    std::string device_path = device_io_->GetDeviceInfo().device_path;
    return ntfs_parser_->Parse(device_path, entries);
}

bool DeviceIOAdapter::ParseAPFS(std::vector<FileEntry> &entries) {
    /**
     * Parse APFS filesystem using APFSParser
     *
     * TODO: Integrate actual DeviceIO reads into APFS parser
     * Current: Uses mock implementation
     * Future: Replace APFSParser stubs with actual I/O via device_io_
     */

    if (!apfs_parser_) {
        return false;
    }

    // For now, use the mock parser implementation
    // In Phase 5E+, integrate real device_io_ reads:
    //   - ReadContainerSuperblock() via device_io_->ReadBlock()
    //   - ParseVolumeSuperblock() via device_io_->ReadBlock()
    //   - ParseVolumeBTree() via device_io_->ReadBlock()

    std::string device_path = device_io_->GetDeviceInfo().device_path;
    return apfs_parser_->Parse(device_path, entries);
}

bool DeviceIOAdapter::ParseEXT4(std::vector<FileEntry> &entries) {
    /**
     * Parse ext4 filesystem using EXT4Parser
     *
     * TODO: Integrate actual DeviceIO reads into ext4 parser
     * Current: Uses mock implementation
     * Future: Replace EXT4Parser stubs with actual I/O via device_io_
     */

    if (!ext4_parser_) {
        return false;
    }

    // For now, use the mock parser implementation
    // In Phase 5E+, integrate real device_io_ reads:
    //   - ReadSuperblock() via device_io_->ReadBlock()
    //   - ParseGroupDescriptors() via device_io_->ReadBlock()
    //   - ParseInodeTable() via device_io_->ReadBlock()

    std::string device_path = device_io_->GetDeviceInfo().device_path;
    return ext4_parser_->Parse(device_path, entries);
}

}  // namespace io
}  // namespace rsn
