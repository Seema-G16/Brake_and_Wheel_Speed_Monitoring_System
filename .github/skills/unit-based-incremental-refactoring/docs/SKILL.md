---
name: unit-based-incremental-refactoring
description: "Skill for safe, incremental code refactoring using unit-based architecture with continuous validation. Breaks refactoring into small logical units (functions, 3-30 LOC) executed and validated independently. Reduces unpredictability through covering tests & automation; increases reportability through metrics, traceability, and structured workflows. Use when: refactoring production code; improving code quality; reducing complexity; applying design patterns."
argument-hint: "Refactor [component] using unit-based workflow; automate validation; generate refactoring report with metrics and traceability"
user-invocable: true
---

# Unit-Based Incremental Refactoring Skill

## Overview

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
$ python3 scripts/plan_refactoring.py \
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
$ python3 scripts/generate_refactoring_report.py \
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
python3 scripts/plan_refactoring.py \
    --component "BrakeMonitor" \
    --input-file src/brake_monitor.cpp \
    --test-file tests/unit/brake_monitor_test.cpp \
    --output refactoring_plan.json
```

**1.3 Map Units to Tests**

See [Unit-Test Mapping Reference](../references/unit_test_mapping.md) for guidelines.

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

**Validation Guarantee**: If ANY code path breaks, a covering test WILL fail.

---

## Scripts & Automation

| Script | Purpose | Location |
|--------|---------|----------|
| `plan_refactoring.py` | Analyze component, create plan | `../scripts/` |
| `log_unit_completion.py` | Record unit metrics | `../scripts/` |
| `generate_refactoring_report.py` | Create final report | `../scripts/` |
| `validate_unit_tests.ps1` | Automated validation | `../scripts/` |

See [Scripts Reference](../references/scripts_reference.md) for detailed usage.

---

## References & Standards

| Reference | Purpose | Location |
|-----------|---------|----------|
| Unit-Test Mapping | How to identify covering tests | `../references/unit_test_mapping.md` |
| Unit Size Guidelines | Size/time/test count per unit | `../references/unit_size_guidelines.md` |
| Validation Standards | What "passing validation" means | `../references/validation_standards.md` |
| Metrics Definitions | How metrics are measured | `../references/metrics_definitions.md` |
| Scripts Reference | Detailed script usage | `../references/scripts_reference.md` |

---

## Assets & Templates

| Asset | Purpose | Location |
|-------|---------|----------|
| Refactoring Plan Template | Skeleton for planning | `../assets/refactoring_plan_template.json` |
| Unit Log Template | Template for unit logs | `../assets/unit_log_template.json` |
| Report Template | HTML template for reports | `../assets/report_template.html` |
| Checklist | Pre/during/post checklist | `../assets/refactoring_checklist.md` |

---

## Evaluation Criteria

### Pre-Refactoring
- `../evals/eval_readiness.py` - Is refactoring safe to start?
- `../evals/eval_unit_size.py` - Is unit size appropriate?

### During-Refactoring
- Compilation must succeed (no errors)
- Covering tests must pass (3-5 tests)
- Full suite must pass (52 tests)

### Post-Refactoring
- `../evals/eval_metrics.py` - Did metrics improve?
- `../evals/eval_production_readiness.py` - Is component production-ready?

---

## Reducing Unpredictability

1. **Small Unit Scope** (3-30 LOC) → Easy debugging
2. **Covering Tests** (3-5 per function) → Guaranteed regression detection
3. **Continuous Validation** (every 15-90 min) → No surprise failures
4. **Automated Workflows** → Consistency enforced

---

## Troubleshooting

### Test fails after refactoring
1. Identify failing test
2. Review test expectations
3. Fix code or test
4. Re-run full suite
5. Debug time: 5-15 minutes

### Compilation error
1. Check syntax
2. Verify interface unchanged
3. Review includes
4. If fails, revert & debug

### Full suite regression
1. Revert current unit
2. Full suite should pass
3. Identify dependency
4. Refactor more carefully
5. Recovery time: <15 minutes

---

## Success Criteria

✅ **Refactoring is successful when:**
- All tests passing (52/52)
- Metrics improved (complexity ↓, coverage ↑)
- No regressions introduced
- Changes fully documented
- Team confident

---

## Next Steps

See [../README.md](../README.md) for complete file index and quick start guide.
