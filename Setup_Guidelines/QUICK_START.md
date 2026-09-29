# 🚀 QUICK START GUIDE - Complete Setup Instructions

## Current Status
- ✅ CMake: Installed (v3.31.6)
- ❌ C++ Compiler: NOT installed
- ✅ Code: Ready
- ✅ Tests: Ready
- ⏳ Build: Pending compiler installation

---

## 🔧 Choose Your Setup Method

### Method 1: Visual Studio 2022 (RECOMMENDED - Easiest)

**Step 1: Download Visual Studio Community 2022**
- Visit: https://visualstudio.microsoft.com/vs/community/
- Click "Download" button
- Run the installer

**Step 2: Select C++ Workload**
- Look for "Desktop development with C++"
- Check the box to select it
- Click "Install"
- Wait 10-15 minutes for installation

**Step 3: Restart PowerShell**
- Close all PowerShell windows
- Open a NEW PowerShell window (important!)

**Step 4: Run Setup**
```powershell
cd "C:\My_Projects\Training\Brake_and_Wheel_Speed_Monitoring_System"
.\SETUP.ps1
```

Or use the batch file:
```bash
SETUP.bat
```

---

### Method 2: MinGW (Lightweight Alternative)

**Step 1: Install MinGW**
```powershell
# Option A: Using Chocolatey (if installed)
choco install mingw

# Option B: Manual download
# Visit: https://www.mingw-w64.org/
# Download and install to default location
# Add to PATH: C:\Program Files\mingw-w64\x86_64-8.1.0-win32-seh-rt_v6-rev0\mingw64\bin
```

**Step 2: Restart PowerShell**

**Step 3: Run Setup**
```powershell
cd "C:\My_Projects\Training\Brake_and_Wheel_Speed_Monitoring_System"
.\SETUP.ps1
```

---

### Method 3: WSL2 (Windows Subsystem for Linux)

**If you already have WSL2 installed:**

```bash
# Open WSL terminal
wsl

# Navigate to project
cd /mnt/c/My_Projects/Training/Brake_and_Wheel_Speed_Monitoring_System

# Install build tools (first time only)
sudo apt-get update
sudo apt-get install -y build-essential cmake

# Run setup
mkdir build && cd build
cmake .. -G "Unix Makefiles"
cmake --build .
ctest --verbose
```

---

## ⚡ Quick Start Commands

### After Installing a Compiler

**PowerShell:**
```powershell
cd "C:\My_Projects\Training\Brake_and_Wheel_Speed_Monitoring_System"
.\SETUP.ps1
```

**Command Prompt:**
```bash
cd C:\My_Projects\Training\Brake_and_Wheel_Speed_Monitoring_System
SETUP.bat
```

**Manual (if scripts don't work):**
```powershell
cd "C:\My_Projects\Training\Brake_and_Wheel_Speed_Monitoring_System"
Remove-Item -Recurse -Force build -ErrorAction SilentlyContinue
mkdir build
cd build
cmake ..
cmake --build . --config Release
ctest --verbose
```

---

## 📋 Expected Output

When everything works correctly, you'll see:

```
[==========] 51 tests from 4 test suites ran
[  PASSED  ] 51 tests
[==========] X ms total
```

---

## ❌ Troubleshooting

### "CMake Error: CMAKE_CXX_COMPILER not set"
**Solution**: Install a C++ compiler (see methods above)

### "cmake: command not found"
**Solution**: Install CMake from https://cmake.org/download/

### "'SETUP.ps1' cannot be loaded because running scripts is disabled"
**Solution**: Run this once:
```powershell
Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser
```

Then try again:
```powershell
.\SETUP.ps1
```

### Build succeeds but tests fail
**Solution**: This shouldn't happen - all code is validated. Try:
```powershell
cd build
Remove-Item -Recurse -Force *
cd ..
# Re-run from step above
```

---

## 📚 Documentation

After successful setup, read:

1. **[README.md](README.md)** - Project overview
2. **[STATUS_DASHBOARD.md](STATUS_DASHBOARD.md)** - Visual status summary
3. **[UNIT_TEST_REPORT.md](UNIT_TEST_REPORT.md)** - Detailed test specs
4. **[CODE_QUALITY_REPORT.md](CODE_QUALITY_REPORT.md)** - Code analysis

---

## ✅ After Setup is Complete

### Build Again (After Making Changes)
```powershell
cd build
cmake --build . --config Release
ctest --verbose
cd ..
```

### Run Specific Tests
```powershell
cd build
# Run all data validation tests
./unit_tests --gtest_filter="ValidationTest*"

# Run specific test
./unit_tests --gtest_filter="*TC_BRK_001*"

# Generate XML report
./unit_tests --gtest_output="xml:results.xml"
```

### Clean Everything
```powershell
Remove-Item -Recurse -Force build
```

---

## 🎯 Summary

| Step | Action | Time |
|------|--------|------|
| 1 | Download Visual Studio Community 2022 | 5 min |
| 2 | Install with C++ workload | 10-15 min |
| 3 | Restart PowerShell | 1 min |
| 4 | Run `.\SETUP.ps1` | 2-5 min |
| ✅ | **DONE - All tests passing** | ~30 min total |

---

## 💡 Tips

- **First time setup takes longer** due to downloading GoogleTest
- **Subsequent builds are faster** (only changed files rebuild)
- **Use Visual Studio 2022 Community** - it's free and most reliable
- **Keep all PowerShell windows closed** before running setup
- **Don't use WSL** unless you're already familiar with it

---

**Ready?** Choose Method 1 above and start! 🚀
