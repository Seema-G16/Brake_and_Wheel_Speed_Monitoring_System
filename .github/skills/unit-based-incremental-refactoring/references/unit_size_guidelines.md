# Unit Size Guidelines - Reference

## Quick Reference: Unit Sizes by Metrics

```
LOC Range    │ Tests    │ Time      │ Risk      │ Best Practice
─────────────┼──────────┼───────────┼───────────┼──────────────────
1-5 LOC      │ 1-2      │ 10-15 min │ VERY_LOW  │ Refactor together
5-10 LOC     │ 2-3      │ 15-30 min │ LOW       │ Standalone unit
10-20 LOC    │ 3-4      │ 30-45 min │ LOW       │ Standalone unit
20-30 LOC    │ 4-5      │ 45-60 min │ MEDIUM    │ Standalone unit
30-40 LOC    │ 5+       │ 60-90 min │ MEDIUM    │ Break into 2 units
40+ LOC      │ 6+       │ 90+ min   │ MEDIUM_HI │ Break into 3+ units
```

## Function Classification

### Type 1: Trivial (1-5 LOC)
- Examples: Simple getters, straightforward predicates
- Tests: 1-2
- Refactoring time: 10-15 min
- Approach: Combine with next function or refactor independently

### Type 2: Simple (5-15 LOC)
- Examples: Basic validation, simple calculations
- Tests: 2-3
- Refactoring time: 15-30 min
- Approach: Standalone refactoring unit

### Type 3: Moderate (15-30 LOC)
- Examples: Logic with branching, multi-step processes
- Tests: 3-5
- Refactoring time: 30-60 min
- Approach: Standalone refactoring unit

### Type 4: Complex (30-60 LOC)
- Examples: Multiple branches, nested logic, many dependencies
- Tests: 5-8
- Refactoring time: 60-120 min
- Approach: Break into multiple phases OR extend timeline

### Type 5: Very Complex (60+ LOC)
- Examples: State machines, algorithms with many paths
- Tests: 8+
- Refactoring time: 120+ min
- Approach: Break into 2-3 refactoring units

## Covering Test Ratios

| Function Type | Ideal Test Count | Minimum |
|---------------|------------------|---------|
| Trivial | 1 | 1 |
| Simple | 2-3 | 2 |
| Moderate | 3-5 | 3 |
| Complex | 5-8 | 4 |
| Very Complex | 8+ | 5 |

## Refactoring Time Estimates

### Time Breakdown (per unit)

```
Task              │ Duration   │ Notes
──────────────────┼────────────┼────────────────────
Understand code   │ 5-10 min   │ Includes reading tests
Plan changes      │ 5 min      │ Rough approach
Implement refactor│ 5-40 min   │ Depends on unit size
Compile & test    │ 2-5 min    │ Build + test execution
Debug (if needed) │ 5-15 min   │ Usually not needed
Log results       │ 2 min      │ Automated mostly
──────────────────┼────────────┼────────────────────
TOTAL             │ 15-90 min  │ Per unit
```

### Component Refactoring Estimates

```
Component      │ Functions │ Total LOC │ Est. Time │ Phases
───────────────┼───────────┼──────────┼───────────┼──────
Small          │ 2-3       │ 30-60    │ 1-2 hrs   │ 2-3
Medium         │ 3-5       │ 60-120   │ 2-3 hrs   │ 3-5
Large         │ 5-8       │ 120-250  │ 3-5 hrs   │ 5-8
Very Large     │ 8+        │ 250+     │ 5-8 hrs   │ 8+
```

## Risk Assessment Matrix

### Low Risk Units
- ✓ LOC: < 30
- ✓ Tests: 3+ covering tests
- ✓ Complexity: ≤ 5 cyclomatic
- ✓ Time: < 60 minutes

### Medium Risk Units
- ⚠ LOC: 30-60
- ⚠ Tests: 4-5 covering tests
- ⚠ Complexity: 6-10 cyclomatic
- ⚠ Time: 60-90 minutes

### High Risk Units
- ✗ LOC: > 60
- ✗ Tests: < 4 covering tests
- ✗ Complexity: > 10 cyclomatic
- ✗ Time: > 90 minutes
- **Action**: Break into multiple units

## Phase Duration Guidelines

```
Phase Duration │ Recommendation
───────────────┼──────────────────────────────────
< 15 minutes   │ Too small, combine with next unit
15-60 minutes  │ ✓ IDEAL (fits workflow easily)
60-90 minutes  │ ✓ ACCEPTABLE (one focus session)
90-120 minutes │ ⚠ BORDERLINE (consider splitting)
> 120 minutes  │ ✗ TOO LONG (definitely split)
```

## Validation Gates by Unit Size

```
Unit Size    │ Compilation │ Covering Tests │ Full Suite │ Manual Review
─────────────┼─────────────┼────────────────┼────────────┼──────────────
< 15 LOC     │ REQUIRED    │ REQUIRED       │ REQUIRED   │ 2 min
15-30 LOC    │ REQUIRED    │ REQUIRED       │ REQUIRED   │ 5 min
30-60 LOC    │ REQUIRED    │ REQUIRED       │ REQUIRED   │ 10 min
60+ LOC      │ REQUIRED    │ REQUIRED       │ REQUIRED   │ 15+ min
```

## Anti-Patterns: When Units are Too Large

```
❌ SIGN 1: "This is taking > 90 minutes"
   ACTION: Stop, revert, split into smaller units

❌ SIGN 2: "I can't remember what I changed"
   ACTION: Break down future refactoring into smaller pieces

❌ SIGN 3: "Multiple tests are failing"
   ACTION: Revert, narrow scope further

❌ SIGN 4: "I'm getting confused between functions"
   ACTION: Do ONE function per session, take a break

✓ SIGN 1: "This is done in 15-45 minutes"
   STATUS: Good unit size

✓ SIGN 2: "Only one test is failing, easy to debug"
   STATUS: Perfect scope

✓ SIGN 3: "Everything else still works"
   STATUS: Good isolation
```

## Decision Tree: Choose Your Unit Size

```
START: Component selected
  │
  ├─ Does it have > 60 LOC?
  │  ├─ YES → Break into 2-3 components
  │  └─ NO → Continue
  │
  ├─ How many functions?
  │  ├─ 1-2 → Do as one component
  │  ├─ 3-5 → Break into 2-3 refactoring phases
  │  └─ 6+ → Break into 3-4 refactoring phases
  │
  ├─ For each function, does it have > 40 LOC?
  │  ├─ YES → Break into 2 refactoring units
  │  └─ NO → Do as one unit
  │
  └─ RESULT: Estimated phases (one per unit)

Each phase should be 15-90 minutes
```

## Your Project's Unit Breakdown

```
Component              │ LOC  │ Functions │ Avg/Func │ Phases │ Est. Time
───────────────────────┼──────┼───────────┼──────────┼────────┼──────────
Validation             │  50  │     2     │   25     │   2    │   45 min
Brake Monitoring       │  60  │     4     │   15     │   3    │  1.5 hrs
Wheel Speed Monitoring │  90  │     5     │   18     │   3    │  1.5 hrs
Fault Management       │ 100  │     5     │   20     │   4    │   2 hrs
───────────────────────┼──────┼───────────┼──────────┼────────┼──────────
TOTAL                  │ 300  │    16     │  18.75   │  12    │  6.5 hrs
```

**Average per function**: 18.75 LOC → Excellent refactoring candidate! ✓

