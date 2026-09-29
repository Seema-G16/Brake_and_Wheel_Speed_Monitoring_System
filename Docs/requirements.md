# Brake & Wheel-Speed Monitoring System

## 1. Purpose

The Brake & Wheel-Speed Monitoring System shall monitor basic vehicle braking and wheel-speed signals and identify abnormal conditions.

The system is intended as a training MVP for automotive software development and testing. The implementation shall use simulated brake and wheel-speed monitoring data and shall not interface with a real vehicle or ECU.

---

## 2. Input Signals

The system shall receive the following vehicle signals:

| Signal             | Unit    | Description                      |
| ------------------ | ------- | -------------------------------- |
| Vehicle Speed      | km/h    | Current vehicle speed            |
| Brake Pedal Status | boolean | TRUE when brake pedal is pressed |
| Brake Pressure     | bar     | Measured brake pressure          |
| FL Wheel Speed     | km/h    | Front-left wheel speed           |
| FR Wheel Speed     | km/h    | Front-right wheel speed          |
| RL Wheel Speed     | km/h    | Rear-left wheel speed            |
| RR Wheel Speed     | km/h    | Rear-right wheel speed           |

Each brake and wheel-speed monitoring sample shall contain all seven signals and a timestamp.

---

## 3. Brake Monitoring Requirements

### BRK-01 — Brake System Active

The system shall consider the vehicle to be in a braking condition when:

- Brake Pedal Status = TRUE, and
- Vehicle Speed > 0 km/h.

### BRK-02 — Brake Pressure Monitoring

When the vehicle is in a braking condition, brake pressure shall be greater than 0 bar.

### BRK-03 — Brake Pressure Fault

If the vehicle is in a braking condition and brake pressure is equal to 0 bar, the system shall report:

`BRAKE_PRESSURE_FAULT`

### BRK-04 — Stationary Vehicle

When Vehicle Speed = 0 km/h, the system shall not report `BRAKE_PRESSURE_FAULT` solely because brake pressure is 0 bar.

### BRK-05 — Brake Pressure Range

The valid brake-pressure range shall be:

`0 bar <= Brake Pressure <= 100 bar`

If the input is outside this range, the system shall report:

`INVALID_BRAKE_PRESSURE`

---

## 4. Wheel-Speed Monitoring Requirements

### WHL-01 — Wheel-Speed Comparison

The system shall compare each wheel speed against the vehicle speed.

For each wheel:

`Difference = |Wheel Speed - Vehicle Speed|`

### WHL-02 — Wheel-Speed Mismatch

If the absolute difference between a wheel speed and vehicle speed is greater than 10 km/h, the system shall report a wheel-speed mismatch for that wheel.

Example:

```text
Vehicle speed = 80 km/h
FL = 82 km/h     → Normal
FR = 79 km/h     → Normal
RL = 68 km/h     → Fault
RR = 81 km/h     → Normal
```

The system shall report:

`RL_WHEEL_SPEED_MISMATCH`

### WHL-03 — Multiple Wheel Mismatch

If more than one wheel exceeds the 10 km/h threshold, the system shall report all affected wheels.

### WHL-04 — Boundary Condition

A difference of exactly 10 km/h shall not be considered a wheel-speed mismatch.

A difference greater than 10 km/h shall be considered a mismatch.

---

## 5. Braking Wheel-Speed Monitoring

The system shall perform an additional wheel-speed consistency check while braking.

### WHL-05 — Braking Condition

The braking wheel-speed check shall be active only when:

- Brake Pedal Status = TRUE, and
- Vehicle Speed > 0 km/h.

### WHL-06 — Wheel-Speed Spread

During braking, the system shall determine:

`Wheel Speed Spread = Maximum Wheel Speed - Minimum Wheel Speed`

### WHL-07 — Braking Wheel-Speed Fault

If the wheel-speed spread during braking is greater than 15 km/h, the system shall report:

`BRAKING_WHEEL_SPEED_MISMATCH`

Example:

```text
Vehicle Speed = 100 km/h
Brake Pedal   = TRUE

FL = 98 km/h
FR = 97 km/h
RL = 95 km/h
RR = 75 km/h

Maximum = 98
Minimum = 75

Spread = 23 km/h

Result:
BRAKING_WHEEL_SPEED_MISMATCH
```

### WHL-08 — Boundary Condition

A wheel-speed spread of exactly 15 km/h shall not be considered a fault.

A spread greater than 15 km/h shall be considered a fault.

---

## 6. Fault Management

### FLT-01 — Fault Status

The system shall maintain the following fault statuses:

```text
NO_FAULT
BRAKE_PRESSURE_FAULT
INVALID_BRAKE_PRESSURE
FL_WHEEL_SPEED_MISMATCH
FR_WHEEL_SPEED_MISMATCH
RL_WHEEL_SPEED_MISMATCH
RR_WHEEL_SPEED_MISMATCH
BRAKING_WHEEL_SPEED_MISMATCH
INVALID_MONITORING_DATA
```

### FLT-02 — Multiple Faults

The system shall be capable of reporting multiple faults simultaneously.

Example:

```text
BRAKE_PRESSURE_FAULT
RL_WHEEL_SPEED_MISMATCH
RR_WHEEL_SPEED_MISMATCH
```

### FLT-03 — Fault Recovery

A fault shall no longer be reported when the corresponding fault condition is no longer present.

Example:

```text
Sample 1:
RL wheel mismatch → FAULT

Sample 2:
RL wheel returns to normal → NO RL mismatch
```

### FLT-04 — Overall Status

The system shall provide an overall status:

```text
NORMAL
FAULT
INVALID_DATA
```

The overall status shall be:

- `NORMAL` when no fault is active.
- `FAULT` when one or more valid fault conditions are active.
- `INVALID_DATA` when required input data is invalid.

---

## 7. Input Validation

### DAT-01 — Invalid Vehicle Speed

Vehicle speed shall not be negative.

Negative vehicle speed shall result in:

`INVALID_MONITORING_DATA`

### DAT-02 — Invalid Wheel Speed

Wheel speeds shall not be negative.

A negative wheel speed shall result in:

`INVALID_MONITORING_DATA`

### DAT-03 — Missing Data

If any required brake or wheel-speed monitoring value is missing, the system shall report:

`INVALID_MONITORING_DATA`

### DAT-04 — Invalid Brake Pedal Status

Brake pedal status shall contain only:

```text
TRUE
FALSE
```

Any other representation shall be treated as invalid input.

---

## 8. Output Requirements

For every brake and wheel-speed monitoring sample, the system shall provide:

```text
Timestamp
Vehicle Speed
Brake Pedal Status
Brake Pressure
FL Wheel Speed
FR Wheel Speed
RL Wheel Speed
RR Wheel Speed

Brake Status
Wheel-Speed Status
Active Faults
Overall Status
```

Example:

```text
========================================
       BRAKE MONITOR TEST RESULT
========================================

Timestamp        : 10.500 s
Vehicle Speed    : 100 km/h
Brake Pedal      : PRESSED
Brake Pressure   : 42 bar

Wheel Speeds:
  FL             : 98 km/h
  FR             : 97 km/h
  RL             : 96 km/h
  RR             : 74 km/h

Brake Status     : NORMAL
Wheel Status     : FAULT

Active Faults:
  - RR_WHEEL_SPEED_MISMATCH
  - BRAKING_WHEEL_SPEED_MISMATCH

Overall Status   : FAULT
========================================
```

---

## 9. Test Requirements

The implementation shall be verified using automated tests.

At minimum, the test suite shall cover:

### Normal Conditions

- Vehicle stationary.
- Vehicle moving without braking.
- Vehicle braking with normal brake pressure.
- All wheel speeds within expected difference.

### Brake Tests

- Brake pedal released.
- Brake pedal pressed at 0 km/h.
- Brake pedal pressed above 0 km/h with normal pressure.
- Brake pedal pressed with 0 bar pressure.
- Brake pressure exactly 0 bar.
- Brake pressure exactly 100 bar.
- Brake pressure greater than 100 bar.
- Negative brake pressure.

### Wheel-Speed Tests

- All wheel speeds equal to vehicle speed.
- Wheel difference exactly 10 km/h.
- Wheel difference greater than 10 km/h.
- One wheel mismatch.
- Two wheel mismatches.
- All wheels mismatching.

### Braking Wheel-Speed Tests

- Wheel-speed spread exactly 15 km/h.
- Wheel-speed spread greater than 15 km/h.
- Normal wheel-speed spread during braking.
- Multiple faults during braking.

### Data Validation Tests

- Missing vehicle speed.
- Missing wheel speed.
- Negative vehicle speed.
- Negative wheel speed.
- Invalid brake pedal value.
- Multiple invalid inputs.

### Fault Recovery Tests

- Wheel-speed fault appears and then disappears.
- Brake-pressure fault appears and then disappears.
- Multiple faults appear and individual faults recover.

---

## 10. Technical Constraints

- Language: C++17
- Application type: Command-line application
- Input: CSV brake and wheel-speed monitoring data file
- Output: Console report and test result summary
- Unit testing: GoogleTest
- Build system: CMake
- Source control: Git/GitHub
- CI: GitHub Actions
- Deployment: AWS EC2
- No real vehicle data
- No real CAN communication
- No GUI required for the MVP

The architecture shall separate:

```text
Input
  ↓
Brake & Wheel-Speed Monitoring Model
  ↓
Validation
  ↓
Brake Monitor
  ↓
Wheel-Speed Monitor
  ↓
Fault Manager
  ↓
Report Generator
```

---

## 11. Acceptance Criteria

The MVP shall be considered complete when:

1. The application successfully reads valid brake and wheel-speed monitoring data.
2. All defined requirements are automatically evaluated.
3. Individual wheel faults are correctly identified.
4. Brake-pressure faults are correctly identified.
5. Multiple simultaneous faults are supported.
6. Invalid input is detected.
7. Fault recovery is demonstrated.
8. Automated unit tests are available for the defined requirements.
9. All tests pass in the local build.
10. GitHub Actions successfully builds and tests the application.
11. The application can be executed on the target AWS environment.
12. A test result/report can be generated from the sample brake and wheel-speed monitoring data.
