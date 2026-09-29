# validate_unit_tests.ps1 - Automated unit test validation
# Validates unit refactoring meets quality standards
# Usage: .\validate_unit_tests.ps1 -UnitId 1 -ComponentPath "src/brake_monitor.cpp"

param(
    [int]$UnitId = 0,
    [string]$ComponentPath = "",
    [switch]$FullValidation
)

function Write-Result {
    param([string]$Message, [bool]$Success)
    $Icon = if ($Success) { "✓" } else { "✗" }
    $Color = if ($Success) { "Green" } else { "Red" }
    Write-Host "$Icon $Message" -ForegroundColor $Color
}

function Validate-Compilation {
    Write-Host "`n=== VALIDATION 1: Compilation ===" -ForegroundColor Cyan
    
    Push-Location build
    $output = cmake --build . 2>&1
    $success = $LASTEXITCODE -eq 0
    Pop-Location
    
    Write-Result "Build succeeds without errors" $success
    if (-not $success) {
        Write-Host "Build output:`n$output" -ForegroundColor Yellow
    }
    return $success
}

function Validate-CoveringTests {
    param([string[]]$TestPatterns)
    
    Write-Host "`n=== VALIDATION 2: Covering Tests ===" -ForegroundColor Cyan
    
    Push-Location build
    
    if ($TestPatterns.Count -eq 0) {
        Write-Host "No test patterns provided" -ForegroundColor Yellow
        Pop-Location
        return $true
    }
    
    $filterExpression = $TestPatterns -join "|"
    $output = ctest -R $filterExpression --verbose 2>&1
    $success = $LASTEXITCODE -eq 0
    
    Pop-Location
    
    Write-Result "All covering tests pass ($($TestPatterns.Count) tests)" $success
    if (-not $success) {
        Write-Host "Test output:`n$output" -ForegroundColor Yellow
    }
    return $success
}

function Validate-FullSuite {
    Write-Host "`n=== VALIDATION 3: Full Test Suite ===" -ForegroundColor Cyan
    
    Push-Location build
    $output = ctest --verbose 2>&1
    $success = $LASTEXITCODE -eq 0
    
    # Count passed tests
    $passedCount = ([regex]::Matches($output, "PASS")).Count
    Pop-Location
    
    Write-Result "Full test suite passes (regression check)" $success
    if ($passedCount -gt 0) {
        Write-Host "  Tests passed: $passedCount" -ForegroundColor Cyan
    }
    return $success
}

function Validate-InterfaceUnchanged {
    param([string]$ComponentPath, [string]$FunctionName)
    
    Write-Host "`n=== VALIDATION 4: Interface Unchanged ===" -ForegroundColor Cyan
    
    if (-not (Test-Path $ComponentPath)) {
        Write-Result "Component file found" $false
        return $false
    }
    
    $content = Get-Content $ComponentPath -Raw
    $success = $content -match [regex]::Escape($FunctionName)
    
    Write-Result "Function interface preserved" $success
    return $success
}

# Main validation flow
function Main {
    Write-Host "`n╔════════════════════════════════════════════════════════════════╗" -ForegroundColor Cyan
    Write-Host "║           UNIT REFACTORING VALIDATION                           ║" -ForegroundColor Cyan
    Write-Host "╚════════════════════════════════════════════════════════════════╝`n" -ForegroundColor Cyan
    
    if ($UnitId -gt 0) {
        Write-Host "Validating Unit $UnitId..." -ForegroundColor Cyan
    }
    
    $results = @()
    
    # Validation 1: Compilation
    $results += Validate-Compilation
    
    # Validation 2: Covering tests (if specified)
    if ($FullValidation) {
        $results += Validate-CoveringTests @()
    }
    
    # Validation 3: Full suite
    $results += Validate-FullSuite
    
    # Validation 4: Interface unchanged
    if ($ComponentPath) {
        $results += Validate-InterfaceUnchanged -ComponentPath $ComponentPath -FunctionName "is_braking"
    }
    
    # Summary
    Write-Host "`n=== VALIDATION SUMMARY ===" -ForegroundColor Cyan
    $passCount = @($results | Where-Object { $_ -eq $true }).Count
    $totalCount = $results.Count
    
    if ($passCount -eq $totalCount) {
        Write-Host "✓ All validations passed ($passCount/$totalCount)" -ForegroundColor Green
        exit 0
    } else {
        Write-Host "✗ Some validations failed ($passCount/$totalCount)" -ForegroundColor Red
        exit 1
    }
}

Main
