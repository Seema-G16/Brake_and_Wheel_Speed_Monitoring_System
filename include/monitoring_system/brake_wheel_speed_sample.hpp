#pragma once

/**
 * @file brake_wheel_speed_sample.hpp
 * @brief Data structure for brake and wheel-speed monitoring samples
 */

namespace bwsms {

/**
 * Represents a single brake and wheel-speed monitoring sample
 * Contains all 8 input signals as specified in requirements.md Section 2
 */
struct BrakeWheelSpeedSample {
    /// Timestamp in seconds
    double timestampSeconds = 0.0;
    
    /// Vehicle speed in km/h (must be >= 0)
    double vehicleSpeedKph = 0.0;
    
    /// Brake pedal status (true = pressed, false = released)
    bool brakePedalPressed = false;
    
    /// Brake pressure in bar (must be 0 <= pressure <= 100)
    double brakePressureBar = 0.0;
    
    /// Front-left wheel speed in km/h (must be >= 0)
    double frontLeftWheelSpeedKph = 0.0;
    
    /// Front-right wheel speed in km/h (must be >= 0)
    double frontRightWheelSpeedKph = 0.0;
    
    /// Rear-left wheel speed in km/h (must be >= 0)
    double rearLeftWheelSpeedKph = 0.0;
    
    /// Rear-right wheel speed in km/h (must be >= 0)
    double rearRightWheelSpeedKph = 0.0;
};

}  // namespace bwsms
