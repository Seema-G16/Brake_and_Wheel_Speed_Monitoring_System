# ⚠️ Please See: [docs/README.md](./docs/README.md)

This file has been moved to the **docs/** folder for better organization.

---

## 📂 Organized Folder Structure

All documentation files have been moved to the `docs/` folder:

```
unit-based-incremental-refactoring/
├─ INDEX.md                          ← Start here for navigation
├─ docs/
│  ├─ SKILL.md                       ← Main documentation
│  ├─ README.md                      ← Complete guide (MOVED HERE)
│  ├─ REORGANIZATION_SUMMARY.md      ← Organization details
│  └─ quick-start/
├─ scripts/
├─ references/
├─ assets/
└─ evals/
```

---

## 🎯 Quick Navigation

| Document | Purpose | Location |
|----------|---------|----------|
| **INDEX** | Folder navigation hub | [INDEX.md](./INDEX.md) |
| **SKILL** | Main documentation | [docs/SKILL.md](./docs/SKILL.md) |
| **README** | Complete guide | [docs/README.md](./docs/README.md) |
| **Organization** | How files are organized | [docs/REORGANIZATION_SUMMARY.md](./docs/REORGANIZATION_SUMMARY.md) |

---

## 🗂️ Skill Structure

```
.github/skills/unit-based-incremental-refactoring/
├─ SKILL.md                          ← START HERE (main documentation)
│
├─ scripts/                           ← Automation & tools
│  ├─ plan_refactoring.py             (Analyze component, create plan)
│  ├─ log_unit_completion.py          (Record unit completion)
│  ├─ generate_refactoring_report.py  (Create comprehensive report)
│  └─ validate_unit_tests.ps1         (PowerShell validation)
│
├─ references/                        ← Standards & guidelines
│  ├─ unit_size_guidelines.md         (Quick reference for unit sizing)
│  ├─ validation_standards.md         (What passing validation means)
│  ├─ unit_test_mapping.md            (How to identify covering tests)
│  └─ metrics_definitions.md          (How metrics are measured)
│
├─ assets/                            ← Templates & checklists
│  ├─ refactoring_plan_template.json  (Skeleton for refactoring plans)
│  ├─ refactoring_checklist.md        (Complete task checklist)
│  └─ unit_log_template.json          (Template for unit logs)
│
└─ evals/                             ← Evaluation criteria
   ├─ eval_readiness.py               (Is refactoring safe to start?)
   ├─ eval_unit_size.py               (Is unit size appropriate?)
   ├─ eval_metrics.py                 (Did metrics improve?)
   └─ eval_production_readiness.py    (Is component production-ready?)
```

---

## 🚀 Quick Start (5 Minutes)

### Step 1: Read the Main Documentation
```bash
# Open and read
cat SKILL.md | less
```

### Step 2: Review Your Component
```bash
# Example: BrakeMonitor component
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
# Use the refactoring checklist for each step
cat assets/refactoring_checklist.md
```

### Step 5: Execute & Report
```bash
# After each unit refactoring:
python3 scripts/log_unit_completion.py \
    --unit-id 1 \
    --function-name "is_braking" \
    --status "PASS" \
    --time-minutes 15 \
    --complexity-before 2 \
    --complexity-after 1 \
    --tests-passed 3 \
    --tests-total 3 \
    --full-suite-passed 52 \
    --full-suite-total 52

# After all units complete:
python3 scripts/generate_refactoring_report.py \
    --component "BrakeMonitor" \
    --log-file refactoring_log.json
```

---

## 📖 Documentation Map

### For Understanding (Learning Path)

**Start Here** (2 min read):
→ [SKILL.md](./SKILL.md) - Overview and key concepts

**Then Read** (5 min read):
→ [references/unit_size_guidelines.md](./references/unit_size_guidelines.md) - Unit sizing rules

**Deep Dive** (15 min read):
→ [references/validation_standards.md](./references/validation_standards.md) - Validation checkpoints

### For Execution (Step-by-Step)

**Plan Your Refactoring**:
→ [assets/refactoring_plan_template.json](./assets/refactoring_plan_template.json) - Template
→ Use: `scripts/plan_refactoring.py`

**Execute Each Unit**:
→ [assets/refactoring_checklist.md](./assets/refactoring_checklist.md) - Per-unit checklist
→ Follow validation: [references/validation_standards.md](./references/validation_standards.md)

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

**Detailed Usage**:
→ See [references/scripts_reference.md](./references/scripts_reference.md)

---

## 📋 References Quick Links

| Reference | Topic | Size |
|-----------|-------|------|
| [unit_size_guidelines.md](./references/unit_size_guidelines.md) | Unit sizes, time estimates, risk | Medium |
| [validation_standards.md](./references/validation_standards.md) | Validation checkpoints, pass/fail criteria | Large |
| [unit_test_mapping.md](./references/unit_test_mapping.md) | How to identify covering tests | Medium |
| [metrics_definitions.md](./references/metrics_definitions.md) | Complexity, coverage, LOC definitions | Small |
| [scripts_reference.md](./references/scripts_reference.md) | Detailed script usage | Medium |

---

## 🎯 Assets (Templates & Checklists)

| Asset | Purpose | Format |
|-------|---------|--------|
| [refactoring_plan_template.json](./assets/refactoring_plan_template.json) | Skeleton for planning | JSON |
| [refactoring_checklist.md](./assets/refactoring_checklist.md) | Complete task list | Markdown |
| [unit_log_template.json](./assets/unit_log_template.json) | Template for unit logs | JSON |
| [report_template.html](./assets/report_template.html) | HTML report template | HTML |

**How to Use**:
1. Copy template
2. Fill in your details
3. Follow along during refactoring

---

## ✅ Evaluation Criteria (Evals)

| Eval | Checks | When |
|-----|--------|------|
| `eval_readiness.py` | Is refactoring safe to start? | Before starting |
| `eval_unit_size.py` | Is unit size appropriate? | When planning |
| `eval_metrics.py` | Did metrics improve? | After refactoring |
| `eval_production_readiness.py` | Is component production-ready? | Before merging |

**Usage**:
```python
# Example: Check if ready to start refactoring
python3 evals/eval_readiness.py --component "BrakeMonitor"
```

---

## 🔄 Typical Workflow

### Session 1: Planning (30 minutes)
```
1. Read SKILL.md (overview)
2. Review component code
3. Run plan_refactoring.py
4. Review refactoring_plan.json
5. Map covering tests
6. Get approval from team
```

### Session 2-N: Execute Units (15-90 min per unit)
```
FOR EACH UNIT:
  1. Understand function & covering tests
  2. Refactor code (one logical change)
  3. Compile (cmake --build .)
  4. Run covering tests (ctest -R "pattern")
  5. Run full suite (ctest --verbose)
  6. Log completion (log_unit_completion.py)
  7. Move to next unit
```

### Final Session: Report & Merge (30 minutes)
```
1. Run generate_refactoring_report.py
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
- [SKILL.md](./SKILL.md) - Overview
- [unit_size_guidelines.md](./references/unit_size_guidelines.md) - Quick rules
- [refactoring_checklist.md](./assets/refactoring_checklist.md) - What to do

### Comprehensive (30-60 min)
- [validation_standards.md](./references/validation_standards.md) - Deep validation
- [unit_test_mapping.md](./references/unit_test_mapping.md) - Test strategies
- [metrics_definitions.md](./references/metrics_definitions.md) - Measurement

### Advanced (1-2 hours)
- Read all references
- Run all evaluation scripts
- Execute a complete refactoring
- Generate and analyze report

---

## 🔍 Troubleshooting

### Question: "How do I know if my unit size is appropriate?"
**Answer**: See [unit_size_guidelines.md](./references/unit_size_guidelines.md) - Size matrix shows LOC → Tests → Time

### Question: "What does it mean to 'pass' validation?"
**Answer**: See [validation_standards.md](./references/validation_standards.md) - Defines all checkpoints

### Question: "How do I identify covering tests?"
**Answer**: See [unit_test_mapping.md](./references/unit_test_mapping.md) - Shows code paths → tests

### Question: "My test is failing. What do I do?"
**Answer**: See [validation_standards.md](./references/validation_standards.md) - Scenario 2: Test Failure

### Question: "I introduced a regression. How to fix?"
**Answer**: See [validation_standards.md](./references/validation_standards.md) - Scenario 3: Regression

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

1. **Read SKILL.md First** - Understand the philosophy
2. **Map Covering Tests** - Know which tests cover which functions
3. **Validate After Each Unit** - Run full suite (52 tests) every time
4. **If Test Fails** - Revert immediately, debug, try again
5. **Use Checklists** - Follow the flow, don't skip steps
6. **Log Everything** - Metrics, time, status for every unit

---

## 📞 Support & Questions

### Documentation Structure
- **Execution**: Start with [SKILL.md](./SKILL.md)
- **Guidelines**: See [references/](./references/)
- **Templates**: Use [assets/](./assets/)
- **Automation**: Run [scripts/](./scripts/)
- **Evaluation**: Use [evals/](./evals/)

### Common Tasks
- **Start refactoring**: Run `plan_refactoring.py`
- **Execute unit**: Follow checklist + validation_standards.md
- **Report progress**: Run `log_unit_completion.py`
- **Generate report**: Run `generate_refactoring_report.py`
- **Check readiness**: Run `eval_readiness.py`

### Getting Help
1. Check [SKILL.md](./SKILL.md) - Section "Troubleshooting"
2. Review [validation_standards.md](./references/validation_standards.md) - Scenarios section
3. Check [unit_size_guidelines.md](./references/unit_size_guidelines.md) - Guidelines match your situation?
4. Ask team/mentor for assistance

---

## 🎯 Next Steps

1. **Read** [SKILL.md](./SKILL.md) (main documentation)
2. **Review** [assets/refactoring_checklist.md](./assets/refactoring_checklist.md)
3. **Plan** your refactoring using `plan_refactoring.py`
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

