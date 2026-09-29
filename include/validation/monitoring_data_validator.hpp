#pragma once

#include "monitoring_system/brake_wheel_speed_sample.hpp"
#include "faults/fault_types.hpp"

/**
 * @file monitoring_data_validator.hpp
 * @brief Input validation for brake and wheel-speed monitoring data
 * Implements requirements DAT-01 through DAT-04
 */

namespace bwsms {

/**
 * MonitoringDataValidator
 * Validates all input signals in a BrakeWheelSpeedSample
 * 
 * Validation rules (from requirements.md):
 * - DAT-01: Vehicle speed must be >= 0 km/h
 * - DAT-02: All wheel speeds must be >= 0 km/h
 * - DAT-03: All required fields must be present (non-null/non-NaN)
 * - DAT-04: Brake pedal status must be boolean (TRUE or FALSE)
 */
class MonitoringDataValidator {
public:
    /**
     * Validate a monitoring sample
     * @param sample The sample to validate
     * @return true if all validation checks pass, false otherwise
     */
    bool is_valid(const BrakeWheelSpeedSample& sample) const;

    /**
     * Get the validation error (if any)
     * @return FaultType indicating the specific validation failure, or NO_FAULT if valid
     */
    FaultType get_validation_error(const BrakeWheelSpeedSample& sample) const;
};

}  // namespace bwsms
