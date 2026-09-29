---
name: ut-test-generation
description: "Use when: designing and generating GoogleTest unit tests; creating test-case CSV specs; validating/fixing testspec.csv; running complete UT test workflows from requirements. Includes automated CSV validation and self-healing via auto-fix script. Scope: tests/unit/ and Docs/ut-test-design/testspec.csv."
argument-hint: "Design UT tests for [requirements]; generate test-case CSV; validate and auto-fix CSV; run tests and report coverage"
user-invocable: true
---

# Unit Test Generation Skill — Brake & Wheel-Speed Monitoring System

Complete end-to-end workflow for designing, generating, validating, and running GoogleTest-based unit tests with integrated CSV validation and auto-fixing.

## When to Use

- **Design test cases**: Create test-case specifications for brake, wheel-speed, fault manager, and data validation requirements.
- **Generate test-case CSV**: Produce a requirements-traceable testspec.csv with all test metadata.
- **Validate CSV format**: Ensure CSV structure and requirement coverage meet project standards.
- **Auto-fix CSV**: Automatically correct common CSV issues (malformed IDs, missing fields, invalid types).
- **Generate UT tests**: Implement GoogleTest code based on approved test-case specs.
- **Run and report**: Build, execute `ctest`, and report coverage with requirement-to-test traceability table.

## Quick Start

1. **Design tests from requirements** (optional if you already have test cases):
   - Read `Docs/requirements.md` (BRK-01–05, WHL-01–08, FLT-01–04, DAT-01–04).
   - Identify every requirement, boundary condition, and error case.
   - Use the todo list to track progress by requirement group.

2. **Generate test-case CSV**:
   - Create/update `Docs/ut-test-design/testspec.csv`.
   - Use the [CSV Schema Reference](./references/csv-schema.md) for field definitions.
   - Ensure every requirement has at least one covering test case.

3. **Validate and fix CSV**:
   - Run [validate-csv.py](./scripts/validate-csv.py) to check format and coverage.
   - If validation fails, run [auto-fix-csv.py](./scripts/auto-fix-csv.py) to auto-correct issues.
   - Repeat until validation passes (max 3 attempts).

4. **Generate GoogleTest code**:
   - Based on the approved testspec.csv, write/extend GoogleTest `.cpp` files under `tests/unit/`.
   - One test function per row in testspec.csv (naming: `TC_XXX_NNN`).
   - Use in-memory `BrakeWheelSpeedSample` test data; never depend on CSV reader in unit tests.

5. **Build and run**:
   - Run CMake configure/build and `ctest` to verify all tests compile and pass.
   - Report results and generate requirement-to-test traceability summary.

## Step-by-Step Procedure

### Step 1: Review Requirements & Plan Tests

1. Read `Docs/requirements.md` in full.
2. Identify all requirements and their boundaries:
   - **BRK-01 to BRK-05**: Brake pressure, braking condition, stationary vehicle, valid range.
   - **WHL-01 to WHL-08**: Wheel-speed comparison, mismatch threshold (10 km/h), braking spread (15 km/h).
   - **FLT-01 to FLT-04**: Multiple faults, aggregation, recovery, persistence.
   - **DAT-01 to DAT-04**: Input validation, invalid-data handling, error reporting.

3. Use the [Requirement Traceability Reference](./references/requirement-traceability.md) to understand which test cases map to each requirement.

4. Plan boundaries:
   - Speed: 0 km/h, 0.1 km/h, 10 km/h, 10.01 km/h, 15 km/h, 15.01 km/h, 100 km/h+
   - Pressure: 0 bar, 0.1 bar, 50 bar, 100 bar, 100.1 bar (negative, out-of-range)
   - Spread: 0 km/h, 5 km/h, 15 km/h, 15.01 km/h, 30 km/h (multi-fault scenarios)

5. Use the todo list tool to track requirement groups:
   ```
   - [ ] DAT-01 to DAT-04: Data Validation
   - [ ] BRK-01 to BRK-05: Brake Monitoring
   - [ ] WHL-01 to WHL-08: Wheel-Speed Monitoring
   - [ ] FLT-01 to FLT-04: Fault Manager
   ```

### Step 2: Generate Test-Case CSV

1. Create or open `Docs/ut-test-design/testspec.csv`.

2. For each requirement (or each boundary/scenario), add a row with:
   - **Testcase ID**: `TC-{TYPE}-{NNN}` (e.g., `TC-BRK-001`, `TC-WHL-007`)
   - **Testcase Name**: Clear, concise description (e.g., "Brake pressure fault at zero speed boundary")
   - **Test Description**: Detailed explanation (e.g., "Verify that BRAKE_PRESSURE_FAULT is not reported when vehicle speed is 0 km/h, even if brake pressure is 0 bar (BRK-04).")
   - **Input**: All 8 signals (Timestamp, VehicleSpeed, BrakePedal, BrakePressure, FL, FR, RL, RR)
   - **Test Case Type**: Positive, Negative, Edge, Boundary, or Recovery
   - **Expected Output**: Fault IDs or status (e.g., `Faults=NO_FAULT`, `Faults=BRAKE_PRESSURE_FAULT`)
   - **Actual Output**: Leave as "Pending"
   - **Automated (Yes/No)**: Yes (for GoogleTest-based tests)
   - **Comments**: Cross-reference requirements (e.g., "Covers BRK-01, BRK-04")

3. See [CSV Schema Reference](./references/csv-schema.md) for field rules and examples.

### Step 3: Validate CSV Format & Coverage

1. Run the validation script:
   ```bash
   python3 .github/skills/ut-test-generation/scripts/validate-csv.py Docs/ut-test-design/testspec.csv
   ```

2. Check for:
   - ✅ All required columns present
   - ✅ Valid Testcase ID format (TC-XXX-NNN)
   - ✅ No empty required fields
   - ✅ Valid Test Case Type values
   - ✅ All requirements covered (BRK-01–05, WHL-01–08, FLT-01–04, DAT-01–04)

3. If validation fails, note the errors.

### Step 4: Auto-Fix CSV Issues

If validation reported errors, run the auto-fix script:

```bash
python3 .github/skills/ut-test-generation/scripts/auto-fix-csv.py Docs/ut-test-design/testspec.csv 3
```

The script will attempt to fix common issues automatically:
- Pad Testcase IDs with zeros (TC-BRK-1 → TC-BRK-001)
- Populate missing required fields with sensible defaults
- Normalize Test Case Type and Automated values
- Run up to 3 fix iterations

After each fix, re-run validation to confirm the issues are resolved.

**Note**: The auto-fix script does NOT create missing test case rows — it only fixes fields within existing rows. If coverage is still incomplete after auto-fixing, you must add more test case rows manually.

### Step 5: Implement GoogleTest Code

Once CSV validation passes:

1. For each test case row in testspec.csv, implement a corresponding GoogleTest test function.

2. Test naming convention:
   - Use the Testcase ID as the basis: `TC_BRK_001` (replace hyphens with underscores)
   - Or use a more descriptive name: `BrakeMonitorTest.BrakePressureExactly100BarIsValid`

3. Test structure:
   ```cpp
   #include <gtest/gtest.h>
   #include "brake/brake_monitor.hpp"
   #include "faults/fault_types.hpp"
   #include "monitoring_system/brake_wheel_speed_sample.hpp"

   TEST(BrakeMonitorTest, TC_BRK_001_BrakingConditionActiveWithNormalPressure)
   {
       BrakeWheelSpeedSample sample{};
       sample.timestampSeconds = 1.0;
       sample.vehicleSpeedKph = 50.0;
       sample.brakePedalPressed = true;
       sample.brakePressureBar = 40.0;
       sample.frontLeftWheelSpeedKph = 50.0;
       sample.frontRightWheelSpeedKph = 50.0;
       sample.rearLeftWheelSpeedKph = 50.0;
       sample.rearRightWheelSpeedKph = 50.0;

       BrakeMonitor monitor;
       const auto faults = monitor.evaluate(sample);

       EXPECT_FALSE(faults.contains(FaultType::BrakePressureFault));
   }
   ```

4. Key principles:
   - One in-memory `BrakeWheelSpeedSample` per test (no CSV file reading in unit tests).
   - Deterministic, order-independent tests.
   - Test both sides of every boundary (speed = 9.99 km/h AND 10.01 km/h for a 10 km/h threshold).
   - Fault names must exactly match those in `Docs/requirements.md` (e.g., `BRAKE_PRESSURE_FAULT`, `RL_WHEEL_SPEED_MISMATCH`).
   - Multi-fault scenarios: run 2+ sequential samples and verify faults change correctly.

5. Save test files under `tests/unit/` (e.g., `brake_monitor_test.cpp`, `wheel_speed_monitor_test.cpp`).

### Step 6: Build and Run Tests

1. Configure and build:
   ```bash
   cd /path/to/Brake_and_Wheel_Speed_Monitoring_System
   mkdir -p build && cd build
   cmake ..
   cmake --build .
   ```

2. Run the test suite:
   ```bash
   ctest --verbose
   ```

3. Verify all tests pass and review the output for any failures.

### Step 7: Report Results & Traceability

Generate a summary report:

1. **Tests Added**: List all new/modified test files and the test functions added.
2. **Build & Test Results**: Confirm CMake build succeeded and `ctest` passed (all tests green).
3. **Requirement-to-Test Traceability Table**:
   | Requirement | Test Case ID(s) | Status |
   |-------------|-----------------|--------|
   | BRK-01      | TC-BRK-001, TC-BRK-002, TC-BRK-004 | ✅ Covered |
   | BRK-02      | TC-BRK-001, TC-BRK-005 | ✅ Covered |
   | ... | ... | ... |

4. **Uncovered Requirements** (if any): List any requirements without a corresponding test.
5. **Production Code Issues** (if any): Report suspected bugs in src/include without fixing them.

## Reference Files

- [CSV Schema Reference](./references/csv-schema.md) — Field definitions, validation rules, auto-fix behavior
- [Requirement Traceability Reference](./references/requirement-traceability.md) — Mapping of requirements to test cases
- [validate-csv.py](./scripts/validate-csv.py) — CSV format and coverage validation script
- [auto-fix-csv.py](./scripts/auto-fix-csv.py) — Automated CSV error correction script

## Constraints & Best Practices

- ✅ **DO**: Test boundary conditions on both sides (e.g., speed = 9.99 and 10.01 km/h).
- ✅ **DO**: Name tests after the requirement and condition (e.g., `BrakePressureExactly100BarIsValid`).
- ✅ **DO**: Map every test to a requirement ID in the Comments column.
- ✅ **DO**: Use in-memory `BrakeWheelSpeedSample` test data (no file I/O in unit tests).
- ❌ **DON'T**: Edit `src/` or `include/` — this skill is test-only. Report production bugs instead.
- ❌ **DON'T**: Mix invalid-data cases with valid faults — invalid input must short-circuit normal evaluation.
- ❌ **DON'T**: Depend on test execution order or hardcoded state.

## Output Checklist

Before considering this skill complete, verify:

- ✅ `Docs/ut-test-design/testspec.csv` contains all test cases and passes validation.
- ✅ All GoogleTest `.cpp` files are under `tests/unit/` and compile without errors.
- ✅ `ctest --verbose` runs and all tests pass (no failures).
- ✅ Requirement-to-test traceability table shows all requirements covered.
- ✅ No uncovered requirements (all BRK-01–05, WHL-01–08, FLT-01–04, DAT-01–04 have tests).
- ✅ Production code issues (if any) are reported but not fixed.
