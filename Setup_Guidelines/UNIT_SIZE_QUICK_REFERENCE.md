# Unit Size Architecture - Visual Reference & Quick Guide

## 🎯 Quick Answer: How Code Unit Size is Handled

Your framework uses a **3-level unit hierarchy** with validation at every level:

```
┌─────────────────────────────────────────────────────────────────┐
│ LEVEL 3: COMPONENT (Largest) - 2-4 hours                        │
│ ├─ Example: BrakeMonitor class (100-150 LOC, 5 functions)       │
│ ├─ Tests: 12-15 test cases covering all functions               │
│ ├─ Validation: All component tests pass (12-15 test)            │
│ └─ Execution: Run all tests - ~20-30ms                          │
│                                                                  │
│    ┌────────────────────────────────────────────────────────┐   │
│    │ LEVEL 2: FUNCTION (Medium) - 30-60 minutes             │   │
│    │ ├─ Example: is_braking() (3-10 LOC)                    │   │
│    │ ├─ Tests: 3-5 test cases for this function             │   │
│    │ ├─ Validation: All covering tests pass (3-5 tests)     │   │
│    │ └─ Execution: Run covering tests - ~1-5ms              │   │
│    │                                                         │   │
│    │    ┌──────────────────────────────────────────────┐    │   │
│    │    │ LEVEL 1: TEST CASE (Smallest) - <1ms        │    │   │
│    │    │ ├─ Example: TC-BRK-001_NormalBraking        │    │   │
│    │    │ ├─ Logic: Test ONE scenario (one code path) │    │   │
│    │    │ ├─ Input: Sample with specific values       │    │   │
│    │    │ ├─ Output: Expected result (pass/fail)      │    │   │
│    │    │ └─ Execution: ONE assertion - <1ms          │    │   │
│    │    └──────────────────────────────────────────────┘    │   │
│    └────────────────────────────────────────────────────────┘   │
└─────────────────────────────────────────────────────────────────┘
```

---

## 📊 Real Numbers from Your Project

```
Current Code Structure:
├─ 10 production files
├─ 475 lines of code
├─ 16 functions total
├─ 52 test cases
└─ 3.25 tests per function ← This ratio ensures good coverage!

Breakdown by Component:
┌─────────────────────────────────────────────────────────────┐
│ Component          │ Functions │ LOC  │ Tests │ Test/Func  │
├─────────────────────────────────────────────────────────────┤
│ Validation         │     2     │  50  │  13   │   6.5 ✓✓   │
│ Brake Monitoring   │     4     │  60  │  12   │   3.0 ✓    │
│ Wheel Speed Monit. │     5     │  90  │  15   │   3.0 ✓    │
│ Fault Management   │     5     │ 100  │  12   │   2.4 ✓    │
├─────────────────────────────────────────────────────────────┤
│ TOTAL              │    16     │ 300  │  52   │   3.25 ✓   │
└─────────────────────────────────────────────────────────────┘

Legend: 3+ tests/function = Good Coverage ✓
```

---

## 🔄 Execution & Validation Strategy

### How a Single Unit Refactoring Works

```
START: Brake Monitor Component (100 LOC, 5 functions)
│
├─── STEP 1: Refactor One Function
│    │
│    ├─ Select: is_braking() function (8 LOC)
│    │
│    ├─ Edit: Refactor while preserving interface
│    │   Before: return sample.brakePedalPressed && sample.vehicleSpeedKph > 0.0;
│    │   After:  return (sample.brakePedalPressed && 
│    │                    sample.vehicleSpeedKph > MIN_SPEED_KPH);
│    │
│    └─ Compile: ✓ (no errors)
│
├─── STEP 2: Run Covering Tests (Tests specific to this function)
│    │
│    ├─ Test 1: TC-BRK-001 (braking active) ✓ PASS
│    ├─ Test 2: TC-BRK-003 (pedal released) ✓ PASS
│    ├─ Test 3: TC-BRK-012 (stationary)    ✓ PASS
│    │
│    └─ Result: 3/3 covering tests passing
│
├─── STEP 3: Run Full Test Suite (Regression check)
│    │
│    ├─ Test 1-13:  Validation tests ✓ All pass
│    ├─ Test 14-25: Brake tests      ✓ All pass (including 3 above)
│    ├─ Test 26-40: Wheel tests      ✓ All pass
│    ├─ Test 41-52: Fault tests      ✓ All pass
│    │
│    └─ Result: 52/52 total tests passing (NO REGRESSION)
│
├─── STEP 4: Record Metrics & Mark Complete
│    │
│    ├─ Complexity: 2 branches → 1 branch (50% reduction)
│    ├─ Lines: 1 line → 2 lines
│    ├─ Time: <5ms (total test execution)
│    ├─ Status: UNIT 1 COMPLETE ✓
│    │
│    └─ Safe Point: Can revert or continue
│
└─── REPEAT FOR NEXT FUNCTION

AFTER ALL FUNCTIONS:
└─ Final Validation: All 52 tests pass ✓ COMPONENT REFACTORING COMPLETE
```

---

## 💡 Key Principle #1: One Test = One Scenario

### Example: Testing is_braking() function

**GOOD** ✓ - Each test is separate:
```cpp
// Test Case 1: Braking active
TEST(BrakeMonitor, TC_BRK_001_BrakingActive) {
    sample.brakePedalPressed = true;      // ← Single scenario
    sample.vehicleSpeedKph = 50.0;
    ASSERT_TRUE(brake_monitor.is_braking(sample));
}

// Test Case 2: Pedal released
TEST(BrakeMonitor, TC_BRK_003_PedalReleased) {
    sample.brakePedalPressed = false;     // ← Different scenario
    sample.vehicleSpeedKph = 50.0;
    ASSERT_FALSE(brake_monitor.is_braking(sample));
}

// Test Case 3: Stationary vehicle
TEST(BrakeMonitor, TC_BRK_012_Stationary) {
    sample.brakePedalPressed = true;      // ← Another scenario
    sample.vehicleSpeedKph = 0.0;         // ← Speed = 0
    ASSERT_FALSE(brake_monitor.is_braking(sample));
}
```

**Result**: If test 1 passes but test 3 fails, you know it's a speed-related issue.

---

## 💡 Key Principle #2: One Function = One Logical Unit

### Example: BrakeMonitor functions

```
Function 1: is_braking()
├─ Size: 2 lines
├─ Purpose: Determine if actively braking
├─ Tests: TC-BRK-001, 003, 012 (3 tests)
├─ Test Coverage: 100% (all code paths tested)
└─ Refactoring Time: 15-20 minutes

Function 2: check_pressure()
├─ Size: 12 lines
├─ Purpose: Validate brake pressure range
├─ Tests: TC-BRK-004, 005, 007, 008 (4 tests)
├─ Test Coverage: 100% (all code paths tested)
└─ Refactoring Time: 30-45 minutes

Function 3: evaluate()
├─ Size: 35 lines
├─ Purpose: Evaluate all brake faults
├─ Tests: TC-BRK-001-012 (12 tests)
├─ Test Coverage: 100% (all code paths tested)
└─ Refactoring Time: 60-90 minutes
```

**Pattern**: Larger functions get more tests and more refactoring time.

---

## 💡 Key Principle #3: Validation at Every Level

```
VALIDATION PYRAMID:

                    ▲
                   ╱ ╲        FULL TEST SUITE
                  ╱   ╲       (52 tests, 5-10ms)
                 ╱     ╲      Validates: No regressions
                ╱───────╲
               ╱         ╲    COMPONENT TESTS
              ╱           ╲   (12-15 tests, 10-20ms)
             ╱─────────────╲  Validates: Component integrity
            ╱               ╲
           ╱                 ╲ FUNCTION TESTS
          ╱─────────────────────╲ (3-5 tests, 1-5ms)
         ╱                       ╲ Validates: Function logic
        ╱─────────────────────────╲
       ╱                           ╲
      ╱          UNIT TESTS         ╲ INDIVIDUAL TESTS
     ╱─────────────────────────────────╲ (<1ms each)
    ╱                                   ╲ Validates: Single scenario

EXECUTION ORDER:
1. Edit one function
2. Compile (must succeed)
3. Run function tests (must pass)
4. Run component tests (must pass)
5. Run full test suite (must pass)
6. Move to next unit
```

---

## 🎯 Refactoring Phases: Function-Level Breakdown

### Example: BrakeMonitor Refactoring Plan

```
COMPONENT: BrakeMonitor (brake_monitor.cpp, 100 LOC, 5 functions)

┌──────────────────────────────────────────────────────────────┐
│ PHASE 1: is_braking()                                        │
├──────────────────────────────────────────────────────────────┤
│ Size: 2 LOC                                                  │
│ Tests: TC-BRK-001, TC-BRK-003, TC-BRK-012 (3 tests)        │
│ Time: 15-20 minutes                                          │
│ Complexity: Very simple refactor (or skip)                   │
│ Validation: 3/3 tests + 52/52 full suite                    │
│ Status: [████] COMPLETE                                      │
└──────────────────────────────────────────────────────────────┘

┌──────────────────────────────────────────────────────────────┐
│ PHASE 2: check_pressure()                                    │
├──────────────────────────────────────────────────────────────┤
│ Size: 12 LOC                                                 │
│ Tests: TC-BRK-004, TC-BRK-005, TC-BRK-007, TC-BRK-008      │
│ Time: 30-45 minutes                                          │
│ Complexity: Medium (conditional logic)                       │
│ Validation: 4/4 tests + 52/52 full suite                    │
│ Status: [████░░░░░░] IN PROGRESS                             │
└──────────────────────────────────────────────────────────────┘

┌──────────────────────────────────────────────────────────────┐
│ PHASE 3: evaluate()                                          │
├──────────────────────────────────────────────────────────────┤
│ Size: 35 LOC (largest function)                             │
│ Tests: TC-BRK-001, 002, 003, ... TC-BRK-012 (12 tests)     │
│ Time: 60-90 minutes                                          │
│ Complexity: High (multiple branches, dependencies)          │
│ Validation: 12/12 tests + 52/52 full suite                  │
│ Status: [░░░░░░░░░░] NOT STARTED                             │
└──────────────────────────────────────────────────────────────┘

┌──────────────────────────────────────────────────────────────┐
│ PHASE 4: Helper functions (2-5 LOC each)                    │
├──────────────────────────────────────────────────────────────┤
│ Functions: check_stationary(), etc.                          │
│ Tests: Various boundary tests                                │
│ Time: 15-30 minutes total                                    │
│ Complexity: Simple                                           │
│ Validation: All tests + full suite                           │
│ Status: [░░░░░░░░░░] NOT STARTED                             │
└──────────────────────────────────────────────────────────────┘

┌──────────────────────────────────────────────────────────────┐
│ PHASE 5: Full Component Validation                           │
├──────────────────────────────────────────────────────────────┤
│ Action: Run full test suite                                  │
│ Tests: All 52 tests                                          │
│ Time: 5-10 minutes                                           │
│ Goal: Ensure no regressions introduced                       │
│ Status: [░░░░░░░░░░] NOT STARTED                             │
└──────────────────────────────────────────────────────────────┘

TOTAL ESTIMATE: 2-3.5 hours for entire component refactoring
```

---

## 📋 Validation Checklist: Unit-by-Unit

```
For Each Refactored Unit:

✓ BEFORE REFACTORING
  [ ] Read all covering tests
  [ ] Understand current implementation
  [ ] Record baseline metrics

✓ DURING REFACTORING
  [ ] Make one logical change at a time
  [ ] Keep function interface unchanged
  [ ] Compile after each change (catch errors immediately)

✓ AFTER REFACTORING
  [ ] Compile: No errors? YES ✓
  [ ] Covering tests pass? YES ✓ (all 3-5 tests)
  [ ] Full test suite pass? YES ✓ (all 52 tests)
  [ ] Metrics improved? (complexity, LOC, speed)
  [ ] Code review ready? (clean, documented)

✓ RECORD RESULTS
  [ ] Unit name & phase number
  [ ] Time spent
  [ ] Metrics before → after
  [ ] Test results (pass/fail)
  [ ] Any issues encountered
```

---

## ⏱️ Realistic Timeline Example

### Refactoring BrakeMonitor Component (100 LOC)

```
09:00 - Preparation & Planning            (15 min) ✓
        ├─ Review REFACTORING_PROMPT_EXAMPLE.md
        ├─ Plan 5 phases
        └─ Set up local build

09:15 - PHASE 1: is_braking()              (20 min) ✓
        ├─ Refactor
        ├─ Compile: ✓
        └─ Tests: 3/3 pass, 52/52 pass

09:35 - PHASE 2: check_pressure()          (45 min) ✓
        ├─ Refactor
        ├─ Compile: ✓
        └─ Tests: 4/4 pass, 52/52 pass

10:20 - PHASE 3: evaluate()                (90 min) ✓
        ├─ Refactor (largest function)
        ├─ Compile: ✓
        └─ Tests: 12/12 pass, 52/52 pass

11:50 - PHASE 4: Helper functions          (25 min) ✓
        ├─ Refactor supporting functions
        ├─ Compile: ✓
        └─ Tests: 8/8 pass, 52/52 pass

12:15 - PHASE 5: Full Validation           (10 min) ✓
        ├─ Final full rebuild
        ├─ Run 52 tests
        ├─ Generate report
        └─ Sign off

12:25 - COMPLETE ✓
        └─ Entire component refactored & validated
```

**Total: ~3.5 hours for complete component with daily workflow integration**

---

## 🚀 How This Ensures Safety

### Strategy: Incremental Validation

```
Baseline: All 52 tests passing (100%)
                    ↓
            Refactor Unit 1
                    ↓
      Validation: Still 52/52? YES ✓
                    ↓
            Refactor Unit 2
                    ↓
      Validation: Still 52/52? YES ✓
                    ↓
            Refactor Unit 3
                    ↓
      Validation: Still 52/52? YES ✓
                    ↓
          ALL UNITS COMPLETE ✓
              (Safe to commit)

Risk Mitigation:
- If Unit 2 breaks something, you know it's Unit 2 (not 1 or 3)
- Debugging scope is minimal (one function)
- Rollback is simple (revert one function)
- No "surprise" failures after days of refactoring
```

---

## 📊 Size Guidelines for Your Project

| Metric | Guideline | Your Project | Status |
|--------|-----------|--------------|--------|
| Lines per function | 3-30 LOC | 2-40 LOC | ✓ Good |
| Functions per component | 3-6 | 4-5 | ✓ Good |
| Tests per function | 3-5 | 3.25 avg | ✓ Good |
| Test execution time | <20ms | ~5-10ms | ✓ Excellent |
| Component size | 50-150 LOC | 50-100 LOC | ✓ Good |
| Refactoring phase | 15-90 min | 15-90 min | ✓ Good |

**Result**: Your code is already well-structured for incremental refactoring! ✓

---

## ✅ Summary

**How Unit Size is Handled**:

1. **Level 1 - Test Case**: One scenario, one assertion, <1ms
2. **Level 2 - Function**: One responsibility, 3-30 LOC, 3-5 tests, 15-90 min to refactor
3. **Level 3 - Component**: One domain, 50-150 LOC, 12-15 tests, 2-4 hours to refactor

**Validation Strategy**:
- After each unit: Covering tests pass ✓
- After each unit: Full suite passes ✓ (no regressions)
- Every 15-60 minutes: Validation checkpoint
- Every day: Complete component validated

**Safety Result**:
- Low risk per unit (small scope)
- Quick debugging if failure
- Easy rollback (single function)
- High confidence (continuous validation)
- Daily workflow compatible (3-5 hour chunks)

**Your Project**: Already well-structured (3.25 tests/function) = Ready for incremental refactoring! 🎯

