# Contributing to RecoverySoftNetz

Thank you for your interest in contributing! This document outlines the process and guidelines for contributing to the RecoverySoftNetz project.

---

## 📋 Table of Contents

1. [Code of Conduct](#code-of-conduct)
2. [Getting Started](#getting-started)
3. [Development Workflow](#development-workflow)
4. [Commit Message Conventions](#commit-message-conventions)
5. [Code Style & Standards](#code-style--standards)
6. [Testing Requirements](#testing-requirements)
7. [Pull Request Process](#pull-request-process)
8. [Reporting Issues](#reporting-issues)

---

## 📜 Code of Conduct

This project is committed to providing a welcoming and inclusive environment for all contributors. Please read [CODE_OF_CONDUCT.md](CODE_OF_CONDUCT.md) before contributing.

**Golden Rule**: Treat all contributors with respect and professionalism.

---

## 🚀 Getting Started

### 1. Fork the Repository

```bash
# Click the "Fork" button on GitHub
# Clone your fork
git clone https://github.com/YOUR_USERNAME/RSN.git
cd RSN
```

### 2. Add Upstream Remote

```bash
# Add the original repo as upstream
git remote add upstream https://github.com/lekesiz/RSN.git

# Verify remotes
git remote -v
# origin    https://github.com/YOUR_USERNAME/RSN.git (fetch/push)
# upstream  https://github.com/lekesiz/RSN.git (fetch)
```

### 3. Create Development Environment

```bash
# See DEVELOPER_SETUP.md for full instructions
source venv/bin/activate
pip install -r requirements.txt
```

---

## 🔄 Development Workflow

### Step 1: Create Feature Branch

```bash
# Update main from upstream
git fetch upstream
git checkout main
git merge upstream/main

# Create feature branch
git checkout -b feature/your-feature-name
# or
git checkout -b fix/issue-description
# or
git checkout -b docs/documentation-update
```

### Branch Naming Convention

```
feature/short-description      # New features
fix/issue-description          # Bug fixes
docs/documentation-topic       # Documentation
refactor/component-name        # Refactoring
test/test-suite-name          # Test additions
chore/maintenance-task         # Maintenance
```

### Step 2: Make Changes

1. **Edit Files**: Make your changes in the appropriate directory
2. **Code Quality**: Follow the code style guidelines (see below)
3. **Test Locally**: Run tests before committing
4. **Update Docs**: Update related documentation if needed

### Step 3: Commit Changes

```bash
# Stage changes
git add .

# Commit with conventional message (see below)
git commit -m "feat: add file recovery algorithm"
```

### Step 4: Push to Fork

```bash
git push origin feature/your-feature-name
```

### Step 5: Create Pull Request

Go to https://github.com/lekesiz/RSN and click "Create Pull Request"

---

## 💬 Commit Message Conventions

Follow **Conventional Commits** format:

```
<type>(<scope>): <subject>

<body>

<footer>
```

### Types

- **feat**: A new feature
- **fix**: A bug fix
- **docs**: Documentation only changes
- **refactor**: Code refactoring without feature changes
- **perf**: Performance improvements
- **test**: Adding or updating tests
- **chore**: Changes to build system, dependencies, etc.
- **ci**: CI/CD configuration changes

### Scope (Optional)

Specify the component affected:
- `core` — Recovery engine
- `fs` — File system parsers
- `ui` — User interface
- `ml` — AI/ML components
- `test` — Testing infrastructure

### Examples

```
feat(core): implement metadata-based recovery algorithm

fix(fs): resolve NTFS inode parsing bug
- Fixes issue with deleted file recovery
- Adds unit test for edge cases

docs: add API reference documentation

refactor(ui): simplify device selection wizard

test(fs): add APFS parser unit tests

chore: update dependencies to latest versions
```

### Commit Message Best Practices

- **Be concise**: Subject line ≤ 50 characters
- **Be descriptive**: Body explains "why", not "what"
- **Use imperative mood**: "fix bug" not "fixed bug"
- **Reference issues**: "Fixes #123" or "Relates to #456"
- **One logical change per commit**: Easier to review and revert if needed

---

## 🎨 Code Style & Standards

### C++ Code Style

**Naming Conventions**:
```cpp
class FileRecoveryEngine { };      // PascalCase for classes
struct FileMetadata { };            // PascalCase for structs
void parseFileSystem() { }          // camelCase for functions
int file_descriptor;               // snake_case for variables
constexpr int MAX_BUFFER = 1024;   // UPPER_CASE for constants
```

**Formatting**:
- Indentation: 2 spaces (see `.editorconfig`)
- Line length: ≤ 100 characters (prefer 80)
- Braces: Allman style (opening brace on new line)
- Comments: Use `//` for single-line, `/* */` for multi-line

**Example**:
```cpp
class FileSystemParser
{
public:
  // Parse file system metadata
  bool Parse(const std::string& device_path)
  {
    // Implementation
    return true;
  }

private:
  std::string device_path_;
  std::vector<Inode> inodes_;
};
```

### Python Code Style

**PEP 8 Compliance**:
```python
# Use Black formatter for auto-formatting
black .

# Use Flake8 for linting
flake8 .

# Use isort for import sorting
isort .
```

**Example**:
```python
from typing import List

class MLPredictor:
    """Predict fragment reassembly patterns."""

    def __init__(self, model_path: str) -> None:
        self.model = self.load_model(model_path)

    def predict(self, fragments: List[bytes]) -> List[int]:
        """Predict order of fragments."""
        return self.model.predict(fragments)
```

### Documentation

- **Doxygen**: C++ code documentation
- **Docstrings**: Python function documentation
- **Markdown**: High-level architecture & guides

---

## 🧪 Testing Requirements

### Unit Tests (Required)

```bash
# Run C++ tests
cd build
ctest --output-on-failure

# Run Python tests
pytest tests/python/ -v
```

### Coverage Target

- **Minimum**: 80% code coverage
- **Preferred**: 90%+

### Test Naming

```cpp
// GoogleTest format
TEST(FileSystemParser, ParseNTFS_ValidDevice_Success)
{
  FileSystemParser parser;
  bool result = parser.Parse("/dev/disk1");
  EXPECT_TRUE(result);
}
```

### New Feature Checklist

- [ ] Unit tests written for new code
- [ ] Integration tests updated (if applicable)
- [ ] Coverage maintained ≥ 80%
- [ ] All tests passing locally
- [ ] Documentation updated

---

## 🔀 Pull Request Process

### Before Submitting

1. **Sync with upstream**:
   ```bash
   git fetch upstream
   git rebase upstream/main
   ```

2. **Run tests locally**:
   ```bash
   cd build
   ctest --output-on-failure
   ```

3. **Check code quality**:
   ```bash
   # C++
   clang-format --check src/**/*.cpp

   # Python
   black --check .
   flake8 .
   ```

### PR Template

Use the template in [.github/PULL_REQUEST_TEMPLATE.md](.github/PULL_REQUEST_TEMPLATE.md)

**Key sections**:
- Description of changes
- Related issues (fixes/relates to #number)
- Type of change (bug/feature/refactor/etc)
- Testing information
- Documentation updates

### After Submission

1. **Respond to feedback**: Address reviewer comments promptly
2. **Update PR**: Push updates to the same branch
3. **Rebase if needed**: Keep history clean with `git rebase -i`
4. **Wait for approval**: At least one approved review required

### Merging

Only maintainers can merge. PR is merged when:
- ✅ All checks pass (CI/CD)
- ✅ At least 1 approval from maintainers
- ✅ No conflicting changes
- ✅ Branch up-to-date with main

---

## 📝 Reporting Issues

### Bug Reports

Use the [Bug Report template](.github/ISSUE_TEMPLATE/bug_report.yml):

1. **Describe the bug**: What happened?
2. **Steps to reproduce**: How to reproduce?
3. **Expected behavior**: What should happen?
4. **Actual behavior**: What actually happened?
5. **Environment**: OS version, app version, etc.
6. **Error log**: Stack trace or error message

### Feature Requests

Use the [Feature Request template](.github/ISSUE_TEMPLATE/feature_request.yml):

1. **Describe feature**: What's the idea?
2. **Motivation**: Why is it useful?
3. **Proposed solution**: How would it work?
4. **Alternatives**: Any other approaches?

### Security Issues

⚠️ **DO NOT** open a public issue for security vulnerabilities.

Instead, email: security@netz-informatique.fr

---

## 📚 Additional Resources

- [Developer Setup](DEVELOPER_SETUP.md)
- [Architecture](docs/ARCHITECTURE.md)
- [Code of Conduct](CODE_OF_CONDUCT.md)
- [Changelog](CHANGELOG.md)

---

## ❓ Questions?

- Join [GitHub Discussions](https://github.com/BOZYILDIZ/RSN/discussions)
- Open an [issue](https://github.com/BOZYILDIZ/RSN/issues)
- Contact: contact@netz-informatique.fr

---

**Thank you for contributing! Every contribution helps make RecoverySoftNetz better.** 🙏

*Maintained by Équipe BOZ (@BOZYILDIZ)*
