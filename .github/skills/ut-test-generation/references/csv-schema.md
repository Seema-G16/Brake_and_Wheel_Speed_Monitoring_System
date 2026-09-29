# Testspec CSV Format & Validation Rules

This document describes the structure and validation rules for `Docs/ut-test-design/testspec.csv`.

## CSV Columns

The testspec.csv file must include the following columns in this order:

| Column | Required | Type | Description |
|--------|----------|------|-------------|
| Testcase ID | Yes | Text | Unique test case identifier (TC-XXX-NNN) |
| Testcase Name | Yes | Text | Short, descriptive name of the test |
| Test Description | Yes | Text | Detailed description of what the test verifies |
| Input | Yes | Text | Input parameters and values for the test |
| Test Case Type | Yes | Choice | Positive, Negative, Edge, Recovery, or Boundary |
| Expected Output | Yes | Text | What the system should output when the test runs |
| Actual Output | No | Text | Actual result (typically "Pending" during test design) |
| Automated (Yes/No) | Yes | Choice | Yes or No (whether the test is automated) |
| Comments | No | Text | Additional notes or cross-references to requirements |

## Field Validation Rules

### Testcase ID
- **Pattern**: `TC-{REQ_TYPE}-{NUM}`
- **Example**: `TC-BRK-001`, `TC-WHL-007`
- **Rules**:
  - Must start with `TC-`
  - REQ_TYPE must be BRK, WHL, FLT, or DAT
  - NUM must be 3 digits (zero-padded)
  - Must be unique across the entire CSV

### Testcase Name
- **Type**: String
- **Rules**:
  - Must not be empty
  - Should clearly describe the test scenario (e.g., "Brake pressure fault at zero speed boundary")
  - 50–150 characters is typical

### Test Description
- **Type**: String (may contain newlines)
- **Rules**:
  - Must not be empty
  - Should explain what is being tested and why
  - Typically 100–300 characters

### Input
- **Type**: String (may contain newlines)
- **Format**: Key=value pairs, comma or newline-separated
  - Example: `VehicleSpeed=50 km/h, BrakePedal=TRUE, BrakePressure=0 bar, FL=50 km/h, FR=50 km/h, RL=50 km/h, RR=50 km/h`
  - Example: `Timestamp=1.0s\nVehicleSpeed=50 km/h\nBrakePedal=TRUE\nBrakePressure=40 bar\nFL=50 km/h\nFR=50 km/h\nRL=50 km/h\nRR=50 km/h`
- **Rules**:
  - Must not be empty
  - Should include all 8 input signals: Timestamp, VehicleSpeed, BrakePedal, BrakePressure, FL, FR, RL, RR

### Test Case Type
- **Type**: Choice
- **Valid Values**: `Positive`, `Negative`, `Edge`, `Recovery`, `Boundary`
- **Guidance**:
  - **Positive**: Normal operation, expected behavior (e.g., braking with valid pressure)
  - **Negative**: Invalid input or error condition (e.g., negative brake pressure)
  - **Edge**: Boundary conditions (e.g., exactly 0 km/h or exactly 100 bar)
  - **Boundary**: Same as Edge; used interchangeably for threshold tests
  - **Recovery**: Fault recovery scenarios (e.g., fault clears when condition resolves)

### Expected Output
- **Type**: String (may contain newlines)
- **Format**: Key=value pairs, comma or newline-separated
  - Example: `BrakeStatus=NORMAL; Faults=NO_FAULT; OverallStatus=NORMAL`
  - Example: `BrakeStatus=FAULT; Faults=BRAKE_PRESSURE_FAULT, RL_WHEEL_SPEED_MISMATCH; OverallStatus=FAULT`
- **Rules**:
  - Must not be empty
  - Fault names must match those defined in `Docs/requirements.md` (BRAKE_PRESSURE_FAULT, RL_WHEEL_SPEED_MISMATCH, etc.)
  - Multiple faults should be comma-separated

### Actual Output
- **Type**: String
- **Valid Values**: `Pending`, `Pass`, `Fail`, or test-specific result
- **Rules**:
  - Typically left as `Pending` during test design
  - Updated to `Pass` or `Fail` after running the test
  - May contain error details if the test fails

### Automated (Yes/No)
- **Type**: Choice
- **Valid Values**: `Yes`, `No`
- **Rules**:
  - Must not be empty
  - GoogleTest-based UT tests should have `Automated=Yes`
  - Manual or exploratory tests should have `Automated=No`

### Comments
- **Type**: String (optional)
- **Rules**:
  - May be empty
  - Should reference requirement IDs (e.g., "Covers BRK-01, BRK-02")
  - Can include cross-references (e.g., "See section 9 of requirements.md")

## Requirement Traceability

Each test case should be mapped to one or more requirement IDs. The mapping is typically done via:
1. **Testcase ID prefix**: `TC-BRK-001` implicitly covers `BRK-*` requirements
2. **Comments column**: Explicit references (e.g., "Covers BRK-01, BRK-02, WHL-03")

**Minimum Coverage**: All requirements (BRK-01–05, WHL-01–08, FLT-01–04, DAT-01–04) must be covered.

## CSV Auto-Fix Rules

The auto-fix script attempts to correct common issues:

| Issue | Auto-Fix Behavior |
|-------|------------------|
| Testcase ID format (e.g., `TC-BRK-1` → `TC-BRK-001`) | Pad with zeros |
| Missing Testcase Name | Use ID as fallback (e.g., "Test case BRK 001") |
| Missing Test Description | Copy from Testcase Name |
| Missing Input | Set to "Standard test input values" |
| Invalid Test Case Type | Infer from description or default to "Positive" |
| Missing Expected Output | Set to "Test passes with no faults" |
| Missing Automated (Yes/No) | Default to "Yes" |
| Invalid Automated values (`true`, `1`, etc.) | Normalize to `Yes` or `No` |
| Missing Actual Output | Set to "Pending" |

**Note**: The auto-fix script does NOT create missing test case rows. It only fixes fields within existing rows.
