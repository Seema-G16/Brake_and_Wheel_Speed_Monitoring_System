# GitHub Workflows Documentation

## Available Workflows

This project includes automated CI/CD workflows for code quality assurance.

---

## 1. Build, Test & Lint (Comprehensive)

**File**: `.github/workflows/build-test-lint.yml`

### Features
- ✅ Multi-platform testing (Ubuntu + Windows)
- ✅ Multiple compiler support (GCC, Clang, MSVC)
- ✅ Automated unit test execution
- ✅ Code linting with cppcheck
- ✅ Static analysis with clang-tidy
- ✅ Code coverage reporting
- ✅ Security scanning with Trivy
- ✅ Artifact upload for test results

### Triggers
- Push to `main` or `develop` branches
- Pull requests to `main` or `develop` branches

### Jobs

#### Build & Test
- Runs on: Ubuntu, Windows
- Compilers: GCC, Clang (Ubuntu); MSVC (Windows)
- Steps:
  1. Checkout code
  2. Setup CMake
  3. Install dependencies
  4. Configure project (C++17, Release)
  5. Build project
  6. Run unit tests (CTest)
  7. Run cppcheck linting
  8. Upload test artifacts

#### Clang-Tidy
- Runs on: Ubuntu
- Checks:
  - Readability issues
  - Performance problems
  - Modernization opportunities
- Integrated into build process

#### Code Coverage
- Runs on: Ubuntu
- Generates: LCOV coverage report
- Uploads to: Codecov
- Excludes: GoogleTest framework, test files

#### Security Scan
- Runs on: Ubuntu
- Tool: Trivy
- Outputs: SARIF format for GitHub Security tab
- Scans: Entire repository for vulnerabilities

#### Status Check
- Aggregates results from all jobs
- Fails if any job fails
- Provides clear pass/fail status

---

## 2. Quick Build & Test (Fast)

**File**: `.github/workflows/quick-ci.yml`

### Features
- ⚡ Fast single-platform testing (Ubuntu)
- ✅ Essential checks only (build, test, lint)
- ✅ Minimal overhead
- ✅ Quick feedback for PR reviews

### Triggers
- Same as comprehensive workflow

### Jobs

#### Quick Check
- Runs on: Ubuntu
- Steps:
  1. Checkout
  2. Setup CMake
  3. Install dependencies
  4. Configure & build
  5. Run tests
  6. Lint source code
  7. Lint tests (non-blocking)

### Use Cases
- Pre-commit checks
- Quick feedback on PRs
- Fast iteration during development

---

## Workflow Comparison

| Feature | build-test-lint.yml | quick-ci.yml |
|---------|-------------------|------------|
| Platforms | Ubuntu + Windows | Ubuntu only |
| Compilers | GCC, Clang, MSVC | GCC only |
| Build & Test | ✅ | ✅ |
| Linting | ✅ | ✅ |
| Clang-tidy | ✅ | ❌ |
| Coverage | ✅ | ❌ |
| Security Scan | ✅ | ❌ |
| Speed | Slow | Fast |
| Use Case | Full validation | Quick checks |

---

## Environment Details

### Ubuntu Latest
- OS: Ubuntu 22.04 LTS
- Compilers: GCC 11+, Clang 14+
- Tools: CMake 3.20+, cppcheck, clang-tidy, lcov

### Windows Latest
- OS: Windows Server 2022
- Compiler: MSVC (Visual Studio 2022)
- Tools: CMake 3.20+, cppcheck

---

## Configuration

### CMake Configuration
```bash
cmake -B build \
  -DCMAKE_CXX_STANDARD=17 \
  -DCMAKE_BUILD_TYPE=Release
```

### Build Command
```bash
cmake --build build --config Release
```

### Test Execution
```bash
cd build
ctest --verbose --output-on-failure
```

### Linting (cppcheck)
```bash
cppcheck --enable=all --suppress=missingIncludeSystem \
  --std=c++17 --error-exitcode=1 \
  src/ include/ tests/
```

### Linting (clang-tidy)
```bash
cmake -DCMAKE_CXX_CLANG_TIDY="clang-tidy;--checks=-*,readability-*,performance-*,modernize-*"
cmake --build build
```

---

## Test Results

### Test Output
- Location: `build/` directory
- Format: CTest output, XML reports
- Upload: GitHub Artifacts (with matrix naming)

### Coverage Report
- Format: LCOV info file
- Upload: Codecov
- Exclusions: GoogleTest, test files

### Security Report
- Format: SARIF
- Visible in: GitHub Security tab
- Tool: Trivy

---

## What Gets Checked

### Compilation
- C++17 standard compliance
- No compilation errors
- No linkage errors

### Unit Tests
- All tests pass (52/52 expected)
- No test failures
- Execution time tracked

### Code Quality
- **Linting** (cppcheck):
  - Logic errors
  - Performance issues
  - Style violations
  - Resource management
  
- **Static Analysis** (clang-tidy):
  - Readability: Clear variable names, const correctness
  - Performance: Unnecessary copies, inefficient patterns
  - Modernization: C++11/14/17 features usage

### Security
- Dependency vulnerabilities
- Known CVEs
- Misconfigurations

### Coverage
- Line coverage (goal: >90%)
- Branch coverage tracking
- Exclusions: Test framework, test code

---

## Artifacts

### Uploaded Artifacts
- Test results from each platform
- Named: `test-results-{os}-{compiler}`
- Retention: 30 days (default)
- Use: Debugging test failures on CI

---

## Status Badges

Add these badges to your README.md:

```markdown
[![Build, Test & Lint](https://github.com/YourOrg/YourRepo/actions/workflows/build-test-lint.yml/badge.svg)](https://github.com/YourOrg/YourRepo/actions/workflows/build-test-lint.yml)

[![Quick CI](https://github.com/YourOrg/YourRepo/actions/workflows/quick-ci.yml/badge.svg)](https://github.com/YourOrg/YourRepo/actions/workflows/quick-ci.yml)

[![codecov](https://codecov.io/gh/YourOrg/YourRepo/branch/main/graph/badge.svg)](https://codecov.io/gh/YourOrg/YourRepo)
```

---

## Troubleshooting

### Build Fails on Windows
- MSVC compiler issue: Ensure C++ workload is installed
- CMake path: Verify CMake is in PATH

### Tests Fail in CI but Pass Locally
- Dependency version: CI might have newer packages
- Path issues: Use absolute or CMake variables
- Floating point: Use epsilon-based comparisons

### Linting Too Strict
- Modify suppressions in `.github/workflows/`
- Create `.cppcheckignore` file
- Add clang-tidy configuration file

### Coverage Upload Fails
- Codecov token: May be required for private repos
- LCOV format: Ensure valid coverage.info format

---

## GitHub Integration

### Pull Request Checks
- Workflows run automatically on PR creation
- Results shown in PR conversation
- Required checks can be enforced in branch protection

### Branch Protection Rules
Recommended settings:

```
Status Checks Required:
- Quick Build & Test ✅
- Build, Test & Lint ✅
```

### Secrets Required
- `CODECOV_TOKEN` (optional, for private repos)
- `GITHUB_TOKEN` (provided automatically)

---

## Performance

### Workflow Execution Times

| Workflow | Time |
|----------|------|
| quick-ci.yml | ~2-3 min |
| build-test-lint.yml (partial) | ~5-10 min |
| build-test-lint.yml (full) | ~15-20 min |
| Code coverage | ~5 min |
| Security scan | ~2 min |

---

## Next Steps

1. **Push a commit** to see workflows in action
2. **Check Actions tab** to monitor execution
3. **Review results** in the workflow summary
4. **Configure branch protection** to require passing checks
5. **Add badges** to README.md for visibility

---

## Resources

- [GitHub Actions Documentation](https://docs.github.com/en/actions)
- [CMake Documentation](https://cmake.org/documentation/)
- [CTest Documentation](https://cmake.org/cmake/help/latest/manual/ctest.1.html)
- [cppcheck Documentation](http://cppcheck.sourceforge.net/)
- [clang-tidy Documentation](https://clang.llvm.org/extra/clang-tidy/)
- [Codecov Documentation](https://docs.codecov.com/)

