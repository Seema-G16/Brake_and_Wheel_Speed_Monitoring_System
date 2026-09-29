#include <gtest/gtest.h>
#include "brake/brake_monitor.hpp"
#include "monitoring_system/brake_wheel_speed_sample.hpp"
#include "faults/fault_types.hpp"

using namespace bwsms;

/**
 * Brake Monitor Tests (BRK requirements)
 * Tests brake pressure monitoring according to BRK-01 through BRK-05
 */

class BrakeMonitorTest : public ::testing::Test {
protected:
    BrakeMonitor monitor_;

    BrakeWheelSpeedSample create_valid_sample() {
        BrakeWheelSpeedSample sample;
        sample.timestampSeconds = 1.0;
        sample.vehicleSpeedKph = 50.0;
        sample.brakePedalPressed = false;
        sample.brakePressureBar = 50.0;
        sample.frontLeftWheelSpeedKph = 50.0;
        sample.frontRightWheelSpeedKph = 50.0;
        sample.rearLeftWheelSpeedKph = 50.0;
        sample.rearRightWheelSpeedKph = 50.0;
        return sample;
    }
};

// TC-BRK-001: Braking with normal pressure (BRK-01, BRK-02)
TEST_F(BrakeMonitorTest, TC_BRK_001_BrakingWithNormalPressure) {
    auto sample = create_valid_sample();
    sample.vehicleSpeedKph = 50.0;
    sample.brakePedalPressed = true;
    sample.brakePressureBar = 40.0;
    
    EXPECT_TRUE(monitor_.is_braking(sample));
    auto faults = monitor_.evaluate(sample);
    EXPECT_TRUE(faults.empty() || faults.count(FaultType::NO_FAULT));
}

// TC-BRK-002: Brake pedal released (BRK-01)
TEST_F(BrakeMonitorTest, TC_BRK_002_BrakePedalReleased) {
    auto sample = create_valid_sample();
    sample.vehicleSpeedKph = 50.0;
    sample.brakePedalPressed = false;
    sample.brakePressureBar = 0.0;
    
    EXPECT_FALSE(monitor_.is_braking(sample));
    auto faults = monitor_.evaluate(sample);
    EXPECT_TRUE(faults.empty() || faults.count(FaultType::NO_FAULT));
}

// TC-BRK-003: Stationary at speed=0 with zero pressure (BRK-04)
TEST_F(BrakeMonitorTest, TC_BRK_003_StationaryWithZeroPressure) {
    auto sample = create_valid_sample();
    sample.vehicleSpeedKph = 0.0;
    sample.brakePedalPressed = true;
    sample.brakePressureBar = 0.0;
    sample.frontLeftWheelSpeedKph = 0.0;
    sample.frontRightWheelSpeedKph = 0.0;
    sample.rearLeftWheelSpeedKph = 0.0;
    sample.rearRightWheelSpeedKph = 0.0;
    
    // Not braking at speed 0
    EXPECT_FALSE(monitor_.is_braking(sample));
    // No fault because speed = 0 (BRK-04)
    auto faults = monitor_.evaluate(sample);
    EXPECT_TRUE(faults.empty() || faults.count(FaultType::NO_FAULT));
}

// TC-BRK-004: Fault just above zero speed (BRK-01, BRK-03, BRK-04)
TEST_F(BrakeMonitorTest, TC_BRK_004_FaultJustAboveZeroSpeed) {
    auto sample = create_valid_sample();
    sample.vehicleSpeedKph = 0.01;
    sample.brakePedalPressed = true;
    sample.brakePressureBar = 0.0;
    
    // Now braking (speed > 0 and pedal pressed)
    EXPECT_TRUE(monitor_.is_braking(sample));
    // Should have BRAKE_PRESSURE_FAULT because pressure = 0 while braking
    auto faults = monitor_.evaluate(sample);
    EXPECT_TRUE(faults.count(FaultType::BRAKE_PRESSURE_FAULT));
}

// TC-BRK-005: Brake pressure fault while braking (BRK-03)
TEST_F(BrakeMonitorTest, TC_BRK_005_BrakePressureFaultWhileBraking) {
    auto sample = create_valid_sample();
    sample.vehicleSpeedKph = 60.0;
    sample.brakePedalPressed = true;
    sample.brakePressureBar = 0.0;
    
    EXPECT_TRUE(monitor_.is_braking(sample));
    auto faults = monitor_.evaluate(sample);
    EXPECT_TRUE(faults.count(FaultType::BRAKE_PRESSURE_FAULT));
}

// TC-BRK-006: Pressure lower boundary valid - not braking (BRK-05)
TEST_F(BrakeMonitorTest, TC_BRK_006_PressureLowerBoundaryNotBraking) {
    auto sample = create_valid_sample();
    sample.vehicleSpeedKph = 30.0;
    sample.brakePedalPressed = false;
    sample.brakePressureBar = 0.0;
    
    EXPECT_FALSE(monitor_.is_braking(sample));
    auto faults = monitor_.evaluate(sample);
    EXPECT_TRUE(faults.empty() || faults.count(FaultType::NO_FAULT));
}

// TC-BRK-007: Pressure upper boundary valid (BRK-05)
TEST_F(BrakeMonitorTest, TC_BRK_007_Pressure100BarWhileBraking) {
    auto sample = create_valid_sample();
    sample.vehicleSpeedKph = 70.0;
    sample.brakePedalPressed = true;
    sample.brakePressureBar = 100.0;
    
    EXPECT_TRUE(monitor_.is_braking(sample));
    auto faults = monitor_.evaluate(sample);
    EXPECT_TRUE(faults.empty() || faults.count(FaultType::NO_FAULT));
}

// TC-BRK-008: Very low pressure but not zero while braking
TEST_F(BrakeMonitorTest, TC_BRK_008_VeryLowPressureNotZero) {
    auto sample = create_valid_sample();
    sample.vehicleSpeedKph = 60.0;
    sample.brakePedalPressed = true;
    sample.brakePressureBar = 0.001;  // Very small, not exactly zero
    
    EXPECT_TRUE(monitor_.is_braking(sample));
    auto faults = monitor_.evaluate(sample);
    // Should NOT have brake pressure fault (pressure is not exactly 0)
    EXPECT_FALSE(faults.count(FaultType::BRAKE_PRESSURE_FAULT));
}

// TC-BRK-009: Exactly zero pressure is fault while braking
TEST_F(BrakeMonitorTest, TC_BRK_009_ExactlyZeroPressureWhileBraking) {
    auto sample = create_valid_sample();
    sample.vehicleSpeedKph = 50.0;
    sample.brakePedalPressed = true;
    sample.brakePressureBar = 0.0;
    
    auto faults = monitor_.evaluate(sample);
    EXPECT_TRUE(faults.count(FaultType::BRAKE_PRESSURE_FAULT));
}

// TC-BRK-010: Multiple braking samples - persistence
TEST_F(BrakeMonitorTest, TC_BRK_010_BrakeFaultPersists) {
    auto sample = create_valid_sample();
    sample.vehicleSpeedKph = 60.0;
    sample.brakePedalPressed = true;
    sample.brakePressureBar = 0.0;
    
    // First evaluation
    auto faults1 = monitor_.evaluate(sample);
    EXPECT_TRUE(faults1.count(FaultType::BRAKE_PRESSURE_FAULT));
    
    // Second evaluation with same conditions
    auto faults2 = monitor_.evaluate(sample);
    EXPECT_TRUE(faults2.count(FaultType::BRAKE_PRESSURE_FAULT));
}

// TC-BRK-011: Brake fault recovery
TEST_F(BrakeMonitorTest, TC_BRK_011_BrakeFaultRecovery) {
    auto sample = create_valid_sample();
    
    // Sample 1: Fault condition
    sample.vehicleSpeedKph = 50.0;
    sample.brakePedalPressed = true;
    sample.brakePressureBar = 0.0;
    auto faults1 = monitor_.evaluate(sample);
    EXPECT_TRUE(faults1.count(FaultType::BRAKE_PRESSURE_FAULT));
    
    // Sample 2: Condition resolved
    sample.brakePressureBar = 35.0;  // Pressure restored
    auto faults2 = monitor_.evaluate(sample);
    EXPECT_FALSE(faults2.count(FaultType::BRAKE_PRESSURE_FAULT));
}

// TC-BRK-012: Braking above 0 km/h with normal pressure
TEST_F(BrakeMonitorTest, TC_BRK_012_BrakingAboveZeroWithNormalPressure) {
    auto sample = create_valid_sample();
    sample.vehicleSpeedKph = 100.0;
    sample.brakePedalPressed = true;
    sample.brakePressureBar = 42.0;
    
    EXPECT_TRUE(monitor_.is_braking(sample));
    auto faults = monitor_.evaluate(sample);
    EXPECT_TRUE(faults.empty() || faults.count(FaultType::NO_FAULT));
}
