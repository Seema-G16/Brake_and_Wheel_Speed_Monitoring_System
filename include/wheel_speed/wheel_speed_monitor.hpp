#pragma once

#include "monitoring_system/brake_wheel_speed_sample.hpp"
#include "faults/fault_types.hpp"
#include <set>

/**
 * @file wheel_speed_monitor.hpp
 * @brief Wheel-speed monitoring and mismatch detection
 * Implements requirements WHL-01 through WHL-08
 */

namespace bwsms {

/**
 * WheelSpeedMonitor
 * Monitors wheel speeds and detects mismatches and braking spread faults
 * 
 * Requirements:
 * - WHL-01: Compare each wheel speed against vehicle speed
 * - WHL-02: Mismatch if |wheel_speed - vehicle_speed| > 10 km/h
 * - WHL-03: Report all wheels exceeding threshold
 * - WHL-04: Exactly 10 km/h difference is NOT a fault
 * - WHL-05: Braking spread check active only when braking (pedal AND speed > 0)
 * - WHL-06: Calculate spread = max_wheel - min_wheel during braking
 * - WHL-07: Report BRAKING_WHEEL_SPEED_MISMATCH if spread > 15 km/h
 * - WHL-08: Exactly 15 km/h spread is NOT a fault
 */
class WheelSpeedMonitor {
private:
    // Wheel-speed mismatch threshold (km/h)
    static constexpr double WHEEL_MISMATCH_THRESHOLD_KPH = 10.0;
    
    // Braking wheel-speed spread threshold (km/h)
    static constexpr double BRAKING_SPREAD_THRESHOLD_KPH = 15.0;

public:
    /**
     * Evaluate wheel-speed conditions in the given sample
     * @param sample The monitoring sample to evaluate
     * @return Set of faults detected (may be empty if no faults)
     */
    std::set<FaultType> evaluate(const BrakeWheelSpeedSample& sample) const;

    /**
     * Check if a specific wheel has a mismatch
     * @param wheel_speed The wheel speed in km/h
     * @param vehicle_speed The vehicle speed in km/h
     * @return true if |wheel_speed - vehicle_speed| > 10 km/h
     */
    bool has_wheel_mismatch(double wheel_speed, double vehicle_speed) const;

    /**
     * Calculate the wheel-speed spread (max - min)
     * @param sample The monitoring sample
     * @return The spread in km/h
     */
    double calculate_wheel_spread(const BrakeWheelSpeedSample& sample) const;

    /**
     * Check if braking spread fault is active
     * @param sample The monitoring sample
     * @return true if braking and spread > 15 km/h
     */
    bool has_braking_spread_fault(const BrakeWheelSpeedSample& sample) const;
};

}  // namespace bwsms
