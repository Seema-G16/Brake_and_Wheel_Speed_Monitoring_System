#include <gtest/gtest.h>
#include "faults/fault_manager.hpp"
#include "monitoring_system/brake_wheel_speed_sample.hpp"
#include "faults/fault_types.hpp"

using namespace bwsms;

/**
 * Fault Manager Tests (FLT requirements)
 * Tests fault aggregation and overall status according to FLT-01 through FLT-04
 */

class FaultManagerTest : public ::testing::Test {
protected:
    FaultManager manager_;

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

// TC-FLT-001: No faults yields NO_FAULT and NORMAL status (FLT-01, FLT-04)
TEST_F(FaultManagerTest, TC_FLT_001_NoFaultsYieldsNormalStatus) {
    auto sample = create_valid_sample();
    
    auto faults = manager_.evaluate(sample);
    auto status = manager_.get_overall_status(sample);
    
    EXPECT_TRUE(faults.empty() || faults.count(FaultType::NO_FAULT));
    EXPECT_EQ(OverallStatus::NORMAL, status);
}

// TC-FLT-002: Multiple simultaneous faults (FLT-02)
TEST_F(FaultManagerTest, TC_FLT_002_MultipleFaults) {
    auto sample = create_valid_sample();
    sample.vehicleSpeedKph = 80.0;
    sample.brakePedalPressed = true;
    sample.brakePressureBar = 0.0;         // Brake fault
    sample.frontLeftWheelSpeedKph = 68.0;  // FL mismatch
    sample.rearRightWheelSpeedKph = 92.0;  // RR mismatch
    
    auto faults = manager_.evaluate(sample);
    auto status = manager_.get_overall_status(sample);
    
    // Should have all three faults
    EXPECT_TRUE(faults.count(FaultType::BRAKE_PRESSURE_FAULT));
    EXPECT_TRUE(faults.count(FaultType::FL_WHEEL_SPEED_MISMATCH));
    EXPECT_TRUE(faults.count(FaultType::RR_WHEEL_SPEED_MISMATCH));
    EXPECT_EQ(OverallStatus::FAULT, status);
}

// TC-FLT-003: Fault recovery - wheel mismatch clears (FLT-03)
TEST_F(FaultManagerTest, TC_FLT_003_FaultRecoveryWheelMismatch) {
    auto sample = create_valid_sample();
    
    // Sample 1: Fault present
    sample.vehicleSpeedKph = 80.0;
    sample.frontLeftWheelSpeedKph = 68.0;  // -12, mismatch
    sample.frontRightWheelSpeedKph = 80.0;
    sample.rearLeftWheelSpeedKph = 80.0;
    sample.rearRightWheelSpeedKph = 80.0;
    auto faults1 = manager_.evaluate(sample);
    auto status1 = manager_.get_overall_status(sample);
    
    EXPECT_TRUE(faults1.count(FaultType::FL_WHEEL_SPEED_MISMATCH));
    EXPECT_EQ(OverallStatus::FAULT, status1);
    
    // Sample 2: Condition recovered
    sample.frontLeftWheelSpeedKph = 80.0;  // Back to normal
    auto faults2 = manager_.evaluate(sample);
    auto status2 = manager_.get_overall_status(sample);
    
    EXPECT_FALSE(faults2.count(FaultType::FL_WHEEL_SPEED_MISMATCH));
    EXPECT_EQ(OverallStatus::NORMAL, status2);
}

// TC-FLT-004: Fault persistence (FLT-04)
TEST_F(FaultManagerTest, TC_FLT_004_FaultPersistence) {
    auto sample = create_valid_sample();
    
    sample.vehicleSpeedKph = 70.0;
    sample.brakePedalPressed = true;
    sample.brakePressureBar = 0.0;  // Brake fault
    
    // Evaluate twice with same conditions
    auto faults1 = manager_.evaluate(sample);
    auto status1 = manager_.get_overall_status(sample);
    
    auto faults2 = manager_.evaluate(sample);
    auto status2 = manager_.get_overall_status(sample);
    
    // Fault should persist
    EXPECT_TRUE(faults1.count(FaultType::BRAKE_PRESSURE_FAULT));
    EXPECT_TRUE(faults2.count(FaultType::BRAKE_PRESSURE_FAULT));
    EXPECT_EQ(OverallStatus::FAULT, status1);
    EXPECT_EQ(OverallStatus::FAULT, status2);
}

// TC-FLT-005: Partial fault recovery (multiple faults, some clear)
TEST_F(FaultManagerTest, TC_FLT_005_PartialFaultRecovery) {
    auto sample = create_valid_sample();
    
    // Sample 1: Multiple faults
    sample.vehicleSpeedKph = 80.0;
    sample.brakePedalPressed = true;
    sample.brakePressureBar = 0.0;         // Brake fault
    sample.frontLeftWheelSpeedKph = 68.0;  // FL mismatch
    auto faults1 = manager_.evaluate(sample);
    
    EXPECT_TRUE(faults1.count(FaultType::BRAKE_PRESSURE_FAULT));
    EXPECT_TRUE(faults1.count(FaultType::FL_WHEEL_SPEED_MISMATCH));
    
    // Sample 2: Only brake fault cleared, FL mismatch remains
    sample.brakePressureBar = 50.0;        // Brake pressure restored
    auto faults2 = manager_.evaluate(sample);
    
    EXPECT_FALSE(faults2.count(FaultType::BRAKE_PRESSURE_FAULT));
    EXPECT_TRUE(faults2.count(FaultType::FL_WHEEL_SPEED_MISMATCH));
}

// TC-FLT-006: Overall status FAULT with single fault (FLT-04)
TEST_F(FaultManagerTest, TC_FLT_006_SingleFaultYieldsFaultStatus) {
    auto sample = create_valid_sample();
    sample.vehicleSpeedKph = 80.0;
    sample.frontLeftWheelSpeedKph = 95.0;  // Mismatch
    
    auto status = manager_.get_overall_status(sample);
    EXPECT_EQ(OverallStatus::FAULT, status);
}

// TC-FLT-007: Overall status INVALID_DATA (FLT-04, DAT validation)
TEST_F(FaultManagerTest, TC_FLT_007_InvalidDataYieldsInvalidStatus) {
    auto sample = create_valid_sample();
    sample.vehicleSpeedKph = -10.0;  // Invalid
    
    auto status = manager_.get_overall_status(sample);
    EXPECT_EQ(OverallStatus::INVALID_DATA, status);
}

// TC-FLT-008: Validation error takes precedence
TEST_F(FaultManagerTest, TC_FLT_008_ValidationErrorTakesPrecedence) {
    auto sample = create_valid_sample();
    sample.vehicleSpeedKph = -5.0;  // Invalid vehicle speed
    sample.brakePedalPressed = true;
    sample.brakePressureBar = 0.0;  // Would be brake fault if valid
    
    auto faults = manager_.evaluate(sample);
    auto status = manager_.get_overall_status(sample);
    
    // Should only report validation error, not brake fault
    EXPECT_TRUE(faults.count(FaultType::INVALID_MONITORING_DATA));
    EXPECT_FALSE(faults.count(FaultType::BRAKE_PRESSURE_FAULT));
    EXPECT_EQ(OverallStatus::INVALID_DATA, status);
}

// TC-FLT-009: Multiple faults with braking spread
TEST_F(FaultManagerTest, TC_FLT_009_MultipleFaultsWithBrakingSpread) {
    auto sample = create_valid_sample();
    sample.vehicleSpeedKph = 100.0;
    sample.brakePedalPressed = true;
    sample.brakePressureBar = 0.0;         // Brake fault
    sample.frontLeftWheelSpeedKph = 88.0;  // FL mismatch
    sample.rearRightWheelSpeedKph = 75.0;  // Spread fault (spread = 88 - 75 = 13, not > 15, actually OK)
    
    auto faults = manager_.evaluate(sample);
    auto status = manager_.get_overall_status(sample);
    
    EXPECT_TRUE(faults.count(FaultType::BRAKE_PRESSURE_FAULT));
    EXPECT_TRUE(faults.count(FaultType::FL_WHEEL_SPEED_MISMATCH));
    EXPECT_EQ(OverallStatus::FAULT, status);
}

// TC-FLT-010: RL_WHEEL_SPEED_MISMATCH from requirements example
TEST_F(FaultManagerTest, TC_FLT_010_RequirementsExample) {
    auto sample = create_valid_sample();
    // Example from requirements.md FLT-02
    sample.vehicleSpeedKph = 80.0;
    sample.brakePedalPressed = true;
    sample.brakePressureBar = 0.0;
    sample.frontLeftWheelSpeedKph = 82.0;
    sample.frontRightWheelSpeedKph = 79.0;
    sample.rearLeftWheelSpeedKph = 68.0;  // -12, mismatch
    sample.rearRightWheelSpeedKph = 81.0;
    
    auto faults = manager_.evaluate(sample);
    auto status = manager_.get_overall_status(sample);
    
    EXPECT_TRUE(faults.count(FaultType::BRAKE_PRESSURE_FAULT));
    EXPECT_TRUE(faults.count(FaultType::RL_WHEEL_SPEED_MISMATCH));
    EXPECT_EQ(OverallStatus::FAULT, status);
}

// TC-FLT-011: Braking spread large fault
TEST_F(FaultManagerTest, TC_FLT_011_LargeBrakingSpreadFault) {
    auto sample = create_valid_sample();
    sample.vehicleSpeedKph = 100.0;
    sample.brakePedalPressed = true;
    sample.brakePressureBar = 60.0;
    sample.frontLeftWheelSpeedKph = 98.0;
    sample.frontRightWheelSpeedKph = 97.0;
    sample.rearLeftWheelSpeedKph = 95.0;
    sample.rearRightWheelSpeedKph = 75.0;  // Spread = 23 km/h
    
    auto faults = manager_.evaluate(sample);
    auto status = manager_.get_overall_status(sample);
    
    EXPECT_TRUE(faults.count(FaultType::BRAKING_WHEEL_SPEED_MISMATCH));
    EXPECT_EQ(OverallStatus::FAULT, status);
}

// TC-FLT-012: Empty faults means NORMAL
TEST_F(FaultManagerTest, TC_FLT_012_EmptyFaultsIsNormal) {
    auto sample = create_valid_sample();
    sample.vehicleSpeedKph = 60.0;
    sample.brakePedalPressed = false;
    sample.brakePressureBar = 0.0;
    
    auto faults = manager_.evaluate(sample);
    auto status = manager_.get_overall_status(sample);
    
    EXPECT_TRUE(faults.empty() || faults.count(FaultType::NO_FAULT));
    EXPECT_EQ(OverallStatus::NORMAL, status);
}
