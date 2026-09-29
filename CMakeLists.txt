cmake_minimum_required(VERSION 3.15)
project(BrakeWheelSpeedMonitoringSystem)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Production source files
set(PRODUCTION_SOURCES
    src/fault_types.cpp
    src/monitoring_data_validator.cpp
    src/brake_monitor.cpp
    src/wheel_speed_monitor.cpp
    src/fault_manager.cpp
)

# Create production library
add_library(bwsms_lib ${PRODUCTION_SOURCES})
target_include_directories(bwsms_lib PUBLIC include)

# Enable testing
enable_testing()

# Fetch GoogleTest automatically
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
    DOWNLOAD_EXTRACT_TIMESTAMP YES
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Unit test sources
set(TEST_SOURCES
    tests/unit/validation_test.cpp
    tests/unit/brake_monitor_test.cpp
    tests/unit/wheel_speed_monitor_test.cpp
    tests/unit/fault_manager_test.cpp
)

# Create test executable
add_executable(unit_tests ${TEST_SOURCES})
target_link_libraries(unit_tests PRIVATE bwsms_lib gtest gtest_main)
target_include_directories(unit_tests PRIVATE include)

# Register tests with ctest using gtest_discover_tests
include(GoogleTest)
gtest_discover_tests(unit_tests)

# Print configuration summary
message(STATUS "Build configuration:")
message(STATUS "  C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Production Library: bwsms_lib")
message(STATUS "  Unit Tests: unit_tests")
message(STATUS "  Test Framework: GoogleTest")
