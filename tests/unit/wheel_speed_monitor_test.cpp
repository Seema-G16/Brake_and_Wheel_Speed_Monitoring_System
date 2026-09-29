#include <gtest/gtest.h>
#include "wheel_speed/wheel_speed_monitor.hpp"
#include "monitoring_system/brake_wheel_speed_sample.hpp"
#include "faults/fault_types.hpp"
#include <cmath>

using namespace bwsms;

/**
 * Wheel Speed Monitor Tests (WHL requirements)
 * Tests wheel-speed monitoring according to WHL-01 through WHL-08
 */

class WheelSpeedMonitorTest : public ::testing::Test {
protected:
    WheelSpeedMonitor monitor_;

    BrakeWheelSpeedSample create_valid_sample() {
        BrakeWheelSpeedSample sample;
        sample.timestampSeconds = 1.0;
        sample.vehicleSpeedKph = 80.0;
        sample.brakePedalPressed = false;
        sample.brakePressureBar = 50.0;
        sample.frontLeftWheelSpeedKph = 80.0;
        sample.frontRightWheelSpeedKph = 80.0;
        sample.rearLeftWheelSpeedKph = 80.0;
        sample.rearRightWheelSpeedKph = 80.0;
        return sample;
    }
};

// TC-WHL-001: Normal wheel speeds - no mismatch (WHL-01, WHL-02)
TEST_F(WheelSpeedMonitorTest, TC_WHL_001_NormalWheelSpeeds) {
    auto sample = create_valid_sample();
    sample.vehicleSpeedKph = 80.0;
    sample.frontLeftWheelSpeedKph = 80.0;
    sample.frontRightWheelSpeedKph = 80.0;
    sample.rearLeftWheelSpeedKph = 79.0;
    sample.rearRightWheelSpeedKph = 81.0;
    
    auto faults = monitor_.evaluate(sample);
    EXPECT_FALSE(faults.count(FaultType::FL_WHEEL_SPEED_MISMATCH));
    EXPECT_FALSE(faults.count(FaultType::FR_WHEEL_SPEED_MISMATCH));
    EXPECT_FALSE(faults.count(FaultType::RL_WHEEL_SPEED_MISMATCH));
    EXPECT_FALSE(faults.count(FaultType::RR_WHEEL_SPEED_MISMATCH));
}

// TC-WHL-002: Wheel diff exactly 10 km/h boundary (WHL-04)
TEST_F(WheelSpeedMonitorTest, TC_WHL_002_ExactlyTenKmhDifference) {
    auto sample = create_valid_sample();
    sample.vehicleSpeedKph = 80.0;
    sample.frontLeftWheelSpeedKph = 90.0;  // Exactly 10 km/h difference
    sample.frontRightWheelSpeedKph = 80.0;
    sample.rearLeftWheelSpeedKph = 80.0;
    sample.rearRightWheelSpeedKph = 70.0;  // Exactly 10 km/h difference (other direction)
    
    // Exactly 10 km/h is NOT a mismatch (WHL-04)
    auto faults = monitor_.evaluate(sample);
    EXPECT_FALSE(faults.count(FaultType::FL_WHEEL_SPEED_MISMATCH));
    EXPECT_FALSE(faults.count(FaultType::RR_WHEEL_SPEED_MISMATCH));
}

// TC-WHL-003: Wheel diff just above 10 km/h boundary (WHL-02, WHL-04)
TEST_F(WheelSpeedMonitorTest, TC_WHL_003_JustAboveTenKmhDifference) {
    auto sample = create_valid_sample();
    sample.vehicleSpeedKph = 80.0;
    sample.frontLeftWheelSpeedKph = 90.01;  // Just above 10 km/h
    sample.frontRightWheelSpeedKph = 80.0;
    sample.rearLeftWheelSpeedKph = 80.0;
    sample.rearRightWheelSpeedKph = 80.0;
    
    // Just above 10 km/h IS a mismatch
    auto faults = monitor_.evaluate(sample);
    EXPECT_TRUE(faults.count(FaultType::FL_WHEEL_SPEED_MISMATCH));
}

// TC-WHL-004: Single wheel mismatch - RL (WHL-02)
TEST_F(WheelSpeedMonitorTest, TC_WHL_004_SingleWheelMismatchRL) {
    auto sample = create_valid_sample();
    sample.vehicleSpeedKph = 80.0;
    sample.frontLeftWheelSpeedKph = 82.0;   // ±2, OK
    sample.frontRightWheelSpeedKph = 79.0;  // ±1, OK
    sample.rearLeftWheelSpeedKph = 68.0;    // -12, mismatch
    sample.rearRightWheelSpeedKph = 81.0;   // +1, OK
    
    auto faults = monitor_.evaluate(sample);
    EXPECT_TRUE(faults.count(FaultType::RL_WHEEL_SPEED_MISMATCH));
    EXPECT_FALSE(faults.count(FaultType::FL_WHEEL_SPEED_MISMATCH));
    EXPECT_FALSE(faults.count(FaultType::FR_WHEEL_SPEED_MISMATCH));
    EXPECT_FALSE(faults.count(FaultType::RR_WHEEL_SPEED_MISMATCH));
}

// TC-WHL-005: Multiple wheel mismatches (WHL-03)
TEST_F(WheelSpeedMonitorTest, TC_WHL_005_MultipleWheelMismatches) {
    auto sample = create_valid_sample();
    sample.vehicleSpeedKph = 80.0;
    sample.frontLeftWheelSpeedKph = 68.0;   // -12, mismatch
    sample.frontRightWheelSpeedKph = 80.0;  // OK
    sample.rearLeftWheelSpeedKph = 80.0;    // OK
    sample.rearRightWheelSpeedKph = 92.0;   // +12, mismatch
    
    auto faults = monitor_.evaluate(sample);
    EXPECT_TRUE(faults.count(FaultType::FL_WHEEL_SPEED_MISMATCH));
    EXPECT_TRUE(faults.count(FaultType::RR_WHEEL_SPEED_MISMATCH));
    EXPECT_FALSE(faults.count(FaultType::FR_WHEEL_SPEED_MISMATCH));
    EXPECT_FALSE(faults.count(FaultType::RL_WHEEL_SPEED_MISMATCH));
}

// TC-WHL-006: All four wheels mismatching (WHL-03)
TEST_F(WheelSpeedMonitorTest, TC_WHL_006_AllFourWheelsMismatching) {
    auto sample = create_valid_sample();
    sample.vehicleSpeedKph = 80.0;
    sample.frontLeftWheelSpeedKph = 68.0;   // -12, mismatch
    sample.frontRightWheelSpeedKph = 92.0;  // +12, mismatch
    sample.rearLeftWheelSpeedKph = 69.0;    // -11, mismatch
    sample.rearRightWheelSpeedKph = 91.0;   // +11, mismatch
    
    auto faults = monitor_.evaluate(sample);
    EXPECT_TRUE(faults.count(FaultType::FL_WHEEL_SPEED_MISMATCH));
    EXPECT_TRUE(faults.count(FaultType::FR_WHEEL_SPEED_MISMATCH));
    EXPECT_TRUE(faults.count(FaultType::RL_WHEEL_SPEED_MISMATCH));
    EXPECT_TRUE(faults.count(FaultType::RR_WHEEL_SPEED_MISMATCH));
}

// TC-WHL-007: Braking with normal spread (WHL-05, WHL-06)
TEST_F(WheelSpeedMonitorTest, TC_WHL_007_BrakingNormalSpread) {
    auto sample = create_valid_sample();
    sample.vehicleSpeedKph = 100.0;
    sample.brakePedalPressed = true;
    sample.brakePressureBar = 80.0;
    sample.frontLeftWheelSpeedKph = 98.0;
    sample.frontRightWheelSpeedKph = 97.0;
    sample.rearLeftWheelSpeedKph = 95.0;
    sample.rearRightWheelSpeedKph = 90.0;  // Spread = 98 - 90 = 8 km/h
    
    auto faults = monitor_.evaluate(sample);
    EXPECT_FALSE(faults.count(FaultType::BRAKING_WHEEL_SPEED_MISMATCH));
}

// TC-WHL-008: Braking spread exactly 15 km/h boundary (WHL-08)
TEST_F(WheelSpeedMonitorTest, TC_WHL_008_BrakingSpreadExactly15Kmh) {
    auto sample = create_valid_sample();
    sample.vehicleSpeedKph = 100.0;
    sample.brakePedalPressed = true;
    sample.brakePressureBar = 80.0;
    sample.frontLeftWheelSpeedKph = 100.0;
    sample.frontRightWheelSpeedKph = 100.0;
    sample.rearLeftWheelSpeedKph = 100.0;
    sample.rearRightWheelSpeedKph = 85.0;   // Spread = 100 - 85 = 15 km/h exactly
    
    // Exactly 15 km/h is NOT a fault (WHL-08)
    auto faults = monitor_.evaluate(sample);
    EXPECT_FALSE(faults.count(FaultType::BRAKING_WHEEL_SPEED_MISMATCH));
}

// TC-WHL-009: Braking spread just above 15 km/h (WHL-07, WHL-08)
TEST_F(WheelSpeedMonitorTest, TC_WHL_009_BrakingSpreadJustAbove15Kmh) {
    auto sample = create_valid_sample();
    sample.vehicleSpeedKph = 100.0;
    sample.brakePedalPressed = true;
    sample.brakePressureBar = 80.0;
    sample.frontLeftWheelSpeedKph = 100.0;
    sample.frontRightWheelSpeedKph = 100.0;
    sample.rearLeftWheelSpeedKph = 100.0;
    sample.rearRightWheelSpeedKph = 84.99;  // Spread = 100 - 84.99 = 15.01 km/h
    
    // Just above 15 km/h IS a fault
    auto faults = monitor_.evaluate(sample);
    EXPECT_TRUE(faults.count(FaultType::BRAKING_WHEEL_SPEED_MISMATCH));
}

// TC-WHL-010: Large wheel spread during braking (WHL-07)
TEST_F(WheelSpeedMonitorTest, TC_WHL_010_LargeWheelSpreadDuringBraking) {
    auto sample = create_valid_sample();
    sample.vehicleSpeedKph = 100.0;
    sample.brakePedalPressed = true;
    sample.brakePressureBar = 60.0;
    sample.frontLeftWheelSpeedKph = 98.0;
    sample.frontRightWheelSpeedKph = 97.0;
    sample.rearLeftWheelSpeedKph = 95.0;
    sample.rearRightWheelSpeedKph = 75.0;   // Spread = 98 - 75 = 23 km/h (from requirements.md)
    
    auto faults = monitor_.evaluate(sample);
    EXPECT_TRUE(faults.count(FaultType::BRAKING_WHEEL_SPEED_MISMATCH));
}

// TC-WHL-011: Wheel mismatch with braking spread (WHL-02, WHL-07)
TEST_F(WheelSpeedMonitorTest, TC_WHL_011_MismatchAndBrakingSpreadFault) {
    auto sample = create_valid_sample();
    sample.vehicleSpeedKph = 100.0;
    sample.brakePedalPressed = true;
    sample.brakePressureBar = 80.0;
    sample.frontLeftWheelSpeedKph = 88.0;   // -12, mismatch
    sample.frontRightWheelSpeedKph = 112.0; // +12, mismatch
    sample.rearLeftWheelSpeedKph = 95.0;
    sample.rearRightWheelSpeedKph = 75.0;   // Spread = 112 - 75 = 37 km/h (also braking fault)
    
    auto faults = monitor_.evaluate(sample);
    EXPECT_TRUE(faults.count(FaultType::FL_WHEEL_SPEED_MISMATCH));
    EXPECT_TRUE(faults.count(FaultType::FR_WHEEL_SPEED_MISMATCH));
    EXPECT_TRUE(faults.count(FaultType::BRAKING_WHEEL_SPEED_MISMATCH));
}

// TC-WHL-012: No braking check when not braking (WHL-05)
TEST_F(WheelSpeedMonitorTest, TC_WHL_012_NoBrakingCheckWhenNotBraking) {
    auto sample = create_valid_sample();
    sample.vehicleSpeedKph = 100.0;
    sample.brakePedalPressed = false;  // NOT braking
    sample.brakePressureBar = 50.0;
    sample.frontLeftWheelSpeedKph = 98.0;
    sample.frontRightWheelSpeedKph = 97.0;
    sample.rearLeftWheelSpeedKph = 95.0;
    sample.rearRightWheelSpeedKph = 75.0;   // Large spread, but not braking
    
    // Braking spread check should not apply (not braking)
    auto faults = monitor_.evaluate(sample);
    EXPECT_FALSE(faults.count(FaultType::BRAKING_WHEEL_SPEED_MISMATCH));
    // But wheel mismatches may still apply
}

// TC-WHL-013: Braking spread gate at zero speed (WHL-05)
TEST_F(WheelSpeedMonitorTest, TC_WHL_013_BrakingSpreadCheckInactiveAtZeroSpeed) {
    auto sample = create_valid_sample();
    sample.vehicleSpeedKph = 0.0;
    sample.brakePedalPressed = true;  // Pedal pressed
    sample.brakePressureBar = 0.0;
    sample.frontLeftWheelSpeedKph = 0.0;
    sample.frontRightWheelSpeedKph = 0.0;
    sample.rearLeftWheelSpeedKph = 0.0;
    sample.rearRightWheelSpeedKph = 30.0;  // Huge spread if applied
    
    // Braking spread check should be inactive (speed = 0)
    auto faults = monitor_.evaluate(sample);
    EXPECT_FALSE(faults.count(FaultType::BRAKING_WHEEL_SPEED_MISMATCH));
}

// TC-WHL-014: Test FR wheel mismatch
TEST_F(WheelSpeedMonitorTest, TC_WHL_014_FrontRightWheelMismatch) {
    auto sample = create_valid_sample();
    sample.vehicleSpeedKph = 80.0;
    sample.frontLeftWheelSpeedKph = 80.0;
    sample.frontRightWheelSpeedKph = 95.0;  // +15, mismatch
    sample.rearLeftWheelSpeedKph = 80.0;
    sample.rearRightWheelSpeedKph = 80.0;
    
    auto faults = monitor_.evaluate(sample);
    EXPECT_TRUE(faults.count(FaultType::FR_WHEEL_SPEED_MISMATCH));
}

// TC-WHL-015: Test RR wheel mismatch
TEST_F(WheelSpeedMonitorTest, TC_WHL_015_RearRightWheelMismatch) {
    auto sample = create_valid_sample();
    sample.vehicleSpeedKph = 80.0;
    sample.frontLeftWheelSpeedKph = 80.0;
    sample.frontRightWheelSpeedKph = 80.0;
    sample.rearLeftWheelSpeedKph = 80.0;
    sample.rearRightWheelSpeedKph = 95.0;  // +15, mismatch
    
    auto faults = monitor_.evaluate(sample);
    EXPECT_TRUE(faults.count(FaultType::RR_WHEEL_SPEED_MISMATCH));
}
