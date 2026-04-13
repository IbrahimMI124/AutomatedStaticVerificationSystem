#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 199309L
#endif

#include <stdbool.h>
#include <stdio.h>
#include <time.h>

#include "avionics_system.h"
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
float altitude;
float airspeed;
bool control; /* corrective control flag */

/* callbacks */
void viol_altitude_min(void) { printf("[VIOL] altitude below minimum\n"); }
void viol_stall_control(void) { printf("[VIOL] airspeed < stall but control not within 2\n"); }

static void tick_sleep(void) {
#ifndef FAST
    struct timespec ts;
    ts.tv_sec = (time_t)(TICK_MS / 1000);
    ts.tv_nsec = (long)(TICK_MS % 1000) * 1000000L;
    nanosleep(&ts, NULL);
#endif
}

int main(void) {
    altitude = 1000.0f;
    airspeed = 80.0f;
    control = false;

    int control_timer = -1;
    const float alt_min = 300.0f;
    const float stall = 50.0f;
    (void)alt_min;

    for (int t = 0; t < MAX_TICKS; t++) {
        /* environment */
        altitude += nondet_float_range(-20.0f, 20.0f);
        airspeed += nondet_float_range(-5.0f, 5.0f);

        if (altitude < 0.0f) altitude = 0.0f;
        if (airspeed < 0.0f) airspeed = 0.0f;

        if (airspeed < stall) {
            control_timer = 2;
        }

        if (control_timer >= 0) {
            if (control_timer == 0) control = true;
            else control_timer--;
        } else {
            control = false;
        }

        /* control improves airspeed */
        if (control) airspeed += 8.0f;

#if INJECT_CASE == 1
        if (t == 10) altitude = alt_min - 10.0f;
#elif INJECT_CASE == 2
        if (t == 10) { airspeed = stall - 5.0f; control = false; control_timer = -1; }
#endif

        printf("t=%d alt=%.1f spd=%.1f control=%d\n", t, altitude, airspeed, control);
        step();
        tick_sleep();
    }

    return 0;
}
