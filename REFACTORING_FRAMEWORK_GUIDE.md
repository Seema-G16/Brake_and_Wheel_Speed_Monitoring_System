# Refactoring Prompt Framework - Complete Guide

## 📁 Files Created in This Framework

| File | Purpose | Length | Audience |
|------|---------|--------|----------|
| **REFACTORING_PROMPT_TEMPLATE.md** | Complete skeleton with 15 sections | 600 lines | Template creators |
| **REFACTORING_PROMPT_EXAMPLE.md** | Real example (Fault Manager refactor) | 400 lines | Developers |
| **REFACTORING_KEYWORDS.md** | Keywords reference & checklist | 350 lines | Team leads |
| **THIS FILE** | Integration & usage guide | 200 lines | Everyone |

---

## 🎯 Three-Part Framework

### Part 1: TEMPLATE (Template-oriented)
**File**: `REFACTORING_PROMPT_TEMPLATE.md`

**Contains**:
- 15 complete sections with explanations
- Keywords that MUST be present
- Usage guide for developers, reviewers, PMs
- Checklist before submission
- Tips for effective prompts

**When to Use**: Creating new refactoring prompts from scratch

**Example Usage**:
```
1. Copy template
2. Fill in each section
3. Check against keyword requirements
4. Verify metrics defined
5. Submit for review
```

---

### Part 2: EXAMPLE (Practice-oriented)
**File**: `REFACTORING_PROMPT_EXAMPLE.md`

**Contains**:
- Real refactoring scenario (Fault Manager)
- Before/after code comparison
- All 15 sections filled with concrete values
- Actual metrics: 8 → <5 complexity
- Real test cases: FLT-001 through FLT-012

**When to Use**: Learning how to write prompts, reference

**Example Usage**:
```
1. Read example completely
2. See how each section is filled
3. Understand complexity reductions
4. Use as template for similar projects
5. Adapt to your specific refactoring
```

---

### Part 3: KEYWORDS (Reference-oriented)
**File**: `REFACTORING_KEYWORDS.md`

**Contains**:
- 8 categories of essential keywords
- Which keywords MUST appear (critical vs. optional)
- Keyword frequency guidance
- Checklist before submission
- Vague vs. specific examples
- Keyword → Section mapping table

**When to Use**: Quality checking, keyword validation

**Example Usage**:
```
1. Before final review, check keyword presence
2. Verify metrics are specific (not vague)
3. Ensure success criteria measurable
4. Confirm constraints clear
5. Validate testing strategy defined
```

---

## 🔄 Recommended Workflow

### Step 1: Prepare (15 mins)
```
1. Use TEMPLATE as skeleton
2. Identify component to refactor
3. Write objective clearly
4. List scope and constraints
```
→ **Output**: Draft sections 1-5

### Step 2: Plan (20 mins)
```
1. Identify patterns to use (Section 6)
2. Plan testing strategy (Section 7)
3. Define quality criteria (Section 8)
4. List deliverables (Section 9)
```
→ **Output**: Sections 6-9 complete

### Step 3: Detail (15 mins)
```
1. Create workflow phases (Section 10)
2. Estimate timeline (Section 14)
3. Identify risks (Section 13)
4. Add context (Section 11)
5. Define success metrics (Section 12)
```
→ **Output**: Sections 10-14 complete

### Step 4: Review (10 mins)
```
1. Check against KEYWORDS guide
2. Verify all keywords present
3. Ensure metrics are specific
4. Confirm success criteria measurable
5. Validate risk mitigation
```
→ **Output**: Final, polished prompt

**Total Time**: ~60 minutes to create comprehensive prompt

---

## 📊 Keywords Hierarchy

### Tier 1: CRITICAL (Must Include)
These keywords must appear multiple times:
```
✓ Scope          - What's in/out
✓ Constraints    - What can't change
✓ Testing        - How to validate
✓ Metrics        - Current → Target
✓ Deliverables   - What's expected
```

### Tier 2: IMPORTANT (Should Include)
These keywords should appear 2+ times:
```
✓ Quality Criteria   - Definition of excellence
✓ Risk               - What could go wrong
✓ Context            - Why this matters
✓ Success Criteria   - Completion proof
✓ Documentation      - What to update
```

### Tier 3: CONTEXTUAL (Nice to Have)
These keywords appear based on specific needs:
```
✓ Performance       - If optimizing speed
✓ Communication     - If cross-team work
✓ Historical Note   - If building on past
```

---

## 🎓 Learning Path

### For First-Time Users
1. **Start**: Read REFACTORING_KEYWORDS.md (15 mins)
2. **Learn**: Study REFACTORING_PROMPT_EXAMPLE.md (30 mins)
3. **Create**: Write your first prompt using TEMPLATE (45 mins)
4. **Review**: Check against KEYWORDS checklist (10 mins)

**Total**: ~2 hours to become proficient

### For Experienced Users
1. **Reference**: Quickly scan KEYWORDS for checklist
2. **Create**: Write prompt using internalized structure
3. **Validate**: Final keyword check
4. **Submit**: Ready for implementation

**Total**: ~30-45 minutes per prompt

### For Team Leads
1. **Audit**: Use KEYWORDS to review prompts from team
2. **Mentor**: Guide using EXAMPLE and TEMPLATE
3. **Quality Gate**: Enforce metrics specificity
4. **Process**: Establish refactoring workflow

---

## 🛠️ Practical Application

### Scenario 1: Small Bug Fix Refactoring
**Time**: 2-4 hours

**Sections to Emphasize**:
- [1] Executive Summary
- [2] Scope (very focused)
- [5] Constraints (maintain compatibility)
- [7] Testing (comprehensive)
- [12] Success Metrics

**Template**: 40% of full template (6 sections)

### Scenario 2: Medium Refactoring
**Time**: 4-8 hours

**Sections to Emphasize**:
- All 15 sections (full template)
- Focus on [6] Patterns & [13] Risks
- Detailed [10] Workflow
- Specific [14] Timeline

**Template**: 100% of full template

### Scenario 3: Large Architecture Refactoring
**Time**: 8+ hours, multiple sprints

**Sections to Emphasize**:
- Extensive [11] Context & Background
- Detailed [13] Risk Management
- Comprehensive [15] Communication Plan
- Detailed Phase breakdowns in [10]

**Template**: 100% + additional context sections

---

## ✅ Quality Assurance Checklist

### Pre-Development Review
- [ ] Objective is crystal clear (1 sentence)
- [ ] Scope: exact files/methods listed
- [ ] Constraints: backward compatibility verified
- [ ] Metrics: current → target defined
- [ ] Testing: all test cases identified

### Mid-Development Review
- [ ] Tests still passing after each change
- [ ] Code follows constraints
- [ ] Documentation updating in parallel
- [ ] Metrics tracking progress

### Post-Development Review
- [ ] All metrics targets met
- [ ] All tests passing (100%)
- [ ] No regressions found
- [ ] Documentation complete
- [ ] Sign-off obtained

---

## 🎯 Success Metrics Framework

### Measurable Success (Always Include)
```
Current State: [Specific number]
Target State: [Specific number]
Success: [Objective proof]

Examples:
✓ Complexity: 8 branches → 4 branches (reduced 50%)
✓ Coverage: 90% → 100% (increased 10%)
✓ Tests: 45/52 passing → 52/52 passing (fixed 7)
✗ "Make it better" (not measurable)
✗ "Should be good" (not objective)
```

### Validation Proof (Always Include)
```
How to Prove Success:
✓ Automated: Run test suite (52/52 must pass)
✓ Metrics: Generate complexity report
✓ Review: Code review approved
✓ Benchmark: Performance unchanged

Not Acceptable:
✗ "Feels better"
✗ "Probably works"
✗ "Should be fine"
```

---

## 📋 Template Adaptation Guide

### Adapting for Different Code Types

#### For C++ Code (like this project)
Use full template sections:
- [6] Patterns: Emphasize C++ idioms
- [8] Quality: C++ standards (C++11, C++17, etc.)
- [7] Testing: GoogleTest specifics

#### For Python Code
Adapt sections:
- [6] Patterns: List Python-specific refactorings
- [8] Quality: PEP-8, linting tools
- [7] Testing: pytest framework

#### For JavaScript/TypeScript
Adapt sections:
- [6] Patterns: ES6+, functional approaches
- [8] Quality: ESLint, TypeScript strict mode
- [7] Testing: Jest, Mocha frameworks

---

## 🚀 Integration with Your Project

### Current Project Status
```
✓ 52 tests passing
✓ 100% requirement coverage
✓ Production ready
✓ Refactoring prompt framework created
```

### Next Steps for Your Team
1. **Adopt**: Use these templates for future refactoring
2. **Customize**: Adapt to your coding standards
3. **Share**: Train team on prompt creation
4. **Enforce**: Require full prompts before refactoring
5. **Measure**: Track refactoring success using metrics

### Recommended First Refactoring
Use the **REFACTORING_PROMPT_EXAMPLE.md**:
- Component: Fault Manager
- Goal: Reduce complexity 8 → <5
- Effort: 4-5 hours
- Risk: LOW (100% test coverage provides safety)
- Benefit: Practice framework + real improvement

---

## 📊 Template Statistics

### TEMPLATE File Breakdown
| Section | Lines | Purpose |
|---------|-------|---------|
| Keywords | 20 | Mandatory content |
| Structure | 50 | 15-section framework |
| Patterns | 60 | Refactoring techniques |
| Examples | 80 | Usage demonstrations |
| Checklist | 30 | Quality gates |
| Tips | 40 | Best practices |
| **TOTAL** | **600** | **Complete template** |

### EXAMPLE File Breakdown
| Section | Lines | Purpose |
|---------|-------|---------|
| Template Instance | 15 | Filled-in sections 1-5 |
| Metrics Table | 10 | Quantified targets |
| Test Cases | 20 | Specific test validation |
| Before/After | 40 | Code comparison |
| Details | 315 | Full sections 6-15 |
| **TOTAL** | **400** | **Working example** |

### KEYWORDS File Breakdown
| Section | Lines | Purpose |
|---------|-------|---------|
| Essential Keywords | 80 | 8 categories |
| Structure | 60 | Pattern organization |
| Checklist | 40 | Validation |
| Examples | 100 | Good vs. bad |
| Tables | 40 | Reference data |
| **TOTAL** | **350** | **Reference guide** |

---

## 🎁 Ready-to-Use Templates

### Quick Copy-Paste Framework

**For New Refactoring Prompt:**
1. Copy REFACTORING_PROMPT_TEMPLATE.md structure
2. Fill in your specific values
3. Check against REFACTORING_KEYWORDS.md
4. Submit for review

**For Review Checklist:**
Use REFACTORING_KEYWORDS.md sections:
- "Essential Keywords"
- "Keyword Frequency & Importance"
- "Keyword Checklist"

**For Team Training:**
Share all three files + this guide

---

## 🎯 Success Indicators

### Framework Adoption Success
- [ ] First prompt created using template
- [ ] All 15 sections completed
- [ ] All keywords present
- [ ] Metrics specific (not vague)
- [ ] Success criteria measurable
- [ ] Team trained on framework
- [ ] Refactoring completed on schedule
- [ ] Metrics targets met
- [ ] All tests passing

---

## 📞 Support & Questions

### For Template Questions
→ See: `REFACTORING_PROMPT_TEMPLATE.md` Section 16: Checklist

### For Practical Guidance
→ See: `REFACTORING_PROMPT_EXAMPLE.md` (real walkthrough)

### For Keyword Validation
→ See: `REFACTORING_KEYWORDS.md` Checklist section

### For Workflow Help
→ See: TEMPLATE Section 10: Implementation Workflow

---

## 🏁 Quick Start

### Copy-Paste This When Creating a Prompt:
```markdown
# REFACTORING PROMPT: [YOUR PROJECT] - [COMPONENT]

## 1. EXECUTIVE SUMMARY
[One sentence objective]

## 2. SCOPE & AFFECTED COMPONENTS
Files In Scope:
- [ ] File 1
- [ ] File 2

## 3. CURRENT STATE ANALYSIS
Issue: [Problem]
Metrics: [Current state]

## 4. DESIRED STATE
Goal: [Target state]
Target Metrics: [Targets]

## 5. CONSTRAINTS & RULES
Must Keep: [Immutable]
Must Not: [Forbidden]
Must Do: [Required]

## 6. REFACTORING PATTERNS
- [ ] Pattern 1
- [ ] Pattern 2

## 7. TESTING REQUIREMENTS
Test Cases: [List]
Success: [All X tests passing]

## 8. QUALITY CRITERIA
Metrics: [Current → Target]

## 9. DELIVERABLES
- [ ] Refactored code
- [ ] Updated tests
- [ ] Documentation

## 10. IMPLEMENTATION WORKFLOW
Phase 1: [hours]
Phase 2: [hours]
Phase 3: [hours]
Phase 4: [hours]

## 11. CONTEXT & BACKGROUND
Why: [Justification]

## 12. SUCCESS METRICS
Proof: [How to verify]

## 13. RISKS & MITIGATION
Risk: [Problem] | Mitigation: [Solution]

## 14. TIMELINE & ESTIMATE
Hours: [X-Y hours]
Priority: [Level]

## 15. COMMUNICATION & HANDOFF
Team: [Members]
```

**Estimated Fill Time**: 45-60 minutes

---

## 📖 Navigation Guide

### If you need...
```
✓ Full template structure    → REFACTORING_PROMPT_TEMPLATE.md
✓ Practical example         → REFACTORING_PROMPT_EXAMPLE.md
✓ Keyword reference         → REFACTORING_KEYWORDS.md
✓ How to use these files    → THIS FILE
```

### If you want to...
```
✓ Create first prompt       → Read EXAMPLE (30 min) + Use TEMPLATE
✓ Review someone's prompt   → Check against KEYWORDS checklist
✓ Train your team          → Share all 3 files + walkthrough
✓ Adapt for your project   → Read EXAMPLE + customize TEMPLATE
```

---

## ✨ Summary

You now have a **complete refactoring prompt framework**:

| File | Use Case | Time |
|------|----------|------|
| TEMPLATE | Create new prompts | 45-60 min |
| EXAMPLE | Learn & reference | 20-30 min |
| KEYWORDS | Validate & checklist | 5-10 min |
| THIS GUIDE | Navigation & overview | 10-15 min |

**Total Framework**: ~2 hours to master, ~30-45 min per prompt thereafter

**Ready to use immediately on your Brake & Wheel Speed Monitoring System!** 🚀

