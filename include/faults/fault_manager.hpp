#pragma once

#include "monitoring_system/brake_wheel_speed_sample.hpp"
#include "faults/fault_types.hpp"
#include "brake/brake_monitor.hpp"
#include "wheel_speed/wheel_speed_monitor.hpp"
#include "validation/monitoring_data_validator.hpp"
#include <set>

/**
 * @file fault_manager.hpp
 * @brief Aggregates and manages all faults across the system
 * Implements requirements FLT-01 through FLT-04
 */

namespace bwsms {

/**
 * FaultManager
 * Orchestrates fault detection and reporting from all monitors
 * 
 * Requirements:
 * - FLT-01: Maintain fault status enum (see FaultType)
 * - FLT-02: Support multiple simultaneous faults
 * - FLT-03: Fault recovery when conditions are resolved
 * - FLT-04: Provide overall status (NORMAL, FAULT, INVALID_DATA)
 */
class FaultManager {
private:
    BrakeMonitor brake_monitor_;
    WheelSpeedMonitor wheel_speed_monitor_;
    MonitoringDataValidator validator_;

public:
    /**
     * Evaluate all fault conditions for a monitoring sample
     * Returns aggregated faults from all monitors
     * 
     * @param sample The monitoring sample to evaluate
     * @return Set of all active faults (may be empty)
     */
    std::set<FaultType> evaluate(const BrakeWheelSpeedSample& sample) const;

    /**
     * Get the overall system status
     * @param sample The monitoring sample
     * @return Overall status (NORMAL, FAULT, or INVALID_DATA)
     */
    OverallStatus get_overall_status(const BrakeWheelSpeedSample& sample) const;

    /**
     * Check if data validation failed
     * @param sample The monitoring sample
     * @return true if validation error occurred
     */
    bool has_validation_error(const BrakeWheelSpeedSample& sample) const;
};

}  // namespace bwsms
