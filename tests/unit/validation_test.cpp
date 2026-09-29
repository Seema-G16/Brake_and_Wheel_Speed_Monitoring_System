#include <gtest/gtest.h>
#include "validation/monitoring_data_validator.hpp"
#include "monitoring_system/brake_wheel_speed_sample.hpp"
#include "faults/fault_types.hpp"
#include <cmath>

using namespace bwsms;

/**
 * Data Validation Tests (DAT requirements)
 * Tests input validation according to requirements DAT-01 through DAT-04
 */

class ValidationTest : public ::testing::Test {
protected:
    MonitoringDataValidator validator_;

    // Helper to create a valid baseline sample
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

// TC-DAT-001: Valid input - all signals nominal
TEST_F(ValidationTest, TC_DAT_001_ValidInputAllSignalsNominal) {
    auto sample = create_valid_sample();
    EXPECT_TRUE(validator_.is_valid(sample));
    EXPECT_EQ(FaultType::NO_FAULT, validator_.get_validation_error(sample));
}

// TC-DAT-002: Negative vehicle speed (DAT-01)
TEST_F(ValidationTest, TC_DAT_002_NegativeVehicleSpeed) {
    auto sample = create_valid_sample();
    sample.vehicleSpeedKph = -10.0;
    EXPECT_FALSE(validator_.is_valid(sample));
    EXPECT_EQ(FaultType::INVALID_MONITORING_DATA, validator_.get_validation_error(sample));
}

// TC-DAT-003: Negative wheel speed (DAT-02)
TEST_F(ValidationTest, TC_DAT_003_NegativeWheelSpeed) {
    auto sample = create_valid_sample();
    sample.frontLeftWheelSpeedKph = -5.0;
    EXPECT_FALSE(validator_.is_valid(sample));
    EXPECT_EQ(FaultType::INVALID_MONITORING_DATA, validator_.get_validation_error(sample));
}

// TC-DAT-004: Invalid brake pressure - negative (BRK-05)
TEST_F(ValidationTest, TC_DAT_004_InvalidBrakePressureNegative) {
    auto sample = create_valid_sample();
    sample.brakePressureBar = -1.0;
    EXPECT_FALSE(validator_.is_valid(sample));
    EXPECT_EQ(FaultType::INVALID_BRAKE_PRESSURE, validator_.get_validation_error(sample));
}

// TC-DAT-005: Invalid brake pressure - exceeds maximum (BRK-05)
TEST_F(ValidationTest, TC_DAT_005_InvalidBrakePressureExceedsMax) {
    auto sample = create_valid_sample();
    sample.brakePressureBar = 101.0;
    EXPECT_FALSE(validator_.is_valid(sample));
    EXPECT_EQ(FaultType::INVALID_BRAKE_PRESSURE, validator_.get_validation_error(sample));
}

// TC-DAT-006-A: Brake pressure zero lower boundary is valid
TEST_F(ValidationTest, TC_DAT_006_BrakePressureZeroIsValid) {
    auto sample = create_valid_sample();
    sample.brakePressureBar = 0.0;
    EXPECT_TRUE(validator_.is_valid(sample));
    EXPECT_EQ(FaultType::NO_FAULT, validator_.get_validation_error(sample));
}

// TC-DAT-007: Wheel speed zero is valid
TEST_F(ValidationTest, TC_DAT_007_WheelSpeedZeroIsValid) {
    auto sample = create_valid_sample();
    sample.vehicleSpeedKph = 0.0;
    sample.frontLeftWheelSpeedKph = 0.0;
    sample.frontRightWheelSpeedKph = 0.0;
    sample.rearLeftWheelSpeedKph = 0.0;
    sample.rearRightWheelSpeedKph = 0.0;
    EXPECT_TRUE(validator_.is_valid(sample));
}

// TC-DAT-008: Brake pressure upper boundary valid (100 bar)
TEST_F(ValidationTest, TC_DAT_008_BrakePressure100BarIsValid) {
    auto sample = create_valid_sample();
    sample.brakePressureBar = 100.0;
    EXPECT_TRUE(validator_.is_valid(sample));
    EXPECT_EQ(FaultType::NO_FAULT, validator_.get_validation_error(sample));
}

// TC-DAT-009: Brake pressure just above upper boundary (100.1 bar)
TEST_F(ValidationTest, TC_DAT_009_BrakePressure100Point1BarIsInvalid) {
    auto sample = create_valid_sample();
    sample.brakePressureBar = 100.1;
    EXPECT_FALSE(validator_.is_valid(sample));
    EXPECT_EQ(FaultType::INVALID_BRAKE_PRESSURE, validator_.get_validation_error(sample));
}

// TC-DAT-010: All wheel speeds must be checked (test FR wheel)
TEST_F(ValidationTest, TC_DAT_010_NegativeFrontRightWheelSpeed) {
    auto sample = create_valid_sample();
    sample.frontRightWheelSpeedKph = -2.0;
    EXPECT_FALSE(validator_.is_valid(sample));
    EXPECT_EQ(FaultType::INVALID_MONITORING_DATA, validator_.get_validation_error(sample));
}

// TC-DAT-011: Test RL wheel negative
TEST_F(ValidationTest, TC_DAT_011_NegativeRearLeftWheelSpeed) {
    auto sample = create_valid_sample();
    sample.rearLeftWheelSpeedKph = -1.5;
    EXPECT_FALSE(validator_.is_valid(sample));
    EXPECT_EQ(FaultType::INVALID_MONITORING_DATA, validator_.get_validation_error(sample));
}

// TC-DAT-012: Test RR wheel negative
TEST_F(ValidationTest, TC_DAT_012_NegativeRearRightWheelSpeed) {
    auto sample = create_valid_sample();
    sample.rearRightWheelSpeedKph = -3.0;
    EXPECT_FALSE(validator_.is_valid(sample));
    EXPECT_EQ(FaultType::INVALID_MONITORING_DATA, validator_.get_validation_error(sample));
}

// TC-DAT-013: Multiple validation errors - only first is reported
TEST_F(ValidationTest, TC_DAT_013_MultipleValidationErrors) {
    auto sample = create_valid_sample();
    sample.vehicleSpeedKph = -5.0;
    sample.brakePressureBar = 150.0;
    // First validation check should catch vehicle speed first
    EXPECT_FALSE(validator_.is_valid(sample));
    // Should report one of the errors
    FaultType error = validator_.get_validation_error(sample);
    EXPECT_NE(FaultType::NO_FAULT, error);
}
