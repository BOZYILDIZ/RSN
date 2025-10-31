# Developer Setup Guide — RecoverySoftNetz

**Last Updated**: 2025-10-31
**Status**: Bootstrap Phase (v0.1)
**Platforms**: macOS, Linux, Windows

---

## 📋 Table of Contents

1. [Quick Start (macOS)](#quick-start-macos)
2. [Prerequisites](#prerequisites)
3. [Installation by OS](#installation-by-os)
4. [Development Environment](#development-environment)
5. [First Build](#first-build)
6. [Testing](#testing)
7. [Troubleshooting](#troubleshooting)
8. [IDE Setup](#ide-setup)

---

## 🚀 Quick Start (macOS)

If you're on macOS and just want to get coding immediately:

```bash
# 1. Clone the repository
git clone https://github.com/BOZYILDIZ/RSN.git ~/projects/RSN
cd ~/projects/RSN

# 2. Install dependencies (Homebrew)
brew install cmake python@3.10 qt

# 3. Create Python virtual environment
python3 -m venv venv
source venv/bin/activate

# 4. Install Python dependencies
pip install -r requirements.txt  # When created

# 5. Create build directory
mkdir -p build && cd build

# 6. Configure with CMake (future)
cmake ..

# 7. Build project (future)
cmake --build .

# 8. Run tests
ctest
```

---

## 📦 Prerequisites

### System Requirements

| Component | Minimum | Recommended |
|-----------|---------|-------------|
| **OS** | macOS 11, Ubuntu 20.04, Windows 10 | macOS 13+, Ubuntu 22.04, Windows 11 |
| **RAM** | 4 GB | 8 GB+ |
| **Disk** | 5 GB | 10 GB+ |
| **Git** | 2.30+ | 2.40+ |

### Required Tools

- **Git**: Version control
- **CMake**: Build system (3.18+)
- **C++ Compiler**: C++17 support (clang, gcc, MSVC)
- **Python**: 3.10+ (for AI/ML components)
- **Qt**: 6.x (cross-platform UI framework)

### Optional Tools

- **CLion** or **Qt Creator**: IDE for C++/Qt development
- **VSCode**: Lightweight editor with C++ extensions
- **Docker**: For containerized builds
- **GitHub CLI (`gh`)**: For interacting with GitHub from terminal

---

## 🔧 Installation by OS

### macOS Setup

#### 1. Xcode Command Line Tools

```bash
# Check if already installed
xcode-select -p

# If not installed, install
xcode-select --install

# Verify installation
clang --version
```

#### 2. Homebrew (Package Manager)

```bash
# Install Homebrew if not present
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Verify
brew --version
```

#### 3. Essential Development Tools

```bash
# Update Homebrew
brew update

# Install CMake
brew install cmake

# Install Git (if not already present)
brew install git

# Install Python 3.10
brew install python@3.10

# Link Python to bin
brew link python@3.10

# Verify Python
python3 --version
```

#### 4. Qt 6.x

```bash
# Option A: Install via Homebrew (simple)
brew install qt

# Option B: Install from Qt Online Installer (recommended)
# Download from https://www.qt.io/download-open-source
# Follow the installer for Qt 6.x + Qt Creator
```

#### 5. GoogleTest (C++ Testing)

```bash
# Install GoogleTest
brew install googletest

# Or: Clone and build locally
git clone https://github.com/google/googletest.git
cd googletest
mkdir build && cd build
cmake ..
make && sudo make install
```

#### 6. Python Virtual Environment

```bash
# Create virtual environment
python3 -m venv venv

# Activate
source venv/bin/activate

# Verify
python --version  # Should show 3.10+
```

---

### Linux Setup (Ubuntu/Debian)

#### 1. Update Package Manager

```bash
sudo apt update
sudo apt upgrade -y
```

#### 2. Install Build Tools

```bash
sudo apt install -y \
  build-essential \
  cmake \
  git \
  python3 python3-pip python3-venv \
  libqt6core6 libqt6gui6 libqt6widgets6 \
  qt6-qpa-plugins

# Or for Qt 6 development headers
sudo apt install -y qt6-base-dev qt6-tools-dev
```

#### 3. Verify Installations

```bash
gcc --version
cmake --version
python3 --version
```

---

### Windows Setup (PowerShell)

#### 1. Install Chocolatey (Package Manager)

```powershell
# Run PowerShell as Administrator
Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser

# Install Chocolatey
[System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072
iex ((New-Object System.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1'))
```

#### 2. Install Development Tools

```powershell
# Using Chocolatey
choco install -y cmake python git

# Or use Microsoft C++ Build Tools
# Download: https://visualstudio.microsoft.com/visual-cpp-build-tools/
```

#### 3. Install Qt 6

```powershell
# Via Qt Online Installer
# Download: https://www.qt.io/download-open-source
# Select Qt 6.x + Qt Creator
```

---

## 🛠️ Development Environment

### Directory Structure

```
~/projects/RSN/
├── src/                 # Source code
├── tests/               # Test suites
├── docs/                # Documentation
├── build/               # Build artifacts (created by CMake)
├── venv/                # Python virtual environment
└── CMakeLists.txt       # CMake configuration
```

### Environment Variables

```bash
# Python virtual environment (automatic when sourced)
source venv/bin/activate

# Optional: Set Qt path (if needed)
export Qt6_DIR=/usr/local/opt/qt6/lib/cmake/Qt6

# Verify
echo $VIRTUAL_ENV
```

---

## 🔨 First Build

### Step 1: Clone Repository

```bash
git clone https://github.com/BOZYILDIZ/RSN.git
cd RSN
```

### Step 2: Create Virtual Environment

```bash
python3 -m venv venv
source venv/bin/activate  # macOS/Linux
# or: venv\Scripts\activate  # Windows
```

### Step 3: Install Python Dependencies

```bash
# When requirements.txt is created
pip install -r requirements.txt

# Current placeholder
pip install pytest flake8 black
```

### Step 4: Create Build Directory

```bash
mkdir -p build
cd build
```

### Step 5: Configure with CMake

```bash
# Basic configuration
cmake ..

# With specific Qt path (if needed)
cmake -DQt6_DIR=/path/to/qt6 ..

# Release build with optimizations
cmake -DCMAKE_BUILD_TYPE=Release ..
```

### Step 6: Build

```bash
# Build all targets
cmake --build .

# Or use Make directly
make -j$(nproc)

# On macOS
make -j$(sysctl -n hw.ncpu)
```

### Step 7: Run Tests

```bash
ctest --output-on-failure
```

---

## 🧪 Testing

### Unit Tests (C++)

```bash
cd build
ctest --output-on-failure

# Or run GoogleTest directly
./tests/unit_tests --gtest_color=yes
```

### Python Tests

```bash
cd build
pytest ../tests/python/ -v
```

### Coverage Report

```bash
ctest --coverage
```

---

## 🆘 Troubleshooting

### Qt Not Found

```bash
# On macOS
export Qt6_DIR=/usr/local/opt/qt6

# On Linux
export Qt6_DIR=/usr/lib/cmake/Qt6

# Then reconfigure CMake
cd build && cmake ..
```

### Python Virtual Environment Issues

```bash
# Deactivate current venv
deactivate

# Remove and recreate
rm -rf venv
python3 -m venv venv
source venv/bin/activate
pip install -r requirements.txt
```

### CMake Build Errors

```bash
# Clean build
cd build
rm -rf *
cmake ..
cmake --build .
```

### Permission Denied on macOS

```bash
# Fix Xcode CLT permissions
sudo xcode-select --reset

# Or reinstall
xcode-select --install
```

---

## 🖥️ IDE Setup

### Qt Creator (Recommended)

1. **Install Qt Creator** (included in Qt 6 installer)
2. **Open Project**: File → Open File or Project → Select `CMakeLists.txt`
3. **Configure Kit**: Select macOS (Clang) or Linux (GCC)
4. **Build**: Ctrl+B (macOS: Cmd+B)
5. **Run**: Ctrl+R (macOS: Cmd+R)

### CLion (IntelliJ)

1. **Install CLion**: https://www.jetbrains.com/clion/
2. **Open Project**: File → Open → Select RSN folder
3. **CMake Configuration**: CLion auto-detects CMakeLists.txt
4. **Build**: Ctrl+F9 (macOS: Cmd+F9)
5. **Run**: Shift+F10 (macOS: Ctrl+R)

### VSCode

1. **Install Extensions**:
   - C/C++ (Microsoft)
   - CMake Tools (Microsoft)
   - CMake (twxs)

2. **Create `.vscode/settings.json`**:
```json
{
  "cmake.sourceDirectory": "${workspaceFolder}",
  "cmake.buildDirectory": "${workspaceFolder}/build",
  "C_Cpp.default.configurationProvider": "ms-vscode.cmake-tools"
}
```

3. **Configure & Build**: Use CMake extension in sidebar

### Xcode (macOS)

```bash
# Generate Xcode project from CMake
cd build
cmake -G Xcode ..

# Open in Xcode
open RecoverySoftNetz.xcodeproj
```

---

## 📚 Documentation

- **Architecture**: See [ARCHITECTURE.md](docs/ARCHITECTURE.md)
- **Contributing**: See [CONTRIBUTING.md](CONTRIBUTING.md)
- **API Reference**: See [docs/API.md](docs/API.md) (future)
- **Build System**: See [CMakeLists.txt](CMakeLists.txt) (future)

---

## 🔗 Useful Resources

- **CMake**: https://cmake.org/documentation/
- **Qt 6**: https://doc.qt.io/qt-6/
- **C++ Standard**: https://en.cppreference.com/
- **GoogleTest**: https://google.github.io/googletest/
- **Python**: https://docs.python.org/3.10/

---

## 💬 Need Help?

- Check [CONTRIBUTING.md](CONTRIBUTING.md) for development guidelines
- Open an issue on GitHub: https://github.com/BOZYILDIZ/RSN/issues
- Join discussions: https://github.com/BOZYILDIZ/RSN/discussions

---

**Happy coding! 🚀**

*Last Updated: 2025-10-31 by Équipe BOZ*
