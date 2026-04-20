#include "monitor.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

// Direct parsing of /proc/[pid]/stat to avoid heavy overhead
int get_process_telemetry(int pid, proc_metrics_t *metrics) {
    char buf[512];
    char path[64];
    snprintf(path, sizeof(path), "/proc/%d/stat", pid);

    int fd = open(path, O_RDONLY);
    if (fd < 0) return -1;

    ssize_t bytes_read = read(fd, buf, sizeof(buf) - 1);
    close(fd);
    
    if (bytes_read <= 0) return -1;
    buf[bytes_read] = '\0';

    // We specifically target: 
    // utime(14), stime(15), num_threads(20), rss(24)
    char *ptr = buf;
    int field = 1;
    while (*ptr != '\0') {
        if (*ptr == ' ') {
            field++;
            if (field == 14) metrics->utime = strtoul(ptr + 1, NULL, 10);
            if (field == 15) metrics->stime = strtoul(ptr + 1, NULL, 10);
            if (field == 20) metrics->threads = strtoul(ptr + 1, NULL, 10);
            if (field == 24) {
                metrics->rss_pages = strtol(ptr + 1, NULL, 10);
                break; // Found everything we need
            }
        }
        ptr++;
    }
    return 0;
}
