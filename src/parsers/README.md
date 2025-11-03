# RecoverySoftNetz — Filesystem Parsers

This directory contains implementations of filesystem parsers for RecoverySoftNetz. Each parser is responsible for:

1. **Detecting** its respective filesystem type
2. **Parsing** the filesystem structures (boot sector, metadata, file records)
3. **Extracting** recoverable files and their metadata
4. **Computing** recovery statistics

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

### APFS Parser (Planned — Phase 5B)

**Filesystem**: APFS (Apple File System)
- macOS 10.13+, iOS 10.3+
- Modern Apple systems

**Planned Features**:
- Container parsing
- B-tree navigation
- Snapshot support
- Encryption handling

---

### ext4 Parser (Planned — Phase 5C)

**Filesystem**: ext4 (Fourth Extended Filesystem)
- Linux primary filesystem
- RHEL, Ubuntu, Debian, Fedora
- Embedded systems

**Planned Features**:
- Superblock parsing
- inode table traversal
- Extent tree parsing
- Sparse file handling

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
