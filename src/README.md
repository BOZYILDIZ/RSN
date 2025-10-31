# RecoverySoftNetz — Source Code Structure

**Phase**: 0.1 (Bootstrap + Skeleton)
**Status**: Core structure in place, implementation in progress

---

## 📁 Directory Layout

```
src/
├── core/
│   ├── recovery_engine.h/cpp      # Main recovery orchestrator
│   ├── file_registry.h/cpp        # Registry of recoverable files
│   └── fragment_assembler.h/cpp   # Fragment reassembly (future)
├── filesystems/
│   ├── filesystem_interface.h     # Abstract base for parsers
│   ├── ntfs_parser.h/cpp          # NTFS parser (Windows)
│   ├── apfs_parser.h/cpp          # APFS parser (macOS)
│   ├── ext4_parser.h/cpp          # ext4 parser (Linux)
│   └── ...
├── ui/
│   ├── main.cpp                   # Application entry point
│   ├── mainwindow.h/cpp           # Main window
│   ├── device_wizard.h/cpp        # Device selection wizard
│   └── results_view.h/cpp         # Results viewer
├── ml/
│   ├── model_interface.h          # ML model interface
│   └── ...
└── common/
    ├── logging.h/cpp              # Logging utilities
    ├── utils.h/cpp                # Common utilities
    └── crypto.h/cpp               # Encryption utilities
```

---

## 🔧 Build Instructions

### Configure
```bash
cd build
cmake ..
```

### Build
```bash
cmake --build .
```

### Run
```bash
./bin/RecoverySoftNetz
```

### Tests
```bash
ctest --output-on-failure
```

---

## 📚 Key Classes

### RecoveryEngine
Main orchestrator for file recovery operations.

```cpp
RecoveryEngine engine;
engine.StartScan("/dev/disk1");
int progress = engine.GetProgress();
int files = engine.GetRecoveredFileCount();
```

### FileSystemInterface
Abstract base for file system parsers.

Each parser (NTFS, APFS, ext4) implements this interface.

---

## 🎯 Phase 1 Roadmap

### Week 1-2 : Foundation
- [ ] Complete RecoveryEngine implementation
- [ ] Implement FileRegistry
- [ ] Setup GoogleTest framework
- [ ] Add CI/CD integration

### Week 3-5 : File System Parsers
- [ ] NTFS parser (metadata-based recovery)
- [ ] APFS parser (macOS support)
- [ ] ext4 parser (Linux support)
- [ ] Unit tests for each parser (>80% coverage)

### Week 6-8 : UI & Basic Recovery
- [ ] Qt main window with device selection
- [ ] Progress monitoring UI
- [ ] Results display (table/tree view)
- [ ] Export functionality
- [ ] Basic file carving algorithm
- [ ] Integration tests

---

## ⚠️ Notes

- Code is currently in **skeleton phase** — stubs with TODO comments
- All `.cpp` implementations are placeholders
- Actual recovery logic to be implemented in Phase 1
- Tests are skeletons ready for mock data

---

**See** : [CMakeLists.txt](../CMakeLists.txt) for build configuration
**See** : [DEVELOPER_SETUP.md](../../DEVELOPER_SETUP.md) for setup instructions
