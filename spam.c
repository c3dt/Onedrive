#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/utsname.h>

#if defined(__linux__)
#include <sys/sysinfo.h>
#endif

#if defined(__APPLE__)
#include <sys/types.h>
#include <sys/sysctl.h>
#include <mach/mach.h>
#endif

#define LOGO_LINES 15
#define MAX_LEN 256

void get_memory(unsigned long *used, unsigned long *total) {
    *used = 0;
    *total = 0;

#if defined(__linux__)
    FILE *fp = fopen("/proc/meminfo", "r");
    if (fp) {
        char line[256];
        unsigned long mem_total = 0;
        unsigned long mem_free = 0;
        unsigned long buffers = 0;
        unsigned long cached = 0;
        unsigned long shmem = 0;
        unsigned long sreclaimable = 0;

        while (fgets(line, sizeof(line), fp)) {
            if (sscanf(line, "MemTotal: %lu kB", &mem_total) == 1) continue;
            if (sscanf(line, "MemFree: %lu kB", &mem_free) == 1) continue;
            if (sscanf(line, "Buffers: %lu kB", &buffers) == 1) continue;
            if (sscanf(line, "Cached: %lu kB", &cached) == 1) continue;
            if (sscanf(line, "Shmem: %lu kB", &shmem) == 1) continue;
            if (sscanf(line, "SReclaimable: %lu kB", &sreclaimable) == 1) continue;
        }
        fclose(fp);

        if (mem_total > 0) {
            *total = mem_total / 1024;
            unsigned long reclaimable_all = buffers + cached + sreclaimable;
            unsigned long total_plus_shmem = mem_total + shmem;
            unsigned long free_plus_cache = mem_free + reclaimable_all;

            if (total_plus_shmem >= free_plus_cache) {
                *used = (total_plus_shmem - free_plus_cache) / 1024;
            }
        }
    }

#elif defined(__APPLE__)
    int mib[2] = {CTL_HW, HW_MEMSIZE};
    uint64_t memsize = 0;
    size_t len = sizeof(memsize);

    if (sysctl(mib, 2, &memsize, &len, NULL, 0) == 0) {
        vm_size_t page_size;
        mach_port_t host = mach_host_self();
        vm_statistics64_data_t vmstat;
        mach_msg_type_number_t count = HOST_VM_INFO64_COUNT;

        if (host_page_size(host, &page_size) == KERN_SUCCESS &&
            host_statistics64(host, HOST_VM_INFO64,
                (host_info64_t)&vmstat, &count) == KERN_SUCCESS) {

            *total = (unsigned long long)memsize / (1024 * 1024);

            unsigned long long used_pages =
                (unsigned long long)vmstat.active_count +
                vmstat.wire_count +
                vmstat.compressor_page_count;

            *used = (used_pages * page_size) / (1024 * 1024);
        }
    }
#endif
}

int main(void) {
    const char *ascii[LOGO_LINES] = {
        "  ⠀⠀⠀⠀  ⠀⠀⠀⠀⠀⢀⡤⠺⠓⠢⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀  ",
        "  ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡎⠉⠚⠓⠚⡭⠿⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀  ",
        "  ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡼⠀⠀⠀⠀⢜⠦⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀  ",
        "  ⠀⠀⠀⠀⠀⠀⠀⠀⠀⢰⡡⠒⠒⠒⠀⠁⠀⠱⡤⢤⣤⣤⣀⠀⠀⠀⠀⠀⠀  ",
        "  ⠀⠀⠀⠀⠀⠀⠀⠀⡠⢺⠀⠀⠀⠀⠀⠀⠀⠀⢳⣁⡀⠈⠹⡳⡀⠀⠀⠀⠀  ",
        "  ⠀⠀⠀⠀⠀⡠⡴⢾⠂⢸⠀⠀⠀⠀⠀⠀⠀⠀⠈⡆⠈⠙⠢⡇⡇⠀⠀⠀⠀  ",
        "  ⠀⠀⢀⢔⠝⠊⣀⠼⠔⠺⠀⠀⠀⠀⠀⠀⠀⠀⠀⢳⠀⠀⠀⠈⠀⠀⠀⠀⠀  ",
        "  ⠀⡰⡱⢁⡴⠋⠀⠀⠀⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⡆⠀⠀⠀⠀⠀⠀⠀⠀  ",
        "  ⢰⠁⡥⠋⠀⠀⠀⠀⠀⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢱⠀⠀⠀⠀⠀⠀⠀⠀  ",
        "  ⠈⠛⠁⠀⠀⠀⠀⠀⠀⠈⡆⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⠀⠀⠀⠀⠀⠀⠀⠀  ",
        "  ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢹⠀⠀⠀⠀⠀⠀⠀⢀⠀⡜⠀⠀⠀⠀⠀⠀⠀⠀  ",
        "  ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡇⠀⠀⢸⠀⠀⢀⠂⡜⠀⠀⠀⠀⠀⠀⠀⠀⠀  ",
        "  ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⡄⠀⢸⢀⣀⣸⠉⠉⠰⣂⣄⠀⠀⠀⠀⠀⠀  ",
        "  ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⣾⣘⡖⣹⡁⠀⠀⠓⠺⠉⠉⠀⠀⠀⠀⠀⠀⠀  ",
        "  ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠷⠄⠸⠑⠂⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀  "
    };

    struct utsname uts;

    char hostname[64] = "unknown";

    const char *user = getenv("USER");
    const char *shell = getenv("SHELL");

    long up_hours = 0;
    long up_mins = 0;

    unsigned long used_ram = 0;
    unsigned long total_ram = 0;

    if (uname(&uts) != 0) {
        memset(&uts, 0, sizeof(uts));
        snprintf(uts.release, sizeof(uts.release), "unknown");
    }

    if (gethostname(hostname, sizeof(hostname) - 1) != 0)
        snprintf(hostname, sizeof(hostname), "unknown");

    hostname[sizeof(hostname) - 1] = '\0';

#if defined(__linux__)
    struct sysinfo si;

    if (sysinfo(&si) == 0) {
        up_hours = si.uptime / 3600;
        up_mins = (si.uptime % 3600) / 60;
    }
#endif

    get_memory(&used_ram, &total_ram);

    char info[LOGO_LINES][MAX_LEN] = {0};

    snprintf(info[1], MAX_LEN,
        "\033[1;34m%s@%s\033[0m",
        user ? user : "user", hostname);

    snprintf(info[2], MAX_LEN, "-------------------");

    snprintf(info[3], MAX_LEN,
        "\033[1;34mKernel:\033[0m %s",
        uts.release);

    snprintf(info[4], MAX_LEN,
        "\033[1;34mUptime:\033[0m %ld hours %ld mins",
        up_hours, up_mins);

    snprintf(info[5], MAX_LEN,
        "\033[1;34mShell:\033[0m %s",
        shell ? shell : "unknown");

    if (total_ram > 0) {
        snprintf(info[6], MAX_LEN,
            "\033[1;34mMemory:\033[0m %lu MiB / %lu MiB",
            used_ram, total_ram);
    } else {
        snprintf(info[6], MAX_LEN,
            "\033[1;34mMemory:\033[0m unavailable");
    }

    printf("\n");

    for (int i = 0; i < LOGO_LINES; i++) {
        printf("\033[1;37m%s\033[0m  %s\n",
            ascii[i], info[i]);
    }

    printf("\n");

    return 0;
}
