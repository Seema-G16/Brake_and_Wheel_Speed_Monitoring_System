# Code Unit Size Architecture & Incremental Validation

## 🎯 Overview: "Unit of Work" Philosophy

Your framework breaks down **code refactoring into executable, validatable units**. This document explains the strategy.

---

## 📊 Three-Level Unit Hierarchy

### Level 1: Test Case (Smallest)
**Granularity**: Single logical scenario  
**Size**: ~10-20 lines of test code  
**Execution Time**: <1ms per test case  
**Validation**: Immediate (pass/fail)

```cpp
// Example: TC-BRK-001 (Braking condition active)
TEST(BrakeMonitorTest, TC_BRK_001_NormalBrakingCondition) {
    BrakeWheelSpeedSample sample{
        .timestamp = 6.0,
        .vehicleSpeedKph = 50.0,
        .brakePedalPressed = true,
        .brakePressureBar = 40.0,
        .frontLeftWheelSpeedKph = 50.0,
        .frontRightWheelSpeedKph = 50.0,
        .rearLeftWheelSpeedKph = 50.0,
        .rearRightWheelSpeedKph = 50.0
    };
    
    // Validate ONE logical condition
    ASSERT_TRUE(brake_monitor.is_braking(sample));
    ASSERT_EQ(FaultType::NO_FAULT, brake_monitor.evaluate(sample));
}
```

**Key**: Each test case validates ONE specific logical branch/scenario.

---

### Level 2: Code Function (Medium)
**Granularity**: Single logical function  
**Size**: 5-30 lines of implementation  
**Unit Tests**: 3-5 test cases per function  
**Execution Time**: ~5-10ms (all tests combined)  
**Validation**: All unit tests must pass

```cpp
// Example: is_braking() function in brake_monitor.cpp
bool BrakeMonitor::is_braking(const BrakeWheelSpeedSample& sample) const {
    // LOGICAL UNIT: "Determine if vehicle is actively braking"
    return sample.brakePedalPressed && sample.vehicleSpeedKph > 0.0;
}
```

**Covering Tests**:
- TC-BRK-001: Braking active (pedal=TRUE, speed>0)
- TC-BRK-003: Pedal released (pedal=FALSE)
- TC-BRK-012: Stationary vehicle (speed=0)

---

### Level 3: Code Component (Large)
**Granularity**: Single logical module/class  
**Size**: 100-300 lines (across all functions)  
**Unit Tests**: 12-15 test cases per component  
**Execution Time**: ~20-30ms (all tests combined)  
**Validation**: 100% test pass rate

```cpp
// Example: BrakeMonitor class (brake_monitor.cpp)
class BrakeMonitor {
    // LOGICAL UNIT: "All brake-related monitoring logic"
    
public:
    bool is_braking(...) { ... }           // 2 lines
    FaultType evaluate(...) { ... }        // 15 lines
    
private:
    bool check_pressure(...) { ... }       // 8 lines
    bool check_stationary(...) { ... }     // 5 lines
};
```

**Covering Tests** (TC-BRK-001 through TC-BRK-012):
- 3 tests for `is_braking()` logic
- 4 tests for `evaluate()` pressure checks
- 5 tests for edge cases & boundaries

---

## 🔄 Incremental Validation Flow

### For Each Unit Level, This Validation Occurs:

```
┌─────────────────────────────────────────────────────────────┐
│ BEFORE REFACTORING                                          │
├─────────────────────────────────────────────────────────────┤
│ ✓ Step 1: Compile production code (src/*.cpp)              │
│ ✓ Step 2: Compile test code (tests/unit/*.cpp)             │
│ ✓ Step 3: Link all objects (libgtest + production lib)     │
│ ✓ Step 4: Run ALL tests (52 total)                         │
│ ✓ Step 5: Verify 100% pass rate                            │
│           Baseline established: 52/52 passing              │
└─────────────────────────────────────────────────────────────┘

            ↓ START REFACTORING (One Unit at a Time)

┌─────────────────────────────────────────────────────────────┐
│ REFACTORING UNIT 1: One function                            │
├─────────────────────────────────────────────────────────────┤
│ ✓ Step 1: Refactor single function (e.g., is_braking)     │
│ ✓ Step 2: Run ONLY covering tests (e.g., 3 tests)         │
│           Expected: 3/3 passing                            │
│ ✓ Step 3: Run ALL tests (52 total)                         │
│           Expected: Still 52/52 passing                    │
│ ✓ Step 4: Record changes & mark UNIT 1 complete           │
└─────────────────────────────────────────────────────────────┘

            ↓ MOVE TO NEXT UNIT

┌─────────────────────────────────────────────────────────────┐
│ REFACTORING UNIT 2: Next function                           │
├─────────────────────────────────────────────────────────────┤
│ ✓ Step 1: Refactor next function (e.g., check_pressure)   │
│ ✓ Step 2: Run ONLY covering tests (e.g., 4 tests)         │
│           Expected: 4/4 passing                            │
│ ✓ Step 3: Run ALL tests (52 total)                         │
│           Expected: Still 52/52 passing                    │
│ ✓ Step 4: Record changes & mark UNIT 2 complete           │
└─────────────────────────────────────────────────────────────┘

            ↓ REPEAT UNTIL COMPLETE

┌─────────────────────────────────────────────────────────────┐
│ AFTER REFACTORING (Validation Gate)                         │
├─────────────────────────────────────────────────────────────┤
│ ✓ Step 1: Recompile ALL code                               │
│ ✓ Step 2: Run full test suite (52 total)                   │
│ ✓ Step 3: Verify 100% pass rate (52/52)                   │
│ ✓ Step 4: Compare metrics (complexity, lines, coverage)    │
│ ✓ Step 5: Sign off refactoring                             │
│           Result: Refactoring COMPLETE & VALIDATED         │
└─────────────────────────────────────────────────────────────┘
```

---

## 💡 Key Principles: How Unit Size is Determined

### Principle 1: **One Test = One Scenario**

Each test case tests **exactly one logical path**:

```cpp
// ✓ GOOD: One scenario per test
TEST(BrakeMonitorTest, TC_BRK_001_BrakingActive) {
    sample.brakePedalPressed = true;
    sample.vehicleSpeedKph = 50.0;
    ASSERT_TRUE(brake_monitor.is_braking(sample));  // One assertion
}

// ✗ BAD: Multiple scenarios in one test
TEST(BrakeMonitorTest, BrakeLogicMultiple) {
    // Scenario 1
    sample.brakePedalPressed = true;
    ASSERT_TRUE(brake_monitor.is_braking(sample));
    
    // Scenario 2 (different input)
    sample.vehicleSpeedKph = 0.0;
    ASSERT_FALSE(brake_monitor.is_braking(sample));  // Two scenarios!
}
```

**Why**: If ONE path fails, you know exactly which scenario broke.

---

### Principle 2: **One Function = One Logical Unit**

Each function does ONE thing:

```cpp
// ✓ GOOD: One responsibility
bool is_braking(const BrakeWheelSpeedSample& sample) const {
    return sample.brakePedalPressed && sample.vehicleSpeedKph > 0.0;
}

// ✗ BAD: Multiple responsibilities
bool is_braking_and_check_pressure(const BrakeWheelSpeedSample& sample) {
    bool braking = sample.brakePedalPressed && sample.vehicleSpeedKph > 0.0;
    
    if (sample.brakePressureBar < 10.0) {
        return false;  // Now it checks pressure too!
    }
    
    return braking;
}
```

**Why**: Small functions are easy to test, understand, and refactor.

---

### Principle 3: **One Class = One Responsibility**

Each class encapsulates ONE domain:

```
┌──────────────────────────────────────┐
│ MonitoringDataValidator              │ ← Data Validation
│ - is_valid()                         │
│ - get_validation_error()             │
└──────────────────────────────────────┘

┌──────────────────────────────────────┐
│ BrakeMonitor                         │ ← Brake Logic
│ - is_braking()                       │
│ - evaluate()                         │
│ - check_pressure()                   │
└──────────────────────────────────────┘

┌──────────────────────────────────────┐
│ WheelSpeedMonitor                    │ ← Wheel Speed Logic
│ - calculate_spread()                 │
│ - detect_mismatch()                  │
│ - evaluate()                         │
└──────────────────────────────────────┘

┌──────────────────────────────────────┐
│ FaultManager                         │ ← Fault Aggregation
│ - evaluate()                         │
│ - get_overall_status()               │
│ - aggregate_faults()                 │
└──────────────────────────────────────┘
```

**Why**: Each module can be tested independently.

---

## 📈 Current Code Unit Sizes (Actual Metrics)

### Component Breakdown

```
Component                    Files    Functions    LOC    Tests    Test/Func Ratio
────────────────────────────────────────────────────────────────────────────────
Validation                   2        2            50     13       6.5 tests/func
  (MonitoringDataValidator)
  
Brake Monitoring             2        4            60     12       3.0 tests/func
  (BrakeMonitor)
  
Wheel Speed Monitoring       2        5            90     15       3.0 tests/func
  (WheelSpeedMonitor)
  
Fault Management             2        5            100    12       2.4 tests/func
  (FaultManager)
────────────────────────────────────────────────────────────────────────────────
TOTAL PRODUCTION             10       16           300    52       3.25 tests/func
TOTAL TESTS                  4        51           830    -        -
```

**Key Insight**: 
- **3+ tests per function** = Good coverage
- **100-300 LOC per component** = Manageable size
- **50-60 LOC per file** = Easy to understand

---

## 🔍 Test-to-Code Mapping: How Units Link

### Example: Wheel Speed Component

```
Component: WheelSpeedMonitor (2 files, 5 functions, 90 LOC)
├── detect_mismatch()          ← Function 1 (8 lines)
│   ├── TC-WHL-001             ← Test: Both within threshold
│   ├── TC-WHL-002             ← Test: One exceeds threshold
│   ├── TC-WHL-003             ← Test: Multiple exceed threshold
│   └── TC-WHL-004             ← Test: Negative speeds
│
├── calculate_spread()         ← Function 2 (6 lines)
│   ├── TC-WHL-005             ← Test: Zero spread
│   ├── TC-WHL-006             ← Test: 15 km/h spread
│   └── TC-WHL-007             ← Test: Large spread
│
├── is_braking_spread_fault()  ← Function 3 (4 lines)
│   ├── TC-WHL-008             ← Test: Spread ≤ 15 km/h
│   ├── TC-WHL-009             ← Test: Spread > 15 km/h
│   └── TC-WHL-010             ← Test: Not braking (gate)
│
├── evaluate()                 ← Function 4 (40 lines)
│   ├── TC-WHL-011             ← Test: No faults
│   ├── TC-WHL-012             ← Test: Mismatch detected
│   ├── TC-WHL-013             ← Test: Spread fault detected
│   └── TC-WHL-014             ← Test: Multiple faults
│
└── gate_condition()          ← Function 5 (5 lines)
    └── TC-WHL-015             ← Test: Gate open/closed states
```

**Execution Strategy**:
```
1. Refactor detect_mismatch() → Run TC-WHL-001 to TC-WHL-004 (4 tests, <5ms)
2. Verify: All production code still compiles
3. Verify: All 52 tests still pass ✓
4. Refactor calculate_spread() → Run TC-WHL-005 to TC-WHL-007 (3 tests, <5ms)
5. Verify: All 52 tests still pass ✓
... continue unit by unit ...
```

---

## 🧪 Validation Checkpoints During Refactoring

### After Each Unit is Refactored:

```
┌──────────────────────────────────────────────────────────┐
│ CHECKPOINT: "Can I run it?"                              │
├──────────────────────────────────────────────────────────┤
│ ✓ Syntax valid (no compiler errors)                      │
│ ✓ Linking successful (all symbols resolved)             │
│ ✓ Covering tests pass (specific to this unit)           │
│ ✓ Full test suite passes (regression check)             │
│ ✓ Metrics recorded (complexity, lines, time)            │
└──────────────────────────────────────────────────────────┘
```

### Example: Refactoring BrakeMonitor::is_braking()

```bash
# Step 1: Edit is_braking() in brake_monitor.cpp
# (Refactor logic while keeping interface the same)

# Step 2: Compile
$ cmake --build build
✓ Compilation successful
✓ 10 source files compiled
✓ All linking symbols resolved

# Step 3: Run covering tests ONLY
$ ctest -R "TC_BRK_001|TC_BRK_003|TC_BRK_012"
Test project c:\...\build
    Start  1: TC_BRK_001_NormalBrakingCondition
✓ Test #1 Passed
    Start  2: TC_BRK_003_PedalReleased
✓ Test #2 Passed
    Start  3: TC_BRK_012_StationaryVehicle
✓ Test #3 Passed

Test project: 3 tests passed, 0 failed

# Step 4: Run FULL test suite (regression check)
$ ctest --verbose
Test project c:\...\build
[Test 1/52] TC_DAT_001: PASS
[Test 2/52] TC_DAT_002: PASS
...
[Test 51/52] TC_FLT_011: PASS
[Test 52/52] TC_FLT_012: PASS

Test project: 52 tests passed, 0 failed ✓

# RESULT: is_braking() refactoring is COMPLETE & VALIDATED
```

---

## 📋 Refactoring Prompt Integration

The **REFACTORING_PROMPT_FRAMEWORK.md** uses these unit-size principles:

### Section 6: Refactoring Patterns
→ Lists specific **functions/methods** to refactor (not entire classes)

### Section 7: Testing Requirements
→ Lists **specific test cases** covering each function (not all tests)

### Section 10: Implementation Workflow
→ Phases broken into **one function per phase** (not one class per phase)

### Section 12: Success Metrics
→ Measured **per function** (complexity, coverage, speed)

**Example Prompt Structure**:
```markdown
## COMPONENT: BrakeMonitor (brake_monitor.cpp)

### PHASE 1: is_braking() Function Refactoring
Duration: 30 mins
Covering Tests: TC-BRK-001, TC-BRK-003, TC-BRK-012
Success: 3/3 tests passing + 50% complexity reduction

### PHASE 2: check_pressure() Function Refactoring
Duration: 45 mins
Covering Tests: TC-BRK-004, TC-BRK-005, TC-BRK-007
Success: 3/3 tests passing + 40% complexity reduction

### PHASE 3: evaluate() Function Refactoring
Duration: 60 mins
Covering Tests: TC-BRK-001, TC-BRK-002, ... TC-BRK-012
Success: 12/12 tests passing + 30% complexity reduction

### PHASE 4: Full Component Validation
Duration: 15 mins
Validation: All 52 tests passing + no regressions
```

---

## 🎯 Size Guidelines: When to Split/Combine Units

### ✓ GOOD Unit Size
```
- Single function: 3-30 lines of code
- Single function: 3-5 covering test cases
- Single function: Testable in <10ms
- Single function: One responsibility/purpose
- Can be reviewed in <5 minutes
- Can be refactored in <1 hour
```

### ✗ TOO SMALL Unit Size
```
- Single line functions (unnecessary micro-testing)
- Function with single test case (inadequate coverage)
- Refactoring phase: <15 minutes (overhead > work)
```

### ✗ TOO LARGE Unit Size
```
- Multiple functions in one refactoring phase (hard to debug)
- >10 test cases per function (likely testing multiple scenarios)
- Component refactoring: >3 hours (breaks daily workflow)
- File with >300 lines (violates SRP - Single Responsibility Principle)
```

---

## 🔄 Daily Workflow with Unit-Based Refactoring

### Day 1: BrakeMonitor Component Refactoring

```
09:00 - Read REFACTORING_PROMPT_EXAMPLE.md        (15 min)
09:15 - Copy REFACTORING_PROMPT_TEMPLATE.md       (5 min)
09:20 - Plan refactoring phases                   (15 min)
09:35 - Validate prompt against KEYWORDS          (5 min)

09:40 - UNIT 1: Refactor is_braking()             (30 min)
        ├─ Edit brake_monitor.cpp
        ├─ Compile (verify no errors)
        ├─ Run tests: TC-BRK-001, 003, 012
        └─ Verify: All 52 tests pass ✓

10:10 - UNIT 2: Refactor check_pressure()         (45 min)
        ├─ Edit brake_monitor.cpp
        ├─ Compile (verify no errors)
        ├─ Run tests: TC-BRK-004, 005, 007
        └─ Verify: All 52 tests pass ✓

10:55 - UNIT 3: Refactor evaluate()               (60 min)
        ├─ Edit brake_monitor.cpp (large function)
        ├─ Compile (verify no errors)
        ├─ Run tests: TC-BRK-001 through TC-BRK-012
        └─ Verify: All 52 tests pass ✓

11:55 - Final Validation                          (15 min)
        ├─ Full rebuild
        ├─ Run all 52 tests
        ├─ Generate metrics report
        └─ Sign off changes ✓

12:10 - COMPLETE ✓
```

**Result**: Entire component refactored and validated in ~3.5 hours with safety checkpoints every 30-60 minutes.

---

## 📊 Risk Management Through Unit Sizes

### Small Units = Low Risk

```
Risk Factor          Small Unit (1 function)    Large Unit (whole class)
──────────────────────────────────────────────────────────────────────
Regression Probability   ~5% (isolated change)    ~20% (cascading effects)
Debug Time if Failure    5-10 min (1 function)    30-60 min (many functions)
Revert Complexity        Simple (1 change)        Complex (many changes)
Daily Validation         3-5 times/day            1-2 times/day
```

### Safety Through Incremental Validation

```
Unit 1 Complete    Unit 2 Complete    Unit 3 Complete    Final Validation
      ✓                  ✓                  ✓                    ✓
   (52/52)            (52/52)            (52/52)              (52/52)
     pass               pass               pass                 pass
      ↓                  ↓                  ↓                    ↓
    If Unit 2 fails, you know it's NOT Unit 1 or Unit 3!
    Debugging scope is minimal.
```

---

## 🎓 Summary: Unit-Based Architecture

| Aspect | Small Units | Large Units |
|--------|-------------|------------|
| **Size** | 1 function (3-30 LOC) | 1 component (100-300 LOC) |
| **Tests** | 3-5 per function | 12-15 per component |
| **Compilation** | <1 second | ~1-2 seconds |
| **Test Execution** | <10ms per unit | ~20-30ms per component |
| **Refactoring Time** | 15-60 minutes | 2-4 hours |
| **Validation Frequency** | Every 30-60 min | End of day |
| **Risk** | Low ✓ | Medium ⚠ |
| **Debugging** | Quick ✓ | Time-consuming ✗ |
| **Revert Ability** | Trivial ✓ | Complex ✗ |

**Best Practice**: Use small units (one function per refactoring phase) with frequent validation checkpoints.

---

## 🚀 Applying This to Your Next Refactoring

### Step 1: Identify the Component
```
Example: FaultManager (fault_manager.cpp)
```

### Step 2: List All Functions
```
1. evaluate()                    [40 LOC] → 3-4 phases
2. get_overall_status()         [10 LOC] → 1 phase
3. has_validation_error()       [5 LOC]  → 1 phase
4. aggregate_faults()           [30 LOC] → 2-3 phases
5. determine_status()           [15 LOC] → 1 phase
```

### Step 3: Create Refactoring Phases (One Function Each)
```
Phase 1 (60 min):  Refactor evaluate()           [40 LOC]
  Tests: TC-FLT-001 to TC-FLT-006
  
Phase 2 (45 min):  Refactor aggregate_faults()  [30 LOC]
  Tests: TC-FLT-007 to TC-FLT-010
  
Phase 3 (30 min):  Refactor determine_status()  [15 LOC]
  Tests: TC-FLT-011, TC-FLT-012
  
Phase 4 (15 min):  Refactor get_overall_status() [10 LOC]
  Tests: All FLT tests (regression check)
  
Phase 5 (15 min):  Refactor has_validation_error() [5 LOC]
  Tests: All tests (final validation)
  
Phase 6 (15 min):  Full Component Validation
  Tests: All 52 tests
```

### Step 4: Execute Each Phase with Validation
```
FOR EACH PHASE:
  1. Edit one function only
  2. Compile (must succeed)
  3. Run covering tests (must pass)
  4. Run all tests (must pass)
  5. Record metrics & mark complete
  THEN MOVE TO NEXT PHASE
```

---

## ✅ Final Checklist

- [ ] Each function is 3-30 lines (reasonable size)
- [ ] Each function has 3-5 covering test cases
- [ ] Each test case tests ONE scenario
- [ ] Each refactoring phase = ONE function
- [ ] Each phase is 15-60 minutes (fits daily workflow)
- [ ] Each phase ends with full test suite validation
- [ ] Metrics are recorded per function (complexity, LOC, time)
- [ ] No phase skips the validation checkpoint
- [ ] Rollback plan is simple (revert one function)

**Result**: Safe, incremental refactoring with continuous validation! 🎯

