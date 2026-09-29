---
name: unit-based-incremental-refactoring
description: "Skill for safe, incremental code refactoring using unit-based architecture with continuous validation. Breaks refactoring into small logical units (functions, 3-30 LOC) executed and validated independently. Reduces unpredictability through covering tests & automation; increases reportability through metrics, traceability, and structured workflows. Use when: refactoring production code; improving code quality; reducing complexity; applying design patterns."
argument-hint: "Refactor [component] using unit-based workflow; automate validation; generate refactoring report with metrics and traceability"
user-invocable: true
---

# ⚠️ Please See: [docs/SKILL.md](./docs/SKILL.md)

This file has been moved to the **docs/** folder for better organization.

**All files are now organized as follows:**

```
unit-based-incremental-refactoring/
├─ INDEX.md                  ← Start here for folder navigation
├─ docs/
│  ├─ SKILL.md               ← Main documentation (MOVED HERE)
│  ├─ README.md              ← Complete guide (MOVED HERE)
│  └─ REORGANIZATION_SUMMARY.md
├─ scripts/
├─ references/
├─ assets/
└─ evals/
```

**📖 Go to:** [docs/SKILL.md](./docs/SKILL.md) for the main documentation.

---

## Quick Navigation

- **Start Here**: [INDEX.md](./INDEX.md)
- **Main Docs**: [docs/SKILL.md](./docs/SKILL.md)
- **Complete Guide**: [docs/README.md](./docs/README.md)
- **All References**: [references/](./references/)
- **Templates**: [assets/](./assets/)
- **Scripts**: [scripts/](./scripts/)
- **Evaluations**: [evals/](./evals/)

---

*This is a redirect file. The actual SKILL.md documentation has been moved to `docs/SKILL.md` for better organization.*

Safe, incremental refactoring of production code through **unit-based architecture** with:
- ✓ Continuous validation (every 15-90 minutes)
- ✓ Automated test coverage verification
- ✓ Structured refactoring workflow
- ✓ Comprehensive metrics & reporting
- ✓ Reduced risk through small, manageable units
- ✓ Clear traceability from requirements → tests → code → metrics

**Prevents**: Surprise regressions, untracked changes, unclear success criteria, risk creep  
**Enables**: Safe production refactoring, daily workflow integration, team collaboration, quality confidence

---

## When to Use This Skill

### ✅ Use This Skill When:
- Refactoring production code (reduce complexity, improve quality)
- Making architectural improvements (patterns, design)
- Reducing technical debt (eliminate duplication, simplify logic)
- Optimizing for readability, maintainability, or performance
- Need to track changes with full traceability and metrics
- Working with untrusted code that needs validation at each step
- Team needs to understand what changed and why

### ❌ Don't Use This Skill When:
- Writing brand-new code (use normal development workflow)
- Making trivial changes (comments, formatting only)
- Urgent hotfixes needed immediately (too slow, too structured)
- No test suite exists (setup tests first)

---

## Quick Start: Three-Step Process

### Step 1: Prepare (15-30 minutes)
```
$ python3 .github/skills/unit-based-refactoring/scripts/plan_refactoring.py \
    --component "BrakeMonitor" \
    --lines-of-code 100 \
    --functions 5

OUTPUT: Refactoring plan with 5 phases (one function each)
        Estimated time: 2-4 hours
        Risk assessment: LOW
```

### Step 2: Execute (per unit: 15-90 minutes)
```
FOR EACH FUNCTION:
  1. Edit function in source code
  2. cmake --build .
  3. Run covering tests (ctest -R "unit_1")
  4. Run full suite (ctest --verbose)
  5. Log results with: python3 scripts/log_unit_completion.py
```

### Step 3: Report (10-15 minutes)
```
$ python3 .github/skills/unit-based-refactoring/scripts/generate_refactoring_report.py \
    --component "BrakeMonitor" \
    --output-format html

OUTPUT: HTML report with:
        - Metrics before → after
        - Test coverage & traceability
        - Timeline & effort
        - Risk assessment
        - Recommendations
```

---

## Detailed Workflow

### Phase 1: Plan & Analyze

**1.1 Review Component**
```bash
# Read production code
cat src/brake_monitor.cpp

# Review test coverage
grep -r "TC_BRK" tests/unit/brake_monitor_test.cpp

# Run existing tests
cd build && ctest --verbose
```

**1.2 Create Refactoring Plan**
```bash
python3 .github/skills/unit-based-refactoring/scripts/plan_refactoring.py \
    --component "BrakeMonitor" \
    --input-file src/brake_monitor.cpp \
    --test-file tests/unit/brake_monitor_test.cpp \
    --output refactoring_plan.json
```

**1.3 Map Units to Tests**

See [Unit-Test Mapping Reference](./references/unit_test_mapping.md) for guidelines.

```bash
# Verify coverage
python3 .github/skills/unit-based-refactoring/scripts/validate_coverage.py \
    --plan refactoring_plan.json
```

### Phase 2: Execute Refactoring (Unit-by-Unit)

**For each unit** (typically 3-6 per component):

**2.1 Select Unit** (function)
```json
{
  "unit_id": 1,
  "function": "is_braking",
  "current_lines": 2,
  "covering_tests": ["TC-BRK-001", "TC-BRK-003", "TC-BRK-012"],
  "estimated_time_minutes": 15
}
```

**2.2 Refactor**
- Edit function in source code
- Keep interface unchanged
- Make one logical improvement

**2.3 Validate Immediately**
```bash
# Step 1: Compile
cd build && cmake --build .
# ✓ Must succeed with no errors

# Step 2: Covering tests
ctest -R "TC_BRK_001|TC_BRK_003|TC_BRK_012" --verbose
# ✓ Must pass (3/3 tests)

# Step 3: Full regression check
ctest --verbose
# ✓ Must pass (all 52 tests)

# Step 4: Log completion
python3 .github/skills/unit-based-refactoring/scripts/log_unit_completion.py \
    --unit-id 1 \
    --status "PASS" \
    --time-minutes 15 \
    --complexity-before 2 \
    --complexity-after 1
```

**2.4 Record Metrics**
```
Unit 1: is_braking()
├─ Time: 15 minutes ✓
├─ Tests: 3/3 pass ✓
├─ Full suite: 52/52 pass ✓
├─ Complexity: 2 → 1 (50% reduction)
├─ Status: COMPLETE ✓
└─ Risk: LOW
```

### Phase 3: Validate & Report

**3.1 Full Component Validation**
```bash
# Rebuild everything
cd build && cmake --build . --clean-first

# Run all tests
ctest --verbose --output-on-failure

# Generate coverage report
python3 .github/skills/unit-based-refactoring/scripts/generate_coverage_report.py
```

**3.2 Generate Final Report**
```bash
python3 .github/skills/unit-based-refactoring/scripts/generate_refactoring_report.py \
    --component "BrakeMonitor" \
    --refactoring-log refactoring_log.json \
    --output-format html \
    --output refactoring_report.html
```

**3.3 Report Contents**
- ✓ Metrics comparison (before → after)
- ✓ Test coverage & traceability matrix
- ✓ Unit-by-unit timeline & effort
- ✓ Risk assessment & mitigation
- ✓ Code quality improvements
- ✓ Recommendations for next refactorings

---

## Key Concepts

### Unit Hierarchy

```
LEVEL 1: Test Case
├─ One scenario per test
├─ <1ms execution time
└─ Example: TC-BRK-001 tests braking=true

LEVEL 2: Function (Unit)
├─ 3-30 LOC per function
├─ 3-5 covering tests
├─ 15-90 min to refactor
└─ Example: is_braking() with 3 tests

LEVEL 3: Component
├─ 50-150 LOC total
├─ 3-6 functions
├─ 12-15 tests total
├─ 2-4 hours to refactor
└─ Example: BrakeMonitor class
```

### Covering Tests

**Definition**: 3-5 test cases that exercise ALL code paths in a function.

**Example**: 
```cpp
// Function: is_braking()
bool is_braking(const Sample& s) {
    return s.brakePedal && s.speed > 0.0;
}

// Covering tests:
TC-BRK-001: Both true → returns true
TC-BRK-003: Pedal false → returns false  
TC-BRK-012: Speed 0 → returns false
```

**Validation Guarantee**: If ANY code path breaks, a covering test WILL fail.

### Incremental Validation

```
Baseline: 52/52 tests passing
         ↓
Refactor Unit 1
         ↓
Validation: Unit 1 tests pass (3/3) + Full suite (52/52) → PROCEED
         ↓
Refactor Unit 2
         ↓
Validation: Unit 2 tests pass (4/4) + Full suite (52/52) → PROCEED
         ↓
... continue ...
         ↓
FINAL: 52/52 still passing (NO REGRESSIONS) → SUCCESS ✓
```

---

## Scripts & Automation

### Available Scripts

| Script | Purpose | Input | Output |
|--------|---------|-------|--------|
| `plan_refactoring.py` | Analyze component, create refactoring plan | Source file, test file | Refactoring plan JSON |
| `validate_coverage.py` | Verify test coverage is adequate | Refactoring plan | Coverage report, warnings |
| `log_unit_completion.py` | Record unit completion metrics | Unit ID, test results, metrics | Log entry |
| `generate_coverage_report.py` | Create test coverage matrix | Test file, source file | Coverage HTML report |
| `generate_refactoring_report.py` | Create final comprehensive report | Refactoring log | Metrics + timeline + recommendations |
| `validate_unit_tests.ps1` | Automated unit test validation | None | Pass/fail report |

**Location**: `.github/skills/unit-based-refactoring/scripts/`

See [Scripts Reference](./references/scripts_reference.md) for detailed usage.

---

## References & Standards

| Reference | Purpose |
|-----------|---------|
| [Unit-Test Mapping](./references/unit_test_mapping.md) | How to identify covering tests for each function |
| [Unit Size Guidelines](./references/unit_size_guidelines.md) | Size/time/test count per unit |
| [Validation Standards](./references/validation_standards.md) | What "passing validation" means |
| [Metrics Definitions](./references/metrics_definitions.md) | How complexity, coverage, etc. are measured |
| [Scripts Reference](./references/scripts_reference.md) | Detailed usage of all automation scripts |

---

## Assets & Templates

| Asset | Purpose |
|-------|---------|
| [Refactoring Plan Template](./assets/refactoring_plan_template.json) | Skeleton for refactoring plans |
| [Unit Log Template](./assets/unit_log_template.json) | Template for recording unit completion |
| [Report Template](./assets/refactoring_report_template.html) | HTML template for final report |
| [Checklist](./assets/refactoring_checklist.md) | Pre/during/post refactoring checklist |

---

## Evaluation Criteria

### Pre-Refactoring Evals

**1. Is refactoring safe to proceed?**
```python
# eval_readiness.py checks:
✓ Test suite exists (>0 tests)
✓ All tests passing (baseline established)
✓ Code is compilable (no syntax errors)
✓ Functions are identifiable (clear boundaries)
```

**2. Is unit size appropriate?**
```python
# eval_unit_size.py checks:
✓ Function LOC: 3-30 (not too small, not too large)
✓ Covering tests: 3-5 (adequate coverage)
✓ Estimated time: 15-90 min (fits workflow)
✓ Risk level: LOW (appropriate scope)
```

### During-Refactoring Evals

**3. Does compilation succeed?**
```bash
cmake --build .
# PASS: No errors
# FAIL: Stop, fix errors
```

**4. Do covering tests pass?**
```bash
ctest -R "unit_pattern"
# PASS: Continue to full suite
# FAIL: Debug unit, don't proceed
```

**5. Does full suite still pass?**
```bash
ctest --verbose
# PASS: Unit validated, log completion
# FAIL: Regression detected, debug issue
```

### Post-Refactoring Evals

**6. Did metrics improve?**
```python
# eval_metrics.py checks:
✓ Complexity reduced (cyclomatic → lower)
✓ LOC maintained or reduced (no bloat)
✓ Coverage maintained (no gaps)
✓ Performance unchanged (no degradation)
```

**7. Is component production-ready?**
```python
# eval_production_readiness.py checks:
✓ All 52 tests passing
✓ No regressions introduced
✓ All files compilable
✓ Metrics improved
✓ Documentation updated
```

---

## Execution Example: Real Refactoring

### Scenario: Refactor BrakeMonitor (100 LOC, 5 functions)

```bash
# Step 1: Plan
$ python3 scripts/plan_refactoring.py --component "BrakeMonitor"
Output: 5-phase plan, 2-3.5 hour estimate, LOW risk

# Step 2: Execute Unit 1 (is_braking)
$ cd build && cmake --build .
Build: ✓ SUCCESS
$ ctest -R "TC_BRK_001|TC_BRK_003|TC_BRK_012"
Tests: ✓ 3/3 PASS
$ ctest --verbose
Suite: ✓ 52/52 PASS
$ python3 scripts/log_unit_completion.py --unit 1 --status PASS
Log: ✓ RECORDED

# Step 3: Execute Unit 2-5 (repeat for each)
... (similar for each function)

# Step 4: Report
$ python3 scripts/generate_refactoring_report.py --component "BrakeMonitor"
Report: refactoring_report.html (comprehensive metrics & timeline)
  ├─ Before: Complexity 12, LOC 100, Coverage 90%
  ├─ After: Complexity 6, LOC 95, Coverage 100%
  ├─ Effort: 3.2 hours (5 units × 15-90 min each)
  ├─ Risk: LOW (100% test coverage at each step)
  └─ Status: ✓ PRODUCTION READY
```

---

## Reducing Unpredictability

### How This Skill Reduces Risk:

1. **Small Unit Scope** (3-30 LOC)
   → If failure, only 1 function to debug (not whole component)
   → Debug time: 5-15 min (not 30-60 min)

2. **Covering Tests** (3-5 per function)
   → All code paths tested
   → Broken code detected immediately
   → Regression detection: Guaranteed

3. **Continuous Validation** (every 15-90 min)
   → Tests run after each unit
   → No "surprise" failures after days of work
   → Easy to identify which unit broke

4. **Automated Workflows** (scripts validate everything)
   → Human error reduced
   → Consistency enforced
   → Metrics collected automatically

### Risk Mitigation:

| Risk | Mitigation |
|------|-----------|
| Code breaks silently | ✓ Covering tests prevent this |
| Regressions sneak in | ✓ Full suite runs after each unit |
| Changes go untracked | ✓ Automated logging records everything |
| Complexity increases | ✓ Metrics tracked before/after |
| Time explodes | ✓ Units planned with time estimates |
| Team doesn't understand | ✓ Comprehensive reports explain changes |

---

## Increasing Reportability

### What Gets Reported:

**Metrics Report**
```
BEFORE REFACTORING:
├─ Cyclomatic Complexity: 12
├─ Lines of Code: 100
├─ Test Coverage: 90%
├─ Avg Function Size: 20 LOC
└─ Duplicate Code: 5%

AFTER REFACTORING:
├─ Cyclomatic Complexity: 6 (50% reduction)
├─ Lines of Code: 95 (5% reduction)
├─ Test Coverage: 100% (+10%)
├─ Avg Function Size: 19 LOC (better)
└─ Duplicate Code: 2% (reduced)
```

**Timeline Report**
```
Unit 1: is_braking()          15 min ✓
Unit 2: check_pressure()      45 min ✓
Unit 3: evaluate()            90 min ✓
Unit 4: Helper functions      25 min ✓
Unit 5: Full validation       10 min ✓
────────────────────────────────────
TOTAL: 185 min (3.1 hours)
```

**Traceability Matrix**
```
Function    → Covering Tests           → Code Changes    → Metrics
is_braking  → TC-BRK-001, 003, 012   → Extract constant → Complexity ↓
check_pressure → TC-BRK-004-008       → Refactor logic   → Coverage ↑
evaluate    → TC-BRK-001-012          → Simplify        → LOC ↓
```

**Sign-Off Report**
```
✓ Refactoring Complete
  ├─ Date: 2024-09-29
  ├─ Duration: 3.1 hours
  ├─ Units Completed: 5/5
  ├─ Tests Passing: 52/52
  ├─ Regressions: 0
  ├─ Metrics Improved: YES
  └─ Production Ready: YES
```

---

## Checklist: Before Starting Refactoring

- [ ] All tests currently passing (baseline established)
- [ ] Component clearly identified
- [ ] Functions to refactor listed
- [ ] Covering tests identified for each function
- [ ] Refactoring goals defined (complexity ↓, readability ↑, etc.)
- [ ] Time estimates created
- [ ] Team notified
- [ ] Backup/version control ready
- [ ] Scripts tested & working
- [ ] Report template prepared

---

## Troubleshooting

### Problem: Test fails after refactoring unit
**Solution**:
1. Identify which test failed
2. Review that test's expectations
3. Compare with refactored code
4. Fix code or test
5. Re-run: Both covering tests AND full suite
6. Log results

**Debug Time**: 5-15 minutes (small scope!)

### Problem: Compilation error
**Solution**:
1. Check syntax
2. Verify function signature unchanged
3. Review includes/dependencies
4. Run `cmake --build .` again
5. If still fails, revert unit & debug

**Prevention**: Keep function interface unchanged

### Problem: Full test suite fails (regression)
**Solution**:
1. Note which tests failed
2. Revert current unit only
3. Run full suite again (should pass)
4. Identify dependency issues
5. Refactor more carefully
6. Re-run unit validation

**Time to Recovery**: <15 minutes

---

## Integration with Refactoring Prompts

Use this skill with [REFACTORING_PROMPT_TEMPLATE.md](../../REFACTORING_PROMPT_TEMPLATE.md):

1. **Create prompt** using REFACTORING_PROMPT_TEMPLATE.md
2. **Plan refactoring** using `plan_refactoring.py`
3. **Execute units** following this skill's workflow
4. **Log metrics** using `log_unit_completion.py`
5. **Report results** using `generate_refactoring_report.py`

---

## Success Criteria

✅ **Refactoring is successful when:**
- All 52 tests passing (100%)
- Metrics improved (complexity ↓, coverage ↑, LOC optimal)
- No regressions introduced
- Changes fully documented & traced
- Team confident in code quality
- Time spent < estimate (efficiency improved)
- Report shows clear before/after

---

## References

- [Unit Size Guidelines](./references/unit_size_guidelines.md)
- [Covering Tests Mapping](./references/unit_test_mapping.md)
- [Validation Standards](./references/validation_standards.md)
- [Scripts Usage](./references/scripts_reference.md)
- [Metrics Definitions](./references/metrics_definitions.md)

---

## Related Documentation

- [UNIT_SIZE_ARCHITECTURE.md](../../UNIT_SIZE_ARCHITECTURE.md) - Deep technical explanation
- [UNIT_REFACTORING_WORKFLOW.md](../../UNIT_REFACTORING_WORKFLOW.md) - Detailed execution guide
- [REFACTORING_PROMPT_TEMPLATE.md](../../REFACTORING_PROMPT_TEMPLATE.md) - Creating quality prompts

