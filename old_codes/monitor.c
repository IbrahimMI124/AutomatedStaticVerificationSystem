#include "monitor.h"

void monitor_init(monitor_t *m) {
    m->state = 0;
}

void monitor_step(monitor_t *m, int trigger) {
    if (m->state == 0 && trigger) {
        m->state = 1;   // enter bad state
    }
}

void monitor_assert(monitor_t *m) {
    assert(m->state == 0);
}

