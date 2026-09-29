#include "validation/monitoring_data_validator.hpp"
#include <cmath>

namespace bwsms {

bool MonitoringDataValidator::is_valid(const BrakeWheelSpeedSample& sample) const {
    return get_validation_error(sample) == FaultType::NO_FAULT;
}

FaultType MonitoringDataValidator::get_validation_error(const BrakeWheelSpeedSample& sample) const {
    // Check for NaN values (missing data) - DAT-03
    if (std::isnan(sample.vehicleSpeedKph) || std::isnan(sample.brakePressureBar) ||
        std::isnan(sample.frontLeftWheelSpeedKph) || std::isnan(sample.frontRightWheelSpeedKph) ||
        std::isnan(sample.rearLeftWheelSpeedKph) || std::isnan(sample.rearRightWheelSpeedKph)) {
        return FaultType::INVALID_MONITORING_DATA;
    }

    // DAT-01: Vehicle speed must be >= 0 km/h
    if (sample.vehicleSpeedKph < 0.0) {
        return FaultType::INVALID_MONITORING_DATA;
    }

    // DAT-02: All wheel speeds must be >= 0 km/h
    if (sample.frontLeftWheelSpeedKph < 0.0 || sample.frontRightWheelSpeedKph < 0.0 ||
        sample.rearLeftWheelSpeedKph < 0.0 || sample.rearRightWheelSpeedKph < 0.0) {
        return FaultType::INVALID_MONITORING_DATA;
    }

    // BRK-05: Brake pressure range 0 <= pressure <= 100 bar
    if (sample.brakePressureBar < 0.0 || sample.brakePressureBar > 100.0) {
        return FaultType::INVALID_BRAKE_PRESSURE;
    }

    return FaultType::NO_FAULT;
}

}  // namespace bwsms
