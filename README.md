#Embedded-linux-service-watchdog
build: passing
C17
Systemd
CMake 3.14+
tests: 24 passing
A production-grade Linux service watchdog daemon written in C, orchestrated with Bash, and managed as a native Systemd unit. The daemon monitors registered child processes for liveness, enforces CPU and memory thresholds, and applies configurable restart policies — logging all events to the system journal via journald.

Built to mirror real embedded Linux engineering: close-to-hardware process management, deterministic state transitions, and strict unit test coverage via Google Test (C++ test harness wrapping the C core).

Architecture
C · watchdog_core
Daemon Core
PID monitoring, /proc polling, restart policy FSM, signal handling (SIGTERM/SIGHUP)
Bash · scripts/
Orchestration Layer
Service registration, config validation, build pipeline, log rotation helper
Systemd · units/
Service Units
Unit file with WatchdogSec, Restart=on-failure, sd_notify() integration
C++ · tests/
Google Test Suite
24 unit tests covering FSM transitions, threshold logic, PID parse, and restart backoff
File structure
embedded-linux-service-watchdog/ ├── src/ │ ├── watchdog.c # daemon entry, signal handling │ ├── monitor.c # /proc polling, threshold checks │ ├── policy.c # restart FSM, backoff logic │ └── journal.c # journald logging wrapper ├── include/ │ ├── watchdog.h │ ├── monitor.h │ └── policy.h ├── scripts/ │ ├── register_service.sh # register a PID/config │ ├── build.sh # cmake wrapper + install │ └── log_rotate.sh # journald log archiver ├── units/ │ └── watchdog.service # systemd unit file ├── tests/ │ ├── test_monitor.cpp │ ├── test_policy.cpp │ └── test_journal.cpp └── CMakeLists.txt
Build and install
git clone https://github.com/Atri2-code/embedded-linux-service-watchdog
cd embedded-linux-service-watchdog
chmod +x scripts/build.sh && ./scripts/build.sh
sudo systemctl enable --now watchdog.service
Running the test suite
mkdir build && cd build
cmake .. -DBUILD_TESTS=ON
make && ctest --output-on-failure
All 24 tests pass on GCC 11+ / Linux kernel 5.15+. CI via GitHub Actions on ubuntu-latest.

Key technical decisions
/proc/[pid]/stat parsed directly in C for zero-dependency CPU accounting
Restart backoff implemented as a finite state machine with configurable ceiling
sd_notify(READY=1) signals Systemd readiness — daemon is Type=notify
SIGHUP triggers live config reload without restart
Google Test harness wraps the C core via extern "C" — no test-only code paths
