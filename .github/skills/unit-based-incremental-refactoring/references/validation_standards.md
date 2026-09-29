# Validation Standards - Reference

## Validation Checkpoints

### Checkpoint 1: Compilation Success

**What**: Code compiles without errors or warnings  
**When**: After refactoring each unit  
**Command**:
```bash
cd build && cmake --build .
```

**Pass Criteria**:
- ✓ Exit code: 0
- ✓ No "error:" messages
- ✓ No "undefined reference" errors
- ✓ All object files created
- ✓ Library/executable links successfully

**Fail Criteria**:
- ✗ Exit code: non-zero
- ✗ Any compilation errors
- ✗ Linking failures
- ✗ Missing includes/symbols

**Next Step If Pass**: Continue to covering tests  
**Next Step If Fail**: Debug compilation errors, revert if necessary

---

### Checkpoint 2: Covering Tests Pass

**What**: All 3-5 tests for this function pass  
**When**: After successful compilation  
**Command**:
```bash
ctest -R "TC_UNIT_001|TC_UNIT_003|TC_UNIT_012"
```

**Pass Criteria**:
- ✓ All specified tests run
- ✓ All tests report: PASSED
- ✓ No timeout errors
- ✓ Execution time < 10ms (all tests combined)

**Fail Criteria**:
- ✗ Any test reports: FAILED
- ✗ Segmentation fault
- ✗ Assertion failure
- ✗ Test timeout (> 10s)

**Failure Analysis**:
1. Identify which test failed
2. Read test expectation
3. Compare with refactored code
4. Debug: Is code wrong or test expectation wrong?
5. Fix issue
6. Re-run covering tests

**Expected Debug Time**: 5-15 minutes (small scope!)

**Next Step If Pass**: Continue to full suite regression check  
**Next Step If Fail**: Debug and fix the issue (see above)

---

### Checkpoint 3: Full Test Suite Passes (Regression Check)

**What**: All 52 tests still pass (no regressions introduced)  
**When**: After covering tests pass  
**Command**:
```bash
ctest --verbose
```

**Pass Criteria**:
- ✓ All 52 tests run
- ✓ All 52 tests pass
- ✓ No regressions from baseline
- ✓ Total execution time < 30ms

**Fail Criteria**:
- ✗ Any test fails (regression detected!)
- ✗ Total tests < 52 (missing tests)
- ✗ Test reports timeout

**Regression Response**:
1. Note which tests failed
2. Revert current unit only
3. Run full suite again (should pass)
4. Identify affected component
5. Debug interaction with that component
6. Refactor more carefully
7. Re-run validation

**Expected Debug Time**: 10-20 minutes (narrowed scope)

**Next Step If Pass**: Unit refactoring complete, log results  
**Next Step If Fail**: Regression detected, see regression response above

---

### Checkpoint 4: Metrics Comparison (Optional but Recommended)

**What**: Code quality metrics improve or stay same  
**When**: After full suite passes  
**Metrics to Check**:
- Cyclomatic complexity
- Lines of code
- Code duplication
- Test coverage

**Pass Criteria**:
- ✓ Complexity maintained or reduced
- ✓ LOC maintained or reduced
- ✓ Coverage maintained or increased
- ✓ No duplication introduced

**Fail Criteria**:
- ✗ Complexity increased
- ✗ LOC significantly increased (> 10%)
- ✗ Coverage decreased
- ✗ New duplication found

**Remediation**:
- If complexity increased: Review refactoring approach
- If LOC bloated: Simplify further
- If coverage dropped: Add tests
- If duplication found: Extract common code

---

## Quick Validation Checklist

### Before Starting Unit Refactoring
```
[ ] Latest code pulled/committed
[ ] All tests passing (52/52)
[ ] No uncommitted changes
[ ] Function clearly identified
[ ] Covering tests mapped (3-5 tests)
[ ] Refactoring plan documented
```

### During Unit Refactoring
```
[ ] Only ONE function edited
[ ] Function interface unchanged
[ ] One logical change made
[ ] Comments added/updated
```

### After Unit Refactoring (Pre-Validation)
```
[ ] Changes reviewed by self
[ ] Code follows project style
[ ] No commented-out code left
[ ] No debug logging left
```

### Validation Checklist
```
CHECKPOINT 1: Compilation
[ ] cmake --build . exits with 0
[ ] No error messages
[ ] No warnings (ideally)

CHECKPOINT 2: Covering Tests
[ ] All 3-5 covering tests pass
[ ] No assertion failures
[ ] Execution time < 10ms

CHECKPOINT 3: Full Suite
[ ] All 52 tests pass
[ ] No regressions
[ ] Execution time < 30ms

CHECKPOINT 4: Metrics (Optional)
[ ] Complexity improved/same
[ ] LOC improved/same
[ ] Coverage improved/same
```

### Completion Checklist
```
[ ] All checkpoints passed
[ ] Metrics recorded
[ ] Log entry created
[ ] Unit marked complete
[ ] Ready for next unit
```

---

## Validation Failures: Response Procedures

### Scenario 1: Compilation Error

```
ERROR: fatal error C1083: Cannot open include file: 'foo.hpp'

RESPONSE:
  1. Check file exists at correct path
  2. Verify include path is correct (relative vs absolute)
  3. Verify function signature unchanged (didn't rename function)
  4. Rebuild: cmake --build . --clean-first
  5. If still fails: Revert, debug, try again

TIME: 5-10 minutes
```

### Scenario 2: Covering Test Failure

```
TC_BRK_001: FAILED (Assertion failure)
  Expected: true
  Actual:   false

RESPONSE:
  1. Understand what test expects
  2. Review refactored code
  3. Compare logic with original
  4. Is new logic wrong? Fix code.
  5. Is test expectation wrong? Fix test (rare).
  6. Re-run covering tests
  7. Re-run full suite

TIME: 5-15 minutes (single function scope!)
```

### Scenario 3: Regression (Full Suite Fails)

```
ctest output:
  Test #25: FAILED
  Test #30: FAILED
  
Tests failing are NOT in covering tests for current unit.

RESPONSE:
  1. Identify which component those tests cover
  2. Revert current unit only
  3. Run full suite (should pass again)
  4. Identify why current unit breaks those tests
  5. Look for:
     - Changed function interface
     - Changed return value type
     - Changed behavior of dependency
     - Side effects introduced
  6. Fix interaction with that component
  7. Re-run validation

TIME: 10-20 minutes (narrowed scope!)
```

### Scenario 4: Metrics Degradation

```
Before: Complexity 8
After:  Complexity 10

RESPONSE:
  1. Review refactoring changes
  2. Was there unavoidable complexity increase?
  3. Try different refactoring approach
  4. Or: Accept if metrics improve overall
  5. Document reasoning in log

TIME: 5-10 minutes
```

---

## Test Coverage Verification

### Identifying Covering Tests

**For function**: `is_braking()`
```cpp
bool is_braking(const Sample& s) {
    return s.brakePedal && s.speed > 0.0;
}
```

**Code paths** (all must be tested):
1. Both conditions true → returns TRUE
2. First condition false → returns FALSE
3. Second condition false → returns FALSE

**Covering tests** (cover all paths):
```
TC-BRK-001: s.brakePedal=true, speed=50   → Expects TRUE
TC-BRK-003: s.brakePedal=false, speed=50  → Expects FALSE
TC-BRK-012: s.brakePedal=true, speed=0    → Expects FALSE
```

**Validation**: All 3 code paths tested ✓

### Coverage Ratio

**Guideline**: Covering tests = 3-5 tests per function

**Your project**:
```
Component              │ Avg Covering Tests/Func
───────────────────────┼────────────────────────
Validation             │ 6.5 (excellent!)
Brake Monitoring       │ 3.0 (good)
Wheel Speed Monitoring │ 3.0 (good)
Fault Management       │ 2.4 (acceptable)
```

**If ratio < 2**: Likely missing edge cases  
**If ratio 2-3**: Good coverage  
**If ratio 3-5**: Excellent coverage  
**If ratio > 5**: May be testing too much per function

---

## Performance Validation

### Execution Time Expectations

```
Checkpoint             │ Expected Time │ Guideline
───────────────────────┼───────────────┼──────────
Compilation            │ < 2 seconds   │ If > 5s, check CMakeLists.txt
Covering tests (3-5)   │ < 10ms        │ If > 50ms, check test efficiency
Full suite (52 tests)  │ < 30ms        │ If > 100ms, check test efficiency
Manual review          │ 2-5 minutes   │ Depends on LOC
─────────────────────────────────────────────────────
Unit validation total  │ < 5 minutes   │ For 15-30 LOC unit
```

### If Tests Run Too Slowly

**Diagnosis**:
1. Are tests doing file I/O? (should only use memory)
2. Are tests calling sleep/delay? (should not)
3. Are tests creating many objects? (should be minimal)
4. Are tests running in sequence unnecessarily? (use parallelization)

**Solution**:
- Optimize test code first
- Then proceed with refactoring

---

## Validation Report Format

After each unit, document:

```
UNIT VALIDATION REPORT
═════════════════════════════════════════════════

Unit ID:             1
Function Name:       is_braking()
Timestamp:           2024-09-29 10:30
Duration:            15 minutes

CHECKPOINT 1: Compilation
  Status:            ✓ PASS
  Command:           cmake --build .
  Exit Code:         0

CHECKPOINT 2: Covering Tests
  Status:            ✓ PASS (3/3)
  Tests:             TC-BRK-001, TC-BRK-003, TC-BRK-012
  Execution Time:    2ms

CHECKPOINT 3: Full Suite
  Status:            ✓ PASS (52/52)
  Execution Time:    8ms
  Regressions:       NONE

CHECKPOINT 4: Metrics
  Complexity:        2 → 1 (50% reduction)
  LOC:               1 → 2 (1 line added for clarity)
  Status:            ✓ ACCEPTABLE

OVERALL: ✓ VALIDATION PASSED
Next: Proceed to Unit 2
```

