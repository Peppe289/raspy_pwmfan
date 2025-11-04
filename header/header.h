//
// Created by peppe289 on 11/4/25.
//

#ifndef HEADER_H
#define HEADER_H

#define PWM_PERIOD      10000
#define MAX_DUTY_CYCLE  PWM_PERIOD
#define DEFAULT_DUTY    8000

#define PWM_EXPORT "/sys/class/pwm/pwmchip0/export"
#define PWM_ENABLE "/sys/class/pwm/pwmchip0/pwm0/enable"
#define PWM_PERIOD_PATH "/sys/class/pwm/pwmchip0/pwm0/period"
#define PWM_DUTY_CYCLE_PATH "/sys/class/pwm/pwmchip0/pwm0/duty_cycle"
#define CPU_TEMP_PATH "/sys/class/thermal/thermal_zone0/hwmon0/temp1_input"

int open_ro_sysfs(const char *);
int open_rw_sysfs(const char *);
int percentage_pwm_duty_cycle(int percent);

enum steps {
    TEMP_STEP_SILENT    = 0,
    TEMP_STEP_30C       = 30,
    TEMP_STEP_40C       = 40,
    TEMP_STEP_50C       = 50,
    TEMP_STEP_60C       = 60,
};

static int pwm_thermal_table[] = {
    // Temp (°C), Duty Cycle (%)
    [TEMP_STEP_SILENT]  = 0,
    [TEMP_STEP_30C]     = 50,
    [TEMP_STEP_40C]     = 60,
    [TEMP_STEP_50C]     = 70,
    [TEMP_STEP_60C]     = 100,
};

#endif //HEADER_H
