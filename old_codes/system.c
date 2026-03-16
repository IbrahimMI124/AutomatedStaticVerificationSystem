#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include "plane2.h"

/* Define monitor inputs */
bool autopilot;
uint64_t altitude;

/* Nondet for CBMC */
extern _Bool nondet_bool(void);
extern uint64_t nondet_uint64(void);

/* Violation flag */
bool violation_happened = false;

/* Monitor callback */
void recover(uint64_t a, uint64_t t) {
    violation_happened = true;
}

int main() {

    for (int i = 0; i < 5; i++) {

        autopilot = 1;
        altitude = 0;
        step();
    }

    /* We want to prove recover is never called */
    assert(!violation_happened);

    return 0;
}

