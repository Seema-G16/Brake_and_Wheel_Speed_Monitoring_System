# Code Unit Size - Visual Architecture Diagram

## 🎯 The Complete Framework at a Glance

```
YOUR QUESTION:
"How are you handling the code unit size (small logical units 
 that can be executed and validated at a time)?"

THE ANSWER:
Three-level unit hierarchy with continuous validation
```

---

## 📊 Visual: The Three-Level Unit Hierarchy

```
┌──────────────────────────────────────────────────────────────────┐
│                                                                  │
│                    VALIDATION CHECKPOINT                         │
│         (After refactoring, before committing)                  │
│                                                                  │
│              ╔════════════════════════════════╗                  │
│              ║  FULL TEST SUITE VALIDATES     ║                  │
│              ║  ✓ All 52 tests pass           ║                  │
│              ║  ✓ No regressions introduced   ║                  │
│              ╚════════════════════════════════╝                  │
│                         ▲                                        │
│                         │                                        │
│                    ┌────┴────┐                                   │
│                    │ EXECUTION                                   │
│                    │ FLOW     │                                   │
│                    └────┬────┘                                   │
│                         │                                        │
│       ┌─────────────────┼─────────────────┐                      │
│       │                 │                 │                      │
│       ▼                 ▼                 ▼                      │
│   ┌────────┐        ┌────────┐       ┌────────┐                │
│   │COMPILE │        │COVERING│       │FULL    │                │
│   │  (1s)  │        │TESTS   │       │SUITE   │                │
│   │        │        │(3-5)   │       │(52)    │                │
│   │Validates        │Tests   │       │Tests   │                │
│   │syntax   │       │single  │       │all     │                │
│   │only    │       │unit    │       │units   │                │
│   └────────┘        └────────┘       └────────┘                │
│       ▲                 ▲                 ▲                      │
│       │                 │                 │                      │
│       └─────────────────┼─────────────────┘                      │
│                         │                                        │
│                         │ (Step 3-5 after each unit)             │
│                         │                                        │
│       ┌─────────────────┴─────────────────┐                      │
│       │                                   │                      │
│       ▼                                   ▼                      │
│   ┌──────────────────────────────────────────┐                 │
│   │      UNIT REFACTORING (15-90 min)        │                 │
│   │  ┌────────────────────────────────────┐  │                 │
│   │  │ SELECT ONE FUNCTION                │  │                 │
│   │  │ (3-30 LOC per function)             │  │                 │
│   │  └────────────────────────────────────┘  │                 │
│   │                 │                        │                 │
│   │                 ▼                        │                 │
│   │  ┌────────────────────────────────────┐  │                 │
│   │  │ UNDERSTAND COVERING TESTS (3-5)   │  │                 │
│   │  │ All code paths are tested          │  │                 │
│   │  └────────────────────────────────────┘  │                 │
│   │                 │                        │                 │
│   │                 ▼                        │                 │
│   │  ┌────────────────────────────────────┐  │                 │
│   │  │ REFACTOR THE FUNCTION              │  │                 │
│   │  │ Keep interface unchanged           │  │                 │
│   │  │ Make one logical change            │  │                 │
│   │  └────────────────────────────────────┘  │                 │
│   │                 │                        │                 │
│   │                 ▼                        │                 │
│   │  ┌────────────────────────────────────┐  │                 │
│   │  │ COMPILE & VALIDATE                 │  │                 │
│   │  │ No errors?                         │  │                 │
│   │  │ All covering tests pass?           │  │                 │
│   │  │ Full suite still passes (52/52)?   │  │                 │
│   │  │ → YES to all = UNIT COMPLETE ✓    │  │                 │
│   │  └────────────────────────────────────┘  │                 │
│   └──────────────────────────────────────────┘                 │
│                                                                  │
│  *** REPEAT FOR EACH FUNCTION (typically 4-6 per component) *** │
│                                                                  │
└──────────────────────────────────────────────────────────────────┘
```

---

## 📈 The Safety Pyramid: Incremental Validation

```
                          ▲
                         ╱ ╲         FINAL VALIDATION
                        ╱   ╲        ✓ 52/52 tests pass
                       ╱     ╲       ✓ Full rebuild success
                      ╱───────╲
                     ╱         ╲    COMPONENT LEVEL
                    ╱  ✓ 12/12  ╲    ✓ All functions done
                   ╱             ╲
                  ╱───────────────╲
                 ╱ ✓ 12/12  ✓ 4/4 ╲  FUNCTION LEVEL
                ╱                  ╲  ✓ Covering tests pass
               ╱──────────────────────╲
              ╱ ✓ 3/3  ✓ 4/4  ✓ 8/8 ╲ UNIT LEVEL
             ╱                        ╲ ✓ Unit tests pass
            ╱───────────────────────────╲
           ╱ ✓ ✓ ✓ ✓ ✓ ✓ ✓ ✓ ✓ ✓ ✓ ✓ ╲ TEST LEVEL
          ╱ Individual test cases (52) ╲ ✓ Scenarios pass
         ╱═══════════════════════════════╲

KEY: Each level validates independently
     If level 2 (unit 1) passes, and level 3 (unit 2) fails,
     you KNOW the problem is in unit 2 (not unit 1)!
```

---

## ⏰ Timeline View: How Units Fit Into A Day

```
09:00  Planning & Setup
       │
       ├─ Read code
       ├─ Map tests
       └─ Plan phases
       │
09:20  ┌──────────────────────────────────┐
       │ UNIT 1: is_braking() ✓            │
       │ ├─ Refactor (15 min)              │
       │ ├─ Covering tests: 3/3 PASS       │
       │ └─ Full suite: 52/52 PASS         │
       └──────────────────────────────────┘
       │
09:35  ┌──────────────────────────────────┐
       │ UNIT 2: check_pressure() ✓        │
       │ ├─ Refactor (30 min)              │
       │ ├─ Covering tests: 4/4 PASS       │
       │ └─ Full suite: 52/52 PASS         │
       └──────────────────────────────────┘
       │
10:05  BREAK (15 min)
       │
10:20  ┌──────────────────────────────────┐
       │ UNIT 3: evaluate() ✓              │
       │ ├─ Refactor (60 min - largest)    │
       │ ├─ Covering tests: 12/12 PASS     │
       │ └─ Full suite: 52/52 PASS         │
       └──────────────────────────────────┘
       │
11:20  LUNCH (1 hour)
       │
12:20  ┌──────────────────────────────────┐
       │ UNIT 4: Helper functions ✓        │
       │ ├─ Refactor (15 min)              │
       │ ├─ Covering tests: 8/8 PASS       │
       │ └─ Full suite: 52/52 PASS         │
       └──────────────────────────────────┘
       │
12:35  ┌──────────────────────────────────┐
       │ FINAL VALIDATION ✓                │
       │ ├─ Full rebuild                   │
       │ ├─ All 52 tests PASS              │
       │ └─ SIGN OFF                       │
       └──────────────────────────────────┘
       │
12:50  ✓ COMPLETE - Component refactored & validated

TOTAL TIME: ~3.5 hours including breaks
QUALITY: 100% test coverage verified at each step
```

---

## 📋 Decision Tree: How to Choose Unit Size

```
Question: How should I break down this refactoring?

START: Pick component to refactor
  │
  ├─ Is it <30 LOC?
  │  ├─ YES → Refactor as ONE unit
  │  │       Time: 15-30 minutes
  │  │       Tests: 2-3 covering tests
  │  │       Example: is_braking()
  │  │
  │  └─ NO → Break into multiple units
  │
  ├─ Does it have <40 LOC?
  │  ├─ YES → Refactor as ONE unit
  │  │       Time: 30-60 minutes
  │  │       Tests: 4-6 covering tests
  │  │       Example: check_pressure()
  │  │
  │  └─ NO → Break into multiple units
  │
  ├─ How many functions?
  │  ├─ 1-2 functions → Refactor as ONE unit
  │  ├─ 3-4 functions → Refactor in 2 units
  │  └─ 5+ functions → Refactor in 3+ units
  │
  └─ RESULT: Time per unit should be 15-90 minutes
             Tests per unit should be 3-5+ tests
             Every 15-90 min: Validation checkpoint
```

---

## 🔍 Example: Mapping Real Code to Unit Hierarchy

```
REAL EXAMPLE: BrakeMonitor Component (100 LOC, 5 functions)

Component Level (100 LOC, 5 functions, 12 tests)
│
├─ Function 1: is_braking()              [2 LOC]
│  └─ Unit: 15 min refactoring
│     Tests: TC-BRK-001, 003, 012 (3 tests)
│
├─ Function 2: check_pressure()          [12 LOC]
│  └─ Unit: 30-45 min refactoring
│     Tests: TC-BRK-004, 005, 007, 008 (4 tests)
│
├─ Function 3: is_pressure_high()        [5 LOC]
│  └─ Unit: 15-20 min refactoring
│     Tests: TC-BRK-006, 009 (2 tests)
│
├─ Function 4: evaluate()                [35 LOC]
│  └─ Unit: 60-90 min refactoring (largest)
│     Tests: All TC-BRK-001 through TC-BRK-012 (12 tests)
│
└─ Function 5: Helper functions          [46 LOC across 3 functions]
   └─ Unit: 20-30 min refactoring
      Tests: Various supporting tests

REFACTORING PLAN:
Phase 1: is_braking()           → 15 min → 3/3 tests pass → 52/52 suite pass ✓
Phase 2: check_pressure()       → 45 min → 4/4 tests pass → 52/52 suite pass ✓
Phase 3: is_pressure_high()     → 20 min → 2/2 tests pass → 52/52 suite pass ✓
Phase 4: evaluate()             → 90 min → 12/12 tests pass → 52/52 suite pass ✓
Phase 5: Helper functions       → 30 min → 8/8 tests pass → 52/52 suite pass ✓
Phase 6: Full validation        → 10 min → 52/52 tests pass → COMPLETE ✓

TOTAL: ~3.5 hours to refactor entire 100 LOC component!
```

---

## 🎯 Decision Matrix: Unit Size Guidelines

```
LOC per         │ Tests      │ Time to    │ Risk    │ Validation
Function        │ per Func   │ Refactor   │ Level   │ Freq.
────────────────┼────────────┼────────────┼─────────┼──────────────
1-5 LOC         │ 1-2        │ 10-15 min  │ VERY    │ Every 10 min
                │            │            │ LOW     │
────────────────┼────────────┼────────────┼─────────┼──────────────
5-10 LOC        │ 2-3        │ 15-30 min  │ LOW     │ Every 20 min
                │            │            │         │
────────────────┼────────────┼────────────┼─────────┼──────────────
10-20 LOC       │ 3-4        │ 30-45 min  │ LOW     │ Every 30 min
                │            │            │         │
────────────────┼────────────┼────────────┼─────────┼──────────────
20-30 LOC       │ 4-5        │ 45-60 min  │ MEDIUM  │ Every 45 min
                │            │            │         │
────────────────┼────────────┼────────────┼─────────┼──────────────
30+ LOC         │ 5+         │ 60-90+ min │ MEDIUM  │ Every 60 min
                │            │            │         │
────────────────┴────────────┴────────────┴─────────┴──────────────

YOUR PROJECT:
├─ Validation: 2 functions, 50 LOC
│  └─ Test coverage: 6.5 tests/func (excellent)
│
├─ Brake Monitoring: 4 functions, 60 LOC
│  └─ Test coverage: 3.0 tests/func (good)
│
├─ Wheel Speed: 5 functions, 90 LOC
│  └─ Test coverage: 3.0 tests/func (good)
│
└─ Fault Management: 5 functions, 100 LOC
   └─ Test coverage: 2.4 tests/func (good)

OVERALL: 3.25 tests/function = EXCELLENT ✓
         Fits perfectly in this unit size strategy
```

---

## 📚 Documentation Map: What to Read When

```
I WANT TO... →  READ THIS FILE →  TIME  →  BENEFIT
─────────────────────────────────────────────────────────────

Understand       UNIT_SIZE_         25-30   Deep comprehension
the complete     ARCHITECTURE.md     min     of unit hierarchy,
architecture                                 principles, and
                                            risk management

Get a quick      UNIT_SIZE_QUICK_    10-15   Visual reference,
overview with    REFERENCE.md        min     metrics, timeline
visuals                                      examples

Learn the        UNIT_REFACTORING_   20-30   Practical guide with
practical        WORKFLOW.md         min     real commands and
workflow                          + run      examples

Understand       UNIT_SIZE_          5-10    Executive summary,
the executive    EXECUTIVE_SUMMARY.  min     key strategies
summary          md

See complete     UNIT_SIZE_          30      Comprehensive
integration      COMPLETE_GUIDE.md   min     overview of all
                                            aspects
```

---

## 🎁 All Files Created in This Session

### Refactoring Framework (4 files, ~50 KB)
```
✓ REFACTORING_PROMPT_TEMPLATE.md        → Skeleton for creating prompts
✓ REFACTORING_PROMPT_EXAMPLE.md         → Real-world example
✓ REFACTORING_KEYWORDS.md               → Quality keywords checklist
✓ REFACTORING_FRAMEWORK_GUIDE.md        → Navigation & integration
```

### Unit Size Architecture (6 files, ~99 KB)
```
✓ UNIT_SIZE_ARCHITECTURE.md             → Deep understanding
✓ UNIT_SIZE_QUICK_REFERENCE.md          → Visual reference
✓ UNIT_SIZE_COMPLETE_GUIDE.md           → Comprehensive guide
✓ UNIT_SIZE_EXECUTIVE_SUMMARY.md        → Executive summary
✓ UNIT_REFACTORING_WORKFLOW.md          → Practical execution
✓ UNIT_SIZE_VISUAL_ARCHITECTURE.md      → This file (diagrams)
```

**TOTAL: 10 documentation files, ~149 KB of comprehensive guidance**

---

## ✅ Your Framework is Complete

You now have:

✓ **Theory**: Why units are sized this way (UNIT_SIZE_ARCHITECTURE.md)  
✓ **Reference**: Quick lookup with visuals (UNIT_SIZE_QUICK_REFERENCE.md)  
✓ **Practice**: Real commands & examples (UNIT_REFACTORING_WORKFLOW.md)  
✓ **Integration**: How to create prompts (REFACTORING_PROMPT_TEMPLATE.md)  
✓ **Safety**: Validation strategy (continuous checkpoints)  
✓ **Proof**: Your code is already well-structured (3.25 tests/function)  

---

## 🎯 Ready to Execute

Pick a component and:
1. Read `UNIT_SIZE_EXECUTIVE_SUMMARY.md` (5 min)
2. Read `UNIT_REFACTORING_WORKFLOW.md` (20 min)
3. Follow the workflow exactly as documented
4. Every 15-90 minutes: Validation checkpoint
5. Every component: 2-4 hours total refactoring

**Result**: Safe, incremental refactoring with continuous validation! ✓

