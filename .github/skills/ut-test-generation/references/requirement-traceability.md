# Requirement ID to Test Case Mapping

This document describes how requirement IDs map to test cases in the `Docs/ut-test-design/testspec.csv` file.

## Requirement Categories

### BRK — Brake Monitoring Requirements

| Requirement | Description | Test Case ID Pattern |
|-------------|-------------|---------------------|
| BRK-01 | Brake System Active (pedal + speed > 0) | TC-BRK-001, TC-BRK-002, TC-BRK-003 |
| BRK-02 | Brake Pressure Monitoring (when braking, pressure > 0) | TC-BRK-001, TC-BRK-005 |
| BRK-03 | Brake Pressure Fault (braking + pressure = 0) | TC-BRK-004, TC-BRK-005 |
| BRK-04 | Stationary Vehicle (speed = 0 → no fault) | TC-BRK-003, TC-BRK-006 |
| BRK-05 | Brake Pressure Range (0 ≤ pressure ≤ 100 bar) | TC-BRK-007, TC-BRK-008 |

### WHL — Wheel-Speed Monitoring Requirements

| Requirement | Description | Test Case ID Pattern |
|-------------|-------------|---------------------|
| WHL-01 | Wheel-Speed Comparison (compare each wheel vs vehicle speed) | TC-WHL-001 through TC-WHL-003 |
| WHL-02 | Wheel-Speed Mismatch (diff > 10 km/h) | TC-WHL-002, TC-WHL-003 |
| WHL-03 | Multiple Wheel Mismatch (multiple wheels > 10 km/h) | TC-WHL-003, TC-WHL-004 |
| WHL-04 | Boundary Condition (exactly 10 km/h = no fault) | TC-WHL-005, TC-WHL-006 |
| WHL-05 | Braking Condition (pedal + speed > 0 for spread check) | TC-WHL-007 through TC-WHL-010 |
| WHL-06 | Wheel-Speed Spread (max - min during braking) | TC-WHL-007, TC-WHL-008 |
| WHL-07 | Braking Wheel-Speed Fault (spread > 15 km/h) | TC-WHL-008, TC-WHL-009 |
| WHL-08 | Boundary Condition (exactly 15 km/h = no fault) | TC-WHL-010, TC-WHL-011 |

### FLT — Fault Manager Requirements

| Requirement | Description | Test Case ID Pattern |
|-------------|-------------|---------------------|
| FLT-01 | Multiple Simultaneous Faults | TC-FLT-001, TC-FLT-002 |
| FLT-02 | Fault Aggregation (all faults reported) | TC-FLT-001, TC-FLT-002 |
| FLT-03 | Fault Recovery (faults cleared when condition resolves) | TC-FLT-003, TC-FLT-004 |
| FLT-04 | Fault Persistence (fault remains until condition resolves) | TC-FLT-003, TC-FLT-004 |

### DAT — Data Validation Requirements

| Requirement | Description | Test Case ID Pattern |
|-------------|-------------|---------------------|
| DAT-01 | Valid Input Range (all signals within valid range) | TC-DAT-001, TC-DAT-002 |
| DAT-02 | Invalid Data Detection (out-of-range values) | TC-DAT-003, TC-DAT-004 |
| DAT-03 | Invalid Data Handling (invalid data short-circuits fault logic) | TC-DAT-003, TC-DAT-004 |
| DAT-04 | Error Reporting (INVALID_MONITORING_DATA / INVALID_X) | TC-DAT-005, TC-DAT-006 |

## Test Case Naming Convention

Test case IDs follow the pattern: `TC-{REQ_TYPE}-{NUM}`

- `REQ_TYPE`: BRK, WHL, FLT, or DAT
- `NUM`: 3-digit zero-padded number (001–999)

Example: `TC-BRK-001`, `TC-WHL-007`, `TC-FLT-003`, `TC-DAT-004`

## Coverage Requirement

**Minimum**: Every requirement (BRK-01–05, WHL-01–08, FLT-01–04, DAT-01–04) must be covered by at least one test case.

**Ideal**: Boundary conditions should be tested on both sides (e.g., speed = 9.99 km/h and speed = 10.01 km/h for a 10 km/h threshold).
