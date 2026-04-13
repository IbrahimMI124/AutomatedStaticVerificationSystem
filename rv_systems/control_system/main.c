#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 199309L
#endif

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

#include "control_system.h"
#include "nondet.h"

#ifndef TICK_MS
#define TICK_MS 100
#endif

#ifndef MAX_TICKS
#define MAX_TICKS 250
#endif

#ifndef INJECT_CASE
#define INJECT_CASE 0
#endif

/* externs */
float output;
float ref;
float control; /* also used as generic control flag in some properties */
float u;
float velocity;
bool disturbance;
bool startup;

/* callbacks */
void viol_post_condition(void) { printf("[VIOL] post-condition after 50 cycles failed\n"); }
void viol_startup_stabilize(void) { printf("[VIOL] startup stabilize within 50 failed\n"); }
void viol_actuator_saturation(void) { printf("[VIOL] control out of [-100,100]\n"); }
void viol_temporal_consistency(void) { printf("[VIOL] |u(t)-u(t-1)| too large\n"); }
void viol_bounded_stabilisation(void) { printf("[VIOL] disturbance not stabilised within 5\n"); }

static void tick_sleep(void) {
#ifndef FAST
    struct timespec ts;
    ts.tv_sec = (time_t)(TICK_MS / 1000);
    ts.tv_nsec = (long)(TICK_MS % 1000) * 1000000L;
    nanosleep(&ts, NULL);
#endif
}

int main(void) {
    /* simple first-order plant */
    ref = 100.0f;
    output = 0.0f;
    control = 0.0f;
    u = 0.0f;
    velocity = 0.0f;
    disturbance = false;
    startup = true;

    int startup_timer = 50;
    int stabilise_timer = -1;

    for (int t = 0; t < MAX_TICKS; t++) {
        if (t > 5) startup = false;

        /* reference slowly varies */
        ref += nondet_float_range(-0.5f, 0.5f);
        if (ref < 50.0f) ref = 50.0f;
        if (ref > 150.0f) ref = 150.0f;

        /* disturbance event */
        disturbance = nondet_bool() && (nondet_u32() % 30u == 0u);
        if (disturbance) {
            stabilise_timer = 5;
            velocity += nondet_float_range(-10.0f, 10.0f);
        }

        /* controller output u with delta-limit by construction */
        float du = nondet_float_range(-2.0f, 2.0f);
        u += du;

        /* saturation for control signal */
        control = u;
        if (control > 100.0f) control = 100.0f;
        if (control < -100.0f) control = -100.0f;

        /* plant update */
        float err = ref - output;
        output += 0.05f * err + 0.01f * control;

        /* velocity tends to decay */
        velocity *= 0.7f;

        if (stabilise_timer >= 0) {
            if (stabilise_timer > 0) stabilise_timer--;
        }

        /* attempt to satisfy startup stabilize within 50:
           after startup true, we push output toward ref quickly for 50 ticks */
        if (startup) {
            startup_timer = 50;
        }
        if (startup_timer >= 0) {
            /* accelerate convergence */
            output += 0.1f * (ref - output);
            if (startup_timer > 0) startup_timer--;
        }

#if INJECT_CASE == 1
        /* break post-condition after 50 */
        if (t == 70) output = ref * 1.2f;
#elif INJECT_CASE == 2
        /* break saturation */
        if (t == 20) control = 200.0f;
#elif INJECT_CASE == 3
        /* break temporal consistency */
        if (t == 20) u += 20.0f;
#elif INJECT_CASE == 4
        /* break bounded stabilisation */
        if (t == 20) { disturbance = true; stabilise_timer = 0; velocity = 100.0f; }
#endif

        printf("t=%d ref=%.1f out=%.1f control=%.1f u=%.1f vel=%.2f startup=%d\n",
               t, ref, output, control, u, velocity, startup);
        step();
        tick_sleep();
    }

    return 0;
}
