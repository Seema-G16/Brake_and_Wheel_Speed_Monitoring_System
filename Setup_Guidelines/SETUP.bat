@echo off
REM Brake & Wheel Speed Monitoring System - Simple Setup for Windows

setlocal enabledelayedexpansion

echo.
echo ╔════════════════════════════════════════════════════════════════════════════╗
echo ║                                                                            ║
echo ║    BRAKE ^& WHEEL SPEED MONITORING SYSTEM - WINDOWS SETUP                  ║
echo ║                                                                            ║
echo ╚════════════════════════════════════════════════════════════════════════════╝
echo.

REM Check CMake
echo [1/4] Checking CMake...
cmake --version >nul 2>&1
if errorlevel 1 (
    echo ERROR: CMake not found!
    echo Install from: https://cmake.org/download/
    exit /b 1
)
echo ✓ CMake found

REM Check for compiler
echo [2/4] Checking C++ Compiler...
where cl.exe >nul 2>&1
if errorlevel 1 (
    echo.
    echo WARNING: No C++ compiler detected!
    echo.
    echo Please install one of these:
    echo.
    echo Option 1: Visual Studio Community 2022 (RECOMMENDED)
    echo   - Download: https://visualstudio.microsoft.com/vs/community/
    echo   - Run installer
    echo   - Select "Desktop development with C++"
    echo   - After install, restart this terminal
    echo.
    echo Option 2: MinGW
    echo   - Download: https://www.mingw-w64.org/download/
    echo   - Add to PATH and restart terminal
    echo.
    echo Option 3: Use WSL2 (Windows Subsystem for Linux)
    echo   - Run: wsl
    echo   - Then: cd /mnt/c/My_Projects/Training/Brake_and_Wheel_Speed_Monitoring_System
    echo.
    exit /b 1
)
echo ✓ C++ Compiler found

REM Clean build directory
echo [3/4] Cleaning and configuring build...
if exist build (
    rmdir /s /q build >nul 2>&1
    timeout /t 1 /nobreak >nul
)
mkdir build
cd build

REM Configure
cmake ..
if errorlevel 1 (
    echo ERROR: CMake configuration failed!
    exit /b 1
)
echo ✓ CMake configuration successful

REM Build
echo [4/4] Building and testing...
cmake --build . --config Release
if errorlevel 1 (
    echo ERROR: Build failed!
    exit /b 1
)

REM Run tests
ctest --verbose
if errorlevel 1 (
    echo ERROR: Tests failed!
    exit /b 1
)

echo.
echo ╔════════════════════════════════════════════════════════════════════════════╗
echo ║                  ✓ SETUP COMPLETE - ALL TESTS PASSED                      ║
echo ╚════════════════════════════════════════════════════════════════════════════╝
echo.

cd ..
