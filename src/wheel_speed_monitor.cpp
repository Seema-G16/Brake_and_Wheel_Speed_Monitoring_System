#include "wheel_speed/wheel_speed_monitor.hpp"
#include <cmath>
#include <algorithm>

namespace bwsms {

bool WheelSpeedMonitor::has_wheel_mismatch(double wheel_speed, double vehicle_speed) const {
    // WHL-02 & WHL-04: Mismatch if difference > 10 km/h (not >=)
    double difference = std::abs(wheel_speed - vehicle_speed);
    return difference > WHEEL_MISMATCH_THRESHOLD_KPH;
}

double WheelSpeedMonitor::calculate_wheel_spread(const BrakeWheelSpeedSample& sample) const {
    // WHL-06: Spread = max wheel speed - min wheel speed
    double max_speed = std::max({sample.frontLeftWheelSpeedKph,
                                 sample.frontRightWheelSpeedKph,
                                 sample.rearLeftWheelSpeedKph,
                                 sample.rearRightWheelSpeedKph});
    double min_speed = std::min({sample.frontLeftWheelSpeedKph,
                                 sample.frontRightWheelSpeedKph,
                                 sample.rearLeftWheelSpeedKph,
                                 sample.rearRightWheelSpeedKph});
    return max_speed - min_speed;
}

bool WheelSpeedMonitor::has_braking_spread_fault(const BrakeWheelSpeedSample& sample) const {
    // WHL-05: Braking check active only when braking (pedal pressed AND speed > 0)
    bool is_braking = sample.brakePedalPressed && sample.vehicleSpeedKph > 0.0;
    if (!is_braking) {
        return false;
    }

    // WHL-07 & WHL-08: Fault if spread > 15 km/h (not >=)
    double spread = calculate_wheel_spread(sample);
    return spread > BRAKING_SPREAD_THRESHOLD_KPH;
}

std::set<FaultType> WheelSpeedMonitor::evaluate(const BrakeWheelSpeedSample& sample) const {
    std::set<FaultType> faults;

    // WHL-01, WHL-02, WHL-03, WHL-04: Check individual wheel mismatches
    if (has_wheel_mismatch(sample.frontLeftWheelSpeedKph, sample.vehicleSpeedKph)) {
        faults.insert(FaultType::FL_WHEEL_SPEED_MISMATCH);
    }
    if (has_wheel_mismatch(sample.frontRightWheelSpeedKph, sample.vehicleSpeedKph)) {
        faults.insert(FaultType::FR_WHEEL_SPEED_MISMATCH);
    }
    if (has_wheel_mismatch(sample.rearLeftWheelSpeedKph, sample.vehicleSpeedKph)) {
        faults.insert(FaultType::RL_WHEEL_SPEED_MISMATCH);
    }
    if (has_wheel_mismatch(sample.rearRightWheelSpeedKph, sample.vehicleSpeedKph)) {
        faults.insert(FaultType::RR_WHEEL_SPEED_MISMATCH);
    }

    // WHL-05, WHL-06, WHL-07, WHL-08: Check braking wheel-speed spread
    if (has_braking_spread_fault(sample)) {
        faults.insert(FaultType::BRAKING_WHEEL_SPEED_MISMATCH);
    }

    return faults;
}

}  // namespace bwsms
