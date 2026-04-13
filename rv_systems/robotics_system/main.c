#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 199309L
#endif

#include <stdbool.h>
#include <stdio.h>
#include <time.h>

#include "robotics_system.h"
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
float angle;
float servo;
bool e_stop;
float motor_power;

/* callbacks */
void viol_joint_limits(void) { printf("[VIOL] joint angle outside limits\n"); }
void viol_servo_delta(void) { printf("[VIOL] servo changed too fast\n"); }
void viol_estop_motor_power(void) { printf("[VIOL] e-stop asserted but motor_power != 0\n"); }

static void tick_sleep(void) {
#ifndef FAST
    struct timespec ts;
    ts.tv_sec = (time_t)(TICK_MS / 1000);
    ts.tv_nsec = (long)(TICK_MS % 1000) * 1000000L;
    nanosleep(&ts, NULL);
#endif
}

int main(void) {
    angle = 0.0f;
    servo = 0.0f;
    e_stop = false;
    motor_power = 0.0f;

    for (int t = 0; t < MAX_TICKS; t++) {
        /* joint angle within limits by default */
        angle = nondet_float_range(-1.0f, 1.0f);

        /* servo rate limit by default (delta <= 2) */
        servo += nondet_float_range(-2.0f, 2.0f);

        /* e-stop */
        e_stop = (nondet_u32() % 40u) == 0u;
        if (e_stop) {
            motor_power = 0.0f;
        } else {
            motor_power = nondet_float_range(0.0f, 100.0f);
        }

#if INJECT_CASE == 1
        if (t == 10) angle = 5.0f;
#elif INJECT_CASE == 2
        if (t == 10) servo += 10.0f;
#elif INJECT_CASE == 3
        if (t == 10) { e_stop = true; motor_power = 50.0f; }
#endif

        printf("t=%d angle=%.2f servo=%.2f estop=%d power=%.1f\n", t, angle, servo, e_stop, motor_power);
        step();
        tick_sleep();
    }

    return 0;
}
