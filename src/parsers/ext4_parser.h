#ifndef EXT4_PARSER_H
#define EXT4_PARSER_H

#include "../filesystems/filesystem_interface.h"
#include <vector>
#include <string>
#include <cstdint>

/**
 * @class EXT4Parser
 * @brief ext4 (Fourth Extended Filesystem) parser implementation
 *
 * Responsible for:
 * - Detecting ext4 filesystems
 * - Parsing ext4 structures (superblock, group descriptors, inode table)
 * - Extracting recoverable files and metadata
 * - Supporting journal recovery (JBD2)
 * - Computing recovery statistics
 *
 * Supports:
 * - Linux kernel 2.6.28+ (ext4 support)
 * - RHEL, Ubuntu, Debian, Fedora, Alpine, and other distributions
 * - ext4 feature sets (extents, flex_bg, 64-bit, metadata_csum, etc.)
 * - Block sizes: 1KB - 64KB
 * - Inode sizes: 128 - 256+ bytes (ext4 standard)
 * - Journal: JBD2 (Journal Block Device v2)
 *
 * ext4 Architecture:
 * - Superblock: Core metadata (block 0 or 1, offset 1024)
 * - Block Groups: Logical divisions of filesystem
 * - Group Descriptors: Per-group metadata (inode/block bitmaps, inode table)
 * - Inode Table: File metadata records
 * - Data Blocks: Actual file content (referenced via extents)
 * - Directory Entries: File name → inode mappings
 * - Journal: Transaction log for crash recovery
 */
class EXT4Parser : public FileSystemInterface {
public:
    /**
     * @brief Constructor for ext4 parser
     */
    EXT4Parser();

    /**
     * @brief Destructor
     */
    ~EXT4Parser() override = default;

    /**
     * @brief Get the filesystem type identifier
     * @return "ext4" string identifier
     */
    std::string GetFileSystemType() const override;

    /**
     * @brief Check if parser can handle device/partition
     * @param device_path Path to device (e.g., /dev/sda1, /dev/nvme0n1p2)
     * @return true if ext4 filesystem detected, false otherwise
     */
    bool CanHandle(const std::string& device_path) const override;

    /**
     * @brief Parse ext4 filesystem and extract file entries
     * @param device_path Device or partition path
     * @param [out] entries Vector to store extracted FileEntry structures
     * @return true if parsing successful, false on error
     *
     * Process:
     * 1. Read and validate ext4 superblock (offset 1024, magic 0xEF53)
     * 2. Parse block group descriptors
     * 3. Iterate through block groups
     * 4. Read inode bitmaps and tables
     * 5. Extract inode records and metadata
     * 6. Parse directory entries for file names
     * 7. Handle extents and block references
     * 8. Build recoverable file list
     * 9. Store in entries vector
     */
    bool Parse(const std::string& device_path,
               std::vector<FileEntry>& entries) override;

    /**
     * @brief Get recovery statistics
     * @return Pair of (total_recoverable_files, total_deleted_files)
     *
     * Provides metrics on what can be recovered:
     * - Total files that can be recovered
     * - Number of deleted files still recoverable (unlinked inodes)
     * - Statistics updated after Parse() call
     */
    std::pair<int, int> GetRecoveryStats() const override;

    /**
     * @brief Get journal information for crash recovery
     * @return String describing journal status and recovery options
     */
    std::string GetJournalInfo() const;

private:
    // ext4 Constants
    static constexpr int EXT4_SUPERBLOCK_OFFSET = 1024;  // Offset in bytes
    static constexpr int EXT4_SUPERBLOCK_SIZE = 256;
    static constexpr uint16_t EXT4_MAGIC = 0xEF53;  // Superblock magic

    // ext4 Feature Flags
    static constexpr uint32_t EXT4_FEATURE_INCOMPAT_EXTENTS = 0x00000040;
    static constexpr uint32_t EXT4_FEATURE_INCOMPAT_64BIT = 0x00000080;
    static constexpr uint32_t EXT4_FEATURE_INCOMPAT_FLEX_BG = 0x00000200;
    static constexpr uint32_t EXT4_FEATURE_INCOMPAT_METADATA_CSUM = 0x00000400;

    // Inode-related Constants
    static constexpr int EXT4_ROOT_INO = 2;  // Root directory inode
    static constexpr int EXT4_INODE_INLINE_DATA_FL = 0x10000000;
    static constexpr int EXT4_INODE_EA_INODE_FL = 0x00200000;

    // File type constants (directory, regular file, etc.)
    static constexpr uint16_t EXT4_S_IFREG = 0x8000;   // Regular file
    static constexpr uint16_t EXT4_S_IFDIR = 0x4000;   // Directory
    static constexpr uint16_t EXT4_S_IFLNK = 0xA000;   // Symbolic link

    /**
     * @struct EXT4Superblock
     * @brief ext4 superblock structure
     *
     * Located at byte offset 1024 on filesystem
     * Contains core filesystem metadata
     */
    struct EXT4Superblock {
        uint32_t inodes_count;              // Total inodes
        uint32_t blocks_count;              // Total blocks (legacy, see blocks_count_hi)
        uint32_t r_blocks_count;            // Reserved blocks
        uint32_t free_blocks_count;         // Free blocks
        uint32_t free_inodes_count;         // Free inodes
        uint32_t first_data_block;          // First data block (usually 0 for ext4)
        uint32_t log_block_size;            // Block size = 1024 << log_block_size
        uint32_t log_frag_size;             // Fragment size
        uint32_t blocks_per_group;          // Blocks per group
        uint32_t frags_per_group;           // Fragments per group
        uint32_t inodes_per_group;          // Inodes per group
        uint32_t mtime;                     // Mount time
        uint32_t wtime;                     // Write time
        uint16_t mnt_count;                 // Mount count
        uint16_t max_mnt_count;             // Max mounts before fsck
        uint16_t magic;                     // Magic number (0xEF53)
        uint16_t state;                     // State (clean, errors, etc.)
        uint16_t errors;                    // Error handling
        uint16_t minor_rev_level;           // Minor revision level
        uint32_t lastcheck;                 // Last check time
        uint32_t checkinterval;             // Max check interval
        uint32_t creator_os;                // Creator OS
        uint32_t rev_level;                 // Revision level
        uint16_t def_resuid;                // Default reserved UID
        uint16_t def_resgid;                // Default reserved GID
        uint32_t first_ino;                 // First inode (usually 11)
        uint16_t inode_size;                // Inode size (usually 256)
        uint16_t block_group_nr;            // Block group number
        uint32_t feature_compat;            // Compatible feature flags
        uint32_t feature_incompat;          // Incompatible feature flags
        uint32_t feature_ro_compat;         // Read-only compatible features
        char uuid[16];                      // UUID
        char volume_name[16];               // Volume name
        char last_mounted[64];              // Last mounted on
        uint32_t algorithm_usage_bitmap;    // Algorithm usage bitmap
        uint8_t s_prealloc_blocks;          // Preallocated blocks
        uint8_t s_prealloc_dir_blocks;      // Preallocated directory blocks
        uint16_t s_reserved_gdt_blocks;     // Reserved GDT blocks
        uint8_t s_journal_uuid[16];         // Journal UUID
        uint32_t s_journal_inum;            // Journal inode
        uint32_t s_journal_dev;             // Journal device
        uint32_t s_last_orphan;             // Last orphan inode
        // ... more fields for ext4
    };

    /**
     * @struct EXT4GroupDescriptor
     * @brief ext4 group descriptor
     *
     * Describes one block group (partition of filesystem)
     */
    struct EXT4GroupDescriptor {
        uint32_t bg_block_bitmap;           // Block bitmap block
        uint32_t bg_inode_bitmap;           // Inode bitmap block
        uint32_t bg_inode_table;            // Inode table block
        uint16_t bg_free_blocks_count;      // Free blocks in group
        uint16_t bg_free_inodes_count;      // Free inodes in group
        uint16_t bg_used_dirs_count;        // Used directories count
        uint16_t bg_pad;                    // Padding
        uint32_t bg_reserved[3];            // Reserved
        // ext4 64-bit extensions
        uint32_t bg_block_bitmap_hi;        // High 32 bits of block bitmap
        uint32_t bg_inode_bitmap_hi;        // High 32 bits of inode bitmap
        uint32_t bg_inode_table_hi;         // High 32 bits of inode table
        uint16_t bg_free_blocks_count_hi;   // High 16 bits of free blocks
        uint16_t bg_free_inodes_count_hi;   // High 16 bits of free inodes
        uint16_t bg_used_dirs_count_hi;     // High 16 bits of used dirs
        uint16_t bg_itable_unused_hi;       // High 16 bits of unused inodes
        uint32_t bg_exclude_bitmap;         // Exclude bitmap block (sparse)
        uint16_t bg_block_bitmap_csum;      // Block bitmap checksum
        uint16_t bg_inode_bitmap_csum;      // Inode bitmap checksum
        uint32_t bg_itable_unused;          // Unused inodes in table
        uint32_t bg_checksum;               // Group descriptor checksum
    };

    /**
     * @struct EXT4Inode
     * @brief ext4 inode structure
     *
     * File metadata record
     */
    struct EXT4Inode {
        uint16_t i_mode;                    // File mode (type + permissions)
        uint16_t i_uid;                     // User ID
        uint32_t i_size;                    // File size (low 32 bits)
        uint32_t i_atime;                   // Access time
        uint32_t i_ctime;                   // Change time
        uint32_t i_mtime;                   // Modification time
        uint32_t i_dtime;                   // Deletion time (if deleted)
        uint16_t i_gid;                     // Group ID
        uint16_t i_links_count;             // Hard links count
        uint32_t i_blocks;                  // Total blocks allocated
        uint32_t i_flags;                   // File flags
        uint32_t i_osd1;                    // OS-dependent field
        uint32_t i_block[15];               // Block pointers (extents or direct blocks)
        uint32_t i_generation;              // Generation number
        uint32_t i_file_acl;                // Extended attribute block
        uint32_t i_size_hi;                 // File size (high 32 bits, ext4)
        uint32_t i_obso_faddr;              // Obsolete fragment address
        uint32_t i_blocks_hi;               // Blocks (high 32 bits, ext4)
        uint32_t i_file_acl_hi;             // Extended attribute block (high bits, ext4)
        uint32_t i_uid_high;                // UID (high 16 bits, ext4)
        uint32_t i_gid_high;                // GID (high 16 bits, ext4)
        uint32_t i_checksum_lo;             // Inode checksum (low bits, ext4)
        uint32_t i_pad;                     // Padding
        uint32_t i_ctime_extra;             // Extra precision times (ext4)
        uint32_t i_mtime_extra;
        uint32_t i_atime_extra;
        uint32_t i_crtime;                  // Creation time (ext4)
        uint32_t i_crtime_extra;
        uint32_t i_version_hi;              // Version (high 32 bits, ext4)
        uint32_t i_projid;                  // Project ID (ext4)
        uint32_t i_checksum;                // Inode checksum (full, ext4)
    };

    /**
     * @struct EXT4DirectoryEntry
     * @brief ext4 directory entry
     *
     * Maps filename to inode number
     */
    struct EXT4DirectoryEntry {
        uint32_t inode;                     // Inode number
        uint16_t rec_len;                   // Record length
        uint8_t name_len;                   // Filename length
        uint8_t file_type;                  // File type (for ext4)
        char name[255];                     // Filename (variable length, UTF-8)
    };

    // Internal state
    int total_recoverable_files_ = 0;
    int total_deleted_files_ = 0;
    std::string journal_info_;
    std::string last_parsed_device_;
    bool is_initialized_ = false;

    /**
     * @brief Read and validate ext4 superblock
     * @param device_path Device path
     * @param [out] superblock Filled EXT4Superblock structure
     * @return true if valid ext4 superblock found
     */
    bool ReadSuperblock(const std::string& device_path,
                       EXT4Superblock& superblock) const;

    /**
     * @brief Parse block group descriptors
     * @param device_path Device path
     * @param superblock Valid superblock
     * @param [out] group_descs Filled group descriptor list
     * @return true if parsing successful
     */
    bool ParseGroupDescriptors(const std::string& device_path,
                              const EXT4Superblock& superblock,
                              std::vector<EXT4GroupDescriptor>& group_descs);

    /**
     * @brief Parse inode table for a block group
     * @param device_path Device path
     * @param superblock Valid superblock
     * @param group_desc Block group descriptor
     * @param [out] entries Extracted file entries
     * @return true if parsing successful
     */
    bool ParseInodeTable(const std::string& device_path,
                        const EXT4Superblock& superblock,
                        const EXT4GroupDescriptor& group_desc,
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
     * @brief Parse directory entries for filename
     * @param inode_data Raw inode data
     * @param [out] filename Extracted filename
     * @return true if filename found
     */
    bool ParseDirectoryEntry(const std::vector<uint8_t>& inode_data,
                            std::string& filename) const;

    /**
     * @brief Check if inode is deleted
     * @param inode_data Raw inode data
     * @return true if inode is deleted, false if active
     */
    bool IsInodeDeleted(const std::vector<uint8_t>& inode_data) const;

    /**
     * @brief Parse journal information (JBD2)
     * @param device_path Device path
     * @param superblock Valid superblock
     * @return Journal info string
     */
    std::string ParseJournal(const std::string& device_path,
                            const EXT4Superblock& superblock);
};

#endif  // EXT4_PARSER_H
