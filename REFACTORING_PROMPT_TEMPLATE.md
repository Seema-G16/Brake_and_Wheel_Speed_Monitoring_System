# Refactoring Code Prompt Template

## Purpose
This template provides a structured skeleton for creating effective code refactoring prompts. It ensures all critical information is included and helps guide the refactoring process systematically.

---

## Essential Keywords & Components

### Keywords That MUST Be Present
- ✓ **Objective**: Clear goal of refactoring (readability, performance, maintainability, etc.)
- ✓ **Scope**: Which files/functions/modules are affected
- ✓ **Constraints**: What cannot be changed (API contracts, behavior, etc.)
- ✓ **Quality Criteria**: How to measure success
- ✓ **Testing**: How changes will be validated
- ✓ **Backward Compatibility**: Breaking vs. non-breaking changes
- ✓ **Timeline/Priority**: Urgency level
- ✓ **Context**: Why this refactoring is needed

### Optional But Recommended
- Performance metrics/targets
- Code metrics (complexity, duplication)
- Architecture principles to follow
- Coding standards to maintain
- Documentation requirements

---

## Refactoring Prompt Structure

```markdown
# REFACTORING PROMPT TEMPLATE

## 1. EXECUTIVE SUMMARY
[One-sentence goal of refactoring]

### Why This Matters
[Business/technical justification]

---

## 2. SCOPE & AFFECTED COMPONENTS

### Files/Modules In Scope
- [ ] File/Module 1
- [ ] File/Module 2
- [ ] File/Module 3

### Out of Scope
- Explicitly list what should NOT be changed
- Reason for exclusion

---

## 3. CURRENT STATE ANALYSIS

### What Needs Improvement
**Issue 1**: [Describe problem]
- Impact: [How does this affect the system]
- Metrics: [Current state numbers if applicable]

**Issue 2**: [Describe problem]
- Impact: ...
- Metrics: ...

### Code Metrics (if applicable)
- Cyclomatic Complexity: [Current → Target]
- Code Duplication: [Current → Target]
- Lines of Code: [Current → Target]
- Test Coverage: [Current → Target]

---

## 4. DESIRED STATE

### End Goal
[Description of what refactored code should look like]

### Key Improvements Expected
1. [Improvement 1]
2. [Improvement 2]
3. [Improvement 3]

### Target Metrics
- Cyclomatic Complexity: [Target]
- Code Duplication: [Target]
- Test Coverage: [Target]
- Performance: [Specific metrics]

---

## 5. CONSTRAINTS & RULES

### Must Keep (Backward Compatibility)
- [ ] Public API unchanged
- [ ] Function signatures preserved
- [ ] Behavior identical to current implementation
- [ ] Return types same
- [ ] Performance within X% of current

### Must Not
- ✗ Don't change external interfaces
- ✗ Don't modify database schema
- ✗ Don't alter algorithm fundamentals
- ✗ Don't introduce new dependencies

### Must Do
- ✓ Add/update unit tests
- ✓ Update documentation
- ✓ Follow coding standards
- ✓ Maintain error handling

---

## 6. REFACTORING PATTERNS & TECHNIQUES

### Recommended Approaches
- [ ] Extract Method
- [ ] Extract Class
- [ ] Rename Variable/Function
- [ ] Remove Duplication
- [ ] Simplify Conditional
- [ ] Reduce Cyclomatic Complexity
- [ ] Improve Error Handling
- [ ] Optimize Performance

### Anti-Patterns to Avoid
- Don't create god classes
- Don't use magic numbers
- Don't duplicate logic
- Don't ignore error cases

---

## 7. TESTING REQUIREMENTS

### Pre-Refactoring
- [ ] All tests passing (baseline)
- [ ] Code coverage measured
- [ ] Performance baseline established

### During Refactoring
- [ ] Unit tests pass after each change
- [ ] No regression in functionality
- [ ] All edge cases tested

### Post-Refactoring
- [ ] 100% of tests pass
- [ ] Code coverage maintained/improved
- [ ] Performance targets met
- [ ] New tests added for refactored code

### Validation
```
Success Criteria:
- [ ] All X tests passing
- [ ] Code coverage: ≥ Y%
- [ ] Complexity reduced by ≥ Z%
- [ ] No performance regression
- [ ] Documentation updated
```

---

## 8. QUALITY CRITERIA

### Code Quality Standards
- **Style**: [Coding standard to follow]
- **Naming**: [Convention to use]
- **Comments**: [Comment requirements]
- **Documentation**: [Doc standards]
- **Error Handling**: [Error handling approach]

### Metrics Threshold
| Metric | Current | Target | ✓/✗ |
|--------|---------|--------|-----|
| Cyclomatic Complexity | X | < X | |
| Duplication % | X% | < X% | |
| Test Coverage | X% | ≥ X% | |
| Code Churn | High | Low | |

---

## 9. DELIVERABLES

### What Needs to Be Delivered
- [ ] Refactored source code
- [ ] Updated unit tests
- [ ] Updated integration tests
- [ ] Documentation updates
- [ ] Migration guide (if needed)
- [ ] Performance report
- [ ] Refactoring summary

### Documentation to Update
- [ ] README.md
- [ ] Code comments
- [ ] Architecture docs
- [ ] API documentation
- [ ] Release notes

---

## 10. IMPLEMENTATION WORKFLOW

### Phase 1: Preparation (2-3 hours)
- [ ] Review current code thoroughly
- [ ] Understand existing tests
- [ ] Document baseline metrics
- [ ] Plan refactoring strategy

### Phase 2: Refactoring (X hours)
- [ ] Apply refactoring patterns
- [ ] Keep tests green after each change
- [ ] Update documentation incrementally
- [ ] Commit logical chunks

### Phase 3: Testing & Validation (1-2 hours)
- [ ] Run full test suite
- [ ] Verify metrics
- [ ] Performance testing
- [ ] Code review

### Phase 4: Documentation (30 mins)
- [ ] Summarize changes
- [ ] Update guides
- [ ] Create migration notes if needed

---

## 11. CONTEXT & BACKGROUND

### Why This Refactoring
[Explain business or technical drivers]

### Related Issues/PRs
- Issue #X: [Link and brief description]
- PR #Y: [Link and brief description]

### Historical Context
[Any previous refactoring attempts or related work]

---

## 12. SUCCESS METRICS & VERIFICATION

### How We Know It's Done
```
✓ All tests pass (52/52)
✓ Code coverage: ≥ 90%
✓ Cyclomatic complexity: Reduced by ≥ 20%
✓ Code duplication: < 5%
✓ Performance: No regression
✓ Documentation: Updated
✓ Code review: Approved
```

### Sign-Off Criteria
- [ ] Developer: Tests pass, metrics met
- [ ] Code reviewer: Approved refactoring
- [ ] QA: Validated in test environment
- [ ] Product: Approved for release

---

## 13. RISKS & MITIGATION

### Potential Risks
| Risk | Probability | Impact | Mitigation |
|------|-------------|--------|-----------|
| Regression | Low | High | Comprehensive testing |
| Performance Issue | Low | Medium | Benchmark before/after |
| Breaking Change | Low | High | API contract validation |

---

## 14. TIMELINE & ESTIMATE

### Effort Estimation
- Complexity Level: [LOW / MEDIUM / HIGH]
- Estimated Hours: [X-Y hours]
- Priority Level: [HIGH / MEDIUM / LOW]
- Deadline: [Date if applicable]

### Milestones
- Phase 1 Complete: [Date]
- Phase 2 Complete: [Date]
- Testing Complete: [Date]
- Ready for Review: [Date]

---

## 15. COMMUNICATION & HANDOFF

### Team Members Involved
- Developer: [Name]
- Reviewer: [Name]
- QA: [Name]

### Status Updates
- Daily standup: Check progress
- Code review: Continuous feedback
- Final approval: Before merge

---

## Example Prompt (Fill-in Template)

```markdown
# REFACTOR: [PROJECT NAME] - [COMPONENT]

## Executive Summary
Refactor [component] to improve [readability/performance/maintainability] by [specific approach].

## Scope
**Files**: [List files]
**Functions**: [List functions]

## Current Issues
1. **Issue**: [Problem description]
   - **Impact**: [Negative effect]
   - **Metric**: Cyclomatic complexity = 15 (Target: < 10)

## Goals
- Reduce cyclomatic complexity by 30%
- Eliminate 100% of code duplication
- Maintain 100% test coverage
- Zero performance regression

## Constraints
✓ Public API must remain unchanged
✗ Cannot modify external interfaces
✗ Cannot add new dependencies

## Testing
- Pre: All 52 tests passing ✓
- During: Continuous test validation
- Post: 100% tests passing + ≥90% coverage

## Success Criteria
- [ ] Complexity: 15 → 10
- [ ] Coverage: 90% → 95%
- [ ] Tests: 52/52 passing
- [ ] Documentation: Updated
- [ ] Code review: Approved

## Estimated Effort
- Complexity: MEDIUM
- Hours: 8-12
- Priority: HIGH
- Deadline: [Date]
```

---

## Usage Guide

### For Developers
1. Read entire prompt carefully
2. Note constraints and scope
3. Focus on quality criteria
4. Test continuously
5. Follow workflow phases

### For Code Reviewers
1. Verify scope respected
2. Check quality metrics met
3. Ensure tests comprehensive
4. Validate no regressions
5. Check documentation updated

### For Project Managers
1. Monitor timeline/milestones
2. Track estimated vs actual
3. Note risks and mitigations
4. Ensure communication clear
5. Validate deliverables complete

---

## Tips for Effective Refactoring Prompts

✓ **Be Specific**: "Reduce cyclomatic complexity" → "Reduce from 15 to <10"
✓ **Set Boundaries**: Clear scope prevents scope creep
✓ **Define Success**: Measurable criteria, not vague goals
✓ **Respect Constraints**: Clearly state what cannot change
✓ **Plan Testing**: Test strategy from the start
✓ **Include Context**: Why matters more than what sometimes
✓ **Provide Examples**: Show before/after if possible
✓ **Update as You Go**: Modify prompt if requirements change

---

## Common Refactoring Patterns

### Extract Method
```
Problem: Long function with multiple concerns
Solution: Break into smaller, focused functions
Benefit: Improved readability and testability
```

### Extract Class
```
Problem: Class has too many responsibilities
Solution: Separate concerns into distinct classes
Benefit: Better separation of concerns, reusability
```

### Remove Duplication
```
Problem: Same code in multiple places
Solution: Create shared function/method
Benefit: Maintenance, consistency, reduced errors
```

### Simplify Conditional
```
Problem: Complex if/else logic
Solution: Use guard clauses, early returns
Benefit: Easier to read and understand flow
```

---

## Checklist Before Submitting Refactoring Prompt

- [ ] Clear objective stated
- [ ] Scope well-defined
- [ ] Constraints listed
- [ ] Success criteria measurable
- [ ] Testing strategy clear
- [ ] Timeline realistic
- [ ] Resources identified
- [ ] Risks considered
- [ ] Context provided
- [ ] Deliverables explicit
- [ ] Quality standards defined
- [ ] Communication plan included

