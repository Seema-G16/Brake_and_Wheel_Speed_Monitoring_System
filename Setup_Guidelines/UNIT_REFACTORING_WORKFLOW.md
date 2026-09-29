# Unit-Based Refactoring - Practical Execution Guide

## 🎯 Real-World Workflow: Refactoring One Unit at a Time

This guide shows exactly how to refactor your code in incremental, validatable units with commands and checkpoints.

---

## 📋 The 5-Step Unit Refactoring Process

```
START: Component selected for refactoring
  │
  ├─ Step 1: SELECT ONE FUNCTION
  │   └─ Pick smallest/simplest first
  │
  ├─ Step 2: UNDERSTAND COVERAGE
  │   └─ Identify 3-5 test cases covering this function
  │
  ├─ Step 3: REFACTOR SAFELY
  │   └─ Make one logical change
  │   └─ Keep interface unchanged
  │
  ├─ Step 4: VALIDATE IMMEDIATELY
  │   └─ Compile (must succeed)
  │   └─ Run covering tests (must pass)
  │   └─ Run full suite (regression check)
  │
  └─ Step 5: RECORD & MOVE NEXT
      └─ Document metrics
      └─ Pick next function
      └─ Repeat
```

---

## 🔍 Example 1: Refactoring is_braking() Function

### Background
```
Component: BrakeMonitor
File: src/brake_monitor.cpp
Function: is_braking()
Current Code:  return sample.brakePedalPressed && sample.vehicleSpeedKph > 0.0;
Size: 1 line
Tests: TC-BRK-001, TC-BRK-003, TC-BRK-012 (3 test cases)
```

### Step 1: Understand Current Coverage

**Look at the test cases**:
```cpp
// tests/unit/brake_monitor_test.cpp

TEST(BrakeMonitorTest, TC_BRK_001_NormalBrakingCondition) {
    BrakeWheelSpeedSample sample{...};
    sample.brakePedalPressed = true;
    sample.vehicleSpeedKph = 50.0;
    ASSERT_TRUE(brake_monitor.is_braking(sample));
}

TEST(BrakeMonitorTest, TC_BRK_003_PedalReleasedWhileDriving) {
    BrakeWheelSpeedSample sample{...};
    sample.brakePedalPressed = false;
    sample.vehicleSpeedKph = 50.0;
    ASSERT_FALSE(brake_monitor.is_braking(sample));
}

TEST(BrakeMonitorTest, TC_BRK_012_StationaryVehicleNoBrakingDetection) {
    BrakeWheelSpeedSample sample{...};
    sample.brakePedalPressed = true;
    sample.vehicleSpeedKph = 0.0;
    ASSERT_FALSE(brake_monitor.is_braking(sample));
}
```

**Understanding**: These 3 tests cover all code paths:
- Path 1: Both conditions TRUE → returns TRUE
- Path 2: Pedal FALSE → returns FALSE
- Path 3: Speed 0 → returns FALSE

### Step 2: Current Metrics (Baseline)

```bash
# Terminal: Run once to establish baseline
$ cd C:\My_Projects\Training\Brake_and_Wheel_Speed_Monitoring_System\build

# Build current code
$ cmake --build .
-- Build files have been written to: ...build
[1/6] Building CXX object CMakeFiles/bwsms_lib.dir/src/brake_monitor.cpp.obj
[2/6] Linking CXX static library bwsms_lib.lib
[3/6] Building CXX object CMakeFiles/unit_tests.dir/tests/unit/brake_monitor_test.cpp.obj
[4/6] Linking CXX executable unit_tests.exe
[5/6] Built target unit_tests
[6/6] Built target all

Build succeeded ✓

# Record baseline metrics:
# Complexity: 2 logical operators (&&, >)
# Lines: 1 line
# Execution: Immediate (true/false)
```

### Step 3: Refactor (Make ONE Logical Change)

**Edit file**: `src/brake_monitor.cpp`

```cpp
// BEFORE (Original):
bool BrakeMonitor::is_braking(const BrakeWheelSpeedSample& sample) const {
    return sample.brakePedalPressed && sample.vehicleSpeedKph > 0.0;
}

// AFTER (Refactored - Extract constant):
namespace {
    constexpr double MIN_SPEED_KPH = 0.1;  // ← Add explicit constant
}

bool BrakeMonitor::is_braking(const BrakeWheelSpeedSample& sample) const {
    // IMPROVEMENT: Explicit constant makes requirement clear
    return sample.brakePedalPressed && sample.vehicleSpeedKph > MIN_SPEED_KPH;
}
```

### Step 4a: Compile & Check Errors

```bash
# Terminal: Compile the refactored code
$ cd C:\My_Projects\Training\Brake_and_Wheel_Speed_Monitoring_System\build

$ cmake --build .
[1/6] Building CXX object CMakeFiles/bwsms_lib.dir/src/brake_monitor.cpp.obj
[2/6] Linking CXX static library bwsms_lib.lib
[3/6] Building CXX object CMakeFiles/unit_tests.dir/tests/unit/brake_monitor_test.cpp.obj
[4/6] Linking CXX executable unit_tests.exe
[5/6] Built target unit_tests
[6/6] Built target all

Build succeeded ✓
No errors or warnings

CHECKPOINT 1 PASSED: Compilation successful
```

### Step 4b: Run Covering Tests ONLY

```bash
# Terminal: Run ONLY the tests for is_braking()
$ cd C:\My_Projects\Training\Brake_and_Wheel_Speed_Monitoring_System\build

# Run specific test pattern (GoogleTest filter)
$ .\unit_tests.exe --gtest_filter="*TC_BRK_001*:*TC_BRK_003*:*TC_BRK_012*"

Running main() from gmock_main.cc
[==========] Running 3 tests from 1 test suite.
[----------] Global test environment set-up.
[----------] 3 tests from BrakeMonitorTest
[ RUN      ] BrakeMonitorTest.TC_BRK_001_NormalBrakingCondition
[       OK ] BrakeMonitorTest.TC_BRK_001_NormalBrakingCondition (0 ms)
[ RUN      ] BrakeMonitorTest.TC_BRK_003_PedalReleasedWhileDriving
[       OK ] BrakeMonitorTest.TC_BRK_003_PedalReleasedWhileDriving (0 ms)
[ RUN      ] BrakeMonitorTest.TC_BRK_012_StationaryVehicleNoBrakingDetection
[       OK ] BrakeMonitorTest.TC_BRK_012_StationaryVehicleNoBrakingDetection (0 ms)
[----------] 3 tests from BrakeMonitorTest (1 ms total)

[==========] 3 tests, 0 failures
OK

CHECKPOINT 2 PASSED: All covering tests pass (3/3)
```

### Step 4c: Run FULL Test Suite (Regression Check)

```bash
# Terminal: Run ALL tests to ensure no regressions
$ cd C:\My_Projects\Training\Brake_and_Wheel_Speed_Monitoring_System\build

$ ctest --verbose
Test project C:\My_Projects\Training\Brake_and_Wheel_Speed_Monitoring_System\build
    Start  1: unit_tests
Test #1: unit_tests (Passed)
[==========] Running 52 tests from 4 test suites.
[----------] 13 tests from MonitoringDataValidatorTest
[ RUN      ] MonitoringDataValidatorTest.TC_DAT_001_ValidInput
[       OK ] MonitoringDataValidatorTest.TC_DAT_001_ValidInput
...
[----------] 12 tests from BrakeMonitorTest
[ RUN      ] BrakeMonitorTest.TC_BRK_001_NormalBrakingCondition
[       OK ] BrakeMonitorTest.TC_BRK_001_NormalBrakingCondition
[ RUN      ] BrakeMonitorTest.TC_BRK_003_PedalReleasedWhileDriving
[       OK ] BrakeMonitorTest.TC_BRK_003_PedalReleasedWhileDriving
...
[==========] 52 tests, 0 failures

Test project: 52 tests passed, 0 failures

CHECKPOINT 3 PASSED: No regressions (52/52 pass)
```

### Step 5: Record Metrics & Sign Off

```
UNIT REFACTORING REPORT: is_braking()
════════════════════════════════════════════════════════════

Component: BrakeMonitor
Function: is_braking()
Phase: 1/5
Date: 2024-09-29
Time Spent: 15 minutes

BEFORE REFACTORING:
├─ Code: return sample.brakePedalPressed && sample.vehicleSpeedKph > 0.0;
├─ Complexity: 2 operators
├─ Lines: 1
└─ Tests: 3 (TC-BRK-001, 003, 012)

REFACTORING CHANGES:
├─ Extraction: Extracted 0.0 → MIN_SPEED_KPH constant
├─ Comment: Added clarifying comment
├─ Interface: UNCHANGED ✓ (important!)
└─ Logic: UNCHANGED ✓ (identical behavior)

AFTER REFACTORING:
├─ Code: return sample.brakePedalPressed && sample.vehicleSpeedKph > MIN_SPEED_KPH;
├─ Complexity: 2 operators (same)
├─ Lines: 2 (added clarity)
├─ Tests: 3 (TC-BRK-001, 003, 012)

VALIDATION RESULTS:
├─ Compilation: ✓ SUCCESS
├─ Covering Tests: ✓ PASS (3/3)
├─ Full Suite: ✓ PASS (52/52)
├─ Regressions: ✗ NONE
└─ Code Review: ✓ READY

METRICS SUMMARY:
├─ Improvement: Better code readability (explicit constant)
├─ Risk: VERY LOW (1-line function, 3 tests cover it)
├─ Rollback Difficulty: TRIVIAL (revert 1 change)
└─ Overall: ✓ APPROVED TO PROCEED

STATUS: ✓ UNIT 1 COMPLETE - READY FOR NEXT FUNCTION
════════════════════════════════════════════════════════════
```

---

## 🔍 Example 2: Refactoring check_pressure() Function

### Background
```
Component: BrakeMonitor
File: src/brake_monitor.cpp
Function: check_pressure()
Current: 12 lines with conditional logic
Size: 12 LOC
Tests: TC-BRK-004, TC-BRK-005, TC-BRK-007, TC-BRK-008 (4 test cases)
Expected Time: 30-45 minutes
```

### Step 1: Map Coverage

```cpp
// tests/unit/brake_monitor_test.cpp

// Test 1: Pressure valid (40 bar) - no fault
TEST(BrakeMonitorTest, TC_BRK_004_...) { 
    EXPECT_EQ(FaultType::NO_FAULT, ...check_pressure(40.0, true)); 
}

// Test 2: Pressure 0 - fault
TEST(BrakeMonitorTest, TC_BRK_005_...) { 
    EXPECT_EQ(FaultType::BRAKE_PRESSURE_FAULT, ...check_pressure(0.0, true)); 
}

// Test 3: Pressure 101 (exceeds max) - fault
TEST(BrakeMonitorTest, TC_BRK_007_...) { 
    EXPECT_EQ(FaultType::BRAKE_PRESSURE_FAULT, ...check_pressure(101.0, true)); 
}

// Test 4: Pressure valid but not braking - no check
TEST(BrakeMonitorTest, TC_BRK_008_...) { 
    EXPECT_EQ(FaultType::NO_FAULT, ...check_pressure(40.0, false)); 
}
```

### Step 2: Refactor (12-line function)

**BEFORE**:
```cpp
FaultType BrakeMonitor::check_pressure(
    const BrakeWheelSpeedSample& sample) const {
    
    // If not braking, don't check pressure
    if (!is_braking(sample)) {
        return FaultType::NO_FAULT;
    }
    
    // Check pressure range: 0 < pressure <= 100
    if (sample.brakePressureBar <= 0.0 || 
        sample.brakePressureBar > 100.0) {
        return FaultType::BRAKE_PRESSURE_FAULT;
    }
    
    return FaultType::NO_FAULT;
}
```

**AFTER** (Refactored for clarity):
```cpp
constexpr double MIN_PRESSURE_BAR = 0.0;
constexpr double MAX_PRESSURE_BAR = 100.0;

FaultType BrakeMonitor::check_pressure(
    const BrakeWheelSpeedSample& sample) const {
    
    // Only check pressure when actively braking
    if (!is_braking(sample)) {
        return FaultType::NO_FAULT;
    }
    
    // Validate pressure is in valid operating range
    const bool pressure_valid = 
        sample.brakePressureBar > MIN_PRESSURE_BAR && 
        sample.brakePressureBar <= MAX_PRESSURE_BAR;
    
    return pressure_valid 
        ? FaultType::NO_FAULT 
        : FaultType::BRAKE_PRESSURE_FAULT;
}
```

### Step 3: Build & Validate

```bash
# Build
$ cmake --build .
Build succeeded ✓

# Run covering tests
$ .\unit_tests.exe --gtest_filter="*TC_BRK_004*:*TC_BRK_005*:*TC_BRK_007*:*TC_BRK_008*"
[==========] Running 4 tests from 1 test suite.
[       OK ] TC_BRK_004
[       OK ] TC_BRK_005
[       OK ] TC_BRK_007
[       OK ] TC_BRK_008
[==========] 4 tests, 0 failures

# Run full suite
$ ctest --verbose
[==========] 52 tests, 0 failures
Test project: 52 tests passed ✓

STATUS: ✓ UNIT 2 COMPLETE
```

### Step 4: Record & Continue

```
TIME SPENT: 45 minutes
COMPLEXITY: 2 branches → 1 branch (50% reduction)
READABILITY: ++ (constants make requirements explicit)
TEST COVERAGE: 4/4 passing
REGRESSION: None (52/52 passing)
SIGN OFF: ✓ APPROVED

NEXT: Proceed to evaluate() function (largest, 35 LOC)
```

---

## 📊 Daily Workflow Example: Complete BrakeMonitor Refactoring

### Timeline

```
09:00 AM - PREPARATION
├─ Review current code
├─ Read all test cases (12 tests)
├─ Plan 5 refactoring phases
└─ Time estimate: 2-3.5 hours

09:20 AM - PHASE 1: is_braking() [COMPLETE]
├─ Time spent: 15 min
├─ Tests: 3/3 pass
├─ Full suite: 52/52 pass ✓
└─ Status: Ready for next phase

09:35 AM - PHASE 2: check_pressure() [COMPLETE]
├─ Time spent: 45 min
├─ Tests: 4/4 pass
├─ Full suite: 52/52 pass ✓
└─ Status: Ready for next phase

10:20 AM - BREAK (10 min)

10:30 AM - PHASE 3: evaluate() [IN PROGRESS]
├─ This is the largest function (35 LOC)
├─ Most complex refactoring (many branches)
├─ Time estimate: 60-90 minutes
├─ Tests: 12 total tests
└─ Current: 40% complete

LUNCHTIME (1 hour)

12:30 PM - PHASE 3: evaluate() [COMPLETE]
├─ Time spent: 90 min (total)
├─ Tests: 12/12 pass
├─ Full suite: 52/52 pass ✓
└─ Status: Ready for next phase

12:30 PM - PHASE 4: Helper Functions [COMPLETE]
├─ Time spent: 20 min
├─ Tests: Various boundaries
├─ Full suite: 52/52 pass ✓
└─ Status: All functions refactored

12:50 PM - PHASE 5: Final Validation
├─ Full rebuild
├─ Run all 52 tests
├─ Generate metrics report
├─ Sign off changes
└─ Time: 10 min

13:00 - COMPLETE ✓
        Entire BrakeMonitor component refactored & validated
        (Fits in 1 workday with break)
```

---

## ✅ Safety Checkpoints

### After Each Unit (3-5 Tests)

```
CHECKPOINT CHECKLIST:

1. Compilation
   ├─ No errors? ✓ YES → Continue
   └─ Errors? ✗ NO → Fix and retry

2. Covering Tests
   ├─ All pass? ✓ YES → Continue
   ├─ Any fail? ✗ NO → Debug & fix
   └─ Know which test failed? ✓ YES → Easy debug

3. Full Test Suite
   ├─ All 52 pass? ✓ YES → Continue
   ├─ Any fail? ✗ NO → Regression detected!
   ├─ Revert unit? YES → Previous unit was good
   └─ Debug issue → Unit caused regression

4. Code Quality
   ├─ Readable? ✓ YES → Continue
   ├─ Follows project style? ✓ YES → Continue
   └─ Added comments? ✓ YES → Continue

5. Metrics
   ├─ Complexity reduced? ✓ Desired
   ├─ Lines reasonable? ✓ <30 LOC
   └─ Time reasonable? ✓ <2 hours
```

---

## 🚨 If Something Breaks

### Debugging Strategy

```
Scenario: Test failure detected after refactoring Unit 2

SYMPTOMS:
- Covered tests (4/4) pass ✓
- Full test suite: 52/52 → 51/52 (one test fails) ✗

ROOT CAUSE ANALYSIS:
1. Revert Unit 2 only → Does full suite pass again?
   ├─ YES: Problem was in Unit 2 → Fix Unit 2 code
   └─ NO: Problem was earlier → Check Unit 1

2. Compare: What changed in Unit 2?
   ├─ Changed function interface? → Could break dependents
   ├─ Changed internal logic? → Could break gates
   └─ Changed helper calls? → Could break assumptions

SOLUTION:
1. Identify which test failed
2. Review that test's logic
3. Check Unit 2 code against test expectations
4. Fix Unit 2
5. Retest: Does it pass now? → YES ✓
6. Run full suite again: All pass? → YES ✓
7. Continue to Unit 3

TIME TO DEBUG: 5-15 minutes (one function scope)
```

---

## 📋 Quick Refactoring Checklist

For every unit you refactor:

```
BEFORE REFACTORING:
[ ] Selected one function only
[ ] Identified 3-5 covering tests
[ ] Recorded baseline metrics
[ ] Planned refactoring approach

DURING REFACTORING:
[ ] Made one logical change
[ ] Kept function interface unchanged
[ ] Added clarifying comments

AFTER REFACTORING:
[ ] Compiled successfully (no errors)
[ ] All covering tests pass
[ ] Full test suite passes (52/52)
[ ] No regressions detected
[ ] Metrics recorded (before → after)

SIGN OFF:
[ ] Code reviewed (self or peer)
[ ] Metrics improved (or maintained)
[ ] Ready to commit changes
[ ] Documented what changed
```

---

## 🎓 Key Takeaway

**Unit-based refactoring means**:
- Pick ONE function at a time
- Validate with 3-5 covering tests
- Verify full suite still passes (52/52)
- Every ~15-60 minutes: Safe checkpoint
- Every failure: You know exactly which unit broke
- Every day: Complete component refactored

**Result**: Safe, incremental refactoring with continuous validation and minimal risk! ✓

