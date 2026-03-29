#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_IP_LENGTH 32

// Validate range
int validate_range(int start, int end) {
    return (start >= 0 && end <= 255 && start <= end);
}

// Check if host is active
int is_host_active(const char *ip) {
    char command[128];

#ifdef _WIN32
    // Windows ping
    snprintf(command, sizeof(command),
             "ping -n 1 -w 1000 %s > nul", ip);
#else
    // Linux / macOS ping
    snprintf(command, sizeof(command),
             "ping -c 1 -W 1 %s > /dev/null 2>&1", ip);
#endif

    return system(command) == 0;
}

// Scan network
void scan_network(const char *base_ip, int start, int end) {
    for (int i = start; i <= end; i++) {
        char ip[MAX_IP_LENGTH];
        snprintf(ip, sizeof(ip), "%s.%d", base_ip, i);

        if (is_host_active(ip)) {
            printf("%-16s ACTIVE\n", ip);
        } else {
            printf("%-16s INACTIVE\n", ip);
        }
    }
}

int main(int argc, char *argv[]) {

    if (argc != 4) {
        printf("Usage: %s <base_ip> <start> <end>\n", argv[0]);
        printf("Example: %s 192.168.1 1 50\n", argv[0]);
        return 1;
    }

    char *base_ip = argv[1];
    int start = atoi(argv[2]);
    int end = atoi(argv[3]);

    if (!validate_range(start, end)) {
        printf("Invalid range! Must be 0–255 and start <= end\n");
        return 1;
    }

    printf("\nScanning %s.%d-%d ...\n\n", base_ip, start, end);

    scan_network(base_ip, start, end);

    printf("\nScan complete.\n");

    return 0;
}
