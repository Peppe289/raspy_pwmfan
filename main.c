#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <asm-generic/errno-base.h>

#include "header/header.h"

int enable_pwm_sysfs()
{
    const int fd = open_wo_sysfs(PWM_EXPORT);

    if (write(fd, "0", 2) < 0) {
        perror("write export");
        return -1 | close(fd);
    }

    return close(fd);
}

int set_duty_cycle(int duty)
{
    char buff[10] = {0};
    const int fd = open_wo_sysfs(PWM_DUTY_CYCLE_PATH);

    sprintf(buff, "%d", duty);
    if (write(fd, buff, strlen(buff)) < 0) {
        perror("write duty_cycle");
        return -1 | close(fd);
    }

    return 0 | close(fd);
}

int configure_pwm()
{
    char buff[10] = {0};
    int fd = open_wo_sysfs(PWM_PERIOD_PATH);

    sprintf(buff, "%d", PWM_PERIOD);
    if (write(fd, buff, strlen(buff)) < 0) {
        perror("write period");
        return -1 | close(fd);
    }

    close(fd);

    set_duty_cycle(DEFAULT_DUTY);

    fd = open_wo_sysfs(PWM_ENABLE);

    if (write(fd, "1", 2) < 0)
    {
        perror("write enable");
        return -1 | close(fd);
    }

    return 0 | close(fd);
}

int init_service()
{
    int ret = enable_pwm_sysfs();
    if (ret)
        return -1;

    return 0;
}

int get_cpu_temperature()
{
    const int fd = open_ro_sysfs(CPU_TEMP_PATH);
    char buff[16] = {0};
    if (read(fd, buff, sizeof(buff)-1) < 0) {
        perror("read temp");
        return -1 | close(fd);
    }
    close(fd);
    return strtol(buff, NULL, 10) / 1000;
}

int main (int argc, char *argv[])
{
    int old_temperature = 0;
    if (argc >= 2)
    {
        printf("Enter only one argument --service to run as a background service.\n");
        if (strcmp(argv[1], "--service") == 0)
        {
            if (init_service() || configure_pwm())
                return -1;
        }
        return 0;
    }

    printf("Starting PWM Fan Control Service...\n");
    while (1)
    {
        const int temp = get_cpu_temperature();
        printf("[TEMP]: Current CPU Temperature: %d°C\n", temp);
        int index = 0;

        if (temp < TEMP_STEP_SILENT)
        {
            index = 0;
        } else if (temp >= TEMP_STEP_30C && temp < TEMP_STEP_40C)
        {
            index = TEMP_STEP_30C;
        }
        else if (temp >= TEMP_STEP_40C && temp < TEMP_STEP_50C)
        {
            index = TEMP_STEP_40C;
        }
        else if (temp >= TEMP_STEP_50C && temp < TEMP_STEP_60C)
        {
            index = TEMP_STEP_50C;
        }

        set_duty_cycle(percentage_pwm_duty_cycle(pwm_thermal_table[index]));

        if (temp != old_temperature)
        {
            printf("[TEMP]: Change state for %d°C\nNew fan speed to: %d/%d index: %d\n", temp, percentage_pwm_duty_cycle(pwm_thermal_table[index]), MAX_DUTY_CYCLE, index);
        }

        old_temperature = temp;
        if (temp >= 50)
            sleep(30);
        else
            sleep(5);
    }

    return 0;
}
