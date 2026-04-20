# 🛡️ Embedded Linux Service Watchdog

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Language: C11](https://img.shields.io/badge/Language-C11-blue.svg)]()
[![Platform: Linux](https://img.shields.io/badge/Platform-Embedded_Linux-lightgrey.svg)]()

An industrial-strength, low-overhead system daemon designed for **Embedded Linux** environments. This watchdog provides high-availability monitoring for critical services by interfacing directly with the Linux kernel via `/proc` telemetry and integrating deeply with the **Systemd** lifecycle manager.

## 🏗️ Architecture Overview

The watchdog is architected as a modular state machine designed for deterministic execution and minimal memory footprint. It operates close to the hardware boundary, avoiding heavy dependencies and utilizing raw system calls for maximum efficiency.

### Core Components:
* **Telemetry Monitor (`src/monitor.c`):** High-performance parser that extracts CPU, Memory (RSS), and Threading metrics directly from `/proc/[pid]/stat`.
* **Policy Engine (`src/policy.c`):** A configurable threshold-based evaluator that determines recovery actions (Restart, Alert, or Log).
* **Systemd Lifecycle Handler (`src/main.c`):** Implements the `sd_notify` protocol to handle `READY`, `WATCHDOG`, and `STOPPING` signals, ensuring the daemon itself is monitored by the init system.

## 🚀 Key Features

* **Direct Kernel Interfacing:** Zero-overhead monitoring using direct file descriptors and manual pointer arithmetic for parsing kernel telemetry.
* **Self-Healing Logic:** Integrated with `Systemd`'s `Type=notify` and `WatchdogSec` features to ensure 100% service availability.
* **Automated Orchestration:** Custom **Bash** scripts for automated cross-compilation, deployment, and log rotation.
* **Industrial Testing:** 24+ comprehensive unit tests using **Google Test** to validate PID parsing logic and policy state transitions.

## 🛠️ Technology Stack

| Component | Technology |
| :--- | :--- |
| **Languages** | C11 (Daemon), C++17 (Tests), Bash (Scripts) |
| **Build System** | CMake 3.10+ |
| **Init System** | Systemd (libsystemd) |
| **Testing** | Google Test (GTest) |
| **Deployment** | Linux/Embedded Linux (AARCH64/x86_64) |

## 📦 Getting Started

### Prerequisites
* `libsystemd-dev`
* `cmake`
* `build-essential`
* `googletest`

### Build & Installation
```bash
# Clone the repository
git clone [https://github.com/Atri2-code/embedded-linux-service-watchdog.git](https://github.com/Atri2-code/embedded-linux-service-watchdog.git)
cd embedded-linux-service-watchdog

# Build using CMake
mkdir build && cd build
cmake ..
make

# Run the deployment script (Configures Systemd automatically)
sudo ../scripts/deploy.sh
