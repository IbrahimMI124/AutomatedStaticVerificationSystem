#ifndef MONITOR_H
#define MONITOR_H

#include <assert.h>

typedef struct {
    int state;   // 0 = OK, 1 = violation
} monitor_t;

void monitor_init(monitor_t *m);
void monitor_step(monitor_t *m, int trigger);
void monitor_assert(monitor_t *m);

#endif

