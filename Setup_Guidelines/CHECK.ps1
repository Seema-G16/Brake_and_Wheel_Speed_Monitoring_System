#============================================================================
# CHECK PREREQUISITES - Verify if system is ready for build
#============================================================================

Write-Host ""
Write-Host "======================================================================"
Write-Host "SYSTEM READINESS CHECK"
Write-Host "======================================================================"
Write-Host ""

$allGood = $true

# Check CMake
Write-Host "[1] CMake Installation" -ForegroundColor Yellow
try {
    $cmakeVersion = cmake --version | Select-Object -First 1
    Write-Host "  OK: $cmakeVersion" -ForegroundColor Green
} catch {
    Write-Host "  ERROR: CMake NOT FOUND" -ForegroundColor Red
    Write-Host "  Install from: https://cmake.org/download/" -ForegroundColor Cyan
    $allGood = $false
}

# Check C++ Compiler
Write-Host ""
Write-Host "[2] C++ Compiler" -ForegroundColor Yellow
$hasCompiler = $false

Write-Host "  Checking Visual Studio..." -ForegroundColor Gray
try {
    $output = cl.exe 2>&1 | Select-Object -First 1
    Write-Host "  OK: Microsoft Visual C++ Compiler found" -ForegroundColor Green
    $hasCompiler = $true
} catch {
    Write-Host "  - MSVC not found" -ForegroundColor Gray
}

if (-not $hasCompiler) {
    Write-Host "  Checking GCC..." -ForegroundColor Gray
    try {
        $gccVersion = gcc --version | Select-Object -First 1
        Write-Host "  OK: $gccVersion" -ForegroundColor Green
        $hasCompiler = $true
    } catch {
        Write-Host "  - GCC not found" -ForegroundColor Gray
    }
}

if (-not $hasCompiler) {
    Write-Host "  Checking Clang..." -ForegroundColor Gray
    try {
        $clangVersion = clang --version | Select-Object -First 1
        Write-Host "  OK: $clangVersion" -ForegroundColor Green
        $hasCompiler = $true
    } catch {
        Write-Host "  - Clang not found" -ForegroundColor Gray
    }
}

if (-not $hasCompiler) {
    Write-Host "  ERROR: NO C++ COMPILER FOUND" -ForegroundColor Red
    Write-Host "    Option 1: Visual Studio Community 2022 (Recommended)" -ForegroundColor Cyan
    Write-Host "              https://visualstudio.microsoft.com/vs/community/" -ForegroundColor Cyan
    Write-Host "    Option 2: MinGW - https://www.mingw-w64.org/" -ForegroundColor Cyan
    Write-Host "    Option 3: WSL2 with Linux tools" -ForegroundColor Cyan
    $allGood = $false
}

# Check Git
Write-Host ""
Write-Host "[3] Git (optional - for downloading dependencies)" -ForegroundColor Yellow
try {
    $gitVersion = git --version
    Write-Host "  OK: $gitVersion" -ForegroundColor Green
} catch {
    Write-Host "  WARNING: Git not found (optional)" -ForegroundColor Yellow
    Write-Host "  Install from: https://git-scm.com/" -ForegroundColor Gray
}

# Check project files
Write-Host ""
Write-Host "[4] Project Files" -ForegroundColor Yellow
$requiredFiles = @(
    "include\brake\brake_monitor.hpp",
    "include\faults\fault_types.hpp",
    "src\brake_monitor.cpp",
    "src\fault_types.cpp",
    "tests\unit\validation_test.cpp",
    "CMakeLists.txt"
)

$allFilesExist = $true
foreach ($file in $requiredFiles) {
    $path = Join-Path (Get-Location) $file
    if (Test-Path $path) {
        Write-Host "  OK: $file" -ForegroundColor Green
    } else {
        Write-Host "  ERROR: $file NOT FOUND" -ForegroundColor Red
        $allFilesExist = $false
        $allGood = $false
    }
}

# Summary
Write-Host ""
Write-Host "======================================================================"
Write-Host ""
Write-Host "SUMMARY:" -ForegroundColor Yellow
Write-Host ""

if ($allGood) {
    Write-Host "SUCCESS: All prerequisites installed!" -ForegroundColor Green
    Write-Host "You are ready to build the project." -ForegroundColor Green
    Write-Host ""
    Write-Host "Next steps:" -ForegroundColor Green
    Write-Host "  Run: .\SETUP.ps1" -ForegroundColor Green
    Write-Host "  Or:  SETUP.bat" -ForegroundColor Green
    Write-Host ""
} else {
    Write-Host "WARNING: Some prerequisites are missing." -ForegroundColor Red
    Write-Host "Please see above for instructions." -ForegroundColor Red
    Write-Host ""
    Write-Host "Most likely: Install Visual Studio 2022 Community" -ForegroundColor Yellow
    Write-Host "https://visualstudio.microsoft.com/vs/community/" -ForegroundColor Cyan
    Write-Host ""
}

Write-Host "======================================================================"
Write-Host ""
