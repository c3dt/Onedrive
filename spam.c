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
        "\033[1;34mUptime:\033[0m %ld hours, %ld mins",
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
