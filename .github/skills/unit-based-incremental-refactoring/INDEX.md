# Unit-Based Incremental Refactoring Skill

**Organized in Subfolders for Better Navigation**

## 📂 Complete Folder Structure

```
unit-based-incremental-refactoring/
│
├─ INDEX.md                          ← YOU ARE HERE (folder navigation)
│
├─ docs/                             ← Documentation
│  ├─ SKILL.md                       ← Main skill documentation ⭐ START HERE
│  ├─ README.md                      ← Complete index & file guide
│  └─ quick-start/                   ← (Quick-start guides folder)
│
├─ scripts/                          ← Automation & tools
│  ├─ plan_refactoring.py            (Analyze & create plan)
│  ├─ log_unit_completion.py         (Record metrics after each unit)
│  ├─ generate_refactoring_report.py (Generate HTML/JSON reports)
│  └─ validate_unit_tests.ps1        (PowerShell validation)
│
├─ references/                       ← Standards & guidelines
│  ├─ unit_size_guidelines.md        (Unit sizing rules & matrix)
│  ├─ validation_standards.md        (Validation checkpoints & procedures)
│  ├─ unit_test_mapping.md           (How to identify covering tests)
│  ├─ metrics_definitions.md         (Complexity, LOC, coverage definitions)
│  └─ scripts_reference.md           (Detailed script usage)
│
├─ assets/                           ← Templates & checklists
│  ├─ refactoring_plan_template.json (JSON skeleton for planning)
│  ├─ refactoring_checklist.md       (Complete pre/during/post checklist)
│  ├─ unit_log_template.json         (Template for unit completion logs)
│  └─ report_template.html           (HTML template for final report)
│
└─ evals/                            ← Evaluation scripts
   ├─ eval_readiness.py              (Is refactoring safe to start?)
   ├─ eval_unit_size.py              (Is unit size appropriate?)
   ├─ eval_metrics.py                (Did metrics improve?)
   └─ eval_production_readiness.py   (Is component production-ready?)
```

## 🚀 Quick Start

### 1️⃣ Start Here - Read Main Documentation
```bash
# Open main skill documentation (15 min read)
cat docs/SKILL.md
```

### 2️⃣ Understand Your Component
```bash
# Review the source code and tests
cat src/brake_monitor.cpp
cat tests/unit/brake_monitor_test.cpp
```

### 3️⃣ Create Refactoring Plan
```bash
# Analyze component and generate plan
python3 scripts/plan_refactoring.py \
    --component "BrakeMonitor" \
    --input-file src/brake_monitor.cpp \
    --test-file tests/unit/brake_monitor_test.cpp \
    --output refactoring_plan.json
```

### 4️⃣ Follow the Checklist  
```bash
# Use detailed checklist during refactoring
cat assets/refactoring_checklist.md
```

### 5️⃣ Execute Each Unit (15-90 min per unit)
```bash
# After refactoring each function:
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
```

### 6️⃣ Generate Final Report
```bash
# After all units complete
python3 scripts/generate_refactoring_report.py \
    --component "BrakeMonitor" \
    --log-file refactoring_log.json
```

---

## 📖 File Guide

| Folder | Purpose |
|--------|---------|
| **docs/** | Complete documentation & guides |
| **scripts/** | Python & PowerShell automation |
| **references/** | Standards, guidelines, specifications |
| **assets/** | Templates, checklists, JSON templates |
| **evals/** | Evaluation scripts for quality gates |

---

## ✨ What This Skill Does

- ✅ Breaks refactoring into small, safe units (3-30 LOC each)
- ✅ Validates continuously (every 15-90 minutes)
- ✅ Tracks all metrics (before/after complexity, LOC, coverage)
- ✅ Generates comprehensive reports (HTML/JSON)
- ✅ Reduces risk of regressions (full test suite validation)
- ✅ Increases team confidence (complete traceability)

---

## 🎯 Get Started

**New to this skill?**
→ Start with [`docs/SKILL.md`](./docs/SKILL.md) (main documentation)

**Need a checklist?**
→ Use [`assets/refactoring_checklist.md`](./assets/refactoring_checklist.md)

**Want templates?**
→ See [`assets/`](./assets/)

**Need guidelines?**
→ Check [`references/`](./references/)

**Ready to automate?**
→ Use [`scripts/`](./scripts/)

