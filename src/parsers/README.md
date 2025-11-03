# RecoverySoftNetz — Filesystem Parsers

This directory contains implementations of filesystem parsers for RecoverySoftNetz. Each parser is responsible for:

1. **Detecting** its respective filesystem type
2. **Parsing** the filesystem structures (boot sector, metadata, file records)
3. **Extracting** recoverable files and their metadata
4. **Computing** recovery statistics

## Phase 5E: Device I/O Integration

**Status**: I/O Abstraction Layer Ready ✅

The parsers now have access to a **cross-platform Device I/O abstraction layer** (`src/io/device_io.h/cpp`) that handles low-level block device operations:

- **Linux**: POSIX I/O + ioctl(BLKGETSIZE64)
- **macOS**: BSD I/O + ioctl(DKIOCGETBLOCKCOUNT)
- **Windows**: CreateFile() + ReadFile()

### Integration via DeviceIOAdapter

The `DeviceIOAdapter` (`src/io/device_io_adapter.h/cpp`) provides:
- Device open/close management
- Filesystem detection via magic numbers
- Automatic parser routing
- Error handling and statistics collection

See `docs/DEVICE_IO_INTEGRATION.md` for integration details.

### Parser Updates (TODO - Phase 5E+)

Each parser is currently using **mock implementations** but can now be upgraded:

```cpp
// Replace this (mock):
bool NTFSParser::ReadBootSector(const std::string &device_path, ...) {
    // Mock data
}

// With this (real I/O):
bool NTFSParser::ReadBootSector(const std::string &device_path, ...) {
    DeviceIO io;
    io.Open(device_path);
    auto data = io.ReadBlockVector(0, 512);
    // Parse real data
}
```

---

## Implemented Parsers

### NTFS Parser (`ntfs_parser.h/cpp`)

**Status**: Phase 5A — Skeleton Implementation

**Filesystem**: NTFS (NT File System)
- Windows NT, 2000, XP, Vista, 7, 8, 10, 11
- NTFS versions 1.0, 3.0, 3.1
- MBR and GPT partitions

**Supported Features**:
- Cluster sizes: 512B to 64KB
- MFT (Master File Table) parsing
- File record extraction
- Deleted file detection and recovery
- Multi-sector handling

**Key Components**:
- `ReadBootSector()` — Validate NTFS boot sector
- `ParseMFT()` — Parse Master File Table
- `ParseFileRecord()` — Extract individual file records
- `IsFileDeleted()` — Identify deleted files

**Implementation Notes**:
- Boot sector signature: "NTFS    " (offset 3-10)
- MFT record size: typically 1024 bytes
- File record signature: "FILE" (0x454C4946)
- Attribute types: Standard Info (0x10), Filename (0x30), Data (0x80)

**TODO**:
- [ ] Actual device I/O (platform-specific)
- [ ] MFT parsing with sparse files
- [ ] Alternate Data Streams (ADS) support
- [ ] Compressed/encrypted file handling
- [ ] Large filesystem optimization (>1TB)
- [ ] Error recovery for corrupted sectors

---

### APFS Parser (`apfs_parser.h/cpp`)

**Status**: Phase 5B — Skeleton Implementation

**Filesystem**: APFS (Apple File System)
- macOS 10.13+ (High Sierra and later)
- iOS 10.3+, iPadOS, tvOS, watchOS
- APFS versions 1.0, 2.0, 3.0

**Supported Features**:
- Container and volume detection
- Container superblock parsing
- Volume superblock extraction
- B-tree navigation for file records
- Inode parsing and metadata extraction
- Deleted file detection and recovery
- Snapshot support for point-in-time recovery
- Block sizes: 4KB to 64KB (typically 4KB)

**Key Components**:
- `ReadContainerSuperblock()` — Validate APFS container
- `ParseVolumeSuperblock()` — Extract volume metadata
- `ParseVolumeBTree()` — Navigate file tree structure
- `ParseInode()` — Extract individual file records
- `GetAvailableSnapshots()` — List recovery points
- `IsInodeDeleted()` — Identify deleted files

**Implementation Notes**:
- Container magic: "NXSB" (0x4253584E)
- Volume magic: "APSB" (0x42535041)
- Inode magic: "INOD" (0x494E4F44)
- B-tree node magic: "NODE" (0x4E4F4445)
- Default block size: 4096 bytes
- Snapshots: Time Machine and manual snapshots

**TODO**:
- [ ] Actual device I/O (macOS-specific block device APIs)
- [ ] B-tree node parsing and traversal
- [ ] Snapshot timestamp extraction
- [ ] Compression support (zlib, lz4)
- [ ] Encryption handling (FileVault)
- [ ] Large volume optimization (>1TB)
- [ ] Fusion drive support (multi-device volumes)
- [ ] Error recovery for corrupted sectors

---

### ext4 Parser (`ext4_parser.h/cpp`)

**Status**: Phase 5C — Skeleton Implementation

**Filesystem**: ext4 (Fourth Extended Filesystem)
- Linux kernel 2.6.28+ (mainline ext4 support)
- RHEL, Ubuntu, Debian, Fedora, Alpine (and all major distributions)
- Primary filesystem for Linux systems
- Embedded systems and IoT devices

**Supported Features**:
- Superblock detection (magic 0xEF53)
- Block group descriptors parsing
- Inode table traversal
- Directory entry parsing for filenames
- Deleted inode detection and recovery
- Journal support (JBD2)
- Feature flag detection
- Block sizes: 1KB–64KB
- Inode sizes: 128–256+ bytes

**Key Components**:
- `ReadSuperblock()` — Superblock at offset 1024
- `ParseGroupDescriptors()` — Block group metadata
- `ParseInodeTable()` — Inode records extraction
- `ParseDirectoryEntry()` — Filename extraction
- `IsInodeDeleted()` — Detect unlinked inodes
- `GetJournalInfo()` — JBD2 journal information

**Implementation Notes**:
- Superblock magic: 0xEF53
- Superblock offset: 1024 bytes
- Block group descriptors: Follow superblock
- Inode deletion: i_dtime field (non-zero if deleted)
- Journal: JBD2 standard for crash recovery
- Extent trees: Large file support (ext4 feature)

**TODO**:
- [ ] Actual device I/O (block device reading)
- [ ] Extent tree parsing
- [ ] Journal (JBD2) recovery
- [ ] Compressed inode support
- [ ] Encrypted directory handling
- [ ] Large filesystem optimization (>1TB)
- [ ] Inline data and extended attributes
- [ ] Parallel group processing

---

## Architecture

```
FileSystemInterface (abstract base)
    ↓
NTFSParser : public FileSystemInterface
    ├── GetFileSystemType()
    ├── CanHandle()
    ├── Parse()
    └── GetRecoveryStats()
```

## Parser Interface

All parsers implement `FileSystemInterface` (see `../filesystems/filesystem_interface.h`):

```cpp
class FileSystemInterface {
    virtual std::string GetFileSystemType() const = 0;
    virtual bool CanHandle(const std::string& device_path) const = 0;
    virtual bool Parse(const std::string& device_path,
                       std::vector<FileEntry>& entries) = 0;
    virtual std::pair<int, int> GetRecoveryStats() const = 0;
};
```

## File Entry Structure

Parsers populate `FileEntry` structures containing:
- `filename` — Original filename
- `file_size` — File size in bytes
- `creation_time` — Creation timestamp (Unix time)
- `modification_time` — Last modification timestamp
- `is_directory` — True if directory, false if regular file
- `is_deleted` — True if file was deleted (but recoverable)

## Testing

Each parser has comprehensive unit tests:
- `tests/unit/ntfs_parser_test.cpp` — NTFS parser tests
- `tests/unit/apfs_parser_test.cpp` — APFS parser tests (TODO)
- `tests/unit/ext4_parser_test.cpp` — ext4 parser tests (TODO)

Target coverage: **>80%** for each parser

### Running Tests

```bash
cd ~/Desktop/RSN
mkdir build && cd build
cmake ..
cmake --build .
ctest --output-on-failure
```

## Performance Considerations

- **Large filesystems** (>1TB): Optimize MFT traversal with parallel processing
- **Fragmented files**: Handle extent/cluster chains efficiently
- **Corrupted sectors**: Implement error recovery and skip strategies
- **Compression**: Support compressed file detection and extraction
- **Caching**: Cache frequently accessed metadata blocks

## References

### NTFS
- [Wikipedia: NTFS](https://en.wikipedia.org/wiki/NTFS)
- [Microsoft NTFS Documentation](https://docs.microsoft.com/en-us/windows-server/administration/windows-commands/fsutil)
- [NTFS Specification (reverse-engineered)](https://en.wikipedia.org/wiki/File_Allocation_Table#NTFS)

### APFS
- [Apple APFS Overview](https://support.apple.com/en-us/HT204024)
- [APFS Reference](https://developer.apple.com/library/archive/technotes/tn2459/)

### ext4
- [Linux ext4 Documentation](https://www.kernel.org/doc/html/latest/filesystems/ext4/index.html)
- [ext4 Specification](https://ext4.wiki.kernel.org/)

## Development Guidelines

1. **Parsing**: Always validate signatures and checksums
2. **Error Handling**: Don't crash on corrupted filesystems
3. **Recovery**: Attempt recovery even when errors occur
4. **Testing**: Write tests before implementation (TDD)
5. **Documentation**: Document all public methods and key algorithms

## Next Steps

- [ ] Phase 5A: Complete NTFS parser skeleton → implementation
- [ ] Phase 5B: Implement APFS parser
- [ ] Phase 5C: Implement ext4 parser
- [ ] Phase 5D: UI improvements for parser selection
- [ ] Phase 5E: Performance optimization for large filesystems

---

**Équipe BOZ** — Phase 5A Parsers Documentation
