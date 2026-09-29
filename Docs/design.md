# Software Design Document (SWDD)

## 1. Document Purpose

This document defines the software design for the Brake & Wheel-Speed Monitoring System MVP. It translates the requirements in [requirements.md](requirements.md) into an implementable C++17 command-line solution with clear module boundaries, data flow, validation rules, fault handling, and report generation behavior.

## 2. System Overview

The system processes simulated brake and wheel-speed monitoring data from a CSV file. For each sample, the application:

1. Reads the input sample.
2. Validates the sample.
3. Evaluates brake-pressure behavior.
4. Evaluates wheel-speed mismatch behavior.
5. Evaluates braking wheel-speed consistency behavior.
6. Aggregates active faults.
7. Generates a console report.

The application is a training MVP and does not communicate with a real vehicle, ECU, or CAN network.

## 3. Design Goals

- Provide a simple and testable architecture.
- Keep input parsing, validation, monitoring logic, fault aggregation, and reporting separate.
- Support multiple faults in a single sample.
- Support fault recovery by recalculating faults for each new sample.
- Use deterministic logic so automated tests can cover all rules and boundaries.

## 4. Scope

### In Scope

- CSV input parsing.
- Sample validation.
- Brake pressure monitoring.
- Wheel-speed mismatch monitoring.
- Braking wheel-speed spread monitoring.
- Fault aggregation.
- Console output generation.
- Unit tests with GoogleTest.

### Out of Scope

- Real vehicle communication.
- CAN bus integration.
- Graphical user interface.
- Persistent storage or database integration.
- Cloud services beyond CI and deployment support.

## 5. Architectural Style

The design uses a modular layered pipeline:

```text
Input Reader
  -> Monitoring Data Model
  -> Validation
  -> Brake Monitor
  -> Wheel-Speed Monitor
  -> Fault Manager
  -> Report Generator
```

This is a lightweight pipeline architecture with clear separation of responsibilities. Each stage receives a validated data structure and returns a well-defined result.

### 5.1 Requirements Flow Diagram

<svg xmlns="http://www.w3.org/2000/svg" width="1200" height="1320" viewBox="0 0 1200 1320" role="img" aria-labelledby="title desc">
  <title id="title">Brake and Wheel-Speed Monitoring Requirements Flow</title>
  <desc id="desc">A clean vertical flowchart showing input, validation, monitoring, fault aggregation, and reporting.</desc>
  <defs>
    <style>
      .bg { fill: #ffffff; }
      .box { fill: #f8fafc; stroke: #1f2937; stroke-width: 2.5; rx: 14; ry: 14; }
      .decision { fill: #fff7ed; stroke: #c2410c; stroke-width: 2.5; }
      .terminal { fill: #ecfdf5; stroke: #047857; stroke-width: 2.5; rx: 26; ry: 26; }
      .fault { fill: #fef2f2; stroke: #b91c1c; stroke-width: 2.5; rx: 14; ry: 14; }
      .text { font-family: Arial, Helvetica, sans-serif; font-size: 23px; fill: #111827; }
      .small { font-family: Arial, Helvetica, sans-serif; font-size: 17px; fill: #4b5563; }
      .title { font-family: Arial, Helvetica, sans-serif; font-size: 34px; font-weight: bold; fill: #111827; }
      .subtitle { font-family: Arial, Helvetica, sans-serif; font-size: 18px; fill: #4b5563; }
      .arrow { stroke: #374151; stroke-width: 3; fill: none; marker-end: url(#arrowhead); }
      .yes { font-family: Arial, Helvetica, sans-serif; font-size: 18px; fill: #047857; font-weight: bold; }
      .no { font-family: Arial, Helvetica, sans-serif; font-size: 18px; fill: #b91c1c; font-weight: bold; }
    </style>
    <marker id="arrowhead" markerWidth="10" markerHeight="10" refX="8" refY="5" orient="auto">
      <path d="M 0 0 L 10 5 L 0 10 z" fill="#374151" />
    </marker>
  </defs>

  <rect class="bg" x="0" y="0" width="1200" height="1320" />
  <text x="600" y="52" text-anchor="middle" class="title">Brake &amp; Wheel-Speed Monitoring Requirements Flow</text>
  <text x="600" y="82" text-anchor="middle" class="subtitle">The diagram shows one sample moving through validation, monitoring, fault handling, and reporting.</text>

  <rect class="terminal" x="470" y="115" width="260" height="58" />
  <text x="600" y="152" text-anchor="middle" class="text">Start</text>
  <line class="arrow" x1="600" y1="173" x2="600" y2="220" />

  <rect class="box" x="390" y="220" width="420" height="66" />
  <text x="600" y="261" text-anchor="middle" class="text">Read CSV sample</text>
  <line class="arrow" x1="600" y1="286" x2="600" y2="332" />

  <rect class="box" x="330" y="332" width="540" height="70" />
  <text x="600" y="375" text-anchor="middle" class="text">Build brake and wheel-speed monitoring sample</text>
  <line class="arrow" x1="600" y1="402" x2="600" y2="452" />

  <polygon class="decision" points="600,452 740,542 600,632 460,542" />
  <text x="600" y="538" text-anchor="middle" class="text">Valid input?</text>
  <text x="776" y="520" class="no">No</text>
  <text x="776" y="570" class="yes">Yes</text>

  <line class="arrow" x1="460" y1="542" x2="250" y2="542" />
  <rect class="fault" x="70" y="510" width="180" height="64" />
  <text x="160" y="549" text-anchor="middle" class="text">INVALID_DATA</text>
  <line class="arrow" x1="160" y1="574" x2="160" y2="628" />
  <rect class="fault" x="35" y="628" width="250" height="72" />
  <text x="160" y="671" text-anchor="middle" class="text">Report INVALID_MONITORING_DATA</text>
  <line class="arrow" x1="160" y1="700" x2="160" y2="760" />
  <rect class="terminal" x="70" y="760" width="180" height="58" />
  <text x="160" y="797" text-anchor="middle" class="text">End</text>

  <line class="arrow" x1="600" y1="632" x2="600" y2="692" />
  <text x="625" y="666" class="yes">Yes</text>

  <rect class="box" x="350" y="692" width="500" height="68" />
  <text x="600" y="734" text-anchor="middle" class="text">Brake monitor: braking condition and brake pressure</text>
  <line class="arrow" x1="600" y1="760" x2="600" y2="814" />

  <rect class="box" x="315" y="814" width="570" height="68" />
  <text x="600" y="856" text-anchor="middle" class="text">Wheel-speed monitor: mismatch and braking spread</text>
  <line class="arrow" x1="600" y1="882" x2="600" y2="934" />

  <rect class="box" x="390" y="934" width="420" height="68" />
  <text x="600" y="976" text-anchor="middle" class="text">Fault manager aggregates active faults</text>
  <line class="arrow" x1="600" y1="1002" x2="600" y2="1054" />

  <polygon class="decision" points="600,1054 740,1144 600,1234 460,1144" />
  <text x="600" y="1140" text-anchor="middle" class="text">Any valid faults active?</text>
  <text x="776" y="1124" class="no">No</text>
  <text x="776" y="1174" class="yes">Yes</text>

  <rect class="box" x="350" y="1248" width="500" height="68" />
  <text x="600" y="1290" text-anchor="middle" class="text">Generate console report</text>
  <line class="arrow" x1="600" y1="1234" x2="600" y2="1248" />

  <line class="arrow" x1="460" y1="1144" x2="300" y2="1144" />
  <rect class="fault" x="70" y="1110" width="180" height="64" />
  <text x="160" y="1149" text-anchor="middle" class="text">Overall: NORMAL</text>

  <line class="arrow" x1="740" y1="1144" x2="900" y2="1144" />
  <rect class="fault" x="900" y="1110" width="180" height="64" />
  <text x="990" y="1149" text-anchor="middle" class="text">Overall: FAULT</text>
</svg>

### 5.2 Component Architecture Diagram

<svg xmlns="http://www.w3.org/2000/svg" width="1200" height="620" viewBox="0 0 1200 620" role="img" aria-labelledby="arch-title arch-desc">
  <title id="arch-title">Brake and Wheel-Speed Monitoring Component Architecture</title>
  <desc id="arch-desc">Boxes for each module connected in pipeline order, with the Fault Types data structure feeding the Fault Manager and Report Generator.</desc>
  <defs>
    <style>
      .abg { fill: #ffffff; }
      .abox { fill: #f8fafc; stroke: #1f2937; stroke-width: 2.5; rx: 14; ry: 14; }
      .adata { fill: #eef2ff; stroke: #4338ca; stroke-width: 2.5; rx: 14; ry: 14; }
      .atext { font-family: Arial, Helvetica, sans-serif; font-size: 19px; fill: #111827; }
      .asmall { font-family: Arial, Helvetica, sans-serif; font-size: 14px; fill: #4b5563; }
      .atitle { font-family: Arial, Helvetica, sans-serif; font-size: 30px; font-weight: bold; fill: #111827; }
      .aarrow { stroke: #374151; stroke-width: 3; fill: none; marker-end: url(#arrowhead2); }
      .aarrow-dashed { stroke: #4338ca; stroke-width: 2.5; fill: none; stroke-dasharray: 8 6; marker-end: url(#arrowhead2); }
    </style>
    <marker id="arrowhead2" markerWidth="10" markerHeight="10" refX="8" refY="5" orient="auto">
      <path d="M 0 0 L 10 5 L 0 10 z" fill="#374151" />
    </marker>
  </defs>

  <rect class="abg" x="0" y="0" width="1200" height="620" />
  <text x="600" y="44" text-anchor="middle" class="atitle">Component Architecture</text>

  <rect class="abox" x="30" y="90" width="180" height="80" />
  <text x="120" y="122" text-anchor="middle" class="atext">CSV Reader</text>
  <text x="120" y="144" text-anchor="middle" class="asmall">src/input</text>

  <line class="aarrow" x1="210" y1="130" x2="260" y2="130" />

  <rect class="adata" x="260" y="90" width="200" height="80" />
  <text x="360" y="122" text-anchor="middle" class="atext">BrakeWheelSpeedSample</text>
  <text x="360" y="144" text-anchor="middle" class="asmall">monitoring_system</text>

  <line class="aarrow" x1="460" y1="130" x2="510" y2="130" />

  <rect class="abox" x="510" y="90" width="200" height="80" />
  <text x="610" y="122" text-anchor="middle" class="atext">MonitoringDataValidator</text>
  <text x="610" y="144" text-anchor="middle" class="asmall">validation</text>

  <line class="aarrow" x1="710" y1="130" x2="760" y2="130" />

  <rect class="abox" x="760" y="90" width="200" height="80" />
  <text x="860" y="122" text-anchor="middle" class="atext">BrakeMonitor</text>
  <text x="860" y="144" text-anchor="middle" class="asmall">brake</text>

  <line class="aarrow" x1="860" y1="170" x2="860" y2="230" />

  <rect class="abox" x="760" y="230" width="200" height="80" />
  <text x="860" y="262" text-anchor="middle" class="atext">WheelSpeedMonitor</text>
  <text x="860" y="284" text-anchor="middle" class="asmall">wheel_speed</text>

  <line class="aarrow" x1="860" y1="310" x2="860" y2="370" />

  <rect class="abox" x="760" y="370" width="200" height="80" />
  <text x="860" y="402" text-anchor="middle" class="atext">FaultManager</text>
  <text x="860" y="424" text-anchor="middle" class="asmall">faults</text>

  <line class="aarrow" x1="760" y1="410" x2="510" y2="410" />

  <rect class="adata" x="310" y="370" width="200" height="80" />
  <text x="410" y="402" text-anchor="middle" class="atext">FaultTypes / Status</text>
  <text x="410" y="424" text-anchor="middle" class="asmall">faults</text>

  <line class="aarrow" x1="860" y1="450" x2="860" y2="510" />

  <rect class="abox" x="760" y="510" width="200" height="80" />
  <text x="860" y="542" text-anchor="middle" class="atext">ReportGenerator</text>
  <text x="860" y="564" text-anchor="middle" class="asmall">reporting</text>

  <line class="aarrow-dashed" x1="410" y1="370" x2="410" y2="230" />
  <line class="aarrow-dashed" x1="410" y1="230" x2="710" y2="230" />
  <text x="420" y="200" class="asmall">validated sample</text>

  <line class="aarrow-dashed" x1="410" y1="450" x2="410" y2="550" />
  <line class="aarrow-dashed" x1="410" y1="550" x2="760" y2="550" />
  <text x="420" y="580" class="asmall">aggregated result feeds report</text>

  <rect class="abox" x="30" y="510" width="200" height="80" />
  <text x="130" y="542" text-anchor="middle" class="atext">main.cpp</text>
  <text x="130" y="564" text-anchor="middle" class="asmall">orchestrates the pipeline</text>

  <line class="aarrow" x1="230" y1="550" x2="760" y2="550" opacity="0" />
</svg>

The component diagram shows the module boundaries and data hand-off points: `CSV Reader` produces a `BrakeWheelSpeedSample`, which flows through `MonitoringDataValidator`, `BrakeMonitor`, and `WheelSpeedMonitor`. Both monitors and the validator feed `FaultManager`, which produces the fault list and overall status consumed by `ReportGenerator`. `main.cpp` orchestrates the full pipeline end to end.

## 6. High-Level Components

### 6.1 CSV Reader

Responsible for reading the CSV file and converting each row into a monitoring sample.

Responsibilities:

- Open and read the input file.
- Parse each row into fields.
- Convert field values into strongly typed data.
- Detect missing columns or malformed rows.
- Forward each parsed sample to validation.

### 6.2 Monitoring Data Model

Represents one sample of brake and wheel-speed monitoring data.

Responsibilities:

- Store timestamp.
- Store vehicle speed.
- Store brake pedal status.
- Store brake pressure.
- Store front-left, front-right, rear-left, and rear-right wheel speeds.

### 6.3 Validation Module

Checks whether the incoming sample is valid before any monitoring logic executes.

Responsibilities:

- Reject missing data.
- Reject negative vehicle speed.
- Reject negative wheel speed values.
- Reject invalid brake pedal status representations.
- Reject brake pressure outside the supported range.

### 6.4 Brake Monitor

Evaluates brake-pressure behavior.

Responsibilities:

- Determine whether the vehicle is in a braking condition.
- Check that brake pressure is greater than 0 bar during braking.
- Report brake-pressure faults.
- Do not raise brake-pressure fault when the vehicle is stationary.

### 6.5 Wheel-Speed Monitor

Evaluates wheel speed versus vehicle speed and braking spread behavior.

Responsibilities:

- Compare each wheel speed to vehicle speed.
- Report mismatch faults when the difference is greater than 10 km/h.
- Evaluate wheel-speed spread while braking.
- Report braking wheel-speed mismatch when spread is greater than 15 km/h.

### 6.6 Fault Manager

Collects active faults and determines overall status.

Responsibilities:

- Store zero or more active faults.
- Distinguish valid faults from invalid input conditions.
- Return NORMAL, FAULT, or INVALID_DATA as the overall status.

### 6.7 Report Generator

Produces a console report for each sample.

Responsibilities:

- Print the sample values.
- Print brake status and wheel-status results.
- Print active faults.
- Print overall status.

## 7. Detailed Module Design

### 7.1 Data Model

#### Class: BrakeWheelSpeedSample

Suggested header: `include/monitoring_system/brake_wheel_speed_sample.hpp`

Fields:

- `double timestampSeconds`
- `double vehicleSpeedKph`
- `bool brakePedalPressed`
- `double brakePressureBar`
- `double frontLeftWheelSpeedKph`
- `double frontRightWheelSpeedKph`
- `double rearLeftWheelSpeedKph`
- `double rearRightWheelSpeedKph`

Notes:

- All wheel-speed and brake-related values are numeric except brake pedal status.
- A CSV row maps directly into this structure.

### 7.2 Validation Design

#### Class: MonitoringDataValidator

Suggested header: `include/validation/monitoring_data_validator.hpp`

Responsibilities:

- Check field presence.
- Check numeric ranges.
- Check boolean representation for brake pedal status.

Validation Rules:

- Vehicle speed must be greater than or equal to 0.
- Wheel speeds must be greater than or equal to 0.
- Brake pressure must be within 0 to 100 bar.
- Brake pedal status must be valid boolean input.
- Missing data invalidates the sample.

Validation Result:

- `isValid`: true or false
- `fault`: `INVALID_MONITORING_DATA` when invalid data is detected

Design note:

- Invalid pressure is treated as invalid input, not a normal fault condition.

### 7.3 Brake Monitoring Design

#### Class: BrakeMonitor

Suggested header: `include/brake/brake_monitor.hpp`

Responsibilities:

- Determine whether braking is active.
- Evaluate brake pressure during braking.
- Emit brake-related faults.

Algorithm:

1. Check if brake pedal is pressed.
2. Check if vehicle speed is greater than 0 km/h.
3. If both are true, braking is active.
4. If braking is active and brake pressure is 0 bar, return `BRAKE_PRESSURE_FAULT`.
5. If vehicle speed is 0 km/h, do not report brake-pressure fault solely because pressure is 0 bar.

### 7.4 Wheel-Speed Monitoring Design

#### Class: WheelSpeedMonitor

Suggested header: `include/wheel_speed/wheel_speed_monitor.hpp`

Responsibilities:

- Compare each wheel speed against vehicle speed.
- Evaluate braking spread during active braking.

Algorithm for mismatch checks:

1. Compute absolute difference between each wheel speed and vehicle speed.
2. If difference > 10 km/h, add the corresponding wheel mismatch fault.
3. If difference = 10 km/h, do not report a fault.

Algorithm for braking spread check:

1. Check if braking is active.
2. If active, compute maximum wheel speed and minimum wheel speed.
3. Calculate spread = max - min.
4. If spread > 15 km/h, add `BRAKING_WHEEL_SPEED_MISMATCH`.

### 7.5 Fault Management Design

#### Class: FaultManager

Suggested header: `include/faults/fault_manager.hpp`

Responsibilities:

- Store active faults in a deterministic container.
- Preserve all simultaneous faults.
- Distinguish invalid input from valid fault conditions.

Suggested fault model:

- `NO_FAULT`
- `BRAKE_PRESSURE_FAULT`
- `INVALID_BRAKE_PRESSURE`
- `FL_WHEEL_SPEED_MISMATCH`
- `FR_WHEEL_SPEED_MISMATCH`
- `RL_WHEEL_SPEED_MISMATCH`
- `RR_WHEEL_SPEED_MISMATCH`
- `BRAKING_WHEEL_SPEED_MISMATCH`
- `INVALID_MONITORING_DATA`

Overall status rules:

- `INVALID_DATA` if validation fails.
- `FAULT` if one or more valid fault conditions are active.
- `NORMAL` if no faults are active.

### 7.6 Report Generation Design

#### Class: ReportGenerator

Suggested header: `include/reporting/report_generator.hpp`

Responsibilities:

- Render a readable console report.
- Print input values and computed statuses.
- List active faults one per line.

Suggested output sections:

- Timestamp
- Vehicle Speed
- Brake Pedal Status
- Brake Pressure
- Wheel Speeds
- Brake Status
- Wheel-Speed Status
- Active Faults
- Overall Status

## 8. Data Flow

The processing flow for one sample is:

1. Read row from CSV.
2. Convert row into `BrakeWheelSpeedSample`.
3. Validate the sample.
4. If invalid, produce invalid-data output and stop rule evaluation for that sample.
5. If valid, run brake monitoring.
6. Run wheel-speed monitoring.
7. Aggregate faults.
8. Generate report.

This flow ensures that invalid input does not trigger normal fault logic.

## 9. State and Fault Handling

The application does not maintain long-lived vehicle state across samples beyond reporting continuity. Each CSV sample is evaluated independently.

This approach supports fault recovery naturally:

- If a fault condition exists in one sample, the fault appears.
- If the next sample no longer meets that condition, the fault disappears.

No historical fault memory is required for the core MVP.

## 10. Interface Contracts

### 10.1 Validator Interface

Expected behavior:

```text
validate(sample) -> ValidationResult
```

Where `ValidationResult` indicates whether the sample is valid and, if not, returns `INVALID_MONITORING_DATA`.

### 10.2 Brake Monitor Interface

Expected behavior:

```text
evaluate(sample) -> set of brake-related faults
```

### 10.3 Wheel-Speed Monitor Interface

Expected behavior:

```text
evaluate(sample) -> set of wheel-related faults
```

### 10.4 Fault Manager Interface

Expected behavior:

```text
collect(validationResult, brakeFaults, wheelFaults) -> aggregated result
```

### 10.5 Report Generator Interface

Expected behavior:

```text
generate(sample, aggregatedResult) -> console output
```

## 11. Error Handling Strategy

### Invalid Input Cases

- Missing value
- Negative vehicle speed
- Negative wheel speed
- Invalid brake pedal representation
- Brake pressure outside 0 to 100 bar

### Handling Approach

- Mark sample as invalid.
- Set overall status to `INVALID_DATA`.
- Report `INVALID_MONITORING_DATA` or `INVALID_BRAKE_PRESSURE` according to the validation rule.
- Skip valid-fault evaluation when the sample itself is invalid.

## 12. Boundary Conditions

The design explicitly supports the requirement boundaries:

- Brake pressure = 0 bar
- Brake pressure = 100 bar
- Brake pressure > 100 bar
- Wheel-speed difference = 10 km/h
- Wheel-speed difference > 10 km/h
- Braking spread = 15 km/h
- Braking spread > 15 km/h

Boundary behavior is strict and deterministic.

## 13. File and Folder Structure

```text
Brake_and_Wheel_Speed_Monitoring_System/
|- .github/
|  |- Instructions/
|  |  |- copilot-instructions.md
|  |- workflows/
|  |  |- ci.yml
|- Docs/
|  |- requirements.md
|  |- design.md
|- data/
|  |- sample_brake_wheel_speed_data.csv
|- include/
|  |- monitoring_system/
|  |  |- brake_wheel_speed_sample.hpp
|  |- validation/
|  |  |- monitoring_data_validator.hpp
|  |- brake/
|  |  |- brake_monitor.hpp
|  |- wheel_speed/
|  |  |- wheel_speed_monitor.hpp
|  |- faults/
|  |  |- fault_types.hpp
|  |  |- fault_manager.hpp
|  |- reporting/
|  |  |- report_generator.hpp
|- src/
|  |- input/
|  |  |- csv_reader.cpp
|  |- monitoring_system/
|  |  |- brake_wheel_speed_sample.cpp
|  |- validation/
|  |  |- monitoring_data_validator.cpp
|  |- brake/
|  |  |- brake_monitor.cpp
|  |- wheel_speed/
|  |  |- wheel_speed_monitor.cpp
|  |- faults/
|  |  |- fault_manager.cpp
|  |- reporting/
|  |  |- report_generator.cpp
|  |- main.cpp
|- tests/
|  |- unit/
|  |  |- brake_monitor_test.cpp
|  |  |- wheel_speed_monitor_test.cpp
|  |  |- validation_test.cpp
|  |  |- fault_recovery_test.cpp
|- CMakeLists.txt
```

## 14. Test Strategy

The unit test suite should cover:

- Normal conditions
- Brake-pressure boundaries
- Wheel-speed mismatch boundaries
- Braking spread boundaries
- Missing and invalid input
- Multiple simultaneous faults
- Fault recovery scenarios

Suggested GoogleTest coverage:

- Brake pressure zero while braking.
- Brake pressure valid at 100 bar.
- Brake pressure invalid above 100 bar.
- Wheel difference exactly 10 km/h.
- Wheel difference greater than 10 km/h.
- Braking spread exactly 15 km/h.
- Braking spread greater than 15 km/h.
- Missing speed or wheel value.
- Invalid brake pedal status.
- Combined faults in one sample.

## 15. Traceability to Requirements

- BRK-01 to BRK-05 map to BrakeMonitor and Validation.
- WHL-01 to WHL-04 map to WheelSpeedMonitor.
- WHL-05 to WHL-08 map to WheelSpeedMonitor.
- FLT-01 to FLT-04 map to FaultManager and ReportGenerator.
- DAT-01 to DAT-04 map to Validation.
- Output requirements map to ReportGenerator.
- Test requirements map to the GoogleTest suite.

## 16. Implementation Notes

- Use `std::vector` or `std::set` for active fault collection depending on whether fault ordering or uniqueness is preferred.
- Prefer strong types and enums for fault names and overall status.
- Keep CSV parsing isolated so the rule logic can be tested with in-memory samples.
- Keep the report format stable so automated tests can compare outputs.
