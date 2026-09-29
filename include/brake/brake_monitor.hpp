#pragma once

#include "monitoring_system/brake_wheel_speed_sample.hpp"
#include "faults/fault_types.hpp"
#include <set>

/**
 * @file brake_monitor.hpp
 * @brief Brake pressure monitoring and fault detection
 * Implements requirements BRK-01 through BRK-05
 */

namespace bwsms {

/**
 * BrakeMonitor
 * Monitors brake pressure and detects faults
 * 
 * Requirements:
 * - BRK-01: Braking condition = (pedal pressed AND speed > 0)
 * - BRK-02: During braking, pressure must be > 0 bar
 * - BRK-03: Report BRAKE_PRESSURE_FAULT if braking and pressure = 0
 * - BRK-04: No fault if speed = 0, even with pressure = 0
 * - BRK-05: Valid pressure range is 0 <= pressure <= 100 bar
 */
class BrakeMonitor {
public:
    /**
     * Evaluate brake pressure conditions in the given sample
     * @param sample The monitoring sample to evaluate
     * @return Set of faults detected (may be empty if no faults)
     */
    std::set<FaultType> evaluate(const BrakeWheelSpeedSample& sample) const;

    /**
     * Check if braking condition is active
     * @param sample The monitoring sample
     * @return true if pedal pressed AND speed > 0, false otherwise
     */
    bool is_braking(const BrakeWheelSpeedSample& sample) const;
};

}  // namespace bwsms
