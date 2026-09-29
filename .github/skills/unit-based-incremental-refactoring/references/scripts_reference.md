# Scripts Reference - Detailed Usage Guide

## Overview

All scripts are located in `scripts/` folder.

| Script | Language | Purpose |
|--------|----------|---------|
| `plan_refactoring.py` | Python 3 | Analyze component, create refactoring plan |
| `log_unit_completion.py` | Python 3 | Record unit completion metrics |
| `generate_refactoring_report.py` | Python 3 | Generate HTML/JSON report |
| `validate_unit_tests.ps1` | PowerShell | Automated validation (Windows) |

---

## 1. plan_refactoring.py

### Purpose
Analyze a C++ component and create a structured refactoring plan.

### Usage
```bash
python3 scripts/plan_refactoring.py \
    --component "BrakeMonitor" \
    --input-file src/brake_monitor.cpp \
    --test-file tests/unit/brake_monitor_test.cpp \
    --output refactoring_plan.json
```

### Parameters
- `--component`: Component name (e.g., "BrakeMonitor")
- `--input-file`: Source file to analyze
- `--test-file`: Associated test file
- `--output`: JSON output file (default: refactoring_plan.json)

### Output
JSON file containing:
```json
{
  "component": "BrakeMonitor",
  "total_loc": 100,
  "functions": [
    {
      "id": 1,
      "name": "is_braking",
      "loc": 2,
      "complexity": 2,
      "estimated_time_minutes": 15
    }
  ],
  "total_estimated_time_minutes": 180
}
```

### Example
```bash
# Analyze BrakeMonitor
python3 scripts/plan_refactoring.py \
    --component "BrakeMonitor" \
    --input-file src/brake_monitor.cpp \
    --test-file tests/unit/brake_monitor_test.cpp \
    --output brake_monitor_plan.json

# Output: brake_monitor_plan.json with complete analysis
```

---

## 2. log_unit_completion.py

### Purpose
Record unit completion metrics in structured format.

### Usage
```bash
python3 scripts/log_unit_completion.py \
    --unit-id 1 \
    --function-name "is_braking" \
    --status "PASS" \
    --time-minutes 15 \
    --complexity-before 2 \
    --complexity-after 1 \
    --tests-passed 3 \
    --tests-total 3 \
    --full-suite-passed 52 \
    --full-suite-total 52 \
    --output refactoring_log.json
```

### Parameters
- `--unit-id`: Unit number (1, 2, 3, etc.)
- `--function-name`: Function being refactored
- `--status`: PASS or FAIL
- `--time-minutes`: Actual time spent
- `--complexity-before`: Cyclomatic complexity before
- `--complexity-after`: Cyclomatic complexity after
- `--tests-passed`: Number of covering tests passed
- `--tests-total`: Total covering tests
- `--full-suite-passed`: Full suite tests passed
- `--full-suite-total`: Total full suite tests
- `--output`: Log file (default: refactoring_log.json)

### Output
JSON log entry:
```json
{
  "unit_id": 1,
  "function": "is_braking",
  "timestamp": "2024-09-29T10:30:00",
  "status": "PASS",
  "metrics": {
    "time_minutes": 15,
    "complexity": {"before": 2, "after": 1},
    "tests": {"passed": 3, "total": 3},
    "full_suite": {"passed": 52, "total": 52}
  }
}
```

### Example
```bash
# After refactoring unit 1
python3 scripts/log_unit_completion.py \
    --unit-id 1 \
    --function-name "is_braking" \
    --status "PASS" \
    --time-minutes 18 \
    --complexity-before 2 \
    --complexity-after 1 \
    --tests-passed 3 \
    --tests-total 3 \
    --full-suite-passed 52 \
    --full-suite-total 52

# Log updated: refactoring_log.json
```

---

## 3. generate_refactoring_report.py

### Purpose
Generate comprehensive HTML/JSON report from refactoring log.

### Usage
```bash
python3 scripts/generate_refactoring_report.py \
    --component "BrakeMonitor" \
    --log-file refactoring_log.json \
    --output-format html \
    --output refactoring_report.html
```

### Parameters
- `--component`: Component name
- `--log-file`: Refactoring log JSON file (from log_unit_completion.py)
- `--output-format`: html or json (default: html)
- `--output`: Output file name (default: refactoring_report.html)

### Output

**HTML Report** includes:
- Executive summary
- Metrics before/after
- Unit-by-unit timeline
- Effort breakdown
- Risk assessment
- Recommendations
- Appendix with detailed metrics

**JSON Report** includes:
```json
{
  "component": "BrakeMonitor",
  "summary": {
    "total_units": 5,
    "total_time_minutes": 185,
    "complexity_before": 12,
    "complexity_after": 6,
    "all_tests_passing": true,
    "regressions": 0
  },
  "units": [...]
}
```

### Example
```bash
# Generate HTML report
python3 scripts/generate_refactoring_report.py \
    --component "BrakeMonitor" \
    --log-file refactoring_log.json \
    --output-format html \
    --output brake_monitor_report.html

# Output: brake_monitor_report.html (professional report)
```

---

## 4. validate_unit_tests.ps1

### Purpose
Automated validation of unit tests (Windows PowerShell only).

### Usage
```powershell
.\scripts\validate_unit_tests.ps1 -TestPattern "TC_BRK" -Verbose
```

### Parameters
- `-TestPattern`: CTest filter pattern (optional)
- `-Verbose`: Show detailed output
- `-BuildDir`: Build directory (default: ./build)

### Validation Steps
1. **Compile**: `cmake --build .`
2. **Covering Tests**: `ctest -R "pattern"`
3. **Full Suite**: `ctest --verbose`
4. **Report**: Pass/fail summary

### Output
```
Building project...
✓ Build successful

Running covering tests...
✓ 3/3 tests passed

Running full suite...
✓ 52/52 tests passed

Validation: PASS ✓
```

### Example
```powershell
# Validate BrakeMonitor unit tests
.\scripts\validate_unit_tests.ps1 -TestPattern "TC_BRK" -Verbose

# Output: Validation summary with all checks
```

---

## Workflow Integration

### Session 1: Plan
```bash
python3 scripts/plan_refactoring.py \
    --component "BrakeMonitor" \
    --input-file src/brake_monitor.cpp \
    --test-file tests/unit/brake_monitor_test.cpp
```

### Session 2-N: Execute & Log
```bash
# For each unit:
cmake --build .
ctest -R "pattern"
ctest --verbose

python3 scripts/log_unit_completion.py \
    --unit-id 1 \
    --function-name "is_braking" \
    --status "PASS" \
    --time-minutes 15 \
    --complexity-before 2 \
    --complexity-after 1 \
    --tests-passed 3 \
    --tests-total 3 \
    --full-suite-passed 52 \
    --full-suite-total 52
```

### Final: Report
```bash
python3 scripts/generate_refactoring_report.py \
    --component "BrakeMonitor" \
    --log-file refactoring_log.json \
    --output-format html
```

---

## Troubleshooting

### Script not found
```bash
# Make sure you're in project root
cd /path/to/project
python3 .github/skills/unit-based-incremental-refactoring/scripts/plan_refactoring.py
```

### Python not found
```bash
# Try python3 explicitly
python3 --version  # Should be 3.7+

# Or use full path
/usr/bin/python3 scripts/plan_refactoring.py
```

### Permission denied (PowerShell)
```powershell
# On Windows, may need to allow script execution
Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser
```

### JSON parsing error
```bash
# Verify JSON file format
python3 -m json.tool refactoring_log.json

# If error, manually check file format
```

