#pragma once

#include <string>
#include <set>

/**
 * @file fault_types.hpp
 * @brief Fault type definitions and status enumerations for the monitoring system
 */

namespace bwsms {

/**
 * Enumeration of all possible fault conditions in the system
 */
enum class FaultType {
    NO_FAULT,
    BRAKE_PRESSURE_FAULT,
    INVALID_BRAKE_PRESSURE,
    FL_WHEEL_SPEED_MISMATCH,
    FR_WHEEL_SPEED_MISMATCH,
    RL_WHEEL_SPEED_MISMATCH,
    RR_WHEEL_SPEED_MISMATCH,
    BRAKING_WHEEL_SPEED_MISMATCH,
    INVALID_MONITORING_DATA
};

/**
 * Overall system status
 */
enum class OverallStatus {
    NORMAL,
    FAULT,
    INVALID_DATA
};

/**
 * Convert FaultType to string representation
 */
std::string fault_type_to_string(FaultType fault);

/**
 * Convert FaultType from string
 */
FaultType fault_type_from_string(const std::string& fault_str);

/**
 * Convert OverallStatus to string
 */
std::string overall_status_to_string(OverallStatus status);

}  // namespace bwsms
