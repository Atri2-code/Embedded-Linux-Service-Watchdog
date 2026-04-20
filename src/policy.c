#include "policy.h"

policy_action_t evaluate_resource_usage(proc_metrics_t *current, thresholds_t *limit) {
    // Stage 1: CPU Hog Detection
    if ((current->utime + current->stime) > limit->cpu_ticks_threshold) {
        return ACTION_RESTART_SERVICE;
    }

    // Stage 2: Memory Leak Detection (Resident Set Size)
    if (current->rss_pages > limit->max_rss_pages) {
        return ACTION_RESTART_SERVICE;
    }

    // Stage 3: Zombie Thread Detection
    if (current->threads > limit->max_threads) {
        return ACTION_ALERT_ONLY;
    }

    return ACTION_NONE;
}
