# Brake & Wheel-Speed Monitoring System Instructions

## 1. Project Name

Brake & Wheel-Speed Monitoring System

## 2. Description

This project is a training MVP for automotive software development and testing. It reads simulated brake and wheel-speed monitoring data from a CSV file, validates the input, evaluates braking and wheel-speed conditions, detects faults, and generates a console report.

The system does not interface with a real vehicle, ECU, CAN network, or any production automotive hardware.

## 3. Tech Stack Used

- Language: C++17
- Application Type: Command-line application
- Input Format: CSV
- Build System: CMake
- Unit Testing: GoogleTest
- Source Control: Git and GitHub
- CI: GitHub Actions
- Deployment Target: AWS EC2

## 4. Design Pattern

- Current design pattern: Placeholder, to be finalized
- Recommended architectural flow: Input -> Brake & Wheel-Speed Monitoring Model -> Validation -> Brake Monitor -> Wheel-Speed Monitor -> Fault Manager -> Report Generator
- Suggested approach: Prefer a modular pipeline with clear separation of responsibilities between parsing, validation, rule evaluation, fault handling, and reporting

## 5. Coding Standards

- Use C++17 only.
- Keep header files in `include/` and implementation files in `src/`.
- Use `.hpp` for headers and `.cpp` for source files.
- Prefer small, single-purpose classes and functions.
- Keep business rules separated from file I/O and console formatting.
- Avoid hardcoded values when they represent requirement thresholds; define named constants instead.
- Use descriptive names such as `brakePressure`, `vehicleSpeed`, and `activeFaults`.
- Do not use one-letter variable names except for narrow loop counters.
- Validate all input data before running brake or wheel-speed fault checks.
- Ensure fault recovery is supported by recomputing active faults from each input sample.
- Keep functions deterministic and easy to unit test.
- Add tests for every rule boundary and every invalid-data case.
- Preserve consistency with the requirement names when naming faults and test cases.

## 6. Sample Test Code

```cpp
#include <gtest/gtest.h>

#include "brake/brake_monitor.hpp"
#include "faults/fault_types.hpp"
#include "monitoring_system/brake_wheel_speed_sample.hpp"

TEST(BrakeMonitorTest, ReportsBrakePressureFaultWhileBrakingAtZeroPressure)
{
	BrakeWheelSpeedSample sample{};
	sample.timestampSeconds = 10.5;
	sample.vehicleSpeedKph = 50.0;
	sample.brakePedalPressed = true;
	sample.brakePressureBar = 0.0;
	sample.frontLeftWheelSpeedKph = 50.0;
	sample.frontRightWheelSpeedKph = 49.0;
	sample.rearLeftWheelSpeedKph = 50.0;
	sample.rearRightWheelSpeedKph = 48.0;

	BrakeMonitor monitor;
	const auto faults = monitor.evaluate(sample);

	EXPECT_TRUE(faults.contains(FaultType::BrakePressureFault));
}
```

## 7. Folder Structure

```text
Brake_and_Wheel_Speed_Monitoring_System/
|- .github/
|  |- Instructions/
|  |  |- copilot-instructions.md
|  |- workflows/
|  |  |- ci.yml
|- Docs/
|  |- requirements.md
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

## 8. Sensitive Data Handling

- Do not store secrets, passwords, API keys, tokens, or private certificates in the repository.
- Use GitHub Actions secrets or environment variables for CI or deployment credentials.
- Do not commit AWS credentials, access keys, or `.env` files containing sensitive values.
- Use simulated brake and wheel-speed monitoring data only.
- Do not introduce real vehicle identifiers, VINs, customer data, or proprietary production data.
- Sanitize logs and reports so they contain only test or sample monitoring data.
- If deployment scripts are added later, keep configuration values external to source code.

## 9. Additional Guidance

- Keep the implementation aligned with `Docs/requirements.md`.
- Support multiple simultaneous faults.
- Ensure invalid data is reported separately from valid fault conditions.
- Treat exact threshold boundaries as defined in the requirements.
- Maintain clear separation between validation, monitoring logic, fault aggregation, and report generation.