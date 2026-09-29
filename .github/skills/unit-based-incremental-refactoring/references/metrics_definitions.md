# Metrics Definitions

How metrics are calculated and what they mean for code quality.

## Cyclomatic Complexity

**Definition**: Number of independent code paths through a function.

**Calculation**: Count decision points (if, for, while, switch, etc.) + 1

**Example**:
```cpp
void checkBraking(const Sample& s) {          // Complexity = 1
    if (s.brakePedal) {                       // +1 → Complexity = 2
        if (s.speed > 0) {                    // +1 → Complexity = 3
            activate();
        }
    }
}
```

**Interpretation**:
- 1-3: Simple, low risk
- 4-6: Moderate, moderate risk
- 7-10: Complex, high risk
- >10: Very complex, very high risk

**Goal**: Reduce complexity → Easier testing, fewer bugs, better maintainability

---

## Lines of Code (LOC)

**Definition**: Number of non-comment, non-blank lines in a function.

**Calculation**: Count executable statements

**Example**:
```cpp
// This is a comment (not counted)
bool is_braking(const Sample& s) {
    return s.brakePedal && s.speed > 0.0;    // 1 LOC
}
// Total: 1 LOC
```

**Interpretation**:
- 1-5: Tiny, trivial logic
- 6-15: Small, digestible
- 16-30: Medium, reasonable
- 31-50: Large, complex
- >50: Very large, refactor

**For Refactoring**:
- Target per function: 3-30 LOC
- Too small: Avoid micro-refactoring
- Too large: Break into smaller units

**Goal**: Optimal LOC → Readability, maintainability, testability

---

## Test Coverage

**Definition**: Percentage of code paths executed by tests.

**Calculation**: (Executed paths / Total paths) × 100%

**Example**:
```cpp
bool is_braking(const Sample& s) {
    return s.brakePedal && s.speed > 0.0;
}
// Total paths: 3 (both T/T, F/T, T/F)
// Tested paths: 3
// Coverage: 3/3 = 100%
```

**Interpretation**:
- 0-60%: Poor, risky
- 60-80%: Moderate, acceptable
- 80-95%: Good, confident
- 95-100%: Excellent, very confident

**For This Project**: Target 100% (covering tests = 100% coverage)

---

## Function Size Matrix

| LOC | Complexity | Covering Tests | Time to Refactor | Risk |
|-----|-----------|----------------|------------------|------|
| 1-5 | 1-2 | 2-3 | 5-15 min | VERY LOW |
| 6-15 | 2-4 | 3-4 | 15-45 min | LOW |
| 16-30 | 4-6 | 4-5 | 45-90 min | MEDIUM |
| 31-50 | 6-8 | 5-7 | 90-180 min | HIGH |
| >50 | >8 | >7 | >180 min | VERY HIGH |

---

## What to Track Before/After

### Complexity Metrics
- Cyclomatic complexity (measure with clang/gcc -fprofile-arcs)
- Number of conditional branches
- Nesting depth

### Size Metrics
- Lines of code per function
- Lines of code per component
- Average function size

### Coverage Metrics
- Percentage of code paths covered
- Number of covering tests per function
- Test execution time

### Quality Metrics
- Number of code violations (if using linter)
- Code duplication percentage
- Maintainability index

---

## Recording Metrics

Use template in `../assets/refactoring_plan_template.json`:

```json
{
  "success_metrics": {
    "complexity": {
      "current": 8,
      "target": 4,
      "unit": "cyclomatic complexity"
    },
    "lines_of_code": {
      "current": 45,
      "target": 35
    },
    "test_coverage": {
      "current": "85%",
      "target": "100%"
    }
  }
}
```

---

## Interpreting Improvements

### Complexity Reduction
```
Before: 8, After: 4 → 50% reduction ✓
Impact: Code much easier to understand and test
```

### LOC Reduction
```
Before: 45, After: 35 → 22% reduction ✓
Impact: More concise, easier to maintain
```

### Coverage Improvement
```
Before: 85%, After: 100% → 15% improvement ✓
Impact: All code paths tested, high confidence
```

---

## Red Flags

🚨 If after refactoring:
- Complexity INCREASED → Refactor not effective
- Coverage DECREASED → Missing tests
- LOC INCREASED significantly → Over-engineering
- Test time INCREASED significantly → Performance regression

