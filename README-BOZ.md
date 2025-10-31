# 🎼 Équipe BOZ — Plan Technique & Backlog RSN

**Document généré par Équipe BOZ orchestrator (Claude)**
📅 Date : 2025-10-31
🎯 Statut : Phase 1 Préparation
👤 Propriétaire : @BOZYILDIZ (Hasan Biçer)

---

## 🚀 Vue d'ensemble

RecoverySoftNetz (RSN) est actuellement en phase **Planning & Documentation**. Ce document fournit un plan opérationnel pour transformer la documentation en projet de développement prêt pour l'équipe.

### Équipe BOZ Assignée
- **Claude** (Lead/Orchestrator) : Planification, validation, arbitrage
- **ChatGPT-Dev-A** (Développement) : Code, refactoring, CI/CD, scripts
- **ChatGPT-Dev-B** (Tests & Review) : Code review, tests, documentation, QA
- **Gemini-Research-A** (Analyse Code) : Architecture, synthèse technique, codebase
- **Gemini-Research-B** (Veille Téchnique) : Dépendances, sécurité, licences, perf

---

## 📊 État actuel du dépôt

| Métrique | Valeur | Status |
|----------|--------|--------|
| **Fichiers totaux** | 7 | 📋 Docs only |
| **Code de production** | 0 | ❌ Pas commencé |
| **Tests** | 0 | ❌ Pas commencé |
| **Documentation** | 4,018 lignes | ✅ Complète |
| **Lignes de code** | 0 | ❌ À créer |
| **Couverture de test** | 0% | ❌ À établir |

### Composition actuelle
```
RSN/
├── README.md (projet overview)
├── LICENSE (propriétaire)
├── .gitignore
├── GITHUB_UPLOAD_GUIDE.md (guide upload)
├── RecoverySoftNetz_Technical_Documentation_TR.md (150+ pages)
├── RecoverySoftNetz_Executive_Summary_EN.md (executive overview)
└── RecoverySoftNetz_Implementation_Roadmap.md (15-month plan)
```

---

## 🎯 Priorités Immédiates (Semaines 1-2)

### Priority 1️⃣ : Infrastructure de Développement

**Objectif** : Créer l'environnement fondamental pour le développement

#### ✅ Task 1.1 : Créer structure répertoires de base
- **Description** : Organiser le repo avec `src/`, `tests/`, `docs/`, `build/`, `scripts/`
- **Ownership** : ChatGPT-Dev-A
- **Estimation** : 30 min
- **Commit** : `chore: add project directory structure`

**Résultat attendu** :
```
RSN/
├── src/
│   ├── core/              (moteur récupération)
│   ├── ui/                (interface utilisateur)
│   ├── ai_ml/             (composants IA/ML)
│   └── common/            (utilitaires partagés)
├── tests/
│   ├── unit/              (tests unitaires)
│   ├── integration/       (tests intégration)
│   └── performance/       (benchmarks)
├── docs/
│   ├── architecture/      (diagrams, flux)
│   ├── api/               (API reference)
│   └── guides/            (developer guides)
├── build/                 (CMake, Make artefacts)
├── scripts/               (utilitaires build/deploy)
└── .github/
    ├── workflows/         (GitHub Actions)
    ├── ISSUE_TEMPLATE/    (issue templates)
    └── PULL_REQUEST_TEMPLATE.md
```

---

#### ✅ Task 1.2 : Créer DEVELOPER_SETUP.md
- **Ownership** : ChatGPT-Dev-B
- **Estimation** : 2 heures
- **Résultat** : Fichier `/DEVELOPER_SETUP.md` avec :
  - Requirements système (Qt 6, Python 3.10+, C++17 compiler, CMake 3.18+)
  - Installation étape-par-étape (Windows/macOS/Linux)
  - First build guide
  - IDE setup (VSCode, CLion, Qt Creator)
  - Debugging & development tools
  - Common issues & troubleshooting

---

#### ✅ Task 1.3 : Configurer GitHub Actions CI/CD
- **Ownership** : ChatGPT-Dev-A
- **Estimation** : 3 heures
- **Fichiers** :
  - `.github/workflows/build.yml` — Compile sur multiple platforms
  - `.github/workflows/test.yml` — Execute unit tests
  - `.github/workflows/lint.yml` — Code quality checks

**Workflows requis** :
```yaml
# build.yml
name: Build
on: [push, pull_request]
jobs:
  build:
    runs-on: [windows-latest, ubuntu-latest, macos-latest]
    steps:
      - uses: actions/checkout@v3
      - name: Install dependencies (platform-specific)
      - name: Build
      - name: Test
      - name: Upload artifacts

# test.yml
name: Tests
on: [push, pull_request]
jobs:
  test:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v3
      - name: Run unit tests
      - name: Generate coverage
      - name: Upload coverage
```

---

#### ✅ Task 1.4 : Créer templates GitHub Issues & PRs
- **Ownership** : ChatGPT-Dev-B
- **Estimation** : 1.5 heures
- **Fichiers** :
  - `.github/ISSUE_TEMPLATE/bug_report.md`
  - `.github/ISSUE_TEMPLATE/feature_request.md`
  - `.github/ISSUE_TEMPLATE/documentation.md`
  - `.github/PULL_REQUEST_TEMPLATE.md`

---

### Priority 2️⃣ : Contribution & Quality Standards

#### ✅ Task 2.1 : Créer CONTRIBUTING.md
- **Ownership** : ChatGPT-Dev-B
- **Estimation** : 2 heures
- **Contenu** :
  - Processus de contribution (fork → branch → PR)
  - Code style guidelines (C++, Python, Qt)
  - Commit message conventions (`feat:`, `fix:`, `refactor:`, etc.)
  - Code review process
  - Testing requirements (min 80% coverage for new code)
  - Documentation requirements

---

#### ✅ Task 2.2 : Créer CODE_OF_CONDUCT.md
- **Ownership** : ChatGPT-Dev-B
- **Estimation** : 1 heure
- **Référence** : Contributor Covenant v2.1

---

#### ✅ Task 2.3 : Créer CHANGELOG.md
- **Ownership** : ChatGPT-Dev-A
- **Estimation** : 1 heure
- **Template** : Keep a Changelog format
- **Statut initial** : Version 0.0.0-planning

---

### Priority 3️⃣ : Documentation Technique

#### ✅ Task 3.1 : Créer ARCHITECTURE.md
- **Ownership** : Gemini-Research-A
- **Estimation** : 3 heures
- **Contenu** :
  - System overview diagram (Mermaid)
  - Module dependencies
  - Data flow diagrams
  - Component responsibilities
  - File system parsers support matrix (30+ filesystems)
  - Recovery algorithms overview
  - AI/ML pipeline architecture
  - Security considerations

---

#### ✅ Task 3.2 : Créer DOCUMENTATION_INDEX.md
- **Ownership** : Gemini-Research-B
- **Estimation** : 2 heures
- **Contenu** :
  - Table of contents pour tous les docs
  - Roadmap link (15-month plan)
  - Architecture diagrams
  - API reference (structure future)
  - Team member directory
  - Resource allocation matrix

---

#### ✅ Task 3.3 : Standardiser langue documentation
- **Ownership** : Gemini-Research-B
- **Estimation** : 4 heures
- **Actions** :
  - English = langue primaire (tous les docs techniques)
  - Turkish = langue secondaire (docs marketing/exec si approprié)
  - Créer `README_TR.md` (version Turkish du README)
  - Créer `CONTRIBUTING_TR.md` (guidelines contribution en Turkish)
  - Mettre à jour headers pour indiquer langue + dernière révision

---

## 🛠️ Tâches Phase 1 (Semaines 3-8)

### Sprint 1 : Fondation Core Engine (Semaines 3-5)

#### Core Architecture
- Établir C++ base project structure (CMake)
- Créer classe abstraite `RecoveryEngine`
- Implémenter file system interface générique
- Construire data structure pour "recovered files registry"

#### File System Support (Phase 1.1)
- **Priorité haute** : NTFS, APFS, ext4 (Windows, macOS, Linux)
- **Priorité moyenne** : Btrfs, ZFS, exFAT
- Chaque parser: Parser class, test suite, validation tools

#### Testing Infrastructure
- Setup GoogleTest pour C++
- Setup pytest pour Python/ML components
- CI/CD automation (GitHub Actions)
- Code coverage tracking (>80% target)

---

### Sprint 2 : Basic UI & Recovery (Semaines 6-8)

#### Qt 6 UI Foundation
- Main application window
- Wizard for device selection
- Progress monitoring UI
- Result visualization (table, tree view)

#### Basic Recovery Engine
- Metadata-based recovery (deleted file inodes)
- File signature detection (basic carving)
- Output to USB drive or local storage
- Batch recovery support

#### Testing & QA
- Integration tests
- User workflow testing
- Performance baseline measurements
- Bug tracking & triaging

---

## 📋 Backlog Détaillé (Format GitHub Issues)

### Issue Template : Development Task

```markdown
## Phase 1 Development Backlog

### Category: Core Engine Architecture
- [ ] Task: Create C++ project structure with CMake
- [ ] Task: Design Recovery Engine class hierarchy
- [ ] Task: Implement generic file system interface
- [ ] Task: Create data structures for file recovery metadata

### Category: File System Support
- [ ] Task: Implement NTFS parser
- [ ] Task: Implement APFS parser
- [ ] Task: Implement ext4 parser
- [ ] Task: Test parsers on real file system images
- [ ] Task: Support RAID reconstruction algorithms
- [ ] Task: Support SSD-specific recovery (TRIM-aware)

### Category: Recovery Engines
- [ ] Task: Metadata-based recovery engine
- [ ] Task: File carving engine (signature matching)
- [ ] Task: Fragment reassembly logic
- [ ] Task: Duplicate detection & handling
- [ ] Task: Corruption repair algorithms

### Category: UI & UX
- [ ] Task: Qt 6 base application setup
- [ ] Task: Device selection wizard
- [ ] Task: Scan progress UI
- [ ] Task: Results visualization (table/tree)
- [ ] Task: Export to multiple formats (CSV, JSON, custom)

### Category: AI/ML Integration (Phase 2)
- [ ] Task: ML model training framework
- [ ] Task: Pattern recognition for file fragments
- [ ] Task: Intelligent corruption repair models
- [ ] Task: Performance optimization with ML

### Category: Security & Licensing
- [ ] Task: Implement license key validation
- [ ] Task: Add telemetry/usage tracking (optional)
- [ ] Task: Encrypt sensitive data paths
- [ ] Task: Security audit & penetration testing

### Category: Testing & QA
- [ ] Task: Unit test suite (GoogleTest)
- [ ] Task: Integration tests
- [ ] Task: Performance benchmarks
- [ ] Task: Real-world recovery scenarios testing
- [ ] Task: Regression test suite

### Category: Documentation
- [ ] Task: API documentation (doxygen)
- [ ] Task: User manual (GUI help)
- [ ] Task: Administrator guide (enterprise features)
- [ ] Task: Troubleshooting guide
```

---

## 🎯 Quick Wins (À faire cette semaine)

### Wins pour ChatGPT-Dev-A (Est. 4 heures)
1. [ ] Créer structure répertoires (`src/`, `tests/`, etc.)
2. [ ] Setup CMake project skeleton + build scripts
3. [ ] Create `.github/workflows/build.yml`
4. [ ] Add `CHANGELOG.md` (version 0.0.0-planning)

### Wins pour ChatGPT-Dev-B (Est. 4 heures)
1. [ ] Write `DEVELOPER_SETUP.md`
2. [ ] Create GitHub issue & PR templates
3. [ ] Write `CONTRIBUTING.md`
4. [ ] Write `CODE_OF_CONDUCT.md`

### Wins pour Gemini-Research-A (Est. 3 heures)
1. [ ] Create `ARCHITECTURE.md` avec diagrammes Mermaid
2. [ ] Design module dependency matrix
3. [ ] Document file system support strategy

### Wins pour Gemini-Research-B (Est. 4 heures)
1. [ ] Create `DOCUMENTATION_INDEX.md`
2. [ ] Standardize language (English primary)
3. [ ] Create `CONTRIBUTING_TR.md` (Turkish version)
4. [ ] Add dependency & security audit checklist

---

## 📊 Métriques de Succès

### Phase 0 (This Week)
- [ ] ✅ 5 agents BOZ online
- [ ] ✅ Repo cloned on Desktop
- [ ] ✅ Collaborator access requested
- [ ] ✅ README-BOZ.md + boz-log.txt generated

### Phase 1 (End of Week 2)
- [ ] ✅ Directory structure created
- [ ] ✅ All dev documentation written (DEVELOPER_SETUP, CONTRIBUTING, ARCHITECTURE)
- [ ] ✅ CI/CD workflows configured & passing
- [ ] ✅ GitHub templates active
- [ ] ✅ Repo ready for first code commit

### Phase 1 (End of Week 8)
- [ ] ✅ Core C++ engine foundation laid
- [ ] ✅ 3 primary file systems supported (NTFS, APFS, ext4)
- [ ] ✅ Basic Qt UI functional
- [ ] ✅ Unit tests passing (>80% coverage)
- [ ] ✅ CI/CD fully automated

---

## 🔗 Liens Importants

- **GitHub Repo** : https://github.com/lekesiz/RSN
- **Desktop Location** : ~/Desktop/RSN
- **Project Lead** : @BOZYILDIZ (Hasan Biçer)
- **Netz Informatique** : https://netz-informatique.fr

---

## 📝 Notes pour l'Équipe

1. **Timing ambitieux** : Le plan vise Phase 1 en 8 semaines. Ajuster si ressources insuffisantes.
2. **Collaborateur access** : Issue auto-créée. @lekesiz à approuver pour push rights.
3. **Language mixing** : Docs techniques en English (international team), admin guides en Turkish si approprié.
4. **Open-source readiness** : Infra mise en place pour transition vers open-source (CONTRIBUTING, CODE_OF_CONDUCT, etc.)
5. **Maintenance continue** : BOZ reste assigné pour PR reviews, architecture oversight, dependency updates.

---

**Prochaine étape** : Approuver ce plan et commencer Phase 1 Task 1.1 (créer structure répertoires).

*Generated by Équipe BOZ Orchestrator — 2025-10-31*
