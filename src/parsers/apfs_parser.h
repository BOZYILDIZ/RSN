#ifndef APFS_PARSER_H
#define APFS_PARSER_H

#include "../filesystems/filesystem_interface.h"
#include <vector>
#include <string>
#include <cstdint>

/**
 * @class APFSParser
 * @brief APFS (Apple File System) parser implementation
 *
 * Responsible for:
 * - Detecting APFS filesystems and containers
 * - Parsing APFS structures (container superblock, volume header, B-tree)
 * - Extracting recoverable files and metadata
 * - Supporting snapshots and recovery points
 * - Computing recovery statistics
 *
 * Supports:
 * - macOS 10.13+ (High Sierra and later)
 * - iOS 10.3+ (modern iOS versions)
 * - iPadOS, tvOS, watchOS (Apple platforms)
 * - APFS 1.0, 2.0, 3.0 (filesystem versions)
 * - MBR and GPT partitions
 * - Block sizes: 4KB - 64KB (typically 4KB)
 *
 * APFS Architecture:
 * - Container: Physical volume containing APFS metadata
 * - Volume: Logical filesystem within container
 * - Snapshots: Point-in-time copies for recovery
 * - B-tree: File record organization
 */
class APFSParser : public FileSystemInterface {
public:
    /**
     * @brief Constructor for APFS parser
     */
    APFSParser();

    /**
     * @brief Destructor
     */
    ~APFSParser() override = default;

    /**
     * @brief Get the filesystem type identifier
     * @return "APFS" string identifier
     */
    std::string GetFileSystemType() const override;

    /**
     * @brief Check if parser can handle device/partition
     * @param device_path Path to device (e.g., /dev/disk1s1, /Volumes/Data)
     * @return true if APFS filesystem detected, false otherwise
     */
    bool CanHandle(const std::string& device_path) const override;

    /**
     * @brief Parse APFS filesystem and extract file entries
     * @param device_path Device or partition path
     * @param [out] entries Vector to store extracted FileEntry structures
     * @return true if parsing successful, false on error
     *
     * Process:
     * 1. Read and validate APFS container superblock
     * 2. Locate and parse volume header
     * 3. Navigate B-tree structures
     * 4. Extract file records and inodes
     * 5. Handle snapshots and recovery points
     * 6. Build recoverable file list
     * 7. Store in entries vector
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
     * - Statistics from all snapshots if available
     * - Updated after Parse() call
     */
    std::pair<int, int> GetRecoveryStats() const override;

    /**
     * @brief Get snapshot information for recovery options
     * @return Vector of snapshot names/timestamps
     *
     * APFS stores snapshots for recovery:
     * - List available recovery points
     * - Can parse from specific snapshots
     * - Useful for multi-version recovery
     */
    std::vector<std::string> GetAvailableSnapshots() const;

private:
    // APFS Constants
    static constexpr int APFS_BLOCK_SIZE = 4096;  // Standard block size
    static constexpr int APFS_MAX_BLOCK_SIZE = 65536;
    static constexpr uint32_t APFS_CONTAINER_SB_MAGIC = 0x4253584E;  // "NXSB"
    static constexpr uint32_t APFS_VOLUME_SB_MAGIC = 0x42535041;     // "APSB"
    static constexpr uint32_t APFS_BTREE_NODE_MAGIC = 0x4E4F4445;    // "NODE"
    static constexpr uint32_t APFS_INODE_MAGIC = 0x494E4F44;         // "INOD"

    // Container Entry Types
    static constexpr uint32_t APFS_ENTRY_TYPE_VOLUME = 1;
    static constexpr uint32_t APFS_ENTRY_TYPE_RESERVED = 2;
    static constexpr uint32_t APFS_ENTRY_TYPE_SNAPSHOT = 3;

    /**
     * @struct APFSContainerSuperblock
     * @brief APFS container superblock structure
     *
     * Located at block 0 of container
     * Contains container-wide metadata and volume references
     */
    struct APFSContainerSuperblock {
        uint32_t magic;                 // "NXSB" = 0x4253584E
        uint32_t block_size;            // Block size (usually 4096)
        uint64_t block_count;           // Total blocks in container
        uint64_t features;              // Feature flags
        uint32_t readonly;              // Readonly flag
        uint32_t version;               // APFS version
        uint32_t checksum;              // Fletcher checksum
        uint32_t volume_count;          // Number of volumes
        uint64_t volume_list_block;     // Block containing volume list
        // ... additional fields
    };

    /**
     * @struct APFSVolumeSuperblock
     * @brief APFS volume superblock structure
     *
     * Located within container, referenced from volume list
     * Contains volume-specific metadata and inode tree references
     */
    struct APFSVolumeSuperblock {
        uint32_t magic;                 // "APSB" = 0x42535041
        uint32_t block_size;            // Block size (inherited from container)
        uint64_t inode_count;           // Total inodes
        uint64_t deleted_count;         // Deleted inodes
        uint64_t inode_tree_block;      // B-tree root for inodes
        uint64_t extent_tree_block;     // B-tree root for extents
        char volume_name[256];          // Volume name (UTF-8)
        // ... additional fields
    };

    /**
     * @struct APFSInode
     * @brief APFS inode (file record) structure
     *
     * Represents file metadata:
     * - File type and permissions
     * - Size and timestamps
     * - Extent pointers
     * - Deletion status
     */
    struct APFSInode {
        uint32_t magic;                 // "INOD"
        uint16_t type;                  // File type (regular, directory, etc.)
        uint16_t flags;                 // Inode flags (deleted, etc.)
        uint32_t permissions;           // Unix permissions
        uint64_t size;                  // File size
        uint64_t created_time;          // Creation timestamp
        uint64_t modified_time;         // Modification timestamp
        uint64_t accessed_time;         // Access timestamp
        char name[256];                 // Filename (UTF-8)
        // ... extent and additional fields
    };

    // Internal state
    int total_recoverable_files_ = 0;
    int total_deleted_files_ = 0;
    std::vector<std::string> available_snapshots_;
    std::string last_parsed_device_;
    bool is_initialized_ = false;

    /**
     * @brief Read and validate APFS container superblock
     * @param device_path Device path
     * @param [out] superblock Filled APFSContainerSuperblock structure
     * @return true if valid APFS container found
     */
    bool ReadContainerSuperblock(const std::string& device_path,
                                APFSContainerSuperblock& superblock) const;

    /**
     * @brief Parse volume superblock
     * @param device_path Device path
     * @param container_sb Valid container superblock
     * @param [out] volume_sb Filled APFSVolumeSuperblock structure
     * @return true if volume found and parsed
     */
    bool ParseVolumeSuperblock(const std::string& device_path,
                              const APFSContainerSuperblock& container_sb,
                              APFSVolumeSuperblock& volume_sb);

    /**
     * @brief Parse volume B-tree for file records
     * @param device_path Device path
     * @param volume_sb Valid volume superblock
     * @param [out] entries Extracted file entries
     * @return true if B-tree parsing successful
     */
    bool ParseVolumeBTree(const std::string& device_path,
                         const APFSVolumeSuperblock& volume_sb,
                         std::vector<FileEntry>& entries);

    /**
     * @brief Parse individual inode record
     * @param inode_data Raw inode data
     * @param [out] entry Resulting FileEntry
     * @return true if inode parsed successfully
     */
    bool ParseInode(const std::vector<uint8_t>& inode_data,
                   FileEntry& entry) const;

    /**
     * @brief Extract filename from inode
     * @param inode_data Raw inode data
     * @param [out] filename Extracted filename
     * @return true if filename found
     */
    bool ExtractFilename(const std::vector<uint8_t>& inode_data,
                        std::string& filename) const;

    /**
     * @brief Check if inode is marked as deleted
     * @param inode_data Raw inode data
     * @return true if file is deleted, false if active
     */
    bool IsInodeDeleted(const std::vector<uint8_t>& inode_data) const;

    /**
     * @brief Parse APFS snapshots for recovery options
     * @param device_path Device path
     * @param container_sb Valid container superblock
     * @return Vector of snapshot identifiers
     */
    std::vector<std::string> ParseSnapshots(
        const std::string& device_path,
        const APFSContainerSuperblock& container_sb) const;
};

#endif  // APFS_PARSER_H
