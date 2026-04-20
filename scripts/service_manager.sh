#!/bin/bash

# Configuration
WATCHDOG_BIN="/usr/local/bin/watchdog_daemon"
LOG_FILE="/var/log/watchdog_health.log"

function check_status() {
    echo "--- [$(date)] Watchdog Status ---" >> $LOG_FILE
    systemctl status watchdog.service --no-pager >> $LOG_FILE
    journalctl -u watchdog.service --since "1 hour ago" >> $LOG_FILE
}

function rotate_logs() {
    if [ $(stat -c%s "$LOG_FILE") -gt 1048576 ]; then
        echo "Log exceeded 1MB. Rotating..."
        mv $LOG_FILE "$LOG_FILE.old"
        touch $LOG_FILE
    fi
}

# Entry point
case "$1" in
    status) check_status ;;
    rotate) rotate_logs ;;
    *) echo "Usage: $0 {status|rotate}" ;;
esac
