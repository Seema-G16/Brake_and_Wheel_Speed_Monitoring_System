#include "faults/fault_manager.hpp"

namespace bwsms {

std::set<FaultType> FaultManager::evaluate(const BrakeWheelSpeedSample& sample) const {
    std::set<FaultType> all_faults;

    // Check data validity first (DAT-03: Invalid data short-circuits fault logic)
    FaultType validation_error = validator_.get_validation_error(sample);
    if (validation_error != FaultType::NO_FAULT) {
        all_faults.insert(validation_error);
        return all_faults;  // Return only validation error, don't evaluate other faults
    }

    // FLT-02: Aggregate faults from all monitors
    auto brake_faults = brake_monitor_.evaluate(sample);
    auto wheel_faults = wheel_speed_monitor_.evaluate(sample);

    all_faults.insert(brake_faults.begin(), brake_faults.end());
    all_faults.insert(wheel_faults.begin(), wheel_faults.end());

    // If no faults, explicitly add NO_FAULT
    if (all_faults.empty()) {
        all_faults.insert(FaultType::NO_FAULT);
    }

    return all_faults;
}

OverallStatus FaultManager::get_overall_status(const BrakeWheelSpeedSample& sample) const {
    // FLT-04: Overall status determination
    FaultType validation_error = validator_.get_validation_error(sample);
    
    if (validation_error != FaultType::NO_FAULT) {
        // Invalid data takes precedence
        return OverallStatus::INVALID_DATA;
    }

    // Check for any valid faults
    auto faults = evaluate(sample);
    
    // If only NO_FAULT is present, system is normal
    if (faults.size() == 1 && *faults.begin() == FaultType::NO_FAULT) {
        return OverallStatus::NORMAL;
    }
    
    // If any valid fault exists, system is in fault state
    if (!faults.empty()) {
        return OverallStatus::FAULT;
    }

    return OverallStatus::NORMAL;
}

bool FaultManager::has_validation_error(const BrakeWheelSpeedSample& sample) const {
    return validator_.get_validation_error(sample) != FaultType::NO_FAULT;
}

}  // namespace bwsms
