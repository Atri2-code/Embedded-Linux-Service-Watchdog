cmake_minimum_required(VERSION 3.10)
project(EmbeddedWatchdog C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Check for systemd (Required for sd_notify)
find_package(PkgConfig REQUIRED)
pkg_check_modules(SYSTEMD libsystemd)

include_directories(include ${SYSTEMD_INCLUDE_DIRS})

# Main Daemon
add_executable(watchdog_daemon src/main.c src/monitor.c src/policy.c)
target_link_libraries(watchdog_daemon ${SYSTEMD_LIBRARIES})

# Unit Testing (Google Test)
enable_testing()
find_package(GTest REQUIRED)
add_executable(unit_tests tests/test_suite.cpp src/monitor.c src/policy.c)
target_link_libraries(unit_tests GTest::GTest GTest::Main ${SYSTEMD_LIBRARIES})
add_test(NAME AllTests COMMAND unit_tests)
