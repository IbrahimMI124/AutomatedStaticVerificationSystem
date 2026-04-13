#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 199309L
#endif

#include <stdbool.h>
#include <stdio.h>
#include <time.h>

#include "industrial_temp_system.h"
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
float temp;
bool alarm;
bool cooling;

/* callbacks */
void viol_alarm_within_2(void) { printf("[VIOL] temp>80 but alarm not within 2\n"); }
void viol_reactor_temp_max(void) { printf("[VIOL] reactor temp exceeded temp_max\n"); }
void viol_cooling_within_2(void) { printf("[VIOL] temp>warn but cooling not within 2\n"); }

static void tick_sleep(void) {
#ifndef FAST
    struct timespec ts;
    ts.tv_sec = (time_t)(TICK_MS / 1000);
    ts.tv_nsec = (long)(TICK_MS % 1000) * 1000000L;
    nanosleep(&ts, NULL);
#endif
}

int main(void) {
    int alarm_timer = -1;
    int cooling_timer = -1;

    temp = 25.0f;
    alarm = false;
    cooling = false;

    const float warn = 80.0f;
    const float temp_max = 120.0f;
    (void)temp_max;

    for (int t = 0; t < MAX_TICKS; t++) {
        /* temperature evolves */
        float delta = nondet_float_range(-2.0f, 4.0f);
        temp += delta;
        if (temp < -20.0f) temp = -20.0f;
        if (temp > 130.0f) temp = 130.0f;

        bool over_warn = temp > warn;
        if (over_warn) {
            alarm_timer = 2;
            cooling_timer = 2;
        }

        /* alarm within 2 */
        if (alarm_timer >= 0) {
            if (alarm_timer == 0) alarm = true;
            else alarm_timer--;
        } else {
            alarm = false;
        }

        /* cooling within 2 */
        if (cooling_timer >= 0) {
            if (cooling_timer == 0) cooling = true;
            else cooling_timer--;
        } else {
            cooling = false;
        }

        /* cooling reduces temperature slightly */
        if (cooling) {
            temp -= 3.0f;
        }

#if INJECT_CASE == 1
        /* Force missing alarm */
        if (t == 10) { temp = 100.0f; alarm = false; alarm_timer = -1; }
#elif INJECT_CASE == 2
        /* Force temp_max violation */
        if (t == 10) { temp = temp_max + 10.0f; }
#elif INJECT_CASE == 3
        /* Force missing cooling */
        if (t == 10) { temp = 100.0f; cooling = false; cooling_timer = -1; }
#endif

        printf("t=%d temp=%.1f alarm=%d cooling=%d\n", t, temp, alarm, cooling);
        step();
        tick_sleep();
    }

    return 0;
}
