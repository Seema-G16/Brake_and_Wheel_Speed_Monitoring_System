# Unit-Based Incremental Refactoring Skill - Complete Index

**← Back to:** [../INDEX.md](../INDEX.md)

## 📋 Overview

**Skill Name**: `unit-based-incremental-refactoring`  
**Purpose**: Safe, predictable refactoring through unit-based architecture with continuous validation  
**Reduces**: Risk, unpredictability, human error  
**Increases**: Reportability, traceability, quality confidence

---

## 🗂️ Skill File Structure

```
.github/skills/unit-based-incremental-refactoring/
├─ INDEX.md                          ← Root index (quick navigation)
├─ README.md (this file)             ← Detailed file guide
├─ docs/
│  ├─ SKILL.md                       ← Main skill documentation
│  ├─ README.md                      ← This file (complete index)
│  └─ quick-start/ (coming soon)
├─ scripts/
│  ├─ plan_refactoring.py
│  ├─ log_unit_completion.py
│  ├─ generate_refactoring_report.py
│  └─ validate_unit_tests.ps1
├─ references/
│  ├─ unit_size_guidelines.md
│  ├─ validation_standards.md
│  ├─ unit_test_mapping.md
│  ├─ metrics_definitions.md
│  └─ scripts_reference.md
├─ assets/
│  ├─ refactoring_plan_template.json
│  ├─ refactoring_checklist.md
│  ├─ unit_log_template.json
│  └─ report_template.html
└─ evals/
   ├─ eval_readiness.py
   ├─ eval_unit_size.py
   ├─ eval_metrics.py
   └─ eval_production_readiness.py
```

---

## 🚀 Quick Start (5 Minutes)

### Step 1: Read the Main Documentation
```bash
cat docs/SKILL.md | less
```

### Step 2: Review Your Component
```bash
cat src/brake_monitor.cpp
cat tests/unit/brake_monitor_test.cpp
```

### Step 3: Plan Refactoring
```bash
python3 scripts/plan_refactoring.py \
    --component "BrakeMonitor" \
    --input-file src/brake_monitor.cpp \
    --test-file tests/unit/brake_monitor_test.cpp \
    --output refactoring_plan.json
```

### Step 4: Follow the Checklist
```bash
cat assets/refactoring_checklist.md
```

### Step 5: Execute & Report
```bash
# After each unit refactoring:
python3 scripts/log_unit_completion.py \
    --unit-id 1 \
    --function-name "is_braking" \
    --status "PASS"

# After all units complete:
python3 scripts/generate_refactoring_report.py \
    --component "BrakeMonitor" \
    --log-file refactoring_log.json
```

---

## 📖 Documentation Map

### For Understanding (Learning Path)

**Start Here** (5 min read):
→ [docs/SKILL.md](./SKILL.md) - Overview and key concepts

**Then Read** (10 min read):
→ [references/unit_size_guidelines.md](../references/unit_size_guidelines.md) - Unit sizing rules

**Deep Dive** (20 min read):
→ [references/validation_standards.md](../references/validation_standards.md) - Validation checkpoints

### For Execution (Step-by-Step)

**Plan Your Refactoring**:
→ [assets/refactoring_plan_template.json](../assets/refactoring_plan_template.json) - Template
→ Use: `scripts/plan_refactoring.py`

**Execute Each Unit**:
→ [assets/refactoring_checklist.md](../assets/refactoring_checklist.md) - Per-unit checklist
→ Follow validation: [references/validation_standards.md](../references/validation_standards.md)

**Record & Report**:
→ Use: `scripts/log_unit_completion.py`
→ Generate: `scripts/generate_refactoring_report.py`

---

## 📊 Scripts Reference

| Script | Purpose | Input | Output |
|--------|---------|-------|--------|
| `plan_refactoring.py` | Analyze component, create plan | Source & test files | `refactoring_plan.json` |
| `log_unit_completion.py` | Record unit metrics | Unit ID, test results | `refactoring_log.json` |
| `generate_refactoring_report.py` | Create final report | Refactoring log | HTML/JSON report |
| `validate_unit_tests.ps1` | Automated validation | (none) | Pass/fail report |

**Location**: `scripts/`  
**Detailed Usage**: See [references/scripts_reference.md](../references/scripts_reference.md)

---

## 📋 References Quick Links

| Reference | Topic | Size |
|-----------|-------|------|
| [unit_size_guidelines.md](../references/unit_size_guidelines.md) | Unit sizes, time estimates, risk | Medium |
| [validation_standards.md](../references/validation_standards.md) | Validation checkpoints, pass/fail criteria | Large |
| [unit_test_mapping.md](../references/unit_test_mapping.md) | How to identify covering tests | Medium |
| [metrics_definitions.md](../references/metrics_definitions.md) | Complexity, coverage, LOC definitions | Small |
| [scripts_reference.md](../references/scripts_reference.md) | Detailed script usage | Medium |

**Location**: `references/`

---

## 🎯 Assets (Templates & Checklists)

| Asset | Purpose | Format | Location |
|-------|---------|--------|----------|
| [refactoring_plan_template.json](../assets/refactoring_plan_template.json) | Skeleton for planning | JSON | `assets/` |
| [refactoring_checklist.md](../assets/refactoring_checklist.md) | Complete task list | Markdown | `assets/` |
| [unit_log_template.json](../assets/unit_log_template.json) | Template for unit logs | JSON | `assets/` |
| [report_template.html](../assets/report_template.html) | HTML report template | HTML | `assets/` |

**How to Use**:
1. Copy template from `assets/`
2. Fill in your details
3. Follow along during refactoring

---

## ✅ Evaluation Criteria (Evals)

| Eval | Checks | When | Location |
|-----|--------|------|----------|
| `eval_readiness.py` | Is refactoring safe to start? | Before starting | `evals/` |
| `eval_unit_size.py` | Is unit size appropriate? | When planning | `evals/` |
| `eval_metrics.py` | Did metrics improve? | After refactoring | `evals/` |
| `eval_production_readiness.py` | Is component production-ready? | Before merging | `evals/` |

**Usage**:
```python
# Example: Check if ready to start refactoring
python3 evals/eval_readiness.py --component "BrakeMonitor"
```

---

## 🔄 Typical Workflow

### Session 1: Planning (30 minutes)
```
1. Read docs/SKILL.md (overview)
2. Review component code
3. Run scripts/plan_refactoring.py
4. Review refactoring_plan.json
5. Map covering tests
6. Get team approval
```

### Session 2-N: Execute Units (15-90 min per unit)
```
FOR EACH UNIT:
  1. Understand function & covering tests
  2. Refactor code (one logical change)
  3. Compile (cmake --build .)
  4. Run covering tests (ctest -R "pattern")
  5. Run full suite (ctest --verbose)
  6. Log completion (scripts/log_unit_completion.py)
  7. Move to next unit
```

### Final Session: Report & Merge (30 minutes)
```
1. Run scripts/generate_refactoring_report.py
2. Review HTML report
3. Stage & commit changes
4. Create pull request
5. Get code review approval
6. Merge to main
7. Verify on main branch
```

---

## 📊 Expected Results

### Before Refactoring
- Baseline established: All tests passing (52/52)
- Metrics recorded: Complexity, LOC, coverage
- Team notified: Goals understood

### During Refactoring
- Unit validation every 15-90 minutes
- Continuous regression checking (52 tests each time)
- Metrics tracked for each unit
- Progress visible and trackable

### After Refactoring
- ✓ All tests passing (52/52)
- ✓ Zero regressions introduced
- ✓ Metrics improved (complexity ↓, coverage ↑)
- ✓ Changes fully documented
- ✓ Team confident in quality
- ✓ Report shows before/after comparison

---

## 🎓 Learning Resources

### Quick Reference (5-15 min)
- [docs/SKILL.md](./SKILL.md) - Overview
- [references/unit_size_guidelines.md](../references/unit_size_guidelines.md) - Quick rules
- [assets/refactoring_checklist.md](../assets/refactoring_checklist.md) - What to do

### Comprehensive (30-60 min)
- [references/validation_standards.md](../references/validation_standards.md) - Deep validation
- [references/unit_test_mapping.md](../references/unit_test_mapping.md) - Test strategies
- [references/metrics_definitions.md](../references/metrics_definitions.md) - Measurement

### Advanced (1-2 hours)
- Read all references
- Run all evaluation scripts
- Execute a complete refactoring
- Generate and analyze report

---

## 🔍 Troubleshooting

### Question: "How do I know if my unit size is appropriate?"
**Answer**: See [references/unit_size_guidelines.md](../references/unit_size_guidelines.md) - Size matrix shows LOC → Tests → Time

### Question: "What does it mean to 'pass' validation?"
**Answer**: See [references/validation_standards.md](../references/validation_standards.md) - Defines all checkpoints

### Question: "How do I identify covering tests?"
**Answer**: See [references/unit_test_mapping.md](../references/unit_test_mapping.md) - Shows code paths → tests

### Question: "My test is failing. What do I do?"
**Answer**: See [references/validation_standards.md](../references/validation_standards.md) - Scenario 2: Test Failure

### Question: "I introduced a regression. How to fix?"
**Answer**: See [references/validation_standards.md](../references/validation_standards.md) - Scenario 3: Regression

---

## 📈 Success Metrics

### Skill Adoption Success
- ✓ All units refactored < estimated time
- ✓ No regressions introduced (52/52 tests pass)
- ✓ Metrics improved (at least 1 major metric better)
- ✓ Team confident in changes
- ✓ Report generated and archived

### Unit-Level Success
- ✓ Covering tests pass (3-5 tests)
- ✓ Full suite passes (52/52 tests)
- ✓ Time spent < estimate
- ✓ Metrics recorded accurately
- ✓ Log entry complete

---

## 🚨 Critical Success Factors

1. **Read docs/SKILL.md First** - Understand the philosophy
2. **Map Covering Tests** - Know which tests cover which functions
3. **Validate After Each Unit** - Run full suite (52 tests) every time
4. **If Test Fails** - Revert immediately, debug, try again
5. **Use Checklists** - Follow the flow, don't skip steps
6. **Log Everything** - Metrics, time, status for every unit

---

## 📞 Support & Questions

### Documentation Structure
- **Execution**: Start with [docs/SKILL.md](./SKILL.md)
- **Guidelines**: See [references/](../references/)
- **Templates**: Use [assets/](../assets/)
- **Automation**: Run [scripts/](../scripts/)
- **Evaluation**: Use [evals/](../evals/)

### Common Tasks
- **Start refactoring**: Run `scripts/plan_refactoring.py`
- **Execute unit**: Follow checklist + [references/validation_standards.md](../references/validation_standards.md)
- **Report progress**: Run `scripts/log_unit_completion.py`
- **Generate report**: Run `scripts/generate_refactoring_report.py`
- **Check readiness**: Run `evals/eval_readiness.py`

### Getting Help
1. Check [docs/SKILL.md](./SKILL.md) - Section "Troubleshooting"
2. Review [references/validation_standards.md](../references/validation_standards.md) - Scenarios section
3. Check [references/unit_size_guidelines.md](../references/unit_size_guidelines.md) - Guidelines match your situation?
4. Ask team/mentor for assistance

---

## 🎯 Next Steps

1. **Read** [docs/SKILL.md](./SKILL.md) (main documentation)
2. **Review** [assets/refactoring_checklist.md](../assets/refactoring_checklist.md)
3. **Plan** your refactoring using `scripts/plan_refactoring.py`
4. **Execute** first unit following checklist
5. **Validate** at each checkpoint
6. **Report** progress using logging scripts
7. **Complete** refactoring
8. **Share** results with team

---

## ✨ Summary

This skill provides:
- ✓ **Structured workflow** for safe refactoring
- ✓ **Automation scripts** for consistent execution
- ✓ **Reference documents** for standards
- ✓ **Checklists & templates** for consistency
- ✓ **Evaluation criteria** for quality gates
- ✓ **Comprehensive reporting** for traceability

**Result**: Predictable, safe, well-documented refactoring that reduces risk and increases confidence! 🎯

---

**← Back to:** [../INDEX.md](../INDEX.md)
