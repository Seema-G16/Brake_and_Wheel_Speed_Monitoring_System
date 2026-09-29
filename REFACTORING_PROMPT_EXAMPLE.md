# REFACTORING PROMPT EXAMPLE
## Brake & Wheel Speed Monitoring System - Code Optimization

---

## 1. EXECUTIVE SUMMARY
Refactor the fault aggregation logic in `fault_manager.cpp` to improve code clarity and reduce cyclomatic complexity while maintaining 100% test coverage and backward compatibility.

### Why This Matters
- **Current State**: Fault manager has 8 branches in evaluate() method
- **Target State**: Simplified logic with <5 branches
- **Benefit**: Easier maintenance, fewer bugs, better code review experience

---

## 2. SCOPE & AFFECTED COMPONENTS

### Files In Scope
- [ ] `src/fault_manager.cpp` (Primary)
- [ ] `include/faults/fault_manager.hpp` (May need interface update)
- [ ] `tests/unit/fault_manager_test.cpp` (Test updates)

### Out of Scope
- Wheel speed monitor logic
- Brake monitor implementation
- Data validator
- Public API changes

**Reason**: These are functioning correctly; focusing on fault manager only.

---

## 3. CURRENT STATE ANALYSIS

### Issues Identified

**Issue 1: Complex Fault Aggregation Logic**
- **Problem**: evaluate() method has multiple conditional branches
- **Impact**: Hard to understand, difficult to test edge cases
- **Metrics**: 
  - Cyclomatic Complexity: 8 (Target: <5)
  - Lines of Code: 25 LOC

**Issue 2: Validation Precedence Not Immediately Clear**
- **Problem**: Order of operations in get_overall_status() is implicit
- **Impact**: Future changes risk breaking precedence rules
- **Metrics**: Comment clarity: 2/5

**Issue 3: Redundant Validation Calls**
- **Problem**: get_validation_error() called twice in different methods
- **Impact**: Potential performance issue if validation becomes expensive
- **Metrics**: Duplication: 2 occurrences

### Code Metrics
| Metric | Current | Target |
|--------|---------|--------|
| Cyclomatic Complexity | 8 | < 5 |
| Code Duplication | 2 calls | 1 call |
| Lines in evaluate() | 25 | < 20 |
| Test Coverage | 100% | 100% |

---

## 4. DESIRED STATE

### End Goal
The fault manager should be refactored to:
1. Make validation precedence explicit
2. Reduce cyclomatic complexity through better abstraction
3. Eliminate redundant validation calls
4. Improve code comments explaining the logic flow

### Key Improvements Expected
1. **Clarity**: Anyone reading the code can understand fault precedence at a glance
2. **Simplicity**: Reduced branch count makes logic easier to verify
3. **Efficiency**: Validation runs once, result is cached
4. **Testability**: Easier to write tests for edge cases

### Target Metrics
| Metric | Target |
|--------|--------|
| Cyclomatic Complexity | 4 (vs 8) |
| Validation Calls | 1 (vs 2) |
| Main Method LOC | 18 (vs 25) |
| Test Coverage | 100% |
| Comments Clarity | 5/5 |

---

## 5. CONSTRAINTS & RULES

### Must Keep (Backward Compatibility)
- [ ] Public API unchanged (`FaultManager` class interface)
- [ ] Function signatures identical
  - `evaluate(sample)` returns `std::set<FaultType>`
  - `get_overall_status(sample)` returns `OverallStatus`
- [ ] Behavior identical to current
- [ ] Return types unchanged
- [ ] Performance within 5% of current

### Must Not
- ✗ Change evaluate() return type
- ✗ Change get_overall_status() return type
- ✗ Add new public methods
- ✗ Change constructor
- ✗ Introduce breaking changes

### Must Do
- ✓ Maintain all 12 existing tests (FLT tests)
- ✓ Pass all 52 tests in full suite
- ✓ Update code comments
- ✓ Add explanatory comments for precedence
- ✓ Document any new helper methods
- ✓ Update architecture documentation

---

## 6. REFACTORING PATTERNS & TECHNIQUES

### Recommended Approaches
- [ ] **Extract Method**: Create helper methods for clarity
  - `_is_validation_error(sample)` - explicit validation check
  - `_collect_faults(sample)` - dedicated fault collection
  - `_determine_status(faults, has_error)` - status logic

- [ ] **Reduce Cyclomatic Complexity**: Simplify conditionals
  - Use early returns for error cases
  - Use guard clauses for invalid data
  
- [ ] **Cache Validation Result**: Avoid redundant calls
  - Call get_validation_error() once
  - Pass result to helper methods

- [ ] **Improve Naming**: Make intent clear
  - Rename confusing variable names
  - Use descriptive method names

### Anti-Patterns to Avoid
- ✗ Don't create god methods
- ✗ Don't use cryptic variable names
- ✗ Don't duplicate validation logic
- ✗ Don't create complex nested conditions

### Refactored Structure Example
```cpp
// Current (complex)
std::set<FaultType> evaluate(const Sample& sample) {
    // 25 lines with 8 branches
    // Validation check mixed with fault collection
}

// Target (simple)
std::set<FaultType> evaluate(const Sample& sample) {
    FaultType error = _get_cached_validation(sample);
    if (error != FaultType::NO_FAULT) {
        return {error};
    }
    return _collect_faults(sample);  // 5-7 lines, 2 branches)
}

private:
    std::set<FaultType> _collect_faults(const BrakeWheelSpeedSample& s) {
        // Clean fault collection logic
    }
```

---

## 7. TESTING REQUIREMENTS

### Pre-Refactoring
- [x] All 52 tests passing (baseline)
- [x] Code coverage: 100%
- [x] Performance baseline established
- [x] FLT tests: 12/12 passing

### During Refactoring
- [ ] After each method extraction, run full test suite
- [ ] No regression in functionality (52/52 must pass)
- [ ] All edge cases remain covered
- [ ] Performance check after optimization

### Post-Refactoring
- [ ] All 52 tests pass
- [ ] Code coverage: ≥ 100%
- [ ] No performance regression
- [ ] New helper methods have clear documentation
- [ ] FaultManager behavior identical to before

### Specific Test Validation
```
Critical Tests (Must Pass):
✓ TC-FLT-001: NoFaultsYieldsNormalStatus
✓ TC-FLT-002: MultipleFaults
✓ TC-FLT-003: FaultRecovery
✓ TC-FLT-004: FaultPersistence
✓ TC-FLT-005: PartialFaultRecovery
✓ TC-FLT-006: SingleFaultYieldsFaultStatus
✓ TC-FLT-007: BrakeAndWheelFaults
✓ TC-FLT-008: ValidationErrorTakesPrecedence
✓ TC-FLT-009: InvalidDataStatus
✓ TC-FLT-010: RequirementsExample
✓ TC-FLT-011: LargeBrakingSpreadFault
✓ TC-FLT-012: EmptyFaultsIsNormal

Success Criteria:
- [ ] All 52 tests passing
- [ ] Coverage: ≥ 100%
- [ ] No new test failures
- [ ] FLT tests: 12/12 passing
```

---

## 8. QUALITY CRITERIA

### Code Quality Standards
- **Style**: C++17, follow existing patterns
- **Naming**: `_private_methods` for helpers, `camelCase` for variables
- **Comments**: Explain WHY, not WHAT
- **Documentation**: Update function doc comments
- **Error Handling**: Maintain current strategy

### Metrics Threshold
| Metric | Current | Target | Pass/Fail |
|--------|---------|--------|-----------|
| Cyclomatic Complexity | 8 | < 5 | ✓ |
| Duplication | 2x | 0x | ✓ |
| LOC in evaluate() | 25 | < 20 | ✓ |
| Test Coverage | 100% | ≥ 100% | ✓ |
| Comment Quality | 2/5 | 5/5 | ✓ |

### Code Review Checklist
- [ ] No magic numbers introduced
- [ ] All variables have clear names
- [ ] Comments explain precedence rules
- [ ] No hidden side effects
- [ ] No performance regression
- [ ] Documentation updated

---

## 9. DELIVERABLES

### What Needs to Be Delivered
- [ ] Refactored `fault_manager.cpp`
- [ ] Updated `fault_manager.hpp` (with doc comments)
- [ ] All 52 tests still passing
- [ ] Refactoring summary (what changed and why)
- [ ] Before/after code metrics comparison
- [ ] Updated code comments

### Documentation to Update
- [ ] Class and method doc comments
- [ ] Explain precedence rules clearly
- [ ] Document any new helper methods
- [ ] Add complexity reduction notes

---

## 10. IMPLEMENTATION WORKFLOW

### Phase 1: Preparation (1 hour)
- [ ] Read current implementation thoroughly
- [ ] Understand all FLT test cases
- [ ] Document baseline metrics
- [ ] Plan extraction points
- [ ] Identify helper methods needed

### Phase 2: Refactoring (2-3 hours)
- [ ] Extract `_get_cached_validation()` method
- [ ] Extract `_collect_faults()` method
- [ ] Extract `_determine_status()` method
- [ ] Simplify main method bodies
- [ ] Add documentation comments
- [ ] Commit logical changes

### Phase 3: Testing (1 hour)
- [ ] Run full test suite
- [ ] Verify all 52 tests pass
- [ ] Check code coverage
- [ ] Performance benchmark
- [ ] Code review pass

### Phase 4: Documentation (30 mins)
- [ ] Summarize changes in commit message
- [ ] Document complexity improvements
- [ ] Update architecture notes if needed

---

## 11. CONTEXT & BACKGROUND

### Why This Refactoring
- **Technical Debt**: Rising complexity making code hard to understand
- **Maintenance Risk**: High complexity increases bug risk
- **Team Feedback**: Code reviews taking longer than expected
- **Future Work**: Easier to add new fault types with cleaner code

### Related Work
- Requirements: All 21 implemented ✓
- Tests: All 52 passing ✓
- Code Review: REVIEW.md process created ✓
- This Refactoring: Improves maintainability

### Historical Context
- Code written in one pass (no previous refactoring)
- Structure is functionally correct
- Optimization needed for long-term maintenance

---

## 12. SUCCESS METRICS & VERIFICATION

### How We Know It's Done
```
Development Verification:
✓ Cyclomatic complexity: 8 → 4
✓ Code duplication: 2 → 0
✓ All 52 tests pass
✓ Test coverage: 100%
✓ No performance regression
✓ Code comments clear and accurate

Code Review Verification:
✓ No logic changes (behavior identical)
✓ API unchanged (backward compatible)
✓ Documentation updated
✓ Helper methods well-named
✓ Precedence rules explicit

QA Verification:
✓ All test scenarios still work
✓ Edge cases still covered
✓ No regressions found
✓ Performance acceptable
```

### Sign-Off Criteria
- [ ] Developer: All metrics met, tests pass
- [ ] Code Reviewer: Approved, logic verified
- [ ] QA: Full regression testing passed
- [ ] Team Lead: Ready for production

---

## 13. RISKS & MITIGATION

| Risk | Probability | Impact | Mitigation |
|------|-------------|--------|-----------|
| Logic Error | Low | High | Run tests after each change |
| Performance Regression | Low | Medium | Benchmark before/after |
| Breaking Change | Low | High | Keep API identical |
| Incomplete Testing | Medium | High | Test all FLT scenarios |

### Risk Management
- **Backup**: Current code is in git, easy to revert
- **Testing**: 52 tests provide safety net
- **Review**: Code review before merge
- **Validation**: Metrics confirm success

---

## 14. TIMELINE & ESTIMATE

### Effort Estimation
- **Complexity Level**: MEDIUM
- **Estimated Hours**: 4-5 hours
  - Phase 1: 1 hour
  - Phase 2: 2-3 hours
  - Phase 3: 1 hour
  - Phase 4: 30 mins
- **Priority Level**: MEDIUM
- **Deadline**: When ready (no hard deadline)

### Milestones
- Phase 1 Complete: [+1 hour]
- Phase 2 Complete: [+3 hours]
- Testing Complete: [+1 hour]
- Ready for Review: [+0.5 hours]

---

## 15. COMMUNICATION & HANDOFF

### Team Members
- **Developer**: Working on refactoring
- **Code Reviewer**: Verifies logic unchanged
- **QA**: Validates test results

### Communication
- Daily standup: Brief status update
- Code review: Link to PR, explain changes
- Final approval: Confirm metrics and tests

---

## Before/After Comparison

### BEFORE
```cpp
std::set<FaultType> FaultManager::evaluate(const BrakeWheelSpeedSample& sample) const {
    std::set<FaultType> all_faults;

    // Check data validity first (DAT-03: Invalid data short-circuits fault logic)
    FaultType validation_error = validator_.get_validation_error(sample);
    if (validation_error != FaultType::NO_FAULT) {
        all_faults.insert(validation_error);
        return all_faults;  // Return only validation error, don't evaluate other faults
    }

    // FLT-02: Aggregate faults from all monitors
    auto brake_faults = brake_monitor_.evaluate(sample);
    auto wheel_faults = wheel_speed_monitor_.evaluate(sample);

    all_faults.insert(brake_faults.begin(), brake_faults.end());
    all_faults.insert(wheel_faults.begin(), wheel_faults.end());

    // If no faults, explicitly add NO_FAULT
    if (all_faults.empty()) {
        all_faults.insert(FaultType::NO_FAULT);
    }

    return all_faults;
}
// Cyclomatic Complexity: 5
// Methods: 1 public, 0 private
```

### AFTER (PROPOSED)
```cpp
std::set<FaultType> FaultManager::evaluate(const BrakeWheelSpeedSample& sample) const {
    // Priority 1: Validation (DAT-03) - short-circuits everything else
    FaultType validation_error = _get_validation_error(sample);
    if (validation_error != FaultType::NO_FAULT) {
        return {validation_error};
    }

    // Priority 2: Collect faults from all monitors (FLT-02)
    auto all_faults = _collect_all_faults(sample);
    
    // Priority 3: Ensure we don't return empty set
    if (all_faults.empty()) {
        all_faults.insert(FaultType::NO_FAULT);
    }

    return all_faults;
}

private:
    FaultType _get_validation_error(const BrakeWheelSpeedSample& sample) const {
        return validator_.get_validation_error(sample);
    }

    std::set<FaultType> _collect_all_faults(const BrakeWheelSpeedSample& sample) const {
        std::set<FaultType> all_faults;
        
        auto brake_faults = brake_monitor_.evaluate(sample);
        all_faults.insert(brake_faults.begin(), brake_faults.end());
        
        auto wheel_faults = wheel_speed_monitor_.evaluate(sample);
        all_faults.insert(wheel_faults.begin(), wheel_faults.end());
        
        return all_faults;
    }

// Cyclomatic Complexity: 3
// Methods: 1 public, 2 private
// Clarity: Precedence rules explicit in comments
```

---

## Key Improvements
| Aspect | Before | After | Benefit |
|--------|--------|-------|---------|
| Cyclomatic Complexity | 5 | 3 | Easier to understand |
| Lines in main method | 20 | 12 | Better readability |
| Helper Methods | 0 | 2 | Clearer intent |
| Comments | Generic | Specific | Better explanation |
| Precedence Clarity | Implicit | Explicit | Prevents bugs |

---

## Usage Instructions for This Prompt

1. **For Developer**: Follow workflow phases systematically
2. **For Code Reviewer**: Verify metrics meet targets
3. **For QA**: Validate all 52 tests pass
4. **For Project Manager**: Track against timeline

**Template Status**: ✅ Ready to use as refactoring guide

