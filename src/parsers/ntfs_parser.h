#ifndef NTFS_PARSER_H
#define NTFS_PARSER_H

#include "../filesystems/filesystem_interface.h"
#include <vector>
#include <string>
#include <cstdint>

/**
 * @class NTFSParser
 * @brief NTFS (NT File System) parser implementation
 *
 * Responsible for:
 * - Detecting NTFS filesystems
 * - Parsing NTFS structures (MFT, boot sector)
 * - Extracting recoverable files
 * - Computing recovery statistics
 *
 * Supports:
 * - Windows NT/2000/XP/Vista/7/8/10/11
 * - NTFS 1.0, 3.0, 3.1
 * - MBR and GPT partitions
 * - Cluster sizes: 512B - 64KB
 */
class NTFSParser : public FileSystemInterface {
public:
    /**
     * @brief Constructor for NTFS parser
     */
    NTFSParser();

    /**
     * @brief Destructor
     */
    ~NTFSParser() override = default;

    /**
     * @brief Get the filesystem type identifier
     * @return "NTFS" string identifier
     */
    std::string GetFileSystemType() const override;

    /**
     * @brief Check if parser can handle device/partition
     * @param device_path Path to device (e.g., /dev/sda1, \\.\C:)
     * @return true if NTFS filesystem detected, false otherwise
     */
    bool CanHandle(const std::string& device_path) const override;

    /**
     * @brief Parse NTFS filesystem and extract file entries
     * @param device_path Device or partition path
     * @param [out] entries Vector to store extracted FileEntry structures
     * @return true if parsing successful, false on error
     *
     * Process:
     * 1. Read and validate NTFS boot sector
     * 2. Locate and parse Master File Table (MFT)
     * 3. Extract file records and attributes
     * 4. Build recoverable file list
     * 5. Store in entries vector
     */
    bool Parse(const std::string& device_path,
               std::vector<FileEntry>& entries) override;

    /**
     * @brief Get recovery statistics
     * @return Pair of (total_recoverable_files, total_deleted_files)
     *
     * Provides metrics on what can be recovered:
     * - Total files that can be recovered
     * - Number of deleted files still recoverable
     * - Statistics updated after Parse() call
     */
    std::pair<int, int> GetRecoveryStats() const override;

private:
    // NTFS Boot Sector Constants
    static constexpr int NTFS_BOOT_SECTOR_SIZE = 512;
    static constexpr int NTFS_SIGNATURE_OFFSET = 3;
    static constexpr const char* NTFS_SIGNATURE = "NTFS    ";

    // NTFS MFT Constants
    static constexpr int MFT_RECORD_SIZE = 1024;  // Typical, can vary
    static constexpr int FILE_RECORD_SIGNATURE = 0x454C4946;  // "FILE"
    static constexpr int INDX_RECORD_SIGNATURE = 0x58444E49;  // "INDX"

    // File attributes
    static constexpr int ATTR_STANDARD_INFORMATION = 0x10;
    static constexpr int ATTR_FILENAME = 0x30;
    static constexpr int ATTR_DATA = 0x80;

    /**
     * @struct NTFSBootSector
     * @brief NTFS boot sector structure
     */
    struct NTFSBootSector {
        uint8_t jump_code[3];           // JMP instruction
        char oem_id[8];                 // "NTFS    "
        uint16_t bytes_per_sector;
        uint8_t sectors_per_cluster;
        uint16_t reserved_sectors;
        uint8_t fats;                   // Usually 0 for NTFS
        uint16_t root_entries;          // Usually 0 for NTFS
        uint16_t total_sectors;         // Usually 0 for NTFS
        uint8_t media_descriptor;
        uint16_t sectors_per_fat;       // Usually 0 for NTFS
        uint16_t sectors_per_track;
        uint16_t heads;
        uint32_t hidden_sectors;
        uint32_t large_total_sectors;   // Usually 0 for NTFS
        uint64_t total_sectors_64;      // Total sectors (64-bit)
        uint64_t mft_start_cluster;     // MFT start cluster
        uint64_t mftmirr_start_cluster; // MFT mirror start cluster
        int32_t mft_record_size_log;    // MFT record size in clusters (or log if negative)
        uint32_t index_record_size_log; // Index record size in clusters
    };

    // Internal state
    int total_recoverable_files_ = 0;
    int total_deleted_files_ = 0;
    std::string last_parsed_device_;
    bool is_initialized_ = false;

    /**
     * @brief Read and validate NTFS boot sector
     * @param device_path Device path
     * @param [out] boot_sector Filled NTFSBootSector structure
     * @return true if valid NTFS boot sector found
     */
    bool ReadBootSector(const std::string& device_path,
                       NTFSBootSector& boot_sector) const;

    /**
     * @brief Parse Master File Table (MFT)
     * @param device_path Device path
     * @param boot_sector Valid NTFS boot sector
     * @param [out] entries Extracted file entries
     * @return true if MFT parsing successful
     */
    bool ParseMFT(const std::string& device_path,
                  const NTFSBootSector& boot_sector,
                  std::vector<FileEntry>& entries);

    /**
     * @brief Parse individual file record
     * @param record_data Raw file record data
     * @param [out] entry Resulting FileEntry
     * @return true if record parsed successfully
     */
    bool ParseFileRecord(const std::vector<uint8_t>& record_data,
                        FileEntry& entry) const;

    /**
     * @brief Extract filename from file record attributes
     * @param record_data Raw file record data
     * @param [out] filename Extracted filename
     * @return true if filename found
     */
    bool ExtractFilename(const std::vector<uint8_t>& record_data,
                        std::string& filename) const;

    /**
     * @brief Check if file record is marked as deleted
     * @param record_data Raw file record data
     * @return true if file is deleted, false if active
     */
    bool IsFileDeleted(const std::vector<uint8_t>& record_data) const;
};

#endif  // NTFS_PARSER_H
