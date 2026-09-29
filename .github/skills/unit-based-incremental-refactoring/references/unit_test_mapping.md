# Unit Test Mapping Guide

How to identify which tests cover which functions for unit-based refactoring.

## Overview

**Covering Tests**: 3-5 test cases that exercise ALL code paths in a function.

**Why This Matters**: If refactoring breaks any code path, a covering test WILL fail immediately.

---

## Test Mapping Strategy

### Step 1: Identify All Code Paths

For each function, enumerate all unique execution paths:

```cpp
bool is_braking(const Sample& s) {
    // Path 1: Both true → return true
    // Path 2: Pedal false → return false  
    // Path 3: Speed 0 → return false
    return s.brakePedal && s.speed > 0.0;
}
```

### Step 2: Find Tests for Each Path

Use grep to find tests that exercise each path:

```bash
# Search for tests related to braking
grep -n "is_braking\|brakePedal\|speed.*0" tests/unit/brake_monitor_test.cpp
```

### Step 3: Verify Coverage

Create test-to-path mapping:

| Test Case | Inputs | Path | Expected | Status |
|-----------|--------|------|----------|--------|
| TC-BRK-001 | pedal=true, speed=50 | Path 1 | true | ✓ |
| TC-BRK-003 | pedal=false, speed=50 | Path 2 | false | ✓ |
| TC-BRK-012 | pedal=true, speed=0 | Path 3 | false | ✓ |

---

## Real Example: BrakeMonitor

### Function: is_braking()
```cpp
bool is_braking(const Sample& s) {
    return s.brakePedal && s.speed > 0.0;
}
```

**Code Paths**:
```
Path A: brakePedal=T, speed>0  → return true
Path B: brakePedal=F, speed>0  → return false
Path C: brakePedal=T, speed≤0  → return false
```

**Covering Tests**:
- TC-BRK-001: Tests Path A (normal braking)
- TC-BRK-003: Tests Path B (no pedal)
- TC-BRK-012: Tests Path C (speed is zero)

**Verification**:
```bash
# Run covering tests only
ctest -R "TC_BRK_(001|003|012)" --verbose

# Expected: 3/3 tests pass
# Time: <5ms
```

---

## Mapping Checklist

For each function to refactor:

- [ ] Read the function code carefully
- [ ] Identify all conditional branches
- [ ] Identify all loop variations
- [ ] Identify all exceptional cases
- [ ] List all unique code paths
- [ ] Search for tests exercising each path
- [ ] Map test→path relationship
- [ ] Verify 3-5 tests cover all paths
- [ ] Document in refactoring plan

---

## Common Patterns

### Boolean Functions
```cpp
// Paths: true case, false case
bool shouldDoX(const Sample& s) {
    return s.flag && s.value > threshold;
}
// Tests: one for true, one for false (minimum 2)
```

### Range Checking Functions
```cpp
// Paths: within range, below range, above range
bool isValid(int value) {
    return value >= MIN && value <= MAX;
}
// Tests: one for each boundary (minimum 3)
```

### Fault Detection Functions
```cpp
// Paths: no fault, fault detected, edge case
FaultStatus checkPressure(float p) {
    if (p < 0 || p > 100) return FaultStatus::INVALID;
    if (p < 50) return FaultStatus::LOW;
    return FaultStatus::OK;
}
// Tests: one for each status (minimum 3)
```

---

## Validation

**Covering test set is adequate when**:
- ✓ Every if/else branch is covered
- ✓ Every loop variant is tested
- ✓ Boundary conditions are tested
- ✓ Error conditions are tested
- ✓ Normal operation is tested
- ✓ All early returns are covered
- ✓ Total tests: 3-5 per function

