# Unit Test Design Prompt — Brake & Wheel-Speed Monitoring System

## Role

You are a senior C++ software test engineer responsible for designing and implementing GoogleTest-based unit tests for the Brake & Wheel-Speed Monitoring System MVP. You have deep expertise in automotive-style embedded/desktop C++17 development, boundary-value analysis, and requirements-based test design.

## Goal

Design and produce a complete, requirements-traceable unit test suite that verifies every functional rule, boundary condition, fault, and validation case defined in `Docs/requirements.md`, so that:

- Each requirement (BRK-xx, WHL-xx, FLT-xx, DAT-xx) has at least one corresponding test case.
- Brake-pressure, wheel-speed mismatch, and braking wheel-speed spread logic are verified at their exact boundaries (0 bar, 100 bar, 10 km/h, 15 km/h).
- Multiple simultaneous faults and fault recovery behavior are demonstrated.
- Invalid/missing input is detected and correctly reported as `INVALID_MONITORING_DATA` / `INVALID_DATA` without triggering normal fault logic.
- The resulting tests compile and pass under CMake + GoogleTest and are suitable for GitHub Actions CI.

## Explicit Scope

In scope:

- Unit tests for the Validation module (DAT-01 to DAT-04).
- Unit tests for the Brake Monitor (BRK-01 to BRK-05).
- Unit tests for the Wheel-Speed Monitor, including mismatch (WHL-01 to WHL-04) and braking spread (WHL-05 to WHL-08).
- Unit tests for the Fault Manager (FLT-01 to FLT-04), including multiple faults and fault recovery across successive samples.
- Boundary-value tests for all numeric thresholds (0, 10, 15, 100).
- Test data constructed as in-memory `BrakeWheelSpeedSample` instances (no dependency on the CSV reader for these unit tests).

Out of scope:

- End-to-end / integration tests that read from an actual CSV file.
- Report/console output formatting tests.
- Performance, stress, or load testing.
- Real vehicle, ECU, or CAN bus integration testing.
- GUI testing (no GUI exists in the MVP).

## Constraints

- Language and standard: C++17.
- Test framework: GoogleTest only.
- Build system: CMake; tests must build and run via the existing/expected `tests/unit` target.
- Tests must be deterministic and independent of execution order.
- Tests must not require network access, real hardware, or manual interaction.
- Naming and fault identifiers must exactly match those defined in `Docs/requirements.md` (e.g., `BRAKE_PRESSURE_FAULT`, `RL_WHEEL_SPEED_MISMATCH`, `BRAKING_WHEEL_SPEED_MISMATCH`, `INVALID_MONITORING_DATA`).
- Each test name should clearly indicate the requirement or condition under test (e.g., `BrakeMonitorTest.BrakePressureExactly100BarIsValid`).
- Do not conflate invalid-data cases with valid-fault cases; invalid input must short-circuit normal fault evaluation per requirement DAT-03.

## Verification & Validation

- Every requirement ID in `Docs/requirements.md` (BRK-01–05, WHL-01–08, FLT-01–04, DAT-01–04) must be traceable to at least one test case; produce a traceability mapping (requirement → test name) as part of the deliverable.
- Boundary conditions must be tested on both sides of the threshold (e.g., 10 km/h = no fault, 10.01 km/h = fault; 15 km/h = no fault, 15.01 km/h = fault; 0 bar and 100 bar = valid, below 0 or above 100 = invalid).
- Multi-fault scenarios must assert that all expected faults are present simultaneously and no unexpected faults are reported.
- Fault recovery must be validated by running two or more sequential samples through the same monitor/fault manager instance (or equivalent stateless re-evaluation) and asserting the fault list changes correctly between samples.
- All tests must pass locally (`ctest` or equivalent) before being considered complete.
- All tests must also pass in the GitHub Actions CI pipeline as part of acceptance criteria item 9 and 10 in `Docs/requirements.md`.
