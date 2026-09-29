# Refactoring Checklist

## Pre-Refactoring Phase

### Environment Setup
- [ ] All code committed to version control
- [ ] No uncommitted changes
- [ ] Latest main branch pulled
- [ ] Build directory clean (rm -rf build)
- [ ] CMake cache cleared

### Baseline Establishment
- [ ] All tests running and passing (52/52 baseline)
- [ ] Build succeeds without errors
- [ ] No compiler warnings (address first)
- [ ] Refactoring plan created and reviewed
- [ ] Covering tests mapped for each function

### Plan Documentation
- [ ] Component identified and analyzed
- [ ] Functions listed with LOC and complexity
- [ ] Covering tests identified (3-5 per function)
- [ ] Estimated time per unit calculated
- [ ] Risk assessment completed
- [ ] Success metrics defined (before/after)

### Team Communication
- [ ] Team notified of refactoring plan
- [ ] Code review assigned
- [ ] Merge strategy decided
- [ ] Timeline communicated

---

## Per-Unit Refactoring Phase

### Unit Selection & Understanding
- [ ] One function selected for refactoring
- [ ] Function code reviewed thoroughly
- [ ] Covering tests reviewed and understood
- [ ] Current behavior documented
- [ ] Refactoring approach planned

### Refactoring Execution
- [ ] **ONE** logical change identified
- [ ] **ONE** function edited only
- [ ] Function interface UNCHANGED
- [ ] Comments added/updated for clarity
- [ ] Code follows project style guide
- [ ] No debug code left
- [ ] No commented-out code left

### Pre-Validation Review
- [ ] Code self-reviewed
- [ ] Changes make sense
- [ ] No obvious regressions
- [ ] Ready for compilation

### Validation: Compilation
- [ ] `cmake --build .` runs
- [ ] Exit code: 0 (success)
- [ ] No compilation errors
- [ ] No linking errors
- [ ] No undefined reference errors
- [ ] Executable/library created successfully

### Validation: Covering Tests
- [ ] Covering tests identified and noted
- [ ] `ctest -R "test_pattern"` runs
- [ ] All covering tests pass
- [ ] No assertion failures
- [ ] Execution time < 10ms
- [ ] Test output reviewed

### Validation: Full Test Suite
- [ ] `ctest --verbose` runs
- [ ] All 52 tests pass
- [ ] No regressions detected
- [ ] Test execution time < 30ms
- [ ] Output reviewed for warnings

### Post-Validation Review
- [ ] Metrics recorded
  - [ ] Complexity before/after
  - [ ] LOC before/after
  - [ ] Test pass rate
- [ ] Time spent recorded
- [ ] Issues/notes documented

### Unit Completion
- [ ] Unit log created with metrics
- [ ] Status marked as PASS/FAIL
- [ ] Ready for next unit or troubleshooting

---

## Troubleshooting Phase (If Failures Occur)

### Compilation Failure
- [ ] Error message read carefully
- [ ] Function signature reviewed (unchanged?)
- [ ] Include paths verified
- [ ] Dependencies checked
- [ ] CMakeLists.txt reviewed
- [ ] Attempted fix applied
- [ ] Rebuild and retest

### Test Failure
- [ ] Failed test identified
- [ ] Test expectations understood
- [ ] Code logic reviewed
- [ ] Covering test specific logic checked
- [ ] Attempted fix applied
- [ ] Covering tests re-run
- [ ] Full suite re-run

### Regression Detected
- [ ] Failing tests identified (not in covering tests!)
- [ ] Current unit reverted temporarily
- [ ] Full suite re-run (should pass)
- [ ] Dependency issue identified
- [ ] Unit refactored with dependency in mind
- [ ] Re-validated

---

## Post-Refactoring Phase

### Unit-by-Unit Validation Complete
- [ ] All units have passed validation
- [ ] All metrics recorded
- [ ] All logs complete

### Component Final Validation
- [ ] Full rebuild from scratch (`cmake --build . --clean-first`)
- [ ] All 52 tests pass
- [ ] No regressions
- [ ] Metrics meet targets

### Report Generation
- [ ] Refactoring report generated
- [ ] HTML/JSON report created
- [ ] Metrics comparison reviewed
- [ ] Timeline documented
- [ ] Risk assessment finalized

### Code Review Preparation
- [ ] All changes staged/committed
- [ ] Commit message clear and concise
- [ ] Pull request created with:
  - [ ] Refactoring goal stated
  - [ ] Metrics before/after
  - [ ] Test results attached
  - [ ] Link to report
- [ ] Ready for code review

### Code Review & Merge
- [ ] Peer review completed
- [ ] Feedback addressed
- [ ] Tests pass on CI/CD (if applicable)
- [ ] Branch merged to main
- [ ] Merged commit verified

### Post-Merge Verification
- [ ] Main branch pulled locally
- [ ] All tests pass locally
- [ ] No regressions on main
- [ ] Refactoring verified complete

### Documentation & Closure
- [ ] Changes documented
- [ ] Team notified of completion
- [ ] Lessons learned captured
- [ ] Metrics stored for future reference
- [ ] **Refactoring marked COMPLETE** ✓

---

## Risk Mitigation Checklist

### Before Each Unit
- [ ] Backup taken (git branch)
- [ ] Rollback plan clear
- [ ] Estimated time reasonable (<90 min)
- [ ] Function scope appropriate

### During Unit Refactoring
- [ ] Only one function edited
- [ ] Interface signature unchanged
- [ ] One logical change made
- [ ] No "while I'm here" changes
- [ ] No large refactors mixed in

### During Validation
- [ ] Compilation must succeed
- [ ] Covering tests must pass
- [ ] Full suite must pass (no regressions!)
- [ ] All checkpoints must pass

### If Anything Fails
- [ ] Revert immediately (git checkout)
- [ ] Restart from clean state
- [ ] Identify root cause
- [ ] Try again with better understanding

### Risk Tracking
- [ ] Issues logged with timestamp
- [ ] Resolution documented
- [ ] Lessons captured for next unit

---

## Quality Assurance Checklist

### Code Quality
- [ ] Complexity improved or maintained
- [ ] LOC improved or maintained
- [ ] Duplication reduced (if applicable)
- [ ] Code is more readable
- [ ] Naming is clear and consistent
- [ ] Comments are helpful

### Test Quality
- [ ] Coverage maintained or improved
- [ ] Covering tests still valid
- [ ] No test flakiness introduced
- [ ] Test execution time unchanged

### Documentation Quality
- [ ] Code comments updated
- [ ] Function documentation updated
- [ ] Refactoring documented
- [ ] Changes explain WHY not just what

---

## Sign-Off Criteria

### Refactoring is COMPLETE when:
- ✓ All units refactored and validated
- ✓ All 52 tests passing
- ✓ Zero regressions detected
- ✓ All metrics meet targets
- ✓ Code reviewed and approved
- ✓ Merged to main branch
- ✓ Report generated and archived
- ✓ Team notified

### Each Unit is COMPLETE when:
- ✓ Code compiled successfully
- ✓ All covering tests pass (3-5 tests)
- ✓ Full test suite passes (52 tests)
- ✓ Metrics recorded
- ✓ Log entry created
- ✓ No issues or regressions
- ✓ Ready for next unit

---

## Post-Completion Review

### Retrospective
- [ ] What went well?
- [ ] What was difficult?
- [ ] What would you do differently?
- [ ] Did actual time match estimate?
- [ ] Were there unexpected issues?

### Metrics Review
- [ ] Did complexity actually reduce?
- [ ] Was test coverage maintained?
- [ ] Did LOC reduce as expected?
- [ ] Performance impact (if any)?

### Knowledge Transfer
- [ ] Lessons documented for team
- [ ] Process improvements identified
- [ ] Templates updated if needed
- [ ] Shared with team for future refactorings

---

## Helpful Commands Reference

### Refactoring Workflow
```bash
# Step 1: Prepare
python3 .github/skills/unit-based-incremental-refactoring/scripts/plan_refactoring.py \
    --component "BrakeMonitor" \
    --input-file src/brake_monitor.cpp \
    --test-file tests/unit/brake_monitor_test.cpp

# Step 2: For each unit
cd build && cmake --build .
ctest -R "TC_PATTERN"
ctest --verbose
python3 ../scripts/log_unit_completion.py --unit-id 1 --status "PASS"

# Step 3: Report
python3 ../scripts/generate_refactoring_report.py \
    --component "BrakeMonitor" \
    --log-file refactoring_log.json
```

### Git Commands
```bash
# Create feature branch
git checkout -b refactor/brake-monitor

# Commit changes
git commit -m "Refactor: BrakeMonitor - reduce complexity (unit 1/5)"

# Create pull request
git push origin refactor/brake-monitor
# (then create PR on GitHub)
```

