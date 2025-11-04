//
// Created by peppe289 on 11/4/25.
//


#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "../header/header.h"

int percentage_pwm_duty_cycle(int percent)
{
    if (percent < 0) percent = 0;
    if (percent > 100) percent = 100;

    return (percent * MAX_DUTY_CYCLE) / 100;
}

int open_sysfs(const char *path, int flags)
{
    const int fd = open(path, flags);
    if (fd < 0) {
        fprintf(stderr, "\n--------------------------------\n");
        fprintf(stderr, "Error opening %s: %s\n", path, strerror(errno));
        fprintf(stderr, "\n--------------------------------\n");
        fflush(stderr);
        exit(-1);
    }

    return fd;
}

int open_ro_sysfs(const char *path)
{
    return open_sysfs(path, O_RDONLY);
}

int open_wo_sysfs(const char *path)
{
    return open_sysfs(path, O_WRONLY);
}
