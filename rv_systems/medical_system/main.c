#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 199309L
#endif

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

#include "medical_system.h"
#include "nondet.h"

#ifndef TICK_MS
#define TICK_MS 100
#endif

#ifndef MAX_TICKS
#define MAX_TICKS 200
#endif

#ifndef INJECT_CASE
#define INJECT_CASE 0
#endif

/* externs */
int32_t hr;
bool pump;
float rate;
bool vitals_out_of_range;
bool alarm;

/* callbacks */
void viol_hr_pump_stop(void) { printf("[VIOL] HR<40 but pump not off within 10 steps\n"); }
void viol_infusion_rate_max(void) { printf("[VIOL] infusion rate exceeded max\n"); }
void viol_vitals_alarm(void) { printf("[VIOL] vitals out of range but alarm not within 1\n"); }

static void tick_sleep(void) {
#ifndef FAST
    struct timespec ts;
    ts.tv_sec = (time_t)(TICK_MS / 1000);
    ts.tv_nsec = (long)(TICK_MS % 1000) * 1000000L;
    nanosleep(&ts, NULL);
#endif
}

int main(void) {
    int pump_timer = -1;
    int alarm_timer = -1;

    hr = 70;
    pump = true;
    rate = 0.0f;
    vitals_out_of_range = false;
    alarm = false;

    const float rate_max = 5.0f;

    for (int t = 0; t < MAX_TICKS; t++) {
        /* heart rate */
        hr += nondet_i32_range(-3, 3);
        if (hr < 20) hr = 20;
        if (hr > 160) hr = 160;

        if (hr < 40) {
            pump_timer = 10;
        }

        if (pump_timer >= 0) {
            if (pump_timer == 0) pump = false;
            else pump_timer--;
        } else {
            pump = true;
        }

        /* infusion rate constrained by controller */
        rate = nondet_float_range(0.0f, rate_max);

        /* vitals alarm within 1 */
        vitals_out_of_range = (hr < 45) || (hr > 140);
        if (vitals_out_of_range) {
            alarm_timer = 1;
        }
        if (alarm_timer >= 0) {
            if (alarm_timer == 0) alarm = true;
            else alarm_timer--;
        } else {
            alarm = false;
        }

#if INJECT_CASE == 1
        if (t == 10) { hr = 30; pump = true; pump_timer = -1; }
#elif INJECT_CASE == 2
        if (t == 10) { rate = rate_max + 1.0f; }
#elif INJECT_CASE == 3
        if (t == 10) { vitals_out_of_range = true; alarm = false; alarm_timer = -1; }
#endif

        printf("t=%d hr=%d pump=%d rate=%.2f vitals_oor=%d alarm=%d\n",
               t, (int)hr, pump, rate, vitals_out_of_range, alarm);

        step();
        tick_sleep();
    }

    return 0;
}
