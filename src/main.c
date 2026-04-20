#include <systemd/sd-daemon.h>
#include <signal.h>
#include "monitor.h"
#include "policy.h"

volatile sig_atomic_t keep_running = 1;

void handle_signal(int sig) {
    keep_running = 0;
}

int main(int argc, char *argv[]) {
    signal(SIGTERM, handle_signal);
    signal(SIGINT, handle_signal);

    thresholds_t sys_limits = { .max_rss_pages = 25600, .cpu_ticks_threshold = 5000, .max_threads = 50 };
    
    // Notify Systemd: Service is starting up
    sd_notify(0, "READY=1\nSTATUS=Watchdog active and monitoring...");

    while (keep_running) {
        proc_metrics_t metrics = {0};
        
        // Monitor a specific critical service PID (usually passed via config)
        if (get_process_telemetry(1, &metrics) == 0) {
            policy_action_t action = evaluate_resource_usage(&metrics, &sys_limits);
            
            if (action == ACTION_RESTART_SERVICE) {
                sd_notify(0, "STATUS=Threshold exceeded! Restarting monitored service...");
                // In a real scenario, use systemd-dbus to restart the target service
            }
        }

        // Heartbeat: If we don't send this, Systemd will kill US
        sd_notify(0, "WATCHDOG=1");
        sleep(2);
    }

    sd_notify(0, "STOPPING=1");
    return 0;
}
