#include "brake/brake_monitor.hpp"

namespace bwsms {

bool BrakeMonitor::is_braking(const BrakeWheelSpeedSample& sample) const {
    // BRK-01: Braking condition = (pedal pressed AND speed > 0)
    return sample.brakePedalPressed && sample.vehicleSpeedKph > 0.0;
}

std::set<FaultType> BrakeMonitor::evaluate(const BrakeWheelSpeedSample& sample) const {
    std::set<FaultType> faults;

    // BRK-03 & BRK-04: Check for brake pressure fault
    // Fault occurs only if braking (speed > 0 AND pedal pressed) and pressure = 0
    if (is_braking(sample) && sample.brakePressureBar == 0.0) {
        faults.insert(FaultType::BRAKE_PRESSURE_FAULT);
    }

    return faults;
}

}  // namespace bwsms
