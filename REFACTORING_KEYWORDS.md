# Refactoring Prompt - Keywords & Quick Reference

## 🎯 Essential Keywords (Must Include)

### 1. OBJECTIVE Keywords
```
- Refactor [component] to [improve X]
- Purpose: [specific goal]
- Goal: [desired end state]
- Improve: readability | performance | maintainability | testability

Examples:
✓ "Refactor fault_manager to reduce cyclomatic complexity"
✓ "Purpose: Improve code readability and maintainability"
✗ "Make the code better" (too vague)
```

### 2. SCOPE Keywords
```
- Scope: [What's included]
- In Scope: [Specific files/methods]
- Out of Scope: [What's excluded]
- Affected Components: [List items]
- Touch Points: [Files that will change]

Examples:
✓ "In Scope: fault_manager.cpp, fault_manager_test.cpp"
✓ "Out of Scope: wheel_speed_monitor.hpp"
✗ "Refactor some files" (too vague)
```

### 3. CONSTRAINTS Keywords
```
- Must Keep:        [What cannot change]
- Must Not:         [Forbidden actions]
- Must Do:          [Required deliverables]
- Backward Compatibility: [API/behavior preservation]
- Cannot Break:     [Critical dependencies]

Examples:
✓ "Must Keep: Public API unchanged, function signatures identical"
✓ "Must Not: Change return types, add new dependencies"
✗ "Try not to break things" (too loose)
```

### 4. QUALITY CRITERIA Keywords
```
- Metrics:          [Measurable targets]
- Success Criteria: [How to verify success]
- Quality Standards: [Code quality level]
- Validation:       [What proves it works]

Examples:
✓ "Metrics: Complexity 8→<5, Coverage 100%"
✓ "Success Criteria: All 52 tests passing, no regression"
✗ "Make it good" (not measurable)
```

### 5. TESTING Keywords
```
- Test Coverage:    [Percentage requirement]
- Test Strategy:    [How to validate]
- Regression Testing: [What to verify]
- Test Scenarios:   [Specific cases]
- Validation:       [Proof of correctness]

Examples:
✓ "Test: All 52 tests must pass, ≥100% coverage"
✓ "Regression: Verify all edge cases still work"
✗ "Hope tests pass" (not a plan)
```

### 6. CONTEXT Keywords
```
- Why This Matters: [Business/technical justification]
- Current Issue:    [Problem statement]
- Motivation:       [Why now?]
- Background:       [Historical context]
- Impact:           [Effect of change]

Examples:
✓ "Why: Rising complexity makes maintenance difficult"
✓ "Current Issue: 8 branches in evaluate() method"
✗ "Just because" (no justification)
```

### 7. RISK Keywords
```
- Risk:             [Potential problems]
- Mitigation:       [How to prevent]
- Fallback:         [If something goes wrong]
- Backup:           [Recovery plan]
- Validation:       [Safety checks]

Examples:
✓ "Risk: Logic error | Mitigation: Run tests after each change"
✓ "Backup: Code in git, easy to revert"
✗ "Hope nothing breaks" (no risk planning)
```

### 8. DELIVERABLES Keywords
```
- Deliverables:     [What will be produced]
- Acceptance:       [How to verify completion]
- Documentation:    [What needs updating]
- Artifacts:        [Files/reports needed]

Examples:
✓ "Deliverables: Refactored code, updated tests, metrics report"
✓ "Documentation: Update code comments, add helper method docs"
✗ "Stuff that's changed" (too vague)
```

---

## 🏗️ Structural Components

### Structure Layer 1: Foundation
```
[EXECUTIVE SUMMARY]
↓
[SCOPE & COMPONENTS]
↓
[CURRENT STATE ANALYSIS]
↓
[DESIRED STATE]
```

### Structure Layer 2: Planning
```
[CONSTRAINTS & RULES]
↓
[PATTERNS & TECHNIQUES]
↓
[TESTING REQUIREMENTS]
↓
[QUALITY CRITERIA]
```

### Structure Layer 3: Execution
```
[DELIVERABLES]
↓
[IMPLEMENTATION WORKFLOW]
↓
[TIMELINE & ESTIMATE]
↓
[COMMUNICATION & HANDOFF]
```

### Structure Layer 4: Validation
```
[SUCCESS METRICS]
↓
[RISKS & MITIGATION]
↓
[CONTEXT & BACKGROUND]
↓
[SIGN-OFF]
```

---

## 📊 Keyword Frequency & Importance

### CRITICAL (Must Have - Appear 3+ times)
| Keyword | Why | Frequency |
|---------|-----|-----------|
| Scope | Must know what's affected | Section 2 + throughout |
| Testing | Validates correctness | Section 7 + verification |
| Metrics | Prove success objectively | Section 3, 8, 12 |
| Constraints | Prevents breaking changes | Section 5 + enforced |
| Deliverables | Know what's expected | Section 9 + handoff |

### IMPORTANT (Should Have - Appear 2+ times)
| Keyword | Why | Frequency |
|---------|-----|-----------|
| Quality Criteria | Defines excellence | Section 8, 12 |
| Risk | Safety planning | Section 13 + mitigation |
| Context | Understanding | Section 11 + motivation |
| Success Metrics | Completion proof | Section 12 + criteria |
| Documentation | Future maintainability | Section 9, deliverables |

### OPTIONAL (Nice to Have - Appear 1+ times)
| Keyword | Why | Frequency |
|---------|-----|-----------|
| Historical Context | Learning from past | Section 11 |
| Performance Metrics | Optimization focus | Section 3, 8 |
| Communication Plan | Team alignment | Section 15 |

---

## 🔑 Keyword Checklist

### Before Submitting Refactoring Prompt, Verify:

#### Scope & Clarity
- [ ] Objective is crystal clear (1-2 sentences)
- [ ] Scope explicitly lists files/methods
- [ ] "Out of Scope" section prevents surprises
- [ ] Constraints are enumerable

#### Measurement & Validation
- [ ] Success criteria are measurable (numbers, %)
- [ ] Metrics have current state → target state
- [ ] All tests documented (52 tests, ✓/✗)
- [ ] Quality thresholds defined

#### Risk & Safety
- [ ] Risks identified and ranked
- [ ] Mitigation for each risk
- [ ] Fallback/rollback plan clear
- [ ] Backward compatibility verified

#### Communication & Delivery
- [ ] Deliverables explicitly listed
- [ ] Timeline is realistic (with hours)
- [ ] Team members identified
- [ ] Sign-off criteria clear

---

## 💡 Common Keywords to USE vs AVOID

### Instead of This... | Use This
```
VAGUE ❌ → SPECIFIC ✓
---
"Make it better"         → "Reduce complexity from 8 to <5"
"Try not to break it"    → "Maintain 100% backward compatibility"
"Test it"                → "All 52 tests passing, ≥100% coverage"
"Update docs"            → "Update method docs, add precedence comments"
"When done"              → "4-5 hours, Phase 1-4 by [date]"
"It should work"         → "Verify with metrics report and code review"
```

---

## 🎨 Keyword Organization Pattern

### Pattern for Objective
```
REFACTOR [component] to [improve quality attribute]
by [specific technique/approach]
to achieve [measurable target]
```

**Examples:**
```
✓ REFACTOR fault_manager to improve readability
  by extracting helper methods
  to achieve cyclomatic complexity < 5

✓ REFACTOR validation logic to improve performance
  by caching results
  to achieve <5ms execution time

✓ REFACTOR error handling to improve maintainability
  by centralizing exception handling
  to achieve 100% error coverage
```

### Pattern for Constraints
```
MUST KEEP:   [Immutable contracts]
MUST NOT:    [Forbidden changes]
MUST DO:     [Required deliverables]
CANNOT BREAK:[Critical dependencies]
```

**Examples:**
```
✓ MUST KEEP: Public API, return types, behavior
✓ MUST NOT: Change function signatures, add dependencies
✓ MUST DO: Update tests, add comments, run validation
✓ CANNOT BREAK: 52 tests, 100% coverage
```

---

## 📋 Template Application Guide

### For Small Refactoring (1-2 hours)
Use sections: 1, 2, 3, 4, 5, 7, 8, 9, 12

### For Medium Refactoring (3-8 hours)
Use sections: 1-12 (all sections)

### For Large Refactoring (8+ hours)
Use sections: 1-15 (add project context and communication)

### For High-Risk Refactoring
Emphasize: 5 (Constraints), 13 (Risks), 14 (Timeline)

---

## 🚀 Quick Generation Checklist

### Step 1: Capture (15 mins)
- [ ] Identify component to refactor
- [ ] Write 1-line objective
- [ ] List scope: files affected
- [ ] Identify constraints

### Step 2: Analyze (15 mins)
- [ ] Current metrics/problems
- [ ] Target metrics/improvements
- [ ] Testing strategy
- [ ] Risk assessment

### Step 3: Plan (15 mins)
- [ ] Deliverables needed
- [ ] Workflow phases
- [ ] Timeline estimate
- [ ] Sign-off criteria

### Step 4: Finalize (10 mins)
- [ ] Review against template
- [ ] Verify all keywords present
- [ ] Check examples provided
- [ ] Get stakeholder buy-in

**Total Time: ~55 minutes to create comprehensive prompt**

---

## 📚 Reference Table: Keyword → Section Mapping

| Keyword | Primary Section | Why Important |
|---------|-----------------|---------------|
| Objective | 1, 4 | Defines the goal |
| Scope | 2 | Prevents surprises |
| Constraints | 5 | Maintains safety |
| Patterns | 6 | Guides implementation |
| Testing | 7 | Validates success |
| Quality | 8 | Defines excellence |
| Deliverables | 9 | Sets expectations |
| Workflow | 10 | Enables execution |
| Timeline | 14 | Manages expectations |
| Risk | 13 | Prevents problems |
| Success | 12 | Proves completion |

---

## ✅ Final Quality Checks

### Clarity Check
- [ ] Could a new team member understand this?
- [ ] Are all jargon terms defined?
- [ ] Are examples concrete, not abstract?

### Completeness Check
- [ ] All 15 sections addressed (or noted as N/A)?
- [ ] Metrics: Current → Target defined?
- [ ] Success Criteria: All measurable?

### Actionability Check
- [ ] Can someone start work immediately?
- [ ] Are steps logically ordered?
- [ ] Are blockers identified and mitigated?

---

## 🎓 Learning from Examples

### Good Refactoring Prompt Traits
✓ Specific metrics: "8 → <5" not "make better"
✓ Clear scope: Lists files, not vague components
✓ Measurable success: "52/52 tests" not "should work"
✓ Risk-aware: Identifies and mitigates problems
✓ Time-bounded: "4-5 hours" not "when done"

### Poor Refactoring Prompt Traits
✗ Vague goals: "Improve code quality"
✗ Undefined scope: "Refactor some stuff"
✗ No validation: "Hope tests pass"
✗ Ignores risk: "Just do it"
✗ Unbounded time: "Take as long as needed"

