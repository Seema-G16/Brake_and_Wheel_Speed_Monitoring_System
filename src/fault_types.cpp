#include "faults/fault_types.hpp"

namespace bwsms {

std::string fault_type_to_string(FaultType fault) {
    switch (fault) {
        case FaultType::NO_FAULT:
            return "NO_FAULT";
        case FaultType::BRAKE_PRESSURE_FAULT:
            return "BRAKE_PRESSURE_FAULT";
        case FaultType::INVALID_BRAKE_PRESSURE:
            return "INVALID_BRAKE_PRESSURE";
        case FaultType::FL_WHEEL_SPEED_MISMATCH:
            return "FL_WHEEL_SPEED_MISMATCH";
        case FaultType::FR_WHEEL_SPEED_MISMATCH:
            return "FR_WHEEL_SPEED_MISMATCH";
        case FaultType::RL_WHEEL_SPEED_MISMATCH:
            return "RL_WHEEL_SPEED_MISMATCH";
        case FaultType::RR_WHEEL_SPEED_MISMATCH:
            return "RR_WHEEL_SPEED_MISMATCH";
        case FaultType::BRAKING_WHEEL_SPEED_MISMATCH:
            return "BRAKING_WHEEL_SPEED_MISMATCH";
        case FaultType::INVALID_MONITORING_DATA:
            return "INVALID_MONITORING_DATA";
        default:
            return "UNKNOWN_FAULT";
    }
}

FaultType fault_type_from_string(const std::string& fault_str) {
    if (fault_str == "NO_FAULT") return FaultType::NO_FAULT;
    if (fault_str == "BRAKE_PRESSURE_FAULT") return FaultType::BRAKE_PRESSURE_FAULT;
    if (fault_str == "INVALID_BRAKE_PRESSURE") return FaultType::INVALID_BRAKE_PRESSURE;
    if (fault_str == "FL_WHEEL_SPEED_MISMATCH") return FaultType::FL_WHEEL_SPEED_MISMATCH;
    if (fault_str == "FR_WHEEL_SPEED_MISMATCH") return FaultType::FR_WHEEL_SPEED_MISMATCH;
    if (fault_str == "RL_WHEEL_SPEED_MISMATCH") return FaultType::RL_WHEEL_SPEED_MISMATCH;
    if (fault_str == "RR_WHEEL_SPEED_MISMATCH") return FaultType::RR_WHEEL_SPEED_MISMATCH;
    if (fault_str == "BRAKING_WHEEL_SPEED_MISMATCH") return FaultType::BRAKING_WHEEL_SPEED_MISMATCH;
    if (fault_str == "INVALID_MONITORING_DATA") return FaultType::INVALID_MONITORING_DATA;
    return FaultType::NO_FAULT;
}

std::string overall_status_to_string(OverallStatus status) {
    switch (status) {
        case OverallStatus::NORMAL:
            return "NORMAL";
        case OverallStatus::FAULT:
            return "FAULT";
        case OverallStatus::INVALID_DATA:
            return "INVALID_DATA";
        default:
            return "UNKNOWN";
    }
}

}  // namespace bwsms
